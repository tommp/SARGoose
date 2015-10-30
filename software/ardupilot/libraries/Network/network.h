#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include <inttypes.h>
#include <AP_SerialManager/AP_SerialManager.h>

#define READ_BUFFER_SIZE 255
#define SEND_BUFFER_SIZE 255

class Network_backend;

class Network{
	friend class Network_backend;
public:
	void init(const AP_SerialManager& serial_manager);
	void update_send(void);
	void update_read(void);
	void send_data_to_buffer(uint8_t* data, size_t data_length);
	void read_data_from_buffer(uint16_t* data, size_t data_length);
	uint16_t* get_read_buffer(){return read_buffer;};
	uint8_t* get_send_buffer(){return send_buffer;};
	void print_data_to_console();
private:
	Network_backend* driver;
	AP_HAL::UARTDriver *port; 

	uint16_t read_buffer[READ_BUFFER_SIZE];
	uint8_t send_buffer[SEND_BUFFER_SIZE];
};

#include "network_backend.h"
#include "esp8266.h"

#endif