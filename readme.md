# Arduino and ESP32 Codes for ArtNet-based LED, DMX, and SBUS Control

This repository contains the codes for controlling LED, DMX, and SBUS devices using Arduino and ESP32 microcontrollers. The devices are controlled using ArtNet protocol for receiving information and sending the control signals. These Codes are just interfaces between existing libraries


## Acknowledgements to the libraries used
* Artnet: https://github.com/rstephan/ArtnetWifi
* Adafruit: https://github.com/adafruit/Adafruit_NeoPixel
* esp_dmx: https://github.com/someweisguy/esp_dmx
* sbusb: https://github.com/bolderflight/sbus

## LED Control via Ardafruit

The artnetArdafruit folder contains the code for controlling LED devices using Arduino and Ardafreit. The code receives ArtNet packets containing RGB values for the LEDs and sends the corresponding signals to the Ardafreit board to control the LED devices.

## DMX Control via ESP32 DMX Interface

The artnetDMX folder contains the code for controlling DMX devices using ESP32 microcontroller and a DMX interface. The code receives ArtNet packets containing DMX channel values and sends the corresponding signals to the DMX interface to control the DMX devices.

## SBUS Control via ESP32

The artnetSBUS folder contains the code for controlling Ronin Gimble devices using ESP32 microcontroller and SBUS protocol. The code receives ArtNet packets containing control values for the Gimble and sends the corresponding signals to the Gimble using the SBUS protocol.

## Usage

    Upload the code to the microcontroller using the Arduino or ESP32 IDE.
    Connect the microcontroller to the corresponding device (Ardafreit for LED, DMX interface for DMX, and Ronin Gimble for SBUS).
    Connect the microcontroller to the network and ensure that it is receiving ArtNet packets.
    Control the devices by sending ArtNet packets containing the corresponding control values.

## Contributing

Contributions to this project are welcome. If you find a bug or have a feature request, please open an issue. If you would like to contribute code, please fork the repository and submit a pull request. Please contact me via hallo@haensse.dev for questions. 

## License
MIT