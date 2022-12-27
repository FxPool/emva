// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org
#include "string.h"
#include "dev_init.h"

static dev_init_func_t *init_list_begin;
static dev_init_func_t *init_list_end;

void DvModuleInitialize(void)
{
	dev_init_func_t *index;
	extern const int INIT_LIST$$Base;
	extern const int INIT_LIST$$Limit;
	init_list_begin = (dev_init_func_t *)&INIT_LIST$$Base;
	init_list_end = (dev_init_func_t *)&INIT_LIST$$Limit;

	for (index = init_list_begin; index < init_list_end; index++)
	{
		index->_init_func();
	}
}
