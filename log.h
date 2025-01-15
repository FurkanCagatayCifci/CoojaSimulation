#include "sys/energest.h"
#include <stdio.h>
#include <string.h>

#define LOG_BUFFER_SIZE 4096  // Size of log storage buffer
static char log_buffer[LOG_BUFFER_SIZE];  // Memory buffer for storing logs
static size_t log_index = 0;              // Current write index in buffer

void print_energest_data(void) {
    if (log_index >= LOG_BUFFER_SIZE - 200) {
        // Prevent buffer overflow
        return;
    }
    
    clock_time_t current_time = clock_time();
    unsigned long cpu_time = energest_type_time(ENERGEST_TYPE_CPU);
    unsigned long lpm_time = energest_type_time(ENERGEST_TYPE_LPM);
    unsigned long transmit_time = energest_type_time(ENERGEST_TYPE_TRANSMIT);
    unsigned long listen_time = energest_type_time(ENERGEST_TYPE_LISTEN);

    // Write log data into buffer
    log_index += snprintf(log_buffer + log_index, LOG_BUFFER_SIZE - log_index,
                          "Energest: time %lu CPU: %lu LPM: %lu Transmit: %lu Listen: %lu\n",
                          (unsigned long)current_time, cpu_time, lpm_time, transmit_time, listen_time);
}

// Function to retrieve the log buffer over serial or network
const char* get_log_buffer(void) {
    return log_buffer;
}

void print_log_via_serial(void) {
    printf("%s", get_log_buffer());
}