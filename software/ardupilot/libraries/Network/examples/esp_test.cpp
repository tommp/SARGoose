/*AT+RST (resets the module)
AT+CWMODE=3 (sets to both access point and device station mode)
AT+CWSAP="SARGoose","password",5,3 (Set access point config, 5=channel, 3=encryption WPA2)
AT+CIPMUX=1 (enable multiple connection)
AT+CIPSERVER=1,80 (Set as tcp server, 1=enable, 80=port)

To get the server ip issue:

AT+CIFSR

When info is recieved, it will be printed to the serial. Respond by issueing:

AT+CIPSEND=0,10 (0=id of sender, usually 0, should check, 10 = length of response!)

Then send response!

*/

#include <stdlib.h>

#include <AP_Common/AP_Common.h>
#include <AP_Progmem/AP_Progmem.h>
#include <AP_Param/AP_Param.h>
#include <AP_HAL/AP_HAL.h>
#include <AP_HAL_AVR/AP_HAL_AVR.h>
#include <AP_HAL_Empty/AP_HAL_Empty.h>
#include <StorageManager/StorageManager.h>
#include <AP_Math/AP_Math.h>
#include <AP_Scheduler/AP_Scheduler.h>
#include <AP_SerialManager/AP_SerialManager.h>

#include "../network.h"

const AP_HAL::HAL& hal = AP_HAL_BOARD_DRIVER;


// This example uses GPS system. Create it.
Network net;

// Serial manager is needed for UART comunications
AP_SerialManager serial_manager;

void setup()
{
    hal.console->println("ESP8266 library test");

    // Initialize the UART for GPS system
    serial_manager.init();
    net.init(serial_manager);
    hal.scheduler->delay(500);
}

void loop()
{
	
	net.update_send();
	hal.scheduler->delay(300);
	net.update_read();
	hal.scheduler->delay(300);
	net.print_data_to_console();

	hal.console->println("Running\n\n");
    // Delay for 10 mS will give us 100 Hz invocation rate
    hal.scheduler->delay(300);
}

// Register above functions in HAL board level
AP_HAL_MAIN();
