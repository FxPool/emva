# emva
Embedded virtual architecture allows you to quickly start the development of single chip microcomputer without having to learn the complex internal principles of single chip microcomputer. You only need to learn all the apis of this architecture. emva can virtualize all single chip microcomputer on the market and provide standard upper level api operation interface
## deploy
- open **emva_create_lib_keilv5** project
- build this project and you will get **emva_lib.lib** file
- create your project and add **emva_lib.lib** file to your project
- implements hal interface 
## implements 
Implement some standard implementations of the interface for your target platform
- hal_arch
```code
struct hal_arch bsp_arch = {
	.Initialize = 'implements your mcu init',
};
```
- hal_usart
```code
struct hal_usart bsp_usart = {
	.Initialize = 'implements init',
	.Write = 'implements write func',
	.Read = 'implements read func',
	.CallBack = 'implements call back func',
};
```
- hal_time
```code
struct hal_time bsp_time = {
	.Initialize = 'implements init',
	.Control = 'implements control ',
	.CallBack = 'implements call back func',
};
```
- hal_gpio 
```code
struct hal_time bsp_time = {
	.write.pin0 = NULL,
};
```
- hal_usb_hid 
```code
struct hal_usb_hid bsp_usb_hid = {
	.Initialize = 'implements',
	.Write = 'implements',
	.CallBack = 'implements',
};
```
- sleep implements
```code
#include "bsp_type.h"
#include "emva_os.h"
inline void os_sleep(uint32_t nms)
{
	// your code
}
inline void os_sleepus(uint32_t nus)
{
	// your code
}
```
- dv mem manage implements
```code
#include "bsp_type.h"
#include "bsp_configure.h"
#include "emva_mem_pool.h"
#include "dv_mem_pool.h"
uint8 eg_dv_mem_pool[DV_MEM_SIZE];
#if defined(__CC_ARM) || defined(__CLANG_ARM) /* ARMCC Complier */
__align(8) uint8 g_dv_membase[DV_MEM_SIZE];
#endif
uint16 g_dvmmt[DV_MMTS];
void dv_mem_pool_init(void)
{
    // your code
}
void *dv_mem_pool_malloc(uint32 size)
{
    // your code
}
uint8 dv_mem_pool_free(void *mem_pointer)
{
    // your code
}
```

- emva mem manage implements
```code
#include "bsp_type.h"
#include "bsp_configure.h"
#include "emva_mem_pool.h"
#include "dv_mem_pool.h"
uint8 eg_emva_mem_pool[EMVA_MEM_SIZE];
#if defined(__CC_ARM) || defined(__CLANG_ARM) /* ARMCC Complier */
__align(8) uint8 g_emva_membase[EMVA_MEM_SIZE];
#endif
uint16 g_emvammt[EMVA_MMTS];
void emva_mem_pool_init(void)
{
    // your code
}
void *emva_mem_pool_malloc(unsigned int size)
{
    // your code
}
uint8 emva_mem_pool_free(void *mem_pointer)
{
    // your code
}
```