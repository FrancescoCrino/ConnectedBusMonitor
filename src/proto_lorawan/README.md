This folder contains 2 files:
- main.c: Main program to flash in the board
- Makefile

The given program is tested using a STM32 B-L072Z-LRWAN1 discovery board equipped with a shield X-NUCLEO-IKS01A2 and a CO2 sensor MH-Z19C.

The program is compiled on a linux environment and the following instructions are valid if working on a linux environment.

In order to run this program we need to clone the [RIOT-OS](https://github.com/RIOT-OS/RIOT).

It is possible to build the program executing the command "make" from the terminal.

To flash the builded program to the lora discovery board we can execute the command

```
DEVEUI=myDevEUI APPEUI=myAppEUI APPKEY=myAppKEY make flash term
```
Inserting the DevEUI, AppEUI and AppKEY of the LoRa device you want to use.

While flashing the program will performs a join procedure and then it will start collecting data and sending the encrypted messages to TTN through LoRa.







