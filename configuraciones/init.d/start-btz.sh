### BEGIN INIT INFO
# Provides:          start-btz.sh
# Required-Start:    $all
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start paring bluetooh
# Description:       Enable bluetooth pair.
### END INIT INFO

#!/bin/bash
#Resetea y conecta la conexion bluetooth

MAC_OBD="00:12:6F:10:53:34"
MAC_MLAIS="0C:00:90:E7:8F:47"
MAC_OBD2="00:0D:18:3A:67:89"

	case "$1" in
	start)
		echo "Iniciando servicio start-rpi..."
		/usr/bin/bluez-test-device trusted $MAC_OBD2 yes
		/usr/bin/rfcomm release 0 $MAC_OBD2
		/usr/bin/rfcomm connect 0 $MAC_OBD2
	;;
	stop)
		echo "Deteniendo el servicio start-rpi..."
		/etc/init.d/bluetooth stop
	;;
	restart)
		echo "Reiniciando el servicio start-btz..."
		/etc/init.d/bluetooth restart
	;;
	*)
	echo "Modo de empleo: /etc/init.d/start-rpi.sh {start|stop}"
	exit 1
	;;
	esac
	exit 0
