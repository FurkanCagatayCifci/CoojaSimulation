#include "mote.h"

PROCESS_THREAD(serial_read_process, ev, data)
{
  PROCESS_BEGIN();
  serial_line_init();
  LOG_INFO("Serial read process started.\n");

  while (1)
  {
    PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message);

    if (data != NULL)
    {
      char *received_string = (char *)data;
      LOG_INFO("Received string: %s\n", received_string);
      
      if (strcmp(received_string, "read_log") == 0)
      {
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
