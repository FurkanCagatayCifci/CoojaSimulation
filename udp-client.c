#define LOG_INTERVAL (3 * CLOCK_SECOND) // Log every 60 secondsS
#define LOGFILENAME "UDP_CLIENT_ENERGEST.txt"
#define LOGNAME "energest_log_client.txt"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO
#define MESSAGE "SAMSUN"
#define WITH_SERVER_REPLY 1
#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678
#define SEND_INTERVAL (10 * CLOCK_SECOND)
#define SERIAL_MESSAGE_LEN 128
#include "client.h"
#include "encryption.c"
#include "mote.c"
/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process, &serial_read_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_client_process, ev, data)
{
	static struct etimer periodic_timer;
	static struct etimer log_timer;
	static char str[128];
	uip_ipaddr_t dest_ipaddr;
	static uint32_t tx_count;
	static uint32_t missed_tx_count;
	memcpy(str, "\0", 128);
	energest_init();
	#ifdef ENCRPTION_ON
	ascon_aead128_init(NULL,&key,&iv);
	#endif
	simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,UDP_SERVER_PORT, udp_rx_callback);
	PROCESS_BEGIN();
	LOG_INFO("Serial port server started.");
	/* Initialize UDP connection */
	etimer_set(&periodic_timer, random_rand() % SEND_INTERVAL);
	etimer_set(&log_timer, random_rand() % LOG_INTERVAL);
	while (1)
	{
		PROCESS_WAIT_EVENT();
		if (ev == PROCESS_EVENT_TIMER && data == &periodic_timer)
		{
			if (NETSTACK_ROUTING.node_is_reachable() &&
					NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr))
			{

				/* Print statistics every 10th TX */
				if (tx_count % 10 == 0)
				{
					LOG_INFO("Tx/Rx/MissedTx: %" PRIu32 "/%" PRIu32 "/%" PRIu32 "\n",
									 tx_count, rx_count, missed_tx_count);
				}
				strcpy(str, MESSAGE);
				/* Send to DAG root */
				LOG_INFO("Encrypting data (str) %s, ", str);
				printf("(hex):");
				for (uint16_t i = 0; i < strlen(str); i++)
				{
					LOG_INFO_("%02x ", (uint8_t)str[i]);
				}
				printf("\n");
				#ifdef ENCRPTION_ON
				encrypt(str, strlen(str), str,&tag,TAG_SIZE,&ad);
				#endif
				LOG_INFO("Sending Data: (str) %s, (hex) ", str);
				for (uint16_t i = 0; i < strlen(str); i++)
				{
					LOG_INFO_("%02x ", (uint8_t)str[i]);
				}
				printf("\n");
				LOG_INFO("Sending To : ");
				LOG_INFO_6ADDR(&dest_ipaddr);
				printf("\n");
				simple_udp_sendto(&udp_conn, str, strlen(str), &dest_ipaddr);
				tx_count++;
			}
			else
			{
				LOG_INFO("Not reachable yet\n");
				if (tx_count > 0)
				{
					missed_tx_count++;
				}
			}
			etimer_reset(&periodic_timer);
		}
		if (ev == PROCESS_EVENT_TIMER && data == &log_timer)
		{
			// Log energy statistics
			log_energest_to_file();
			// Reset energy stats
			energest_flush();
			etimer_reset(&log_timer);
		}
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/