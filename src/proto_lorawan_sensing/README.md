This folder contains 3 files:
	- main.c
	- Makefile
	- README.md


The given program is tested using a STM32 B-L072Z-LRWAN1 discovery board equipped with a shield X-NUCLEO-IKS01A2 and a CO2 sensor MH-Z19C.

The program is compiled on a Linux environment and the following instructions are valid if working on a Linux environment.

In order to run this program we need to clone the [RIOT-OS](https://github.com/RIOT-OS/RIOT).

It is possible to build the program executing the command "make" from the terminal.

To flash the built program to the Lora discovery board we can execute the command

```
DEVEUI=myDevEUI APPEUI=myAppEUI APPKEY=myAppKEY make flash term
```
Inserting the DevEUI, AppEUI and AppKEY of the LoRa device you want to use.

While flashing the program will performs a join procedure, after which it will start acquiring the data for further elaboration.
