#!/bin/bash
#Resetea y conecta la conexion bluetooth

MAC_OBD="00:12:6F:10:53:34"
MAC_MLAIS="0C:00:90:E7:8F:47"

#/etc/init.d/bluetooth restart

/usr/bin/bluez-test-device trusted $MAC_OBD yes

/usr/bin/rfcomm release 0 $MAC_OBD

/usr/bin/rfcomm connect 0 $MAC_OBD
