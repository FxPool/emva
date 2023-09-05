// Copyright (c) 2021-2023 The Eva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "emva_pmic.h"
#include "emva_malloc.h"
#include "dv_character_device.h"
#include "string.h"
#include "emva_config.h"
#if EMVA_PMIC_PMIC_ENABLED == 1
#define PMIC_REBOOT 1
#define PMIC_SHUTDOWN 2
#define PMIC_SLEEP 3
#define PMIC_STANDBY 4
#define PMIC_STOP 5

static int32 g_fd = -1;
static EMVA_BOOL g_isInit = FALSE;

static void PmicReboot(EmvaPmic *this)
{
  character_dev_ioctl(g_fd, this, PMIC_REBOOT, NULL);
}
static void PmicShutdown(EmvaPmic *this)
{
  character_dev_ioctl(g_fd, this, PMIC_SHUTDOWN, NULL);
}
static void PmicSleep(EmvaPmic *this)
{
  character_dev_ioctl(g_fd, this, PMIC_SLEEP, NULL);
}
static void PmicStandby(EmvaPmic *this)
{
  character_dev_ioctl(g_fd, this, PMIC_STANDBY, NULL);
}
static void PmicStop(EmvaPmic *this)
{
  character_dev_ioctl(g_fd, this, PMIC_STOP, NULL);
}
static emva_pmic_property *PmicGetProprety(EmvaPmic *this)
{
  static emva_pmic_property read;
  character_dev_read(g_fd, this, &read, sizeof(emva_pmic_property));
  return &read;
}

EmvaPmic *newPmic(void)
{
  EmvaPmic *pmic = (EmvaPmic *)emva_malloc(sizeof(EmvaPmic));
  if (g_isInit == FALSE || pmic == NULL)
  {
    return NULL;
  }
  pmic->Reboot = PmicReboot;
  pmic->Shutdown = PmicShutdown;
  pmic->Sleep = PmicSleep;
  pmic->Standby = PmicStandby;
  pmic->Stop = PmicStop;
  pmic->GetProprety = PmicGetProprety;
  return pmic;
}

struct EmvaPmicApi EmvaPmicClass = {
    .new = newPmic,
};

EMVA_BOOL EmvaPmicInitialize(void)
{
  g_fd = character_dev_open(EMVA_POWER_DRIVER_NAME, NULL);
  g_isInit = (g_fd == -1) ? FALSE : TRUE;
  return g_isInit;
}
#endif
