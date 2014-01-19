#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Python example program.

Small program to demonstrate the usage
of the swig generated python wrapper

You need to build and install the wrapper first"""

import ftdi1 as ftdi

def main():
    """Main program"""
    context = ftdi.new()

    version_info = ftdi.get_library_version()
    print("[FTDI version] major: %d, minor: %d, micro: %d" \
               ", version_str: %s, snapshot_str: %s" %
               (version_info.major, version_info.minor, version_info.micro,
               version_info.version_str, version_info.snapshot_str))

    # try to open an ftdi 0x6010 or 0x6001
    ret = ftdi.usb_open(context, 0x0403, 0x6010)
    if ret < 0:
        ret = ftdi.usb_open(context, 0x0403, 0x6001)
        
    print("ftdi.usb_open(): %d" % ret)
    print("ftdi.set_baudrate(): %d" % ftdi.set_baudrate(context, 9600))

    ftdi.free(context)

main()
