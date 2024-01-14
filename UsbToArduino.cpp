#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "libserialport/libserialport.h"
#include <chrono>

#include "output.h"

int main(int argc, char *argv[])
{
    // auto start = std::chrono::high_resolution_clock::now();

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
		usleep(90000); //adjust this value to change the frame rate
        // auto current_time = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> elapsed = current_time - start;
        // double frame_rate = (i+1)*3 / elapsed.count();  // i+1 because i starts from 0, and 3 because we take 3 frames per second
        // std::cout << "Frame rate: " << frame_rate << " frames per second" << std::endl;
    }

    // auto stop = std::chrono::high_resolution_clock::now();

    // std::chrono::duration<double> elapsed = stop - start;
    // double frame_rate = total_frames / elapsed.count();
    // std::cout << "Total elapsed time: " << elapsed.count() << " seconds" << std::endl;
    // std::cout << "Average frame rate: " << frame_rate << " frames per second" << std::endl;


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