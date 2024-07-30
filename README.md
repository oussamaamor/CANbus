# CANbus
CAN implementation 
# CAN Bus Project

This project demonstrates a simple implementation of a CAN bus system in C, showcasing the encapsulation and decapsulation of CAN frames. It includes functionality for sending and receiving CAN messages using a structured approach with bit-fields for precise control over the bit sizes of each field in the CAN frame.

## Project Structure

- **main.c**: The main C source file containing the implementation of the CAN bus system.

## Features

- **Initialization**: Initializes the CAN bus system.
- **Encapsulation**: Encapsulates CAN frames with specified identifiers, data length codes, and data payloads.
- **Decapsulation**: Decapsulates and prints the details of received CAN frames.
- **Sending Messages**: Sends CAN frames to the bus.
- **Receiving Messages**: Receives and prints all CAN frames on the bus.

## CAN Frame Structure

The CAN frame structure implemented in this project includes:
- Start of Frame (SOF)
- Identifier (11 bits for standard frame, 29 bits for extended frame)
- Remote Transmission Request (RTR)
- Identifier Extension (IDE)
- Data Length Code (DLC)
- Data (up to 8 bytes)
- Cyclic Redundancy Check (CRC)
- Acknowledgment (ACK)
- End of Frame (EOF)
