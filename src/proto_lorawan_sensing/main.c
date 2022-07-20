
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#include "encrypt.h"
#include "random.h"
#include "crypto/aes.h"
#include "crypto/modes/cbc.h"
#include "crypto/ciphers.h"

#include "msg.h"
#include "thread.h"
#include "fmt.h"

#define TEST_MODE_UART 1
#define TEST_MODE_PWM 2

#ifndef TEST_MODE
#error "TEST_MODE not defined"
#endif

#include "hts221.h"
#include "hts221_params.h"
#include "xtimer.h"

#include "ztimer.h"
#include "mhz19.h"
#include "mhz19_params.h"

#define SLEEP_S     (2U)
static hts221_t dev;

#include "net/loramac.h"


#if IS_USED(MODULE_PERIPH_RTC)
#include "periph/rtc.h"
#else
#include "timex.h"
#include "ztimer.h"
#endif


#if !IS_USED(MODULE_PERIPH_RTC)
static ztimer_t timer;
#endif

//static const char *message = "{\"humidity\":\"40.50\", \"temperature\":\"24.70\",\"co2\":400, \"gps\":\"38.1405228,13.2872489\"}";

mhz19_t dev_co2;
int16_t ppm;
uint16_t hum;
int16_t temp;



static void get_values(void){
    printf("Testing sensors communication...");
        int res = mhz19_get_ppm(&dev_co2, &ppm);
        if (res == 0) {
            puts("CO2 sensor [OK] - ");
        }
        else {
            printf("CO2 sensor [Failed]: %d\n", res);
        }
        if (hts221_read_humidity(&dev, &hum) != HTS221_OK) {
            puts("humidity sensor [FAILED] - ");
        }else{
            puts("humidity sensor [OK] - ");
        }
        if (hts221_read_temperature(&dev, &temp) != HTS221_OK) {
            puts("temperature sensor [FAILED] - ");
        }else{
            puts("temperature sensor [OK] - ");
        }

        printf("\n-> {'humidity': %d.%d%% , 'temperature': %d.%d°C , 'co2': %dppm}\n", (hum / 10), (hum % 10),
               (temp / 10), abs(temp % 10), ppm);

}


int main(void){

    puts("\nLoRaWAN Class A low-power application");
    puts("=====================================\n");

    // hts221 initialization

    printf("Init HTS221 on I2C_DEV(%i)\n", (int)hts221_params[0].i2c);
    if (hts221_init(&dev, &hts221_params[0]) != HTS221_OK) {
        puts("[FAILED]");
        return 1;
    }
    if (hts221_power_on(&dev) != HTS221_OK) {
        puts("[FAILED] to set power on!");
        return 2;
    }
    if (hts221_set_rate(&dev, dev.p.rate) != HTS221_OK) {
        puts("[FAILED] to set continuous mode!");
        return 3;
    }

    // mhzt19 initialization



#if TEST_MODE == TEST_MODE_UART
    mhz19_params_t params = { .uart = TEST_UART };
#endif
#if TEST_MODE == TEST_MODE_PWM
    mhz19_params_t params = { .pin = TEST_PWM };
#endif

    puts("MH-Z19 CO2 sensor test application\n");

    /* initialize the sensor */
#if TEST_MODE == TEST_MODE_UART
    printf("Initializing sensor in UART mode...");
#endif
#if TEST_MODE == TEST_MODE_PWM
    printf("Initializing sensor in PWM mode...");
#endif

    if (mhz19_init(&dev_co2, &params) == 0) {
        puts("[OK]");

       //acquire data every 45 seconds
        while (1){
            get_values();
            ztimer_sleep(ZTIMER_MSEC, 45*1000);
        }
    }
    else {
        puts("[Failed]");
        return 1;
    }



    return 0;
}
