// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the MIT license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#include "emva.h"
#include "emva_kfifo.h"
#include <errno.h>
#include <string.h>
#if EMVA_FM_KFIFO_ENABLED == 1
static uint32 min(uint32 x, uint32 y)
{
	return x < y ? x : y;
}
struct emva_kfifo *emva_kfifo_init(struct emva_kfifo *fifo, byte *buffer, uint32 size)
{
	fifo->buffer = buffer;
	fifo->size = size;
	fifo->in = fifo->out = 0;

	return fifo;
}

/**
 * kfifo_put - puts some data into the FIFO, no locking version
 * @fifo: the fifo to be used.
 * @buffer: the data to be added.
 * @len: the length of the data to be added.
 *
 * This function copies at most @len bytes from the @buffer into
 * the FIFO depending on the free space, and returns the number of
 * bytes copied.
 *
 */
uint32 emva_kfifo_put(struct emva_kfifo *fifo,
					  const byte *buffer, uint32 len)
{
	uint32 l;

	len = min(len, fifo->size - fifo->in + fifo->out);

	/* first put the data starting from fifo->in to buffer end */
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));
	memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);

	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(fifo->buffer, buffer + l, len - l);

	fifo->in += len;

	return len;
}

/**
 * kfifo_get - gets some data from the FIFO, no locking version
 * @fifo: the fifo to be used.
 * @buffer: where the data must be copied.
 * @len: the size of the destination buffer.
 *
 * This function copies at most @len bytes from the FIFO into the
 * @buffer and returns the number of copied bytes.
 *
 * Note that with only one concurrent reader and one concurrent
 * writer, you don't need extra locking to use these functions.
 */
uint32 emva_kfifo_get(struct emva_kfifo *fifo,
					  byte *buffer, uint32 len)
{
	uint32 l;

	len = min(len, fifo->in - fifo->out);

	/* first get the data from fifo->out until the end of the buffer */
	l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
	memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);

	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(buffer + l, fifo->buffer, len - l);

	fifo->out += len;

	return len;
}

void emva_kfifo_putc(struct emva_kfifo *fifo, byte c)
{
	*(fifo->buffer + (fifo->in & (fifo->size - 1))) = c;
	fifo->in++;
	if (fifo->in - fifo->out > fifo->size)
		fifo->out++;
}

int emva_kfifo_getc(struct emva_kfifo *fifo, byte *c)
{
	if (fifo->in == fifo->out)
		return -1;

	*c = *(fifo->buffer + (fifo->out & (fifo->size - 1)));
	fifo->out++;

	return 0;
}
#endif
