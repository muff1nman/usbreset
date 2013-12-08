/*
 * usbreset.cpp
 * Copyright (C) 2013 Andrew DeMaria (muff1nman) <ademaria@mines.edu>
 *
 * All Rights Reserved.
 */

#include <libusb-1.0/libusb.h>
#include <iostream>
#include <error.h>

using namespace std;

#define MICROSOFT 1118

typedef bool (*DeviceFilter)(libusb_device*);

void print_version() {
	const struct libusb_version* v = libusb_get_version();
	cout << "Libusb version: " << v->major << "." << v->minor << endl;
}

bool matches_vendor_microsoft( libusb_device* dev ) {
	struct libusb_device_descriptor info;
	int ok = libusb_get_device_descriptor( dev, &info );
	if( ok < 0 ) {
		error(0,0,"Failed to get device descriptor");
		return false;
	}

	if( info.idVendor == MICROSOFT ) {
		cout << "Found microsoft device with product id [" << (int)info.idProduct << "]" << endl;
		return true;
	}

	return false;

}

void reset_device( libusb_device* dev ) {
	libusb_device_handle* handle;

	// open device
	int ok = libusb_open( dev, &handle );
	if( ok < 0 ) {
		error(0,0,"Could not open device");
	}

	// reset device
	ok = libusb_reset_device( handle );
	if( ok < 0 ) {
		error(0,0,"Could not reset device");
	}

	// close device
	libusb_close( handle );

}

int main( int argc, char** argv ) {
	// print the version just as a sanity check and to see if libusb is avail
	print_version();

	int ret_val;
	ssize_t num_devs;
	
	// Use a context so as to not conflict with anything else also happening to
	// use libusb
	libusb_context* context;
	ret_val = libusb_init( &context );
	if( ret_val != 0 ) {
		error(ret_val,0,"Could not create a usb context");
	}

	// get a list of devices
	libusb_device** devices;
	num_devs = libusb_get_device_list( context, &devices );
	if( num_devs < 0 ) {
		error(num_devs,0,"Could not enumerate devices");
	}

	cout << "Found " << num_devs << " devices" << endl;

	DeviceFilter filter = matches_vendor_microsoft;
	for( size_t i = 0; i < (size_t) num_devs; ++i ) {
		libusb_device* current = devices[i];
		if( filter(current) ) {
			cout << "Reseting device" << endl;
			reset_device( current );
		}
	}

	// clear the list and deallocate all references to devices
	libusb_free_device_list( devices, 1 );
	libusb_exit( context );
}


