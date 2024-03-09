8-Bit Universal Nand reader for FTDI2323H Breakout boards

Based on http://spritesmods.com/?art=ftdinand code and ideas

Heavily modified to support writing nand, universally detecting and dumping nands
using Linux Kernel source code additions.

(c) 2014 B. Kerler <info AT revskills.de>

with nord-data-recall fixes/improvements:
1. Diagnostic function that allows to perform hardware test. It is available by using -d parameter.
   There are two ways to testing. The one is to execute all tests one by one, to accomplish that
   use only the -d parameter. Another way is to do specific single test adding number to -d parameter. 
   For example -d 15 to execute test of IORDY pin.
   
   Please be warned that FTDI chip may restart or react abnormally when pins are touched with bare hands.
   Because of this, tests may sometimes fails or the application may hang or fail with an error.
   In this case restarting the application should help as it re-initialize the FTDI chip.
   
   If using osciloscope or voltmeter ~2.0V might appear on the output pin before ~0.5Hz signal appears
   with amplitude 3.3V. It is caused by internal pull-up resistors in the FTDI chip which looks it they 
   do not pull up to the 3.3V rail.
   
2. Selecting specific FTDI device. Raise condition may occur when the FTDI device is plugged for first
   time and Channel B installed before Channel A. It may also happend when there are more FTDI 
   devices connected to computer. In this case application may connect to wrong device or channel B
   and further communication with NAND chip will fail. To select specific FTDI channel or device use -f
   parameter for example -f 1 to select device #1. To list FTDI devices use diagnostic function.
   
   Parameter also supported in diagnostic mode, but diagnostic mode connects independently to channels
   and by default assumes channel B is after channel A. If opening channel B fails, application will
   try also alternative order (channel B before channel A). There is no option to manually select specific
   FTDI device for channel B during diagnostic mode.