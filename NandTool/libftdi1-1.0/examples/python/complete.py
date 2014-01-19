#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Python example program.

Complete program to demonstrate the usage
of the swig generated python wrapper

You need to build and install the wrapper first"""

import os
import sys
import ftdi1 as ftdi
import time

# initialize
ftdic = ftdi.new()
if ftdic == 0:
    print( 'new failed: %d', ret )
    os._exit( 1 )

# try to list ftdi devices 0x6010 or 0x6001
ret, devlist = ftdi.usb_find_all( ftdic, 0x0403, 0x6010 )
if ret <= 0:
    ret, devlist = ftdi.usb_find_all( ftdic, 0x0403, 0x6001)

if ret < 0:
    print( 'ftdi_usb_find_all failed: %d (%s)' % ( ret, ftdi.get_error_string( ftdic ) ) )
    os._exit( 1 )
print( 'Number of FTDI devices found: %d\n' % ret )
curnode = devlist
i = 0
while( curnode != None ):
    ret, manufacturer, description, serial = ftdi.usb_get_strings( ftdic, curnode.dev )
    if ret < 0:
        print( 'ftdi_usb_get_strings failed: %d (%s)' % ( ret, ftdi.get_error_string( ftdic ) ) )
        os._exit( 1 )
    print( 'Device #%d: manufacturer="%s" description="%s" serial="%s"\n' % ( i, manufacturer, description, serial ) )
    curnode = curnode.next
    i += 1

# open usb
ret = ftdi.usb_open( ftdic, 0x0403, 0x6001 )
if ret < 0:
    print( 'unable to open ftdi device: %d (%s)' % ( ret, ftdi.get_error_string( ftdic ) ) )
    os._exit( 1 )


# bitbang
ret = ftdi.set_bitmode( ftdic, 0xff, ftdi.BITMODE_BITBANG )
if ret < 0:
    print( 'Cannot enable bitbang' )
    os._exit( 1 )
print( 'turning everything on' )
ftdi.write_data( ftdic, chr(0xff), 1 )
time.sleep( 1 )
print( 'turning everything off\n' )
ftdi.write_data( ftdic, chr(0x00), 1 )
time.sleep( 1 )
for i in range( 8 ):
    val = 2**i
    print( 'enabling bit #%d (0x%02x)' % (i, val) )
    ftdi.write_data( ftdic, chr(val), 1 )
    time.sleep ( 1 )
ftdi.disable_bitbang( ftdic )
print( '' )


# read pins
# FIXME: read_pins fails with python3, so I disabled it for now
# tested on ubuntu 12.04 ( python3.2.3 / swig 2.0.4 )
if (sys.version_info[0]<3):
    ret, pins = ftdi.read_pins( ftdic )
    if ( ret == 0 ):
        print( 'pins: %02x' % ord( pins[0] ) )
              

# read chip id
ret, chipid = ftdi.read_chipid( ftdic )
if (ret==0):
    print( 'chip id: %X\n' % chipid )


# read eeprom
eeprom_addr = 1
ret, eeprom_val = ftdi.read_eeprom_location( ftdic, eeprom_addr )
if (ret==0):
    print( 'eeprom @ %d: 0x%04x\n' % ( eeprom_addr, eeprom_val ) )

print( 'eeprom:' )
ret = ftdi.read_eeprom( ftdic )
size = 128
ret, eeprom = ftdi.get_eeprom_buf ( ftdic, size )
if ( ret == 0 ):
    for i in range( size ):
        if isinstance(eeprom[i], str):
            octet = ord( eeprom[i] ) # python2
        else:
            octet = eeprom[i] # python3
        sys.stdout.write( '%02x ' % octet )
        if ( i % 8 == 7 ):
            print( '' )
print( '' )

            
# close usb
ret = ftdi.usb_close( ftdic )
if ret < 0:
    print( 'unable to close ftdi device: %d (%s)' % ( ret, ftdi.get_error_string( ftdic ) ) )
    os._exit( 1 )
    
print ('device closed')    
ftdi.free( ftdic )
