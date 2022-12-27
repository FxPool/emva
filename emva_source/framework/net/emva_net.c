// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva_framework.h"
#include "emva_observer.h"
#include "emva_net.h"
#include "emva_malloc.h"
#include "emva_time.h"
#include "emva_kfifo.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include <stdio.h>
#include "dv_net_device.h"

#if EMVA_FM_NET_ENABLED == 1
#define MAGIC_CLOSE 0
#define MAGIC_READ_LOCAL_ADDR 1
#define MAGIC_SOCKET 2
#define MAGIC_IS_NET_CONNECT 3

typedef void (*event_callbackDef)(void);
event_callbackDef event_callback;
static int32 g_fd = -1;
static EMVA_BOOL g_isInit = FALSE;
uint8 kfifo_buffer[NET_FIFO_BUFFER_SIZE];
EMVA_BOOL EmvaNetInitialize(const char *name);
/**
  * @brief  	Hardware data receiving callback function, judged by V pointer 
	            if the hardware supports multiple sockets
  * @param  	V Conn connection instance D data pointer len data length
  * @retval :   None
**/
static void data_callback(void *v, void *d, uint32 len)
{
  //Add a piece of data to FIFO and generate a callback event
  EmvaConn *c = (EmvaConn *)v;
  dv_kfifo_put(c->fifo, d, len);
  if (event_callback)
  {
    event_callback();
  }
}

/**
  * @brief  	The connection event reads the received data through the event 
                callback notification when the operating system is not in use
  * @param  	C connects the instance p callback function
  * @retval :   Error description
**/
static dv_error *ConnEvent(EmvaConn *this, void (*p)(void))
{
  event_callback = p;
  return NULL;
}
/**
  * @brief  	Read function
  * @param  	C connects instance B data n data length
  * @retval : Error description
**/
static dv_error *ConnRead(EmvaConn *this, byte *b, uint32 nread, int32 *nrecv)
{
  // Read the data from FIFO, the data is blocked state,
  // if not used by the operating systemThe OS - free
  // waitEvent is implemented as a delay function and 1ms
  // query is recommended
  do
  {
    *nrecv = dv_kfifo_get(this->fifo, b, nread);
    emva_sleep(1);
  } while (*nrecv == 0);
  return NULL;
}
/**
  * @brief  	Write the data
  * @param  	C connects instance B data n data length
  * @retval :   Error description
**/
static dv_error *ConnWrite(EmvaConn *this, byte *b, uint32 n)
{
  net_dev_start_xmit(g_fd, this, b, n);

  return NULL;
}
/**
  * @brief  	Close a connection
  * @param  	C connection instance
  * @retval :   Error description
**/
static dv_error *ConnClose(EmvaConn *this)
{
  net_dev_do_ioctl(g_fd, this, MAGIC_CLOSE, NULL);
  return NULL;
}
/**
  * @brief  	Get the local address
  * @param  	C connection instance
  * @retval :   Addr
**/
static Addr ConnLocalAddr(EmvaConn *this)
{
  // Call the event hardware interface to read the native
  // IP address by passing a pointer to the IO CTL method
  // The hardware may be ready at initialization time
  static Addr thisAddr;
  struct AddrInfo
  {
    char *Network;
    char *String;
  };
  struct AddrInfo thisAddrInfo;
  net_dev_do_ioctl(g_fd, this, MAGIC_READ_LOCAL_ADDR, (uint32)&thisAddrInfo);
  thisAddr.Network = thisAddrInfo.Network;
  thisAddr.String = thisAddrInfo.String;
  return thisAddr;
}
/**
  * @brief  	Get a remote address
  * @param  	C connection instance
  * @retval :   Addr
**/
static Addr ConnRemoteAddr(EmvaConn *this)
{
  static Addr thisAddr;
  return thisAddr;
}
/**
  * @brief  	Set the break time
  * @param  	C connection instance
  * @retval :   Error description
**/
static dv_error *ConnSetDeadline(EmvaConn *this, uint32 t)
{
  this->err->new (this->err, "no over write");
  return this->err;
}
/**
  * @brief  	Set read timeout
  * @param  	C Connection instance T timeout value
  * @retval :   Error description
**/
static dv_error *ConnSetReadDeadline(EmvaConn *this, uint32 t)
{
  this->err->new (this->err, "no over write");
  return this->err;
}
/**
  * @brief  	Set write timeout
  * @param  	C Connection instance T timeout value
  * @retval :   Error description
**/
static dv_error *ConnSetWriteDeadline(EmvaConn *this, uint32 t)
{
  this->err->new (this->err, "no over write");
  return this->err;
}
/**
  * @brief  	load driver
  * @param  	DriverName. driver name
  * @retval : bool
**/
static EMVA_BOOL ThisLoadDriver(const char *DriverName)
{
  return EmvaNetInitialize(DriverName) == TRUE ? TRUE : FALSE;
}
/**
  * @brief  	Internet dial-up
  * @param  	D. Example ERR describes the Network type address
  * @retval :   The connection instance
**/
static EmvaConn *ThisDial(dv_error *err, char *Network, char *address)
{
  uint8 netState = 0;
  if (net_dev_do_ioctl(g_fd, &netState, MAGIC_IS_NET_CONNECT, NULL) == -1)
  {
    return NULL;
  }
  if (netState == 0)
  {
    if (err)
      err->err_info = "net disconnet";
    return NULL;
  }
  EmvaConn *conn = (EmvaConn *)emva_malloc(sizeof(EmvaConn));
  if (conn == NULL)
  {
    return NULL;
  }
  conn->Event = ConnEvent;
  conn->Read = ConnRead;
  conn->Write = ConnWrite;
  conn->Close = ConnClose;
  conn->GetLocalAddr = ConnLocalAddr;
  conn->GetRemoteAddr = ConnRemoteAddr;
  conn->SetDeadline = ConnSetDeadline;
  conn->SetReadDeadline = ConnSetReadDeadline;
  conn->SetWriteDeadline = ConnSetWriteDeadline;
  conn->err = dv_error_new(128);
  conn->fifo = (struct dv_kfifo *)emva_malloc(sizeof(struct dv_kfifo));
  dv_kfifo_init(conn->fifo, kfifo_buffer, sizeof(kfifo_buffer));
  /* If the hardware supports multiple sockets */
  struct socket
  {
    char *network;
    char *address;
  };
  struct socket sck = {
      .network = Network,
      .address = address,
  };
  if (net_dev_do_ioctl(g_fd, conn, MAGIC_SOCKET, (uint32)&sck) == -1)
  {
    if (err)
      err->err_info = "sockets fail";
    return NULL;
  }
  else
  {
    net_dev_event(g_fd, conn, data_callback);
  }
  if (err)
    err->err_info = NULL;
  return conn;
}
/* API function */
Dialer EmvaNet = {
    .Dial = ThisDial,
    .LoadDriver = ThisLoadDriver,
};
/**
  * @brief  	Implement hardware operation interface to realize polymorphism
  * @param  	P hardware interface
  * @retval :   None
**/
EMVA_BOOL EmvaNetInitialize(const char *name)
{
  g_fd = net_dev_open(name);
  g_isInit = (g_fd == -1) ? FALSE : TRUE;
  return g_isInit;
}
#endif
