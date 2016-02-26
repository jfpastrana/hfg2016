
# README #

This README would normally document whatever steps are necessary to get your application up and running.

hfg2016
HackForGood 2016


### What is this repository for? ###

* Quick summary
* Version 1
* [Learn Markdown](https://bitbucket.org/tutorials/markdowndemo)

### How do I get set up? ###



1.	Instalación

La base de todo el sistema es la instalación del sistema operativo Raspbian, el cual se puede descargar la última versión de su página oficial
y habrá que montarlo en la tarjeta SD con el programa correspondiente dependiendo desde que sistema operativo se haga.
Tras la instalación de Raspbian habrá que realizar una pequeña configuración del mismo durante la instalación. Se configura usuario,
contraseña, activación de SSH, interfaz gráfica y también se puede “Overclockear” de forma segura el procesador. Por ejemplo, durante 
la realización de este proyecto se ha utilizado la configuración máxima, 1Ghz. Una vez instalado el sistema operativo habrá que instalar (apt-get install)
todos los paquetes necesarios para su utilización y desarrollo. Serán los siguientes (previamente realizar upgrade y update):
•	Para la utilización del dispositivo:
		Driver y aplicación Bluetooth Bluez  y Blueman.
		Servidor mySQL tanto servidor como cliente.
		Librerías mySQL “libmysqlclient-dev”.
•	Para el desarrollo de aplicaciones:
		Editor de texto vim  y nano.
		Compiladores g++ y gcc.
		Depurador gdb.
		Emulación de puertos Cutecom, Minicom y Screen

2.	Configuración de Raspbian

Antes de poder utilizarlo o poder desarrollar habrá que hacer unas pequeñas modificaciones en el sistema operativo. La configuración 
del puerto serie no viene configurada por defecto, por lo que habrá que realizar los siguientes cambios:

	1.	Copia de seguridad del fichero
		cp /boot/cmdline.txt /boot/cmdline_backup.txt

	2.	Editar el fichero
		nano  /boot/cmdline.txt

	3.	Eliminar el contenido en la línea respectiva
		dwc_otg.lpm_enable=0 console=tty1 $

	4.	En el fichero: /etc/inittab comentar la línea 
		T0:23:respawn:/sbin/getty -L ttyAMA0 115200 vt100

	5.	Reiniciar la Rapberry.

Esos pasos habrá que seguirlos tanto si se va a utilizar o si se va a desarrollar. A continuación habrá que incluir los scripts en los “runlevel” 
correspondientes para que los programas desarrollados en este proyecto se ejecuten al inicio/apagado del sistema.
Los scripts (el código) tendrán que estar alojados en la carpeta /etc/init.d, lo que no quiere decir que los programas también tengan
que estar en esa carpeta, ya que serán accedidos en el código del script mediante rutas absolutas. Los ficheros que habrá que copiar
en esa carpeta y dotar de permisos de ejecución son: “start-rpi.sh”, “start-btz.sh”, “stop-rpi”.
Los dos primeros tendrán que estar en “runlevels” de inicio, es decir el 2, 3, 4, 5. Por regla general todo sistema operativo debían 
arranca siempre en el modo 2 (multiusuario), pero pueden estar en el resto. Lo que se hace es crear un acceso directo en el “runlevel”
correspondiente al script alojado en /etc/init.d por lo que desde esa carpeta habrá que ejecutar los siguientes comandos:

	update-rc.d start-rpi.sh start 90 2 3 4 5 .

	update-rc.d start-btz.sh start 05 2 3 4 5 .

Al primer script se le asigna una preferencia de 90, es decir probablemente sea el último, ya que los programas que tiene que ejecutar han 
de inicializarse cuando el resto ya estén preparados. Por otro lado el segundo script inicializa el Bluetooth y crea la conexión con el 
respectivo Dongle por lo que la preferencia es mayor.

El tercer script es para apagar el módulo, por lo que se tendrá que ejecutar en los “runlevel” de apagado y reiniciado, 0, 1 y 6.

	update-rc.d stop-rpi stop 01 0 1 6 .

Es de gran importancia hacer este paso el último para evitar conflictos. Es decir tener instalados todos los programas correspondientes, sobre todo los de Bluetooth y mySQL.

3.	Programa sistema M2M

Una vez configurado todo el sistema ya se puede empezar a utilizar el programa. Si se utiliza el mismo modelo de Raspberry PI no hará 
falta compilar los programas de nuevo ya que utilizan la misma arquitectura. Únicamente tiene especial interés la modificación que 
hay que hacer en la librería “arduPi” en el caso de que se descargue de nuevo, ya que la incluida en este proyecto ya está modificada.
En la línea 64 del fichero “arduPi.cpp” habrá que modificarla de la siguiente manera:
	serialPort="/dev/ttyAMA0";
Esta será la única modificación que habrá que realizar. En cuanto a la compilación del programa se incluyen los comandos a utilizar
en el fichero COMPILAR.txt incluido en el proyecto.

4.	Aspectos eléctricos y hardware


### Lista de programas y liberías ###

	PROGRAMA	DESCRIPCIÓN

###Telemetría y diagnóstico (OBDII)###
	applicationPID	
		•	Configuración comunicación con el Dongle.
		•	Peticiones de los PIDs.
		•	Peticiones DTCs.
		•	Almacenamiento de los datos en base de datos mySQL local.

###Scripts###
	start-rpi.sh	
		•	Inicia aplicación “appplicationPID”
	start-btz.sh
		•	Establece conexión bluetooth
		•	Reinicia conexión con puerto serie “rfcomm0”

	LIBRERÍAS	DESCRIPCIÓN

###GSM y Geolocalización (SistemaM2M)###
	arduPi
		•	Comunicación con el módulo sim908.
	libRPI
		•	Comunicación con el puerto serie ttyAMA0.
		•	Tratamiento de datos.
		•	Control de acceso al medio.
	libmySQL
		•	Comunicación con la tabla de datos de localización en la base de datos mySQL

###Telemetría y diagnóstico (OBDII)###
	arduPi	
		•	Comunicación con el módulo sim908.
	libPORT
		•	Comunicación con el puerto serie “rfcomm0”
	libOBDII
		•	Tratamiento de datos.
		•	Descifrado de datos.
	libmysqlOBDII
		•	Comunicación con la tabla de datos de telemetría en la base de datos mySQL


### Who do I talk to? ###

* Javier Fernandez Pastrana
* jferpas@gmail.com
