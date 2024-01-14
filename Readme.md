# USB to ESP32 Data Streaming

This project is designed to stream data, specifically images or video frames, from a computer to an ESP32 device over a USB serial port connection. 

## Requirements

- C++ Compiler (g++)
- Python 3
- FFmpeg
- libserialport library
- Pillow library for Python

## Usage

1. Extract frames from a video using FFmpeg:

    Rename the video file in inputvideo.mp4

    ```bash
    ffmpeg -i inputvideo.mp4 -vf "select=not(mod(n\,3))" -vsync vfr "images/output_%03d.png"
    ```

2. Convert the frames to byte arrays using the Python script:

    ```bash
    python3 Img2ByteArray.py
    ```

3. Compile the C++ program:

    ```bash
    make
    ```

4. Run the program:

    ```bash
    ./send
    ```
    OR for verbose
    ```bash
    ./send - v
    ```


## How It Works

The C++ program opens a serial port connection to the ESP32 device, sends data over the connection, and then closes the connection. The Python script converts images to byte arrays that can be sent over the serial port connection.

## Note

This project assumes that you have an ESP32 device connected to your computer via USB. If your setup is different, you may need to modify the code.