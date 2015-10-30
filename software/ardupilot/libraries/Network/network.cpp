#include "network.h"

extern const AP_HAL::HAL& hal;

void Network::init(const AP_SerialManager& serial_manager){
	port = serial_manager.find_serial(AP_SerialManager::SerialProtocol_Network, 0);
#if HAL_NETWORK_DEFAULT == HAL_NETWORK_ESP8266
	driver = new Network_esp8266(*this, port);
#else
	driver = NULL;
#endif
}

void Network::update_send(void){
	driver->read_from_frontend();
}

void Network::update_read(void){
	driver->send_to_frontend();
}


void Network::send_data_to_buffer(uint8_t* data, size_t data_length){
	if(data_length <= SEND_BUFFER_SIZE){
		for(int i = 0; i < data_length; i++){
			send_buffer[i] = data[i];
		}
		send_buffer[data_length-1] = '\0';
	}
	else{
		hal.console->println("WARNING: Buffer send overflow, clipping data!");
		for(int i = 0; i < SEND_BUFFER_SIZE-1; i++){
			send_buffer[i] = data[i];
		}
		send_buffer[SEND_BUFFER_SIZE-1] = '\0';
	}
}

void Network::read_data_from_buffer(uint16_t* data, size_t data_length){
	if(data_length <= READ_BUFFER_SIZE){
		int i = 0;
		while(read_buffer[i] != '\0'){
			data[i] = read_buffer[i];
			i++;
		}
	}
	else{
		hal.console->println("WARNING: Buffer read overflow, clipping data!");
		for(int i = 0; i < data_length-1; i++){
			data[i] = read_buffer[i];
		}
		data[data_length-1] = '\0';
	}
}

void Network::print_data_to_console(){
	hal.console->println((const char*)this->read_buffer);
}