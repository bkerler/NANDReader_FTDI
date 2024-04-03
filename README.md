Universal Nand reader for 8 bit NAND memories using FTDI FT2223H.

Based on http://spritesmods.com/?art=ftdinand code and ideas.

Heavily modified to support universally detecting and dumping nands
using Linux Kernel source code additions.

> [!IMPORTANT]
> **Write support disabled**

> [!WARNING]
> Read [**Observed**](Observed/Observed.md) about observed anomalies.

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
   
3. Nice progress indicator showing blocks, percentages and throughput. Progress indicator
   is refreshed every 0.5s to speed up performace and reduce terminal blinking.
   
4. Selecting the range of pages. It is possible to select single page or continous range of pages for read
   and verify operation (technically also for write, if it will be fully implemented). To choose one page
   user parameter -p page_number, to select range -p first_page:last_page. If the selected last_page is
   greater than number of pages in NAND memory, application will reduce the range with warning. If the
   selected last_page is -1 application will silenty reduce range. Using exact same page number as
   first_page and last_page will result in one page being read.
   
5. Handling communication in smaller chunks with configurable size. On some computers the FTDI chip has
   tendency to restart upon reception of large burst of data. From application perspective this could be
   observed as random hang or return with code 1167 during FT_Write. Suspected root cause is USB
   communication errors when sending large continuous burst. The -c parameter gives ability to configure
   size of single chunk. For example the application to read the page size 2048 bytes, sends 4098 bytes
   (1+2x2048+1), then reads 2048 bytes (FTDI@60MHz). When small chunks are used, application will perform
   many smaller write and read operations per page. For example -c 8, will result in transferring page in
   8 byte chunks, which means sending 18 bytes (1+2x8+1), then receiving 8 bytes, and repeating to get
   entire page. Using very small chunks brings a large throughtput penalty, but allows the application to
   be used on some computers. On some computers using chunks size 256 bytes resulted in a 5% increase in
   overal througput. Using chunks larger than the page size does no effect, no more than one page can be
   transferred in one chunk.
   
6. Printing raw ID bytes read from NAND memory.
   
7. Added support for ONFI data. The ONFI section can be read to a file and verified with the file using
   legacy commands -r and -v with selecting region onfi (option -t onfi). Also can be presented in
   human-readable form using the -i (identify chip) command. Alternatively an existing file containing
   ONFI information can be presented in human-readable form, even NAND Reader, using using command
   -show_onfi filename.
   
8. Option to select NAND chip geometry from command line. If the ID read from the NAND memory does not
   match with none of hardcoded list, application does not know how big is memory or how to address
   memory. Sometimes an ID from hardcoded list matches a NAND memory with a completly different geometry.
   To handle such cases, the -g parameter was introduced requiring an argument in format:
   pages:addressing:page_mode:main_size:spare_size, where:  
   *pages* is the total number of pages  
   *addressing* is the addressing format, for example "LPsimple5B"  
   *main_size* is the size of main part of page (in bytes)  
   *spare_size* is the size of spare parta of page (in bytes)  
   Read [**Geometry**](Geometry.md) how to use it 
   
9. Printing information about the process duration in days, hours, minutes and seconds.
   