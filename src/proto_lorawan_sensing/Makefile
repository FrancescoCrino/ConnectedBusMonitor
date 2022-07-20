# name of your application
APPLICATION = lorawan_iotlab_2

# If no BOARD is found in the environment, use this default:
BOARD ?= b-l072z-lrwan1

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../../../RIOT

# --------------- mhz19 --------------
USEMODULE += ztimer
USEMODULE += ztimer_msec

# set default device parameters in case they are undefined
TEST_MODE ?= 1

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

# --------------- crypto ----------------

USEMODULE += random
USEMODULE += crypto_aes_192
USEMODULE += cipher_modes


#----------------------------------------------------------------


DEVEUI ?= 2611261126112611
APPEUI ?= 1122112211221122
APPKEY ?= 2C897BCF7B4A75C28882617656A31DD4

# Pass these enviroment variables to docker
DOCKER_ENV_VARS += DEVEUI
DOCKER_ENV_VARS += APPEUI
DOCKER_ENV_VARS += APPKEY

# Default radio driver is Semtech SX1276 (used by the B-L072Z-LRWAN1 board)
DRIVER ?= sx1276
#DRIVER ?= sx1272

# Default region is Europe and default band is 868MHz
LORA_REGION ?= EU868

# Include the Semtech-loramac package
USEPKG += semtech-loramac

USEMODULE += $(DRIVER)
USEMODULE += fmt
FEATURES_OPTIONAL += periph_rtc

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Default IotLab Config to run the test
ifneq (,$(filter iotlab%,$(MAKECMDGOALS)))
  IOTLAB_NODES ?= 1
  IOTLAB_TYPE  ?= st-lrwan1:sx1276
  IOTLAB_SITE  ?= saclay
  include $(RIOTBASE)/dist/testbed-support/Makefile.iotlab
endif

ifneq (,$(filter test,$(MAKECMDGOALS)))
  DEFAULT_MODULE += test_utils_interactive_sync
endif

include $(RIOTBASE)/Makefile.include

ifndef CONFIG_KCONFIG_USEMODULE_LORAWAN
  # OTAA compile time configuration keys
  CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
  CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
  CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"
endif
