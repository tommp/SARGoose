#include "esp8266.h"

extern const AP_HAL::HAL& hal;

Network_esp8266::Network_esp8266(Network& _net, AP_HAL::UARTDriver *_port) : Network_backend(_net, _port){
		port->println(SET_MODE_COMMAND);
		hal.scheduler->delay(1000);
		send_max_to_frontend();
		port->println(ENABLE_MULTIPLE_CONNECTION_COMMAND);
		hal.scheduler->delay(1000);
		send_max_to_frontend();
		port->println(ENABLE_TCP_SERVER_COMMAND);
		hal.scheduler->delay(1000);
		send_max_to_frontend();
		port->println(GET_IP_COMMAND);
		hal.scheduler->delay(1000);
		send_max_to_frontend();
	};

void Network_esp8266::read_from_frontend(){
	uint8_t* data = frontend.get_send_buffer();
	size_t data_length = strlen((const char*)data);
	if(data_length){
		send_send_data_command(data_length);
		hal.scheduler->delay(1000);

		delete_all_buffered_data();
		hal.scheduler->delay(1000);
		port->println((const char*)data);
		port->println('\n');
		hal.scheduler->delay(1000);
	}
	else{
		hal.console->println("No data to send...");
	}
}

void Network_esp8266::delete_all_buffered_data(){
	while(port->available()){
		port->read();
	}
}

void Network_esp8266::send_send_data_command(size_t data_length){
	port->print(SEND_DATA_COMMAND);
	port->println(data_length);
}

/* Reads all data currently waiting unless the buffer fills up, useful for clearing the uart buffers */
void Network_esp8266::send_max_to_frontend(){
	uint8_t* data = frontend.get_read_buffer();
	uint16_t i = 0;
	if(port->available()){
		while(port->available() && i < READ_BUFFER_SIZE-1){
			data[i] = port->read();
			i++;
		}

		while(i < READ_BUFFER_SIZE) {
			data[i] = '\0';
			i++;
		}
	}
}

/* Reads data from the uart buffer until a newline is encountered */
void Network_esp8266::send_line_to_frontend(){
	uint8_t* data = frontend.get_read_buffer();
	uint16_t i = 0;
	if(port->available()){
		while(port->available() && i < READ_BUFFER_SIZE-1 && data[i] != '\n'){
			data[i] = port->read();
			i++;
		}

		while(i < READ_BUFFER_SIZE) {
			data[i] = '\0';
			i++;
		}
	}
}

void Network_esp8266::send_reset_command(){
	port->println(RESET_COMMAND);
}