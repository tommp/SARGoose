/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/*
   Please contribute your ideas! See http://dev.ardupilot.com for details

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
  SerialManager allows defining the protocol and baud rates for the available
  serial ports and provides helper functions so objects (like a gimbal) can
  find which serial port they should use
 */

#ifndef _AP_SERIALMANAGER_
#define _AP_SERIALMANAGER_

#include <AP_Math/AP_Math.h>
#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include <GCS_MAVLink/GCS_MAVLink.h>

#define SERIALMANAGER_NUM_PORTS 5

 // console default baud rates and buffer sizes
#ifdef HAL_SERIAL0_BAUD_DEFAULT
# define AP_SERIALMANAGER_CONSOLE_BAUD          HAL_SERIAL0_BAUD_DEFAULT
#else
# define AP_SERIALMANAGER_CONSOLE_BAUD          115200
#endif
# define AP_SERIALMANAGER_CONSOLE_BUFSIZE_RX    128
# define AP_SERIALMANAGER_CONSOLE_BUFSIZE_TX    512

// mavlink default baud rates and buffer sizes
#define AP_SERIALMANAGER_MAVLINK_BAUD           57600
#define AP_SERIALMANAGER_MAVLINK_BUFSIZE_RX     128
#define AP_SERIALMANAGER_MAVLINK_BUFSIZE_TX     256

// mavlink default baud rates, use default buffer sizes
#define AP_SERIALMANAGER_FRSKY_DPORT_BAUD       9600
#define AP_SERIALMANAGER_FRSKY_SPORT_BAUD       57600
#define AP_SERIALMANAGER_FRSKY_BUFSIZE_RX       0
#define AP_SERIALMANAGER_FRSKY_BUFSIZE_TX       0

// GPS default baud rates and buffer sizes
// we need a 256 byte buffer for some GPS types (eg. UBLOX)
#define AP_SERIALMANAGER_GPS_BAUD               38400
#define AP_SERIALMANAGER_GPS_BUFSIZE_RX         256
#define AP_SERIALMANAGER_GPS_BUFSIZE_TX         16

/* ################################# Modified for RedningsGåsa */
// Network default baud rates and buffer sizes
#define AP_SERIALMANAGER_NETWORK_BAUD               115200
#define AP_SERIALMANAGER_NETWORK_BUFSIZE_RX         256
#define AP_SERIALMANAGER_NETWORK_BUFSIZE_TX         256
/* ########################################################### */

// AlexMos Gimbal protocol default baud rates and buffer sizes
#define AP_SERIALMANAGER_ALEXMOS_BAUD           115200
#define AP_SERIALMANAGER_ALEXMOS_BUFSIZE_RX     128
#define AP_SERIALMANAGER_ALEXMOS_BUFSIZE_TX     128

#define AP_SERIALMANAGER_SToRM32_BAUD           115200
#define AP_SERIALMANAGER_SToRM32_BUFSIZE_RX     128
#define AP_SERIALMANAGER_SToRM32_BUFSIZE_TX     128


class AP_SerialManager {

public:

    enum SerialProtocol {
        SerialProtocol_Console = 0,
        SerialProtocol_MAVLink = 1,
        SerialProtocol_MAVLink2 = 2,    // do not use - use MAVLink and provide instance of 1
        SerialProtocol_FRSky_DPort = 3,
        SerialProtocol_FRSky_SPort = 4,
        SerialProtocol_GPS = 5,
        SerialProtocol_GPS2 = 6,        // do not use - use GPS and provide instance of 1
        SerialProtocol_AlexMos = 7,
        SerialProtocol_SToRM32 = 8,
        /* Modified for RedningsGåsa */
        SerialProtocol_Network = 9,
        /* ######################### */
    };

    // Constructor
    AP_SerialManager();

    // init_console - initialise console at default baud rate
    void init_console();

    // init - initialise serial ports
    void init();

    // find_serial - searches available serial ports that allows the given protocol
    //  instance should be zero if searching for the first instance, 1 for the second, etc
    //  returns uart on success, NULL if a serial port cannot be found
    AP_HAL::UARTDriver *find_serial(enum SerialProtocol protocol, uint8_t instance) const;

    // find_baudrate - searches available serial ports for the first instance that allows the given protocol
    //  instance should be zero if searching for the first instance, 1 for the second, etc
    //  returns the baudrate of that protocol on success, 0 if a serial port cannot be found
    uint32_t find_baudrate(enum SerialProtocol protocol, uint8_t instance) const;

    // get_mavlink_channel - provides the mavlink channel associated with a given protocol (and instance)
    //  instance should be zero if searching for the first instance, 1 for the second, etc
    //  returns true if a channel is found, false if not
    bool get_mavlink_channel(enum SerialProtocol protocol, uint8_t instance, mavlink_channel_t &mav_chan) const;

    // set_blocking_writes_all - sets block_writes on or off for all serial channels
    void set_blocking_writes_all(bool blocking);

    // set_console_baud - sets the console's baud rate to the rate specified by the protocol
    //  used on APM2 to switch the console between the console baud rate (115200) and the SERIAL1 baud rate (user configurable)
    void set_console_baud(enum SerialProtocol protocol, uint8_t instance) const;

    // parameter var table
    static const struct AP_Param::GroupInfo var_info[];

private:

    // array of uart info
    struct {
        AP_Int8 protocol;
        AP_Int32 baud;
        AP_HAL::UARTDriver* uart;
    } state[SERIALMANAGER_NUM_PORTS];

    uint32_t map_baudrate(int32_t rate) const;

    // protocol_match - returns true if the protocols match
    bool protocol_match(enum SerialProtocol protocol1, enum SerialProtocol protocol2) const;
};

#endif // _AP_SERIALMANAGER_
