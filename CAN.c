#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define CAN_MAX_DATA_LENGTH 8
#define MAX_MESSAGES 100

typedef struct {
    uint32_t id : 11;          // 11-bit identifier
    uint8_t rtr : 1;           // Remote Transmission Request
    uint8_t ide : 1;           // Identifier Extension
    uint8_t reserved : 1;      // Reserved bit
} CAN_StandardFrame;

typedef struct {
    uint32_t id : 29;          // 29-bit identifier
    uint8_t rtr : 1;           // Remote Transmission Request
    uint8_t ide : 1;           // Identifier Extension
    uint8_t reserved : 1;      // Reserved bit
} CAN_ExtendedFrame;

typedef struct {
    uint8_t sof : 1;            // Start of Frame
    union {
        CAN_StandardFrame std;
        CAN_ExtendedFrame ext;
    } id;
    uint8_t dlc : 4;            // Data Length Code (0-8)
    uint8_t data[CAN_MAX_DATA_LENGTH]; // Data
    uint16_t crc : 15;          // CRC
    uint8_t ack : 2;            // ACK Slot + Delimiter
    uint8_t eof : 7;            // End of Frame
} CAN_Frame;

typedef struct {
    CAN_Frame frames[MAX_MESSAGES];
    int frame_count;
} CAN_Bus;

// Initializes the CAN bus
void init_CAN_Bus(CAN_Bus *bus) {
    bus->frame_count = 0;
}

// Encapsulates a CAN frame
void encap_CAN_Frame(CAN_Frame *frame, uint32_t id, uint8_t ide, uint8_t dlc, uint8_t *data) {
    frame->sof = 0; // Typically SOF is dominant (0)
    if (ide) {
        frame->id.ext.id = id;
        frame->id.ext.ide = 1;
    } else {
        frame->id.std.id = id;
        frame->id.std.ide = 0;
    }
    frame->dlc = dlc;
    memcpy(frame->data, data, dlc);
    frame->crc = 0; // CRC calculation would be implemented here
    frame->ack = 0; // Default ACK
    frame->eof = 0x7F; // Typically all bits recessive (1)
}

// Decapsulates a CAN frame
void decap_CAN_Frame(CAN_Frame *frame) {
    printf("CAN Frame - SOF: %u\n", frame->sof);
    if (frame->id.std.ide) {
        printf("Extended ID: %X\n", frame->id.ext.id);
    } else {
        printf("Standard ID: %X\n", frame->id.std.id);
    }
    printf("DLC: %u\n", frame->dlc);
    printf("Data: ");
    for (int i = 0; i < frame->dlc; i++) {
        printf("%02X ", frame->data[i]);
    }
    printf("\nCRC: %X\n", frame->crc);
    printf("ACK: %X\n", frame->ack);
    printf("EOF: %X\n", frame->eof);
}

// Sends a CAN frame
int send_CAN_Frame(CAN_Bus *bus, CAN_Frame *frame) {
    if (bus->frame_count < MAX_MESSAGES) {
        bus->frames[bus->frame_count++] = *frame;
        return 0; // Success
    }
    return -1; // Bus full
}

// Receives and prints all CAN frames on the bus
void receive_CAN_Frames(CAN_Bus *bus) {
    for (int i = 0; i < bus->frame_count; i++) {
        decap_CAN_Frame(&bus->frames[i]);
    }
}

int main() {
    CAN_Bus bus;
    init_CAN_Bus(&bus);

    // Create and send a CAN frame
    uint8_t data1[4] = {0xDE, 0xAD, 0xBE, 0xEF};
    CAN_Frame frame1;
    encap_CAN_Frame(&frame1, 0x123, 0, 4, data1);
    if (send_CAN_Frame(&bus, &frame1) == -1) {
        fprintf(stderr, "Error: CAN bus is full\n");
    }

    // Create and send another CAN frame
    uint8_t data2[2] = {0xCA, 0xFE};
    CAN_Frame frame2;
    encap_CAN_Frame(&frame2, 0x456, 0, 2, data2);
    if (send_CAN_Frame(&bus, &frame2) == -1) {
        fprintf(stderr, "Error: CAN bus is full\n");
    }

    // Receive frames
    receive_CAN_Frames(&bus);

    return 0;
}
