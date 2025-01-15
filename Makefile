TARGET=cooja
# Contiki root directory
CONTIKI = ../..
# Include Contiki's makefile
include $(CONTIKI)/Makefile.include
# Define Contiki projects
CONTIKI_PROJECT = udp-client udp-server
ASCON_INC = LibAscon/inc
ASCON_SRC = LibAscon/src
ASCON_B	= LibAscon/build

all: $(CONTIKI_PROJECT)

# Enable modules and debugging
MODULES += sys/energest
MODULES += dev/serial-line  # Correct module for serial-line communication
DEBUG = 1
# Compiler flags
WARNINGS_AS_ERRORS=0
CFLAGS += -DENERGEST_CONF_ON -Wall -Wextra $(WARNINGS_AS_ERRORS)
CFLAGS += -DPLATFORM_COOJA
CFLAGS += -I$(ASCON_INC)
CFLAGS += -I$(ASCON_SRC)
# Apps to include
APPS += serial-line
# Object files


# Linker flags
LDFLAGS += -L$(ASCON_B) -lascon128  # Corrected linking for Ascon library


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
clean_build:
	$(MAKE) TARGET=$(TARGET) clean
	$(MAKE) TARGET=$(TARGET) build udp-client udp-server
update:
	$(MAKE) TARGET=$(TARGET)