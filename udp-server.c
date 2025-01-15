#include "project-conf.h"
#define LOG_INTERVAL (3 * CLOCK_SECOND)
#define LOGFILENAME "UDP_CLIENT_ENERGEST.txt"
#define LOGNAME "energest_log_client.txt"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO
#define WITH_SERVER_REPLY  1
#define UDP_CLIENT_PORT	8765
#define UDP_SERVER_PORT	5678
#include "server.h"

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