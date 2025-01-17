#define _CLIENT_H_
#include "project-conf.h"
#include "sys/log.h"
#include "sys/energest.h"
#include <stdint.h>
#include <inttypes.h>
#include "filelogging.c"
#include "serial.c"
#include "contiki.h"
#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
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
