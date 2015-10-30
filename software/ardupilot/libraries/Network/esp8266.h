#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "network.h"

#define RESET_COMMAND							"AT+RST\n\r"
#define RESET_COMMAND_SIZE						10
#define SET_MODE_COMMAND						"AT+CWMODE=3"
#define ACCESS_POINT_CONFIG_COMMAND 			"AT+CWSAP="
#define ACCESS_POINT_NAME						"SARGoose"
#define ACCESS_POINT_PASSWORD					"flythedistance"
#define ACCESS_POINT_CHANNEL			 		"5"
#define ACCESS_POINT_ENCRYPTION 				"3\n\r"
#define ENABLE_MULTIPLE_CONNECTION_COMMAND 		"AT+CIPMUX=1\n\r"
#define ENABLE_TCP_SERVER_COMMAND				"AT+CIPSERVER=1,80\n\r"

#define GET_IP_COMMAND							AT+CIFSR\n\r

#define SEND_DATA_COMMAND						AT+CIPSEND=0,10\r\n

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

class Network_esp8266 : public Network_backend{
public:
	Network_esp8266(Network& _net, AP_HAL::UARTDriver *_port) : Network_backend(_net, _port){};
	void read_from_frontend();
	void send_to_frontend();

private:
	uint8_t* reset_command;
	void send_reset_command();
};

#endif