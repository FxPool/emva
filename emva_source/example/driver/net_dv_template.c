// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "dv_net_device.h"
#include "dev_init.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"

#define SCK_MAX_NUM 1
#define MAGIC_CLOSE 0
#define MAGIC_READ_LOCAL_ADDR 1
#define MAGIC_SOCKET 2
#define MAGIC_IS_NET_CONNECT 3

void *sck_obj[SCK_MAX_NUM];
uint8 g_sck_index = 0;
typedef struct
{
    void *obj;
    void (*add_data_to_netbus)(void *obj, void *d, uint32 len);
} NetBusDataSet_t;
NetBusDataSet_t NetBusDataSet;
int get_sck_index(void *boj)
{
    for (uint8 i = 0; i < SCK_MAX_NUM; i++)
    {
        if (sck_obj[i] == boj)
        {
            return i;
        }
    }
    return -1;
}
static int net_dv_temp_ndo_open(void)
{
    return net_dv_tempNetinitialize();
}
static int net_dv_temp_ndo_stop(void *obj) { return 0; }
static int net_dv_temp_ndo_start_xmit(void *obj, void *d, uint32 len)
{
    net_dv_tempSendDt(d, len);
    return 0;
}
static int net_dv_temp_ndo_do_ioctl(void *obj, uint32 cmd, uint32 arg)
{
    struct socket_info
    {
        char *network;
        char *address;
    };
    struct socket_info sck;
    char sck_network[12];
    char sck_parameter[64];
    uint16 port;
    char ip[16];
    char *p_str;
    int sck_index;
    switch (cmd)
    {
    case MAGIC_SOCKET:
        sck = *(struct socket_info *)(arg);
        memset(sck_network, 0, 12);
        memset(sck_parameter, 0, 64);
        sck_parameter[0] = 0;
        memset(ip, 0, 16);
        sck_index = get_sck_index(obj);
        for (int i = 0; i < strlen(sck.network); i++)
        {
            sck_network[i] = tolower(sck.network[i]);
        }
        memcpy(sck_parameter, (char *)sck.address, strlen((char *)sck.address));
        p_str = strtok(sck_parameter, ":");
        memcpy(ip, p_str, strlen(p_str));
        p_str = strtok(NULL, ":");
        port = atoi(p_str);
        if (sck_index != -1)
        {
            // example func
            net_dv_tempTcp(sck_index, ip, port);
        }
        else
        {
            // example func
            net_dv_tempTcp(g_sck_index, ip, port);
            sck_obj[g_sck_index++] = obj;
        }
        break;
    case MAGIC_IS_NET_CONNECT:
            // example func
        *(uint8 *)(obj) = net_dv_tempGetLinkState();
        break;
    }
    return 0;
}

static void wifi_data_callback(uint8 *data, uint16 length)
{
    NetBusDataSet.add_data_to_netbus(NetBusDataSet.obj, data, length);
}

static int net_dv_temp_ndo_data_bus_input(void *obj, void (*ptr)(void *obj, void *d, uint32 len))
{
    net_dv_tempDataCallbackSet(wifi_data_callback);
    NetBusDataSet.obj = obj;
    NetBusDataSet.add_data_to_netbus = ptr;
    return 0;
}

hs_net_interface_t hs_net_interface = {
    .ndo_open = net_dv_temp_ndo_open,
    .ndo_stop = net_dv_temp_ndo_stop,
    .ndo_start_xmit = net_dv_temp_ndo_start_xmit,
    .ndo_do_ioctl = net_dv_temp_ndo_do_ioctl,
    .ndo_data_bus_input = net_dv_temp_ndo_data_bus_input,
};

net_driver_t net_driver = {
    .DeviceName = "EMVA_NET",
    .DeviceNumber = 10032,
    .ops = &hs_net_interface,
};
static void net_dv_temp_init(void)
{
    net_dev_device_registered(&net_driver);
}
DV_MODULE_REG(net_dv_temp_init);
