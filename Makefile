TARGET=cooja
# Contiki root directory
CONTIKI = ../..
# Include Contiki's makefile
include $(CONTIKI)/Makefile.include

# Define Contiki projects
CONTIKI_PROJECT = udp-client udp-server

# Ascon library paths
ASCON_INC = LibAscon/inc
ASCON_SRC = LibAscon/src
ASCON_B = LibAscon/build
TARGETLIBS += $(ASCON_B)/libascon128.a

# Enable modules and debugging
MODULES += sys/energest
MODULES += dev/serial-line  # Correct module for serial-line communication
MODULES_REL += $(ASCON_SRC)
MODULES_REL += $(ASCON_INC)
MODULES_REL += $(ASCON_B)

DEBUG = 1
V=1
# Compiler and linker flags
WARNINGS_AS_ERRORS=0
CFLAGS += -DENERGEST_CONF_ON -Wall -Wextra $(WARNINGS_AS_ERRORS)
CFLAGS += -DPLATFORM_COOJA
CFLAGS += -I${ASCON_INC} -I${ASCON_SRC}
LDFLAGS += -L$(ASCON_B)
LDFLAGS += -lascon128

# Apps to include
APPS += serial-line
# Rule to build the Ascon library
$(ASCON_B)/libascon128.a:
	$(MAKE) -C ${ASCON_B}
$(CONTIKI_PROJECT): $(ASCON_B)/libascon128.a

# Move library after object files
%.$(TARGET): %.o $(TARGETLIBS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@
# Add absolute path to library
TARGETLIBS += $(abspath $(ASCON_B)/libascon128.a)
LDFLAGS += -L$(abspath $(ASCON_B))
LDLIBS += -Wl,--whole-archive -lascon128 -Wl,--no-whole-archive

# Option to treat warnings as errors (default off)
ifeq ($(TREAT_WARNINGS_AS_ERRORS), 1)
  WARNINGS_AS_ERRORS = -Werror
  CFLAGS += $(WARNINGS_AS_ERRORS)
else
  WARNINGS_AS_ERRORS =
  CFLAGS += -Wno-error
endif

# Default target
all: $(ASCON_B)/libascon128.a
	@echo ${PROJECT_CONF_PATH}
	${MAKE} ${CONTIKI_PROJECT}

# Clean and build all targets
clean_build:
	@echo "Cleaning and building"
	$(MAKE) clean TARGET=$(TARGET)
	all
# Clean the Ascon library
clean: 
	${MAKE} clean_build
clean_ascon:
	@if [ -f ${ASCON_B}/Makefile ]; then \
		$(MAKE) -C ${ASCON_B} clean; \
	else \
		echo "No clean target in LibAscon"; \
	fi
clean_build:
	${MAKE} distclean
# Update targets
update:
	@echo "Updating"
	$(MAKE) TARGET=$(TARGET) ${CONTIKI_PROJECT}