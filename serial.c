#include "contiki.h"
#include <dev/serial-line.h>
#include <stdio.h>
#include <string.h>

#ifndef SERIAL_MESSAGE_LEN
#define SERIAL_MESSAGE_LEN 1024
#endif

void serial_serve() {
    char received_string[SERIAL_MESSAGE_LEN];
    unsigned char input_char;
    int string_pos = 0;

    // Continuously read bytes from the serial input
    while (serial_line_input_byte(&input_char) > 0) {
        // Handle end-of-line characters
        if (input_char == '\n' || input_char == '\r') {
            received_string[string_pos] = '\0'; // Null-terminate the string

            // Print the received string
            printf("Received string: %s\n", received_string);

            // Optionally, send the string back to the serial port
            printf("Sending back: %s\n", received_string);

            // Reset the position for the next string
            string_pos = 0;

            // Handle the received command
            if (strcmp(received_string, "read_log") == 0) {
                // Call your log-reading function here
                read_log_file();
            }
            else {
                printf("Unknown command: %s\n", received_string);
            }
        } else {
            // If not a newline, add the byte to the string (if there's space)
            if (string_pos < SERIAL_MESSAGE_LEN - 1) {
                received_string[string_pos++] = input_char;
            }
        }
    }
}
