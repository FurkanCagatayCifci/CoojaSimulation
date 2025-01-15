#ifdef _CLIENT_H_ || _SERVER_H_
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
#endif