#!/usr/bin/env python

from smbus2 import SMBusWrapper

for i in range(256):
    try:
        with SMBusWrapper(7) as bus:
           data = 45 
           bus.write_byte_data(0x57, i, data)
    except IOError:
        pass
