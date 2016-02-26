#!/bin/sh
#SCRIPT para encender todos los programas necesarios al inicio

#echo -e "\nReseteando Modulo....."
#programa ModuleOff

echo "0" > /home/ArduPi/blueberry/port.txt

#echo -e "\nEncendiendo Modulo....."
/home/ArduPi/blueberry/ModuleIgnition

sleep 4

#echo -e "\nIntroducciendo el numero PIN"
/home/ArduPi/blueberry/SIMPin

sleep 10

/home/ArduPi/blueberry/GPRS/setTime &

sleep 15

/home/ArduPi/blueberry/projectOBDII/applicationPID &

sleep 10

/home/ArduPi/blueberry/geolocation &


sleep 120


/home/ArduPi/blueberry/GPRS/sendInfoHTTP &
