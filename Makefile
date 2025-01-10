# Define Contiki projects
CONTIKI_PROJECT = udp-client udp-server
all: $(CONTIKI_PROJECT)

# Contiki root directory
CONTIKI = ../..

# Include Contiki's makefile
include $(CONTIKI)/Makefile.include

# Enable modules and debugging
MODULES += sys/energest
MODULES += dev/serial-line  # Correct module for serial-line communication
DEBUG = 1

# Compiler flags
WARNINGS_AS_ERRORS=0
CFLAGS += -DENERGEST_CONF_ON -Wall -Wextra $(WARNINGS_AS_ERRORS)
CFLAGS += -DPLATFORM_COOJA
APPS += serial-line

# Option to treat warnings as errors (default off)
ifeq ($(TREAT_WARNINGS_AS_ERRORS), 1)
  WARNINGS_AS_ERRORS = -Werror
  CFLAGS += -Wall -Wextra $(WARNINGS_AS_ERRORS)
else
  WARNINGS_AS_ERRORS =
  CFLAGS += -Wall -Wextra -Wno-error
endif

# Rebuild with warnings treated as errors
.PHONY: treat-warnings-as-errors dont-warn
treat-warnings-as-errors:
	@echo "Rebuilding with warnings as errors enabled"
	$(MAKE) clean
	$(MAKE) TREAT_WARNINGS_AS_ERRORS=1

dont-warn:
	@echo "Rebuilding without treating warnings as errors"
	$(MAKE) clean
	$(MAKE) TREAT_WARNINGS_AS_ERRORS=0
