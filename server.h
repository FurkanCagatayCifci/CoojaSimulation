#define _SERVER_H_
#include "project-conf.h"
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
    #ifdef ENCRPTION_ON
    decrypt(decrypted,data,datalen, &tag, TAG_SIZE, ad);
    #endif
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
