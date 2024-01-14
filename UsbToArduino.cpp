#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "libserialport/libserialport.h"

#include "output.h"

int main(int argc, char *argv[])
{
    struct sp_port *port;
    (void) argc; // unused
    //Put your port name here (Linux: /dev/ttyUSB0, /dev/ttyUSB1, etc. Windows: COM1, COM2, etc.)
    int result = sp_get_port_by_name("/dev/ttyUSB0", &port);
    
    if (result != SP_OK) {
        std::cerr << "Error on opening serial port " << std::endl;
        sp_free_port(port);
        return 1;
    }

    result = sp_open(port, SP_MODE_READ_WRITE);
    if (result != SP_OK) {
        std::cerr << "Error on opening serial port " << std::endl;
        sp_free_port(port);
        return 1;
    }

    // serial port settings
    sp_set_baudrate(port, 115200);
    sp_set_bits(port, 8);
    sp_set_parity(port, SP_PARITY_NONE);
    sp_set_stopbits(port, 1);
	int total_frames = sizeof(bytearr) / sizeof(bytearr[0]);
    int frame_byte_lenght = sizeof(bytearr[0]) / sizeof(unsigned char);
	printf("size %d %d\n",  total_frames, frame_byte_lenght);
    
    // Send data to serial port
	for (int i = 0; i < total_frames; ++i)
    {
		result = sp_blocking_write(port, bytearr[i], 864, 100000);
        if (argv[1] != NULL && strcmp(argv[1], "-v") == 0)
		    printf("result: %d\n", result);

		if (result != frame_byte_lenght)
        {
			std::cerr << "Error on sendig data to serial port." << std::endl;
			break;
		} else if (argv[1] != NULL && strcmp(argv[1], "-v") == 0)
        {
			std::cout << "Dati inviati con successo alla porta seriale " << i << std::endl;
		}
		usleep(5000);
	}

    // Close serial port
    sp_close(port);
    sp_free_port(port);

    return 0;
}

/// to run 
// take frames from video
// ffmpeg -i inputvideo.mp4 -vf "select=not(mod(n\,3))" -vsync vfr "images/output_%03d.png" 
// convert to byte array
// python3 convert.py
// compile and run
// g++ pro.cpp -lserialport
// ./a.out

// make sure to connect Arduino and wsl2 in powershell with usbipd attach --busid 2-1 --wsl Ubuntu-22.04