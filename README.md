# Skywalker Prototype
This project abstacts the functionalities of the sensors I am using ([Accelerometer](http://www.st.com/web/en/resource/technical/document/datasheet/DM00040962.pdf  "ST Sensor"), [Gyroscope](http://www.st.com/web/en/resource/technical/document/datasheet/DM00060659.pdf  "ST Sensor"), [Magnetometer](http://www.st.com/web/en/resource/technical/document/datasheet/DM00075867.pdf  "ST Sensor"), [Proximity sensor for close up](http://media.digikey.com/pdf/Data%20Sheets/Sharp%20PDFs/GP2Y0A41SK0F_Spec.pdf  "Sharp Sensor"), [Proximity sensor for far away](http://media.digikey.com/pdf/Data%20Sheets/Sharp%20PDFs/GP2Y0A60SZxF.pdf  "Sharp Sensor")).

## Things I'm using
* Beaglebone Black (TI AM355X ARM® Cortex A8)
* [Ubuntu Xenial](https://rcn-ee.com/rootfs/2016-03-11/elinux/ubuntu-xenial-console-armhf-2016-03-11.tar.xz  "Ubuntu Xenial")
* arm-linux-gnueabi-g++ version 5.2.1
* cmake 3.2.2
* doxygen 1.8.9.1

## Setup
### On the Beaglebone Black
* Copy the Ubuntu Xenial Version onto an SD Card
* Boot the Ubuntu image on the SD Card by pressing the boot-button on your Beaglebone Black (BBB)
### On the computer you will be developing
* Configure the Ubuntu image to your liking
* ```sudo apt-get update```
* ```sudo apt-get install git cmake gcc-arm-linux-gnueabi g++-arm-linux-gnueabi```
* Follow [these](http://elinux.org/BeagleBone_Black_Enable_SPIDEV#SPI0  "spi device") instructions to enable the spi device
