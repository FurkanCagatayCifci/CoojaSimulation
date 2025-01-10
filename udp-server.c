#include "project-conf.h"
#define LOG_INTERVAL (3 * CLOCK_SECOND)
#define LOGFILENAME "UDP_CLIENT_ENERGEST.txt"
#define LOGNAME "energest_log_client.txt"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO
#define WITH_SERVER_REPLY  1
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678
#include "filelogging.c"
#include "serial.c"
#include <contiki.h>
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "sys/log.h"
#include "sys/energest.h"
#include "encryption.c"

static struct simple_udp_connection udp_conn;
static void udp_rx_callback
(
          struct simple_udp_connection *c,
          const uip_ipaddr_t *sender_addr,
          uint16_t sender_port,
          const uip_ipaddr_t *receiver_addr,
          uint16_t receiver_port,
          const uint8_t *data,
          uint16_t datalen
)
{
    char decrypted[datalen + 1];
    printf("\n");
    LOG_INFO("Received Data (%8d length): ",datalen);
    for (uint16_t i = 0; i < datalen; i++)
    {
        LOG_INFO_("%02x ",(uint8_t) data[i]);
    }
    printf("\n");
    decrypt(data,datalen,key);
    for (uint16_t i = 0; i < datalen; i++)
    {
        LOG_INFO_("%02x ",(uint8_t) data[i]);
    }
    printf("\n");
    simple_udp_sendto(&udp_conn, decrypted, datalen, sender_addr);
    LOG_INFO("Respose Sent: ");
        for (uint16_t i = 0; i < datalen; i++)
    {
        LOG_INFO_("%02x ",(uint8_t) data[i]);
    }
    printf("\n");
    LOG_INFO("Response Sent To :");
    LOG_INFO_6ADDR(sender_addr);
    printf("\n");
}

PROCESS(udp_server_process, "UDP server");
PROCESS(serial_read_process, "Serial read process");
AUTOSTART_PROCESSES(&udp_server_process,&serial_read_process);
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
    static struct etimer log_timer;  // Timer to control periodic logging

    PROCESS_BEGIN();
    
    energest_init();  // Initialize the energy estimator
    NETSTACK_ROUTING.root_start();  // Start the routing for the DAG
    
    // Register UDP connection
    simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_CLIENT_PORT, udp_rx_callback);

    // Set up periodic timer for logging energy usage
    etimer_set(&log_timer, LOG_INTERVAL);
    while(1) {
        // Wait for either an event or timer expiration
        PROCESS_WAIT_EVENT();

        // Check if the timer expired
        if (ev == PROCESS_EVENT_TIMER && data == &log_timer) {
            // Log energy usage statistics
            log_energest_to_file();  // Assuming this function writes stats to a file
            
            // Reset energy statistics to prepare for the next period
            energest_flush();

            // Restart the timer for the next logging interval
            etimer_reset(&log_timer);
        }

        // You can handle other events here, if necessary (e.g., UDP messages, etc.)
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(serial_read_process, ev, data) {
    PROCESS_BEGIN();

    LOG_INFO("Serial read process started.\n");
    // Wait for a serial line event
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message);
        if (data != NULL) {
            // Process the received serial message
            char* received_string = (char*)data;
            LOG_INFO("Received string: %s\n", received_string);
            // Handle the command "read_log"
            if (strcmp(received_string, "read_log") == 0) {
                // Call your log-reading function here
                read_log_file();
            } else {
                LOG_ERR("Unknown command: %s\n", received_string);
            }
        }
    }

    PROCESS_END();
}