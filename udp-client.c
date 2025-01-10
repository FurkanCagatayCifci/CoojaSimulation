#include "project-conf.h"
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
#include "filelogging.c"
#include "serial.c"
#include "contiki.h"
#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include <stdint.h>
#include <inttypes.h>
#include "sys/log.h"
#include "sys/energest.h"
#include "encryption.c"

static struct simple_udp_connection udp_conn;
static uint32_t rx_count = 0;

static void udp_rx_callback(
    struct simple_udp_connection *c,
    const uip_ipaddr_t *sender_addr,
    uint16_t sender_port,
    const uip_ipaddr_t *receiver_addr,
    uint16_t receiver_port,
    const uint8_t *data,
    uint16_t datalen)
{

  LOG_INFO("Received response (hex): ");
  for (uint16_t i = 0; i < datalen; i++)
  {
    LOG_INFO_("%02x ", (uint8_t)data[i]);
  }
  printf("\n");
  LOG_INFO("Received From : ");
  LOG_INFO_6ADDR(sender_addr);
  printf("\n");
#if LLSEC802154_CONF_ENABLED
  LOG_INFO(" LLSEC LV:%d", uipbuf_get_attr(UIPBUF_ATTR_LLSEC_LEVEL));
#endif
  LOG_INFO("\n");
  rx_count++;
}
/*---------------------------------------------------------------------------*/
PROCESS(udp_client_process, "UDP client process");
PROCESS(serial_read_process, "Serial read process");
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
  PROCESS_BEGIN();
  LOG_INFO("Serial port server started.");
  energest_init();
  /* Initialize UDP connection */
  simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
                      UDP_SERVER_PORT, udp_rx_callback);

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
        LOG_INFO("Encrypting data (str) %s,", str);
        printf("(hex):");
        for (uint16_t i = 0; i < strlen(str); i++)
        {
          LOG_INFO_("%02x ", (uint8_t)str[i]);
        }
        printf("\n");
        encrypt(str,strlen(str),key);
        LOG_INFO("Sending Data: %s", str);
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
PROCESS_THREAD(serial_read_process, ev, data)
{
  PROCESS_BEGIN();

  LOG_INFO("Serial read process started.\n");

  // Wait for a serial line event
  while (1)
  {
    PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message);

    if (data != NULL)
    {
      // Process the received serial message
      char *received_string = (char *)data;
      LOG_INFO("Received string: %s\n", received_string);
      // Handle the command "read_log"
      if (strcmp(received_string, "read_log") == 0)
      {
        // Call your log-reading function here
        read_log_file();
      }
      else
      {
        LOG_INFO("Unknown command: %s\n", received_string);
      }
    }
  }

  PROCESS_END();
}