# cbox328p - slave driver v.1.0.0

## Atmega328p based LED array driver

An idiot-proof solution for driving and controlling multiple high power PWM LED outputs. This project uses atmega328p microcontroller's timer 1 and timer 2 to control 3 PWM pins and a serial connection over RS485 to communicate with master device.  

## Short description of working principle

### Output  
Driver has 3 PWM outputs which work at a frequency of around 70 kHz ([lowest prescaler setting on 328p](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)) with the resolution of 8 bits. This can be changed to user's preference in file **timer.h**

### Communication protocol
Controller accepts incoming messages through RS485 in a predefined pattern:
```
!<command_char><dev_id>[args...]#
```
"**!**" - a predefined operator of message start (CONF.H)  
Driver will not listen to incoming bytes until message start char has appeared.  

**command_char** - a char that corresponds to a specified command, for example "1" (CONF.H)  

**dev_id** - target device id string (3 chars), driver only executes commands that are targeted to it explicitly (device can have multiple IDs to create groups of devices)  

**args** - differing length string, every command has a different subset of arguments  

"**#**" - a predefined operator of message end (CONF.H) 
Driver will execute a command only after receiving both start and end operator of a message

### Available commands

1. Set outputs directly  
```
!1<ID><O1><O2><O3>#
```
O1, O2, O3 - 3 char numeric values from 000 to 255  

2. Select frame to output
```
!2<ID><F_ID>#
```
F_ID - 3 char numeric frame id  
Looks up saved values for outputs at given id in memory and sets them.  

3. Set frame values
```
!3<ID><O1><O2><O3><F_ID>#
```
F_ID - 3 char numeric frame id  
O1, O2, O3 - 3 char numeric values from 000 to 255  
Saves given values in memory at selected id.  

4. Combined command 2 & 3
```
!4<ID><O1><O2><O3><F_ID>#
```

5. Send raport to master
```
!8<ID>#
```
(WARN: Changes RS485 module into transmit mode)  
Sends a raport to the master device  

_TODO: Service commands_