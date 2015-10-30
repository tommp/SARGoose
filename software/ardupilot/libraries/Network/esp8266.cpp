#include "esp8266.h"

extern const AP_HAL::HAL& hal;

void Network_esp8266::read_from_frontend(){
	port->println("AT\n");
}

void Network_esp8266::send_to_frontend(){
	uint16_t* data = frontend.get_read_buffer();
	int i = 0;
	while(port->available() && i < READ_BUFFER_SIZE - 1){
		data[i] = port->read();
		i++;
	}
	data[i-1] = '\0';
}

void Network_esp8266::send_reset_command(){
	port->write((const uint8_t*)RESET_COMMAND, 10);
}