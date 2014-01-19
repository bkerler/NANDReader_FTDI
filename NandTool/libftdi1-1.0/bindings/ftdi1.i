/* File: ftdi1.i */

%module(docstring="Python interface to libftdi1") ftdi1
%feature("autodoc","1");
#ifdef DOXYGEN
%include "ftdi1_doc.i"
#endif

%{
#include "Python.h"

PyObject* convertString( const char *v, Py_ssize_t len )
{
#if PY_MAJOR_VERSION >= 3
  return PyBytes_FromStringAndSize(v, len);
#else
  return PyString_FromStringAndSize(v, len);
#endif
}
%}

%include <typemaps.i>
%include <cstring.i>

%typemap(in) unsigned char* = char*;

%immutable ftdi_version_info::version_str;
%immutable ftdi_version_info::snapshot_str;

%rename("%(strip:[ftdi_])s") "";

%newobject ftdi_new;
%typemap(newfree) struct ftdi_context *ftdi "ftdi_free($1);";
%delobject ftdi_free;

%define ftdi_usb_find_all_docstring
"usb_find_all(context, vendor, product) -> (return_code, devlist)"
%enddef
%feature("autodoc", ftdi_usb_find_all_docstring) ftdi_usb_find_all;
%typemap(in,numinputs=0) SWIGTYPE** OUTPUT ($*ltype temp) %{ $1 = &temp; %}
%typemap(argout) SWIGTYPE** OUTPUT %{ $result = SWIG_Python_AppendOutput($result, SWIG_NewPointerObj((void*)*$1,$*descriptor,0)); %}
%apply SWIGTYPE** OUTPUT { struct ftdi_device_list **devlist };
    int ftdi_usb_find_all(struct ftdi_context *ftdi, struct ftdi_device_list **devlist,
                          int vendor, int product);
%clear struct ftdi_device_list **devlist;

%define ftdi_usb_get_strings_docstring
"usb_get_strings(context, device) -> (return_code, manufacturer, description, serial)"
%enddef
%feature("autodoc", ftdi_usb_get_strings_docstring) ftdi_usb_get_strings;
%apply char *OUTPUT { char * manufacturer, char * description, char * serial };
%cstring_bounded_output( char * manufacturer, 256 );
%cstring_bounded_output( char * description, 256 );
%cstring_bounded_output( char * serial, 256 );
%typemap(default,noblock=1) int mnf_len, int desc_len, int serial_len { $1 = 256; }
    int ftdi_usb_get_strings(struct ftdi_context *ftdi, struct libusb_device *dev,
                             char * manufacturer, int mnf_len,
                             char * description, int desc_len,
                             char * serial, int serial_len);
%clear char * manufacturer, char * description, char * serial;
%clear int mnf_len, int desc_len, int serial_len;

%define ftdi_read_data_docstring
"read_data(context) -> (return_code, buf)"
%enddef
%feature("autodoc", ftdi_read_data_docstring) ftdi_read_data;
%typemap(in,numinputs=1) (unsigned char *buf, int size) %{ $2 = PyInt_AsLong($input);$1 = (unsigned char*)malloc($2*sizeof(char)); %}
%typemap(argout) (unsigned char *buf, int size) %{ if(result<0) $2=0; $result = SWIG_Python_AppendOutput($result, convertString((char*)$1, $2)); free($1); %}
    int ftdi_read_data(struct ftdi_context *ftdi, unsigned char *buf, int size);
%clear (unsigned char *buf, int size);

%apply int *OUTPUT { unsigned int *chunksize };
    int ftdi_read_data_get_chunksize(struct ftdi_context *ftdi, unsigned int *chunksize);
    int ftdi_write_data_get_chunksize(struct ftdi_context *ftdi, unsigned int *chunksize);
%clear unsigned int *chunksize;

%define ftdi_read_pins_docstring
"read_pins(context) -> (return_code, pins)"
%enddef
%feature("autodoc", ftdi_read_pins_docstring) ftdi_read_pins;
%apply char *OUTPUT { unsigned char *pins };
    int ftdi_read_pins(struct ftdi_context *ftdi, unsigned char *pins);
%clear unsigned char *pins;

%apply char *OUTPUT { unsigned char *latency };
    int ftdi_get_latency_timer(struct ftdi_context *ftdi, unsigned char *latency);
%clear unsigned char *latency;

%apply short *OUTPUT { unsigned short *status };
    int ftdi_poll_modem_status(struct ftdi_context *ftdi, unsigned short *status);
%clear unsigned short *status;

%apply int *OUTPUT { int* value };
    int ftdi_get_eeprom_value(struct ftdi_context *ftdi, enum ftdi_eeprom_value value_name, int* value);
%clear int* value;

%typemap(in,numinputs=1) (unsigned char *buf, int size) %{ $2 = PyInt_AsLong($input);$1 = (unsigned char*)malloc($2*sizeof(char)); %}
%typemap(argout) (unsigned char *buf, int size) %{ if(result<0) $2=0; $result = SWIG_Python_AppendOutput($result, convertString((char*)$1, $2)); free($1); %}
    int ftdi_get_eeprom_buf(struct ftdi_context *ftdi, unsigned char * buf, int size);
%clear (unsigned char *buf, int size);

%define ftdi_read_eeprom_location_docstring
"read_eeprom_location(context, eeprom_addr) -> (return_code, eeprom_val)"
%enddef
%feature("autodoc", ftdi_read_eeprom_location_docstring) ftdi_read_eeprom_location;
%apply short *OUTPUT { unsigned short *eeprom_val };
    int ftdi_read_eeprom_location (struct ftdi_context *ftdi, int eeprom_addr, unsigned short *eeprom_val);
%clear unsigned short *eeprom_val;

%define ftdi_read_eeprom_docstring
"read_eeprom(context) -> (return_code, eeprom)"
%enddef
%feature("autodoc", ftdi_read_eeprom_docstring) ftdi_read_eeprom;

%define ftdi_read_chipid_docstring
"read_pins(context) -> (return_code, chipid)"
%enddef
%feature("autodoc", ftdi_read_chipid_docstring) ftdi_read_chipid;
%apply int *OUTPUT { unsigned int *chipid };
    int ftdi_read_chipid(struct ftdi_context *ftdi, unsigned int *chipid);
%clear unsigned int *chipid;

%include ftdi.h
%{
#include <ftdi.h>
%}

%include ftdi_i.h
%{
#include <ftdi_i.h>
%}
