// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef _EMVA_NET_H
#define _EMVA_NET_H

#include "emva_kfifo.h"
#include "emva_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

        typedef struct _Addr Addr;
        typedef struct _Conn EmvaConn;
        typedef struct _Dialer Dialer;

        struct _Addr
        {
                // The network name
                char *Network;
                // The address in string format
                char *String;
        };

        struct _Conn
        {
                // Data receive cache
                struct emva_kfifo *fifo;
                // error info
                emva_error *err;
                // Timeout is the maximum time that dial can wait for a connection to be established. The default value is no Timeout.
                // If the Deadline field is also set, the dial operation may fail earlier.
                // The operating system may enforce its timeout Settings whether or not they are set.
                // For example, TCP (system) timeouts are typically around 3 minutes.
                uint32 Timeout;
                // Deadline is a specific point in time after which dial fails.
                // If the Timeout field is also set, dial may fail earlier.
                // A zero value indicates that there is no expiration date, that is, obeying the operating system timeout setting.
                uint32 Deadline;
                // localAddr is the local address used when dial an address.
                // The address must be of a network compatible type with Dial.
                // If it is nil, a local address will be selected automatically.
                Addr LocalAddr;
                // DualStack allows a single dial operation on a network of type "TCP",
                // If the destination is a hostname DNS record with multiple addresses,
                // Attempts to establish multiple IPv4 and IPv6 connections and returns the first connection established.
                EMVA_BOOL DualStack;
                // keepAlive specifies the life of an active network connection;If 0, keep-alive will be disabled.
                // Network connections that do not support keep-alive will ignore this field.
                uint32 KeepAlive;
                // Event method notifies automatically when data is generated
                emva_error *(*Event)(EmvaConn *c, void (*p)(void));
                // Read from the connection
                // The Read method may Timeout after a certain time limit and return an error. The Timeout() method for that error returns true
                emva_error *(*Read)(EmvaConn *c, byte *b, uint32 nread, int32 *nrecv);
                // Write writes data from the connection
                // The Write method may Timeout after a certain time limit and return an error. The Timeout() method for that error returns true
                emva_error *(*Write)(EmvaConn *c, byte *b, uint32 n);
                // Close method closes the connection
                // And causes any Read or Write methods in the block to no longer block and return an error
                emva_error *(*Close)(EmvaConn *c);
                // Returns the local network address
                Addr (*GetLocalAddr)(EmvaConn *c);
								// Connect New network address
                EmvaConn * (*ConnectNewAddress)(EmvaConn *c,emva_error *err, char *Network, char *address);
                // Returns the remote network address
                Addr (*GetRemoteAddr)(EmvaConn *c);
                // Set the deadline for this connection to read and write. This is equivalent to calling both setReadDeadline and setWriteDeadline
                // Deadline is an absolute time beyond which I/O operations return directly without blocking due to a failed timeout
                // Deadline takes effect for all subsequent I/O operations, not just the next read or write
                // If the parameter t is zero, the deadline is not set
                emva_error *(*SetDeadline)(EmvaConn *c, uint32 t);
                // Set the read operation Deadline for this connection. A value of zero for t means no deadline is set
                emva_error *(*SetReadDeadline)(EmvaConn *c, uint32 t);
                // Set the write operation deadline for this connection. A zero value for t means no deadline is set
                // Even if the write timeouts, the return value n may be > >;0, indicating that part of the data was successfully written
                emva_error *(*SetWriteDeadline)(EmvaConn *c, uint32 t);
        };
        struct _Dialer
        {
                // Connect address to Conn on network and return Conn.The types of networks available are:
                //" TCP ", "tcp4", "udp", "udp4", "IP", "ip4""
                EmvaConn *(*Dial)(emva_error *err, char *Network, char *address);
                EmvaConn *(*DialTimeout)(emva_error *err, char *Network, char *address, uint32 Millisecond);
                EMVA_BOOL(*LoadDriver)
                (const char *DriverName);
        };
        extern Dialer EmvaNet;
        extern EMVA_BOOL EmvaNetInitialize(const char *name);
#ifdef __cplusplus
}
#endif

#endif
