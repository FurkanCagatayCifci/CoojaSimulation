#ifndef _MOTE_H_
#define _MOTE_H_

#include "contiki.h"
#include "sys/log.h"
#include "filelogging.h"
#include "dev/serial-line.h"

// Process declaration
PROCESS(serial_read_process,"Serial read process");

#endif /* _MOTE_H_ */
