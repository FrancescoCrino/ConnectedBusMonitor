The given program is tested using a STM32 Nucleo-F446ZE boar equipped with a shield X-NUCLEO-IKS01A2 and a CO2 sensor MH-Z19C.

The program is compiled on a linux environment and the following instructions are valid if working on a linux environment.

In order to run this program we need to clone the [RIOT-OS](https://github.com/RIOT-OS/RIOT) github repository that we will use as RIOT base folder in the Makefile.
We also need to install on your local pc the [mosquitto broker](https://mosquitto.org/) and clone the [mosquitto RSMB](https://github.com/eclipse/mosquitto.rsmb) repository to use it as MQTT-SN broker.

Once we have completed our local set up we can start deploying our network through the following steps.

## Set up of our network virtual interfaces

At first we need to create the network virtual interfaces to connect all our network components locally

```
sudo .../RIOT/dist/tools/tapsetup/tapsetup
```

Then we need to assign a site-global prefix to the tapbr0 interface to let mosquitto works.

```
sudo ip a a fec0:affe::1/64 dev tapbr0
```

## Start the mosquitto MQTT-SN/MQTT transparent bridge

Now we can start our mosuqitto that will act as MQTT-SN/MQTT transparent bridge. 
With the following command we will launch mosquitto that will use as a configuration file the file local_transparent_bridge.conf contained in the conf folder.
Just to be safe we will execute the command to stop mosquitto service before to run our new instance of mosquitto.

```
service mosquitto stop

mosquitto -c .../conf/local_transparent_bridge.conf
```

You need to replace the information related to the aws service you want to connect with in the conf file.

## Start the mosquitto RSMB MQTT-SN server

Now that our transparet bridge is set up we can start the mosquitto RSMB MQTT-SN server.


```
cd .../mosquitto.rsmb/rsmb

./src/broker_mqtts .../conf/rsmb_config.conf
```

Executing those commands rsmb server will connect with the mosquitto transparent bridge and it is ready to receive and forward messages.


## Start main program

Go inside the code folder and execute the following command to build and flash the program inside the Nucleo-F446ZE board.
If you are using a different board change it in the Makefile.

```
make clean flash all term
```

This will flash the code inside the board starts running it. 

Now we are in the terminal of the board and we have to assign a site-global prefix to the board and then connect it to the rsmb broker.
We can do that executing the following commands in the boards teminal.


```
ifconfig 4 add fec0:affe::99

con fec0:affe::1 1885
```

At this point the board is connected with the rsmb broker and we can start the main routine by executing the command "run" in the board terminal.

After executing the command "run" we have the collected values printed on the terminal and at the same time they are sent to the cloud.

## Restart the system

When we terminate the main process we need to reset the network virtual iterfaces before to restart it.
To do that we need to stop the mosquitto transparent bridge and the mosquitto rsmb and then executing the following command:

```
sudo .../RIOT/dist/tools/tapsetup/tapsetup -d
```

Now we need to redo the whole procedure.







