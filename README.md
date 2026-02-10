# Automatic Impedance Matcher

Automatic impedance matcher for a RF sputtering chamber using gradient-descent SWR tuning on Teensy 4.1.  
Developed for CMU Hacker Fab.

## Features

- **Manual Mode**: Direct servo control via potentiometers
- **Auto Mode**: Gradient descent algorithm automatically tunes for optimal SWR
- Real-time SWR measurement with noise filtering (20-sample averaging)

## Hardware

- **Microcontroller**: Teensy 4.1
- **Servos**: TX servo (pin 0), Antenna servo (pin 23)
- **Inputs**: 
  - SWR sensors on A0 (forward) and A1 (reverse)
  - Manual dial potentiometers on pins 39 and 38
  - Mode switch on pin 32 (HIGH = Manual, LOW = Auto)

## Algorithm

The auto-tuning uses a one-way gradient descent search with ±2° steps, reversing direction when SWR increases.

## Authors

- Miguel Salvacion  
- William Gao  
- Aiden Magee
