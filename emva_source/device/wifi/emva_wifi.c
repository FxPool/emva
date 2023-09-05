// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_observer.h"
#include "emva_list.h"
#include "emva_wifi.h"
#include "emva_malloc.h"
#include "dv_character_device.h"
#include "stdarg.h"
#include "string.h"

#if EMVA_FM_TIME_ENABLED == 1
#define MAGIC_READ_SUPPLICAN_STATE 0
#define MAGIC_CONNECT 1
#define MAGIC_GET_MAC_ADDR 2
#define MAGIC_GET_RSSI 3
#define MAGIC_GET_SSID 4
static int32 g_fd = -1;
static EMVA_BOOL g_isInit = FALSE;
// Set the connection information
static void this_setConnectInfo(EmvaWifi *this, const char *ssid, const char *pass)
{
    memcpy(this->Conninfo.ssid, ssid, strlen(ssid));
    memcpy(this->Conninfo.pass, pass, strlen(pass));
}
// Get the BSSID of the current access point
static byte *this_getBSSIS(EmvaWifi *this)
{
    return (byte *)"";
}
// Get the IP address
static byte *this_getIPAddress(EmvaWifi *this)
{
    return (byte *)"";
}
// Gets the current connection speed
static uint32 this_getLinkSpeed(EmvaWifi *this)
{
    return 0;
}
// Get the MAC address
static byte *this_getMacAddress(EmvaWifi *this)
{
    byte *read;
    character_dev_ioctl(g_fd, (void *)read, MAGIC_GET_MAC_ADDR, NULL);
    return read;
}
// Get the signal strength indicator for the 802.11n network
static float this_getRssi(EmvaWifi *this)
{
    float read;
    character_dev_ioctl(g_fd, (void *)&read, MAGIC_GET_RSSI, NULL);
    return read;
}
// Get the network SSID
static byte *this_getSSID(EmvaWifi *this)
{
    byte *read;
    character_dev_ioctl(g_fd, (void *)read, MAGIC_GET_SSID, NULL);
    return read;
}
// Returns information about the client status
static SupplicanState this_getSupplicanState(EmvaWifi *this)
{
    uint8 read = 0;
    character_dev_ioctl(g_fd, (void *)&read, MAGIC_READ_SUPPLICAN_STATE, NULL);
    return (read == 1) ? COMPLETED : SUPPLICANT_STATE_CHANGED_ACTION;
}
// Add a new network to the set up network
static EMVA_BOOL this_addNetwork(EmvaWifi *this, WifiConfiguration config)
{
    return TRUE;
}
// Calculate the signal level
static EMVA_BOOL this_calculateSignalLevel(EmvaWifi *this, int rssi, int numLevels)
{
    return TRUE;
}
// Compare the strength of the two signals
static EMVA_BOOL this_compareSignalLevel(EmvaWifi *this, int rssiA, int rssiB)
{
    return TRUE;
}
// Create a Wi-Fi lock that locks the current Wi-Fi connection
static EMVA_BOOL this_createWifiLock(EmvaWifi *this, int lockType, char *tag)
{
    return TRUE;
}
// Let a network connection fail
static EMVA_BOOL this_disableNetWork(EmvaWifi *this, int netId)
{
    return TRUE;
}
// Connect to the current access point
static EMVA_BOOL this_connect(EmvaWifi *this)
{
    WifiConfiguration wg;
    wg.ssid = this->Conninfo.ssid;
    wg.pass = this->Conninfo.pass;
    int16 r = character_dev_ioctl(g_fd, this, MAGIC_CONNECT, (uint32)&wg);
		if(r == -1)
		{
				return FALSE;
		}
		return TRUE;
}
// Disconnect from current access point
static void this_disconnect(EmvaWifi *this)
{
}
// Create a connection to the network you set up before
static void this_enableNetwork(EmvaWifi *this, int netId, EMVA_BOOL disableOthers)
{
}
// The client retrieves the network connection status
static WifiConfiguration *this_getConfiguredNetworks(EmvaWifi *this)
{
    return NULL;
}
// Gets the current connection information
static ConnectionInfo *this_getConnectionInfo(EmvaWifi *this)
{
    return NULL;
}
// Get the DHCP information
static byte *this_getDhcpInfo(EmvaWifi *this)
{
    return (byte *)"";
}
// Get the scan test results
static ScanResults *this_getScanResults(EmvaWifi *this)
{
    return NULL;
}
// Gets if a Wi-Fi access point is valid
static EMVA_BOOL this_getWifiState(EmvaWifi *this)
{
    return FALSE;
}
// Determine if a Wi-Fi connection is valid
static EMVA_BOOL this_isWifiEnabled(EmvaWifi *this)
{
    return TRUE;
}
// Check if the client daemon is responding to the request
static EMVA_BOOL this_pingSupplicant(EmvaWifi *this)
{
    return TRUE;
}
// Reconnect to the access point, even if it is already connected
static void this_ressociate(EmvaWifi *this)
{
}
// If there is no connection now, reconnect
static EMVA_BOOL this_reconnect(EmvaWifi *this)
{
    return TRUE;
}
// Move out of a specific network
static EMVA_BOOL this_removeNetwork(EmvaWifi *this, int netId)
{
    return TRUE;
}
// Preserve a configuration information
static EMVA_BOOL this_saveConfiguration(EmvaWifi *this)
{
    return TRUE;
}
// To make a connection valid or invalid
static void this_setWifiEnabled(EmvaWifi *this, EMVA_BOOL enabled)
{
}
// Start scanning
static void this_startScan(EmvaWifi *this)
{
}
// Update a network connection
static void this_updateNetwork(EmvaWifi *this, WifiConfiguration *config)
{
}
// polymorphic implementation
EmvaWifi *EmvaWifiNew(void)
{
    EmvaWifi *this_wifi = (EmvaWifi *)emva_malloc(sizeof(EmvaWifi));
    if (g_isInit == FALSE || this_wifi == NULL)
    {
        return NULL;
    }
    this_wifi->SetConnectInfo = this_setConnectInfo;
    this_wifi->GetBSSIS = this_getBSSIS;
    this_wifi->GetIPAddress = this_getIPAddress;
    this_wifi->GetLinkSpeed = this_getLinkSpeed;
    this_wifi->GetMacAddress = this_getMacAddress;
    this_wifi->GetRssi = this_getRssi;
    this_wifi->GetSSID = this_getSSID;
    this_wifi->GetSupplicanState = this_getSupplicanState;
    this_wifi->AddNetwork = this_addNetwork;
    this_wifi->CalculateSignalLevel = this_calculateSignalLevel;
    this_wifi->CompareSignalLevel = this_compareSignalLevel;
    this_wifi->CreateWifiLock = this_createWifiLock;
    this_wifi->DisableNetWork = this_disableNetWork;
    this_wifi->Disconnect = this_disconnect;
    this_wifi->Connect = this_connect;
    this_wifi->EnableNetwork = this_enableNetwork;
    this_wifi->GetConfiguredNetworks = this_getConfiguredNetworks;
    this_wifi->GetConnectionInfo = this_getConnectionInfo;
    this_wifi->GetDhcpInfo = this_getDhcpInfo;
    this_wifi->GetScanResults = this_getScanResults;
    this_wifi->GetWifiState = this_getWifiState;
    this_wifi->IsWifiEnabled = this_isWifiEnabled;
    this_wifi->PingSupplicant = this_pingSupplicant;
    this_wifi->Ressociate = this_ressociate;
    this_wifi->Reconnect = this_reconnect;
    this_wifi->RemoveNetwork = this_removeNetwork;
    this_wifi->SaveConfiguration = this_saveConfiguration;
    this_wifi->SetWifiEnabled = this_setWifiEnabled;
    this_wifi->StartScan = this_startScan;
    this_wifi->UpdateNetwork = this_updateNetwork;
    return this_wifi;
}

/* API function */
struct EmvaWifiApi EmvaWifiClass = {
    .new = EmvaWifiNew,
};

EMVA_BOOL EmvaWifiInitialize(const char *name)
{
    g_fd = character_dev_open(name, NULL);
    g_isInit = (g_fd == -1) ? FALSE : TRUE;
    return g_isInit;
}
#endif
