// Copyright (c) 2021-2022 The Eva Core developers
// Distributed under the xxx_xx_xxx software license, see the accompanying
// file COPYING or http://www.virtualarch.org

#include "emva.h"
#include "emva_serial.h"
#include "string.h"

int main(void)
{
    emvaIniaialize();
    EmvaSerial *ems = emva.SerialClass.new();
    ems->portName = 2;
    ems->parity = Parity_No;
    ems->baudRate = 115200;
    ems->dataBits = WordLength_8b;
    ems->stopBits = StopBits_1;
    ems->dtLength = 128;
    ems->open(ems);
    byte buf[20];
    int n;
    while (1)
    {
        ems->read(ems, buf, 20, &n);
        if (n > 0)
        {
            buf[n] = 0;
            ems->write(ems, (byte *)buf, strlen((char *)buf));
        }
    }
}
