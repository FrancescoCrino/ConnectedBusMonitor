# name of your application
APPLICATION = proto_ethos

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f446ze

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

# --------------- mhz19 --------------
USEMODULE += ztimer
USEMODULE += ztimer_msec

# set default device parameters in case they are undefined
TEST_MODE ?= 2

TEST_UART ?= UART_DEV\(1\)
TEST_PWM ?= GPIO_PIN\(PORT_E,9\)

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

# --------------- ethos mqtt conn ----------------

# Include MQTT-SN
USEMODULE += emcute


# Include packages that pull up and auto-init the link layer.
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif

# Specify the mandatory networking modules for IPv6
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_icmpv6_echo
USEMODULE += stdio_ethos
USEMODULE += gnrc_uhcpc

# Address of device
IPV6_PREFIX ?= fe80:2::/64

CFLAGS += -DCONFIG_GNRC_NETIF_IPV6_ADDRS_NUMOF=3

# Default to using ethos for providing the uplink when not on native
UPLINK ?= ethos

# Configure ethos parameters
ETHOS_BAUDRATE ?= 115200
CFLAGS += -DETHOS_BAUDRATE=$(ETHOS_BAUDRATE)

# Ethos/native TAP interface and UHCP prefix can be configured from make command
TAP ?= tap0

host-tools:
	$(Q)env -u CC -u CFLAGS $(MAKE) -C $(RIOTTOOLS)

# Configure terminal parameters
TERMDEPS += host-tools
TERMPROG ?= sudo sh $(RIOTTOOLS)/ethos/start_network.sh
TERMFLAGS ?= $(FLAGS_EXTRAS) $(PORT) $(TAP) $(IPV6_PREFIX) $(ETHOS_BAUDRATE)



include $(RIOTBASE)/Makefile.include