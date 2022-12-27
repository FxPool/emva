// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef __EmvaWifi_H
#define __EmvaWifi_H

#include "emva_os.h"
#include "emva_list.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct
    {
        char *ssid;
        char *pass;
    } WifiConfiguration;
    typedef struct
    {
        char ssid[64];
        char pass[64];
    } ConnectionInfo;
    typedef struct
    {
        Link_List *wifi_scanresult;
    } ScanResults;
    typedef enum
    {
        COMPLETED = 0,
        SUPPLICANT_STATE_CHANGED_ACTION,
    } SupplicanState;
    typedef struct _EmvaWifi EmvaWifi;
    struct _EmvaWifi
    {
        WifiConfiguration Wcinfo;
        ConnectionInfo Conninfo;
        // Set the connection information
        void (*SetConnectInfo)(EmvaWifi *this, const char *ssid, const char *pass);
        // Get the BSSID of the current access point
        byte *(*GetBSSIS)(EmvaWifi *this);
        // Get the IP address
        byte *(*GetIPAddress)(EmvaWifi *this);
        // Gets the current connection speed
        uint32 (*GetLinkSpeed)(EmvaWifi *this);
        // Get the MAC address
        byte *(*GetMacAddress)(EmvaWifi *this);
        // Get the signal strength indicator for the 802.11n network
        float (*GetRssi)(EmvaWifi *this);
        // Get the network SSID
        byte *(*GetSSID)(EmvaWifi *this);
        // Returns information about the client status
        SupplicanState (*GetSupplicanState)(EmvaWifi *this);
        // Add a new network to the set up network
        EMVA_BOOL(*AddNetwork)
        (EmvaWifi *this, WifiConfiguration config);
        // Calculate the signal level
        EMVA_BOOL(*CalculateSignalLevel)
        (EmvaWifi *this, int rssi, int numLevels);
        // Compare the strength of the two signals
        EMVA_BOOL(*CompareSignalLevel)
        (EmvaWifi *this, int rssiA, int rssiB);
        // Create a Wi-Fi lock that locks the current Wi-Fi connection
        EMVA_BOOL(*CreateWifiLock)
        (EmvaWifi *this, int lockType, char *tag);
        // Let a network connection fail
        EMVA_BOOL(*DisableNetWork)
        (EmvaWifi *this, int netId);
        // Disconnect from current access point
        void (*Disconnect)(EmvaWifi *this);
        // Wifi connect
        void (*Connect)(EmvaWifi *this);
        // Create a connection to the network you set up before
        void (*EnableNetwork)(EmvaWifi *this, int netId, EMVA_BOOL disableOthers);
        // The client retrieves the network connection status
        WifiConfiguration *(*GetConfiguredNetworks)(EmvaWifi *this);
        // Gets the current connection information
        ConnectionInfo *(*GetConnectionInfo)(EmvaWifi *this);
        // Get the DHCP information
        byte *(*GetDhcpInfo)(EmvaWifi *this);
        // Get the scan test results
        ScanResults *(*GetScanResults)(EmvaWifi *this);
        // Gets if a Wi-Fi access point is valid
        EMVA_BOOL(*GetWifiState)
        (EmvaWifi *this);
        // Determine if a Wi-Fi connection is valid
        EMVA_BOOL(*IsWifiEnabled)
        (EmvaWifi *this);
        // Check if the client daemon is responding to the request
        EMVA_BOOL(*PingSupplicant)
        (EmvaWifi *this);
        // Reconnect to the access point, even if it is already connected
        void (*Ressociate)(EmvaWifi *this);
        // If there is no connection now, reconnect
        EMVA_BOOL(*Reconnect)
        (EmvaWifi *this);
        // Move out of a specific network
        EMVA_BOOL(*RemoveNetwork)
        (EmvaWifi *this, int netId);
        // Preserve a configuration information
        EMVA_BOOL(*SaveConfiguration)
        (EmvaWifi *this);
        // To make a connection valid or invalid
        void (*SetWifiEnabled)(EmvaWifi *this, EMVA_BOOL enabled);
        // Start scanning
        void (*StartScan)(EmvaWifi *this);
        // Update a network connection
        void (*UpdateNetwork)(EmvaWifi *this, WifiConfiguration *config);
    };
    struct EmvaWifiApi
    {
        EmvaWifi *(*new)(void);
    };
    extern struct EmvaWifiApi EmvaWifiClass;
    extern void EmvaWifiInitialize(const char *name);
#ifdef __cplusplus
}
#endif

#endif
