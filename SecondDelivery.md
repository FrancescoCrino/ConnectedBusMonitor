# **ConnectedBusMonitor Second Delivery**

## First Delivery Review

- why is air monitoring relevant (CO2 values - statistics)
- what are the requirements (Resquirements Specification)
- clear connection of sensors to metrics
- power/energy source from the bus?
- precise evaluation metrics (Requirements validation)' 


## Changes First to Second Delivery

### Concept

- Requirements specification

### Architecture

- Gravity DHT11 Temperature Humidity Sensor + Xtrinsic MMA8451Q 3-Axis Accelerometer replaced by X-NUCLEO-IKS01A2
- GPS NEO 6M UBLOX replaced by X-NUCLEO-GNSS1A1
- Separate LoRa gateway module (composed of a Nucleo board STM32 Nucleo-F446ZE and a LoRa shield) removed from the architecture (redundancy)

### Evaluation

- Cost
- Power consumption (estimation)
- Duty cycle - Transmission period
 


## Technical Work


- X-NUCLEO-IKS01A2 - Data acquisition

- MH-Z19C CO2 SENSOR - Data acquisition

- X-NUCLEO-GNSS1A1 - STM32CUBE (+X-CUBE-GNSS), Tera Term (data acquisition issues)

- LoRa Shield - I-NUCLEO-LRWAN1 connectivity (compatibility issues)

- AWS IoT Core

### Evaluation

- Cost
- Power consumption (estimation)
- Duty cycle - Transmission period

## Future plans

### Technical development

- Geolocation data acquisition through X-NUCLEO-GNSS (long, lat, timestamp/elapsed since last received value)
- LoRa gateway: B-L072Z-LRWAN1 as an alternative to I-NUCLEO-LRWAN1
- Power source: bus grid + battery
- Web dashboard: Front-End (Angular, OpenStreetMaps, chart libraries) + Amazon API Gateway (REST API) + Amazon DynamoDB


### Evaluation

- Network latency and/or throughput
- Power consumption (considering all sensors and expansion cards)
- Transmission period




