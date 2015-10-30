#ifndef __NETWORK_BACKEND_H__
#define __NETWORK_BACKEND_H__

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include <inttypes.h>
#include "network.h"

class Network_backend{
public:
	Network_backend(Network& _net, AP_HAL::UARTDriver *_port) : frontend(_net), port(_port){};
	virtual void read_from_frontend()=0;
	virtual void send_to_frontend()=0;
protected:
	Network& frontend;
	AP_HAL::UARTDriver* port;
};


#endif