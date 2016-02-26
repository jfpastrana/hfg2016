### BEGIN INIT INFO
# Provides:          start-rpi.sh
# Required-Start:    $all
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start services RPI
# Description:       Starting all applications
### END INIT INFO

#!/bin/bash
#SCRIPT para encender todos los programas necesarios al inicio

	case "$1" in
	start)
		echo "Iniciando servicio start-rpi..."
		#echo "Reseteando Modulo....."
		#/home/ArduPi/blueberry/ModuleOff &
		echo "0" > /home/ArduPi/blueberry/port.txt
		sleep 4
		echo "Encendiendo Modulo....."
		/home/ArduPi/blueberry/ModuleIgnition &
		sleep 8
		echo  "Introducciendo el numero PIN"
		/home/ArduPi/blueberry/SIMPin &
		sleep 20
                /home/ArduPi/blueberry/geolocation &
		#sleep 10
		#/home/ArduPi/blueberry/GPRS/setTime &
		sleep 15
		/home/ArduPi/blueberry/projectOBDII/applicationPID &
		sleep 30
		/home/ArduPi/blueberry/GPRS/sendInfoHTTP &
	;;
	stop)
		echo "Deteniendo el servicio start-rpi..."
		pkill ModuleOff
		pkill ModuleIgnition
		pkill SIMPin
		pkill setTime
		pkill applicationPID
		pkill geolocation
		pkill sendInfoHTTP
                echo "0" > /home/ArduPi/blueberry/port.txt
		/home/ArduPi/blueberry/ModuleOff
	;;
	*)
	echo "Modo de empleo: /etc/init.d/start-rpi.sh {start|stop}"
	exit 1
	;;
	esac
	exit 0
