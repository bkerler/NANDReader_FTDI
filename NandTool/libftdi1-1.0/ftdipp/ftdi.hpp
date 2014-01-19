/***************************************************************************
                          ftdi.hpp  -  C++ wrapper for libftdi
                             -------------------
    begin                : Mon Oct 13 2008
    copyright            : (C) 2008-2013 by Marek Vavruša and libftdi developers
    email                : opensource@intra2net.com and marek@vavrusa.com
 ***************************************************************************/
/*
Copyright (C) 2008-2013 by Marek Vavruša and libftdi developers

The software in this package is distributed under the GNU General
Public License version 2 (with a special exception described below).

A copy of GNU General Public License (GPL) is included in this distribution,
in the file COPYING.GPL.

As a special exception, if other files instantiate templates or use macros
or inline functions from this file, or you compile this file and link it
with other works to produce a work based on this file, this file
does not by itself cause the resulting work to be covered
by the GNU General Public License.

However the source code for this file must still be made available
in accordance with section (3) of the GNU General Public License.

This exception does not invalidate any other reasons why a work based
on this file might be covered by the GNU General Public License.
*/
#ifndef __libftdi_hpp__
#define __libftdi_hpp__

#include <list>
#include <string>
#include <boost/shared_ptr.hpp>
#include <ftdi.h>

namespace Ftdi
{

/* Forward declarations*/
class List;
class Eeprom;

/*! \brief FTDI device context.
 * Represents single FTDI device context.
 */
class Context
{
    /* Friends */
    friend class Eeprom;
    friend class List;

public:
    /*! \brief Direction flags for flush().
     */
    enum Direction
    {
        Input,
        Output
    };

    /*! \brief Modem control flags.
     */
    enum ModemCtl
    {
        Dtr,
        Rts
    };

    /* Constructor, Destructor */
    Context();
    ~Context();

    /* Properties */
    Eeprom* eeprom();
    const std::string& vendor();
    const std::string& description();
    const std::string& serial();

    /* Device manipulators */
    bool is_open();
    int open(struct libusb_device *dev = 0);
    int open(int vendor, int product);
    int open(int vendor, int product, const std::string& description, const std::string& serial = std::string(), unsigned int index=0);
    int open(const std::string& description);
    int close();
    int reset();
    int flush(int mask = Input|Output);
    int set_interface(enum ftdi_interface interface);
    void set_usb_device(struct libusb_device_handle *dev);

    /* Line manipulators */
    int set_baud_rate(int baudrate);
    int set_line_property(enum ftdi_bits_type bits, enum ftdi_stopbits_type sbit, enum ftdi_parity_type parity);
    int set_line_property(enum ftdi_bits_type bits, enum ftdi_stopbits_type sbit, enum ftdi_parity_type parity, enum ftdi_break_type break_type);

    /* I/O */
    int read(unsigned char *buf, int size);
    int write(unsigned char *buf, int size);
    int set_read_chunk_size(unsigned int chunksize);
    int set_write_chunk_size(unsigned int chunksize);
    int read_chunk_size();
    int write_chunk_size();

    /* Async IO
    TODO: should wrap?
    int writeAsync(unsigned char *buf, int size);
    void asyncComplete(int wait_for_more);
    */

    /* Flow control */
    int set_event_char(unsigned char eventch, unsigned char enable);
    int set_error_char(unsigned char errorch, unsigned char enable);
    int set_flow_control(int flowctrl);
    int set_modem_control(int mask = Dtr|Rts);
    int set_latency(unsigned char latency);
    int set_dtr(bool state);
    int set_rts(bool state);

    unsigned short poll_modem_status();
    unsigned latency();

    /* BitBang mode */
    int set_bitmode(unsigned char bitmask, unsigned char mode);
    int set_bitmode(unsigned char bitmask, enum ftdi_mpsse_mode mode);
    int bitbang_disable();
    int read_pins(unsigned char *pins);

    /* Misc */
    char* error_string();

protected:
    int get_strings();
    int get_strings_and_reopen();

    /* Properties */
    struct ftdi_context* context();
    void set_context(struct ftdi_context* context);
    void set_usb_device(struct libusb_device *dev);

private:
    class Private;
    boost::shared_ptr<Private> d;
};

/*! \brief Device EEPROM.
 */
class Eeprom
{
public:
    Eeprom(Context* parent);
    ~Eeprom();

    int init_defaults(char *manufacturer, char* product, char * serial);
    int chip_id(unsigned int *chipid);
    int build(unsigned char *output);

    int read(unsigned char *eeprom);
    int write(unsigned char *eeprom);
    int read_location(int eeprom_addr, unsigned short *eeprom_val);
    int write_location(int eeprom_addr, unsigned short eeprom_val);
    int erase();

private:
    class Private;
    boost::shared_ptr<Private> d;
};

/*! \brief Device list.
 */
class List
{
public:
    List(struct ftdi_device_list* devlist = 0);
    ~List();

    static List* find_all(Context &context, int vendor, int product);

    /// List type storing "Context" objects
    typedef std::list<Context> ListType;
    /// Iterator type for the container
    typedef ListType::iterator iterator;
    /// Const iterator type for the container
    typedef ListType::const_iterator const_iterator;
    /// Reverse iterator type for the container
    typedef ListType::reverse_iterator reverse_iterator;
    /// Const reverse iterator type for the container
    typedef ListType::const_reverse_iterator const_reverse_iterator;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    ListType::size_type size() const;
    bool empty() const;
    void clear();

    void push_back(const Context& element);
    void push_front(const Context& element);

    iterator erase(iterator pos);
    iterator erase(iterator beg, iterator end);

private:
    class Private;
    boost::shared_ptr<Private> d;
};

}

#endif
