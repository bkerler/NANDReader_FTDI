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
