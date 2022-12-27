// Copyright (c) 2021-2022 The Emva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.evirtualarch.org

#ifndef _EMVA_KFIFO_H
#define _EMVA_KFIFO_H

#ifdef __cplusplus
extern "C"
{
#endif
	struct emva_kfifo
	{
		byte *buffer;  /* the buffer holding the data */
		uint32 size; /* the size of the allocated buffer */
		uint32 in;   /* data is added at offset (in % size) */
		uint32 out;  /* data is extracted from off. (out % size) */
	};
	struct emva_kfifo *emva_kfifo_init(struct emva_kfifo *fifo, byte *buffer, uint32 size);

	/**
 * kfifo_put - puts some data into the FIFO
 * @fifo: the fifo to be used.
 * @buffer: the data to be added.
 * @len: the length of the data to be added.
 *
 * This function copies at most @len bytes from the @buffer into
 * the FIFO depending on the free space, and returns the number of
 * bytes copied.
 */
	uint32 emva_kfifo_put(struct emva_kfifo *fifo,
							const byte *buffer, uint32 len);

	/**
 * kfifo_get - gets some data from the FIFO
 * @fifo: the fifo to be used.
 * @buffer: where the data must be copied.
 * @len: the size of the destination buffer.
 *
 * This function copies at most @len bytes from the FIFO into the
 * @buffer and returns the number of copied bytes.
 */
	uint32 emva_kfifo_get(struct emva_kfifo *fifo,
							byte *buffer, uint32 len);

	/**
 * kfifo_reset - removes the entire FIFO contents
 * @fifo: the fifo to be emptied.
 */
	static void emva_kfifo_reset(struct emva_kfifo *fifo)
	{
		fifo->in = fifo->out = 0;
	}

	/**
 * kfifo_len - returns the number of bytes available in the FIFO.
 * @fifo: the fifo to be used.
 */
	static uint32 emva_kfifo_len(struct emva_kfifo *fifo)
	{
		return fifo->in - fifo->out;
	}

	void emva_kfifo_putc(struct emva_kfifo *fifo, byte c);
	int emva_kfifo_getc(struct emva_kfifo *fifo, byte *c);
#ifdef __cplusplus
}
#endif
#endif
