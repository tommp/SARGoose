#include "network.h"

extern const AP_HAL::HAL& hal;

void Network::init(const AP_SerialManager& serial_manager){
	port = serial_manager.find_serial(AP_SerialManager::SerialProtocol_Network, 0);
#if HAL_NETWORK_DEFAULT == HAL_NETWORK_ESP8266
	driver = new Network_esp8266(*this, port);
#else
	driver = NULL;
#endif
	memset (read_buffer,'\0',READ_BUFFER_SIZE);
	memset (send_buffer,'\0',SEND_BUFFER_SIZE);
}

void Network::update_send(void){
	driver->read_from_frontend();
	memset (send_buffer,'\0',SEND_BUFFER_SIZE);
}

void Network::update_read(void){
	driver->send_line_to_frontend();
}


void Network::send_data_to_buffer(uint8_t* data, size_t data_length){
	if(data_length < SEND_BUFFER_SIZE){
		for(uint16_t i = 0; i < data_length; i++){
			send_buffer[i] = data[i];
		}
		for (int i = data_length; i < SEND_BUFFER_SIZE; i++){
			send_buffer[i] = '\0';
		}
	}
	else{
		hal.console->println("WARNING: Buffer send overflow, clipping data!");
		for(uint16_t i = 0; i < SEND_BUFFER_SIZE; i++){
			send_buffer[i] = data[i];
		}
		send_buffer[SEND_BUFFER_SIZE-1] = '\0';
	}
}

void Network::read_data_from_buffer(uint8_t* data, size_t data_length){
	if(data_length <= READ_BUFFER_SIZE){
		uint16_t num_bytes = sizeof(read_buffer);
		for(uint16_t i = 0; i < num_bytes; i++){
			data[i] = read_buffer[i];
		}
	}
	else{
		hal.console->println("WARNING: Buffer read overflow, clipping data!");
		for(uint16_t i = 0; i < data_length-1; i++){
			data[i] = read_buffer[i];
		}
		data[READ_BUFFER_SIZE-1] = '\0';
	}
	memset(read_buffer,'\0',READ_BUFFER_SIZE);
}

void Network::print_data_to_console(){
	hal.console->print("Network print to console: ");
	hal.console->write(read_buffer, sizeof(read_buffer));
	hal.console->println('\n');
	memset(read_buffer,'\0',READ_BUFFER_SIZE);
}