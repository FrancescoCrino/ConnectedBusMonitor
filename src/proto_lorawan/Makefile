# name of your application
APPLICATION = proto_ethos

# If no BOARD is found in the environment, use this default:
BOARD ?= b-l072z-lrwan1

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

# --------------- mhz19 --------------
USEMODULE += ztimer
USEMODULE += ztimer_msec

# set default device parameters in case they are undefined
TEST_MODE ?= 2

TEST_UART ?= UART_DEV\(1\)
TEST_PWM ?= GPIO_PIN\(PORT_B,7\)

ifeq ($(TEST_MODE),1)
  USEMODULE += mhz19_uart
endif

ifeq ($(TEST_MODE),2)
  USEMODULE += mhz19_pwm
endif

# export parameters
CFLAGS += -DTEST_MODE=$(TEST_MODE)
CFLAGS += -DTEST_UART=$(TEST_UART)
CFLAGS += -DTEST_PWM=$(TEST_PWM)

include $(RIOTBASE)/tests/Makefile.tests_common

# --------------- hts221 ----------------

USEMODULE += xtimer

DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += hts221

include $(RIOTBASE)/Makefile.include