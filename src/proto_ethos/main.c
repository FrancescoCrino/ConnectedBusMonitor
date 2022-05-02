#include <stdlib.h>
#include <stdio.h>

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

#include "shell.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define SLEEP_S     (2U)
static hts221_t dev;

#ifndef EMCUTE_ID
#define EMCUTE_ID           ("gertrud")
#endif
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
//static char topics[NUMOFSUBS][TOPIC_MAXLEN];

// --- connectivity functions ---

static void *emcute_thread(void *arg){
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, EMCUTE_ID);
    return NULL;    /* should never be reached */
}

static unsigned get_qos(const char *str){
    int qos = atoi(str);
    switch (qos) {
        case 1:     return EMCUTE_QOS_1;
        case 2:     return EMCUTE_QOS_2;
        default:    return EMCUTE_QOS_0;
    }
}

static int cmd_con(int argc, char **argv){
    sock_udp_ep_t gw = { .family = AF_INET6, .port = CONFIG_EMCUTE_DEFAULT_PORT };
    char *topic = NULL;
    char *message = NULL;
    size_t len = 0;

    if (argc < 2) {
        printf("usage: %s <ipv6 addr> [port] [<will topic> <will message>]\n",
                argv[0]);
        return 1;
    }

    /* parse address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, argv[1]) == NULL) {
        printf("error parsing IPv6 address\n");
        return 1;
    }

    if (argc >= 3) {
        gw.port = atoi(argv[2]);
    }
    if (argc >= 5) {
        topic = argv[3];
        message = argv[4];
        len = strlen(message);
    }

    if (emcute_con(&gw, true, topic, message, len, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", argv[1], (int)gw.port);
        return 1;
    }
    printf("Successfully connected to gateway at [%s]:%i\n",
           argv[1], (int)gw.port);

    return 0;
}

static int cmd_pub(int argc, char **argv){
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    if (argc < 3) {
        printf("usage: %s <topic name> <data> [QoS level]\n", argv[0]);
        return 1;
    }

    /* parse QoS level */
    if (argc >= 4) {
        flags |= get_qos(argv[3]);
    }

    printf("pub with topic: %s and name %s and flags 0x%02x\n", argv[1], argv[2], (int)flags);

    /* step 1: get topic id */
    t.name = argv[1];
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    /* step 2: publish data */
    if (emcute_pub(&t, argv[2], strlen(argv[2]), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n",
                t.name, (int)t.id);
        return 1;
    }

    printf("Published %i bytes to topic '%s [%i]'\n",
            (int)strlen(argv[2]), t.name, t.id);

    return 0;
}

static int publish(char *message){

    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    char *topic = "bus_test";

    //printf("pub with topic: %s, message: %s with flags: 0x%02x\n", topic, message, (int)flags);

    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    //publish data
    if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s [%i]'\n",
                t.name, (int)t.id);
        return 1;
    }
    return 0;
}

static int run(int argc, char **argv){

    if (argc > 1) {
        printf("No argument needed for command %s\n", argv[0]);
        return 1;
    }

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

    /* read humidity, temperature and CO2 level every 10 seconds */
    int16_t ppm;
    uint16_t hum;
    int16_t temp;

    char data[50];

    while(1) {

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

        printf("\n-> H: %d.%d%% - T: %d.%d°C - CO2: %d ppm \n\n",
                        (hum / 10), (hum % 10),(temp / 10), abs(temp % 10), ppm);

        sprintf(data, "{\"humidity\":\"%d.%d\", \"temperature\":\"%d.%d\",\"co2\":%d, \"gps\":\"38.1405228,13.2872489\"}",
                        (hum / 10), (hum % 10), (temp / 10), abs(temp % 10), ppm);
        publish((char *)&data);

        /* sleep between measurements */
        ztimer_sleep(ZTIMER_MSEC, 10*1000);
    }
    return 0;
}

static const shell_command_t shell_commands[] = {
    { "con", "connect to MQTT broker", cmd_con },
    { "pub", "publish something", cmd_pub },
    { "run", "Start the water leakage detection system", run },
    { NULL, NULL, NULL }
};

int main(void){

    printf("\n --------------------- BUS MONITOR TEST --------------------- \n");

    /* the main thread needs a msg queue to be able to run `ping`*/
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;

}