#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "msg.h"
#include "thread.h"
#include "fmt.h"

#include "net/loramac.h"
#include "semtech_loramac.h"

#include "encrypt.h"
#include "random.h"
#include "crypto/aes.h"
#include "crypto/modes/cbc.h"
#include "crypto/ciphers.h"

#if IS_USED(MODULE_PERIPH_RTC)
#include "periph/rtc.h"
#else
#include "timex.h"
#include "ztimer.h"
#endif

#if IS_USED(MODULE_SX127X)
#include "sx127x.h"
#include "sx127x_netdev.h"
#include "sx127x_params.h"
#endif

#if IS_USED(MODULE_SX126X)
#include "sx126x.h"
#include "sx126x_netdev.h"
#include "sx126x_params.h"
#endif

/* Messages are sent every 20s to respect the duty cycle on each channel */
#define PERIOD_S            (20U)

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];

static semtech_loramac_t loramac;
#if IS_USED(MODULE_SX127X)
static sx127x_t sx127x;
#endif
#if IS_USED(MODULE_SX126X)
static sx126x_t sx126x;
#endif
#if !IS_USED(MODULE_PERIPH_RTC)
static ztimer_t timer;
#endif

/* Messages are sent every 20s to respect the duty cycle on each channel */
#define PERIOD_S            (20U)

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];

static semtech_loramac_t loramac;
#if IS_USED(MODULE_SX127X)
static sx127x_t sx127x;
#endif
#if IS_USED(MODULE_SX126X)
static sx126x_t sx126x;
#endif
#if !IS_USED(MODULE_PERIPH_RTC)
static ztimer_t timer;
#endif

uint8_t message[150];

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

int16_t ppm;
uint16_t hum;
int16_t temp;


static void get_data(void){

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

        printf("\n-> H: %d.%d%% - T: %d.%d°C - CO2: %d ppm \n\n", (hum / 10), (hum % 10),
               (temp / 10), abs(temp % 10), ppm);
}

static void build_message(void){

    char new_message[50];

    get_data();

    exit(1);

    sprintf(new_message, "\'{\"bus\":%d,\"humidity\":%s,\"temperature\":%s,\"co2\":%d}\'", 10, humidity, temperature, co2);

    printf("[bm] new_message: %s \n", new_message);

    encrypt_message(new_message);

    printf("\n --> Dimension of the payload: %d", pld_size);

    memset(message, 0x00, pld_size);
    memcpy(message, pld_buff, pld_size);

    printf("\n[bm] Bytes to send: \n");
    print_bytes(message, pld_size);

    printf("\n[bm] Enc message: %s\n", message);

}


static void _alarm_cb(void *arg){
    (void) arg;
    msg_t msg;
    msg_send(&msg, sender_pid);
}

static void _prepare_next_alarm(void){
#if IS_USED(MODULE_PERIPH_RTC)
    struct tm time;
    rtc_get_time(&time);
    /* set initial alarm */
    time.tm_sec += PERIOD_S;
    mktime(&time);
    rtc_set_alarm(&time, _alarm_cb, NULL);
#else
    timer.callback = _alarm_cb;
    ztimer_set(ZTIMER_MSEC, &timer, PERIOD_S * MS_PER_SEC);
#endif
}

static void _send_message(void){

    build_message();

    printf("Sending....\n");

    /* Try to send the message */
    uint8_t ret = semtech_loramac_send(&loramac, message, pld_size);

    if (ret != SEMTECH_LORAMAC_TX_DONE)  {
        printf("Cannot send message '%s', ret code: %d\n", message, ret);
        return;
    }

    printf("Message sent!\n");

    exit(1);
}


static void *sender(void *arg){
    (void)arg;

    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);

        /* Trigger the message send */
        _send_message();

        /* Schedule the next wake-up alarm */
        _prepare_next_alarm();
    }

    /* this should never be reached */
    return NULL;
}

int main(void){

    puts("\nLoRaWAN Class A low-power application");
    puts("=====================================\n");

    /* Convert identifiers and application key */
    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);

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

    mhz19_t dev_co2;

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
    }
    else {
        puts("[Failed]");
        return 1;
    }

    /* Initialize the radio driver */
#if IS_USED(MODULE_SX127X)
    sx127x_setup(&sx127x, &sx127x_params[0], 0);
    loramac.netdev = &sx127x.netdev;
    loramac.netdev->driver = &sx127x_driver;
#endif

#if IS_USED(MODULE_SX126X)
    sx126x_setup(&sx126x, &sx126x_params[0], 0);
    loramac.netdev = &sx126x.netdev;
    loramac.netdev->driver = &sx126x_driver;
#endif

    /* Initialize the loramac stack */
    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
     * generated device address and to get the network and application session
     * keys.
     */
    puts("Starting join procedure");
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        return 1;
    }
    puts("Join procedure succeeded");

    /* start the sender thread */
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_PRIO, 0, sender, NULL, "sender");

    /* trigger the first send */
    msg_t msg;
    msg_send(&msg, sender_pid);
    return 0;
}
