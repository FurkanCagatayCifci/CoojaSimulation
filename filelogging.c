#include "project-conf.h"
#include "sys/energest.h"
#include <cfs/cfs.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#ifndef LOGFILENAME
#define LOGFILENAME "log.txt"
#endif
#ifndef LOG_BUFFER_SIZE
#define LOG_BUFFER_SIZE 1024 // Size of log storage buffer
#endif
int log_file_length = 0;
char log_buffer[LOG_BUFFER_SIZE]; // Memory buffer for storing logs

void log_energest_to_file(void)
{
    int log_file_fd;
    float energy_tx, energy_rx, energy_cpu, energy_lpm, energy_deeplpm;

    uint32_t cpu_time = (uint32_t)energest_type_time(ENERGEST_TYPE_CPU);
    uint32_t lpm_time = (uint32_t)energest_type_time(ENERGEST_TYPE_LPM);
    uint32_t deeplpm_time = (uint32_t)energest_type_time(ENERGEST_TYPE_DEEP_LPM);
    uint32_t tx_time = (uint32_t)energest_type_time(ENERGEST_TYPE_TRANSMIT);
    uint32_t rx_time = (uint32_t)energest_type_time(ENERGEST_TYPE_LISTEN);

    // Convert time to milliseconds
    uint32_t cpu_time_ms = cpu_time * 1000 / CLOCK_SECOND;
    uint32_t lpm_time_ms = lpm_time * 1000 / CLOCK_SECOND;
    uint32_t deeplpm_time_ms = deeplpm_time * 1000 / CLOCK_SECOND;
    uint32_t tx_time_ms = tx_time * 1000 / CLOCK_SECOND;
    uint32_t rx_time_ms = rx_time * 1000 / CLOCK_SECOND;

    // Power calculation for each component (TX, RX, CPU, LPM)
    // TX Power is typically higher during transmission
    float power_tx = V_TX * I_TX;
    float power_rx = V_RX * I_RX;
    float power_cpu = V_CPU * I_CPU;

    // Calculate energy for each component (in joules)
    energy_tx = (float)tx_time / CLOCK_SECOND * power_tx;                  // Energy for TX
    energy_rx = (float)rx_time / CLOCK_SECOND * power_rx;                  // Energy for RX
    energy_cpu = (float)cpu_time / CLOCK_SECOND * power_cpu;               // Energy for CPU
    energy_lpm = (float)lpm_time / CLOCK_SECOND * (V_CPU * 0.002);         // Energy for LPM (low power mode)
    energy_deeplpm = (float)deeplpm_time / CLOCK_SECOND * (V_CPU * 0.001); // Energy for Deep LPM

    // Open the log file for appending
    log_file_fd = cfs_open(LOGFILENAME, CFS_WRITE | CFS_APPEND);
    if (log_file_fd < 0)
    {
        printf("Failed to open file: %s\n", LOGFILENAME);
        return;
    }
    else
    {
        
        uint32_t timestamp = clock_seconds();
        snprintf(log_buffer, sizeof(log_buffer),
                 "[Timestamp: %" PRIu32 "] "
                 "CPU time: %" PRIu32 " ms, LPM time: %" PRIu32 " ms, Deep LPM time: %" PRIu32 " ms, "
                 "Tx time: %" PRIu32 " ms, Rx time: %" PRIu32 " ms, "
                 "Energy (TX): %.4f J, Energy (RX): %.4f J, Energy (CPU): %.4f J, "
                 "Energy (LPM): %.4f J, Energy (Deep LPM): %.4f J\n",
                 timestamp, cpu_time_ms, lpm_time_ms, deeplpm_time_ms, tx_time_ms, rx_time_ms,
                 energy_tx, energy_rx, energy_cpu, energy_lpm, energy_deeplpm);
        size_t data_size = strlen(log_buffer); // or some other way to get the size of data to write
        if (data_size > sizeof(log_buffer))
        {
            // Handle truncation case, possibly by truncating the data or reporting an error
            data_size = sizeof(log_buffer); // Avoid truncation by limiting data size
        }
        if (cfs_write(log_file_fd, log_buffer, strlen(log_buffer)) < 0)
        {
            printf("Error: Failed to write to file '%s'\n", LOGFILENAME);
        }
        cfs_close(log_file_fd);
    }
}

void read_log_file()
{
    int fd = cfs_open(LOGFILENAME, CFS_READ);
    if (fd >= 0)
    {
        char buffer[4096];
        int bytes_read;

        printf("Reading log file contents:\n");
        while ((bytes_read = cfs_read(fd, buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytes_read] = '\0'; // Null-terminate the string
            printf("%s\n", buffer);    // Print to serial port
        }
        printf("\n");
        cfs_close(fd);
    }
    else
    {
        printf("Error opening log file for reading.\n");
    }
}
