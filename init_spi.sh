su -c "echo BB-SPI0-01 > /sys/devices/bone_capemgr.*/slots"
sudo chmod a+rw /dev/spidev1.0
ls /dev/spidev1.0
