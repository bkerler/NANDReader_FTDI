# Summary
This section describes the format of geometry parameter.  
The general parameter template is **pages:addressing:main_size:spare_size** and fields are described in next section.

# Fields
## pages
Total number of pages, this should include:
* all pages defined by manufacturer
* all gaps between pages (non existing pages in continous range of pages)
* addressing gaps, when lower bits of addressing bytes are zero by specification

Applying the last hint will result longer read times and unnecessary data in the output file, but it will lead to reading all the data.

## addressing 
This field specify how address bytes will generated for page selection and how pages will be read.
There are six supported adressing modes "SPsimple3B", "SPsimple4B", "SPsimple5B", "LPsimple3B", "LPsimple4B", "LPsimple5B".
All generates the number of bytes they mention in the name, filling them from LSB to MSB.

### SPxxx modes
This is legacy small page addressing mode, it reads:
* main part of page, by commands Read0 (0x00) and Read1 (0x01) (to read both halves of the single page)
* spare part of page, by command Read2 (0x50)

### LPxxx modes
This is legacy large page addressing mode, it reads:
* main part of page, by commands Read0 (0x00) and ReadStart (0x30) with byte offset set to 0
* spare part of page, by command Read0 (0x00) and ReadStart (0x30) with byte offset set to size of main data

In this addressing mode, the first 16 bits of address (two first bytes) will be used as the byte address within the page and the higher bits will be used as the page number.

## main_size
The size of the main data in page, expressed in bytes. Usually high powers of 2, such as 512/1024/2048/4096/8192

## spare_size
The size of the spare data in page, expressed in bytes. Usually small powers of 2, such as 16/32/64/128

# Examples
Here will be some examples.