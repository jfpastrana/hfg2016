/*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  Version 1
*  Author: Javier Fernández Pastrana
*/


/* Librería libRPI. Librería que contiene funciones y metodos necesarios
* para la comunicación con el puerto serie. Así como el tratamiento de esos
* datos, ya sea extrayendolos a convirtiendolos a unidades del Sistema Internacional.
* También incluye las funciones necesarias para el control de acceso al medio, mediante
* el fichero port.txt.
*/


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "libRPI.h"
#include "arduPi.h"


/* Definición de variables globales, necesarias para poder almacenar
* su valor en posteriores accesos a los metodos implementados en esta
* librería.
*/

int numid=0;
double latitud=0;
double longitud=0;
double km=0;



	/******************
 	* Metodos privados *
 	******************/

	/* Metodo que convierte la hora en formato
	* UTC a hora local. +1 horas.
	* ejem: entrada 121024.00 - salida 141024
 	* Entrada dato tipo char salida tipo char
	*/

char * convert2time(char * informacion){

	char data[20]="";
	char auxdata[20]="";
	int numero=0;
	char entero[20]="";
	float decimal=0;
	char * punt=NULL;
	char * ptr = NULL;

	/* Convertimos UTCTime a hora local(Horario de verano) */
	strcpy(data,informacion);
	ptr=strtok(data,".");
	numero = atoi(ptr);

	numero = numero+10000;
	sprintf(auxdata,"%d",numero);

	punt=auxdata;
	return punt;
}

	/* Metodo que convierte la velocidad en nudos
	* o millas nauticas a kilometros por hora.
	* ejem: entrada: 100 salida: 185,2
	* Entrada dato tipo char salida tipo char
	*/

char * convert2kmh(char * informacion){

	char data[20]="";
	char auxdata[20]="";
   	float numero=0;
   	char entero[20]="";
   	float decimal=0;
   	char * punt=NULL;

	/* Convertimos la velocidad de nudos a km/h */
   	strcpy(data,informacion);

	numero = atof(data);
	numero = numero * 1.852;
   	sprintf(auxdata,"%f",numero);

	punt=auxdata;
   	return punt;
}

	/* Convierte la longitud y la latitud a
	* a grados, formato requerido por google.
	* Entrada: ddmm.mmmmmm - Salida dddd.dddddd-
	* Entrada dato tipo char salida tipo char.
	*/

char * convert2degrees(char * posicion){

	float deg=0, minutes=0,numaux=0;
   	char input[20]="";
	char * pposicion=NULL;
	char signo[20]="";
   	char aux[20]="";

   	strcpy(input,posicion);
	strcpy(signo,"-");

   	if(strstr(input,"W")!=NULL||strstr(input,"S")!=NULL){
   		strcat(signo,input);
       		strcpy(input,signo);
   	}

   	numaux = atof(input);
   	deg=int(numaux)/100;
   	minutes=numaux - deg*100;
   	deg=deg+minutes/60;
   	sprintf(aux,"%f",deg);
	pposicion=aux;
	return pposicion;
}

	/* Método que calcula la distancia entre dos
	* puntos, expresados en grados mediante el
	* metodo de Haversine. Así como los
	* kilometros recorridos desde el inicio del
	* módulo GPS.
	* Entrada dato tipo gprm y salida gprm. Tipo
	* de datos definido en libRPI.h.
	*/

gprm calculateKm(gprm auxstruct){

	/* Ejemplo del formato latitud y longitud
	*double lat1=41.653378,lat2=41.661190,long1=-4.737745,long2=-4.710999;
	*/
	double lat1=0 , lat2=0, long1=0, long2=0;
	double distance = 0;
	char aux[20]="";
	char * pkm = NULL;
	double copiakm=0;


	strcpy(aux,auxstruct.latitude);
	lat1 = atof(aux);
	strcpy(aux,auxstruct.longitude);
	long1 = atof(aux);

	lat2 = latitud;
	long2 = longitud;

	copiakm = km;

	if(lat2==0 && long2==0){
		distance = 0;
	}
	else {
		double PI = 4.0*atan(1.0);
       	double dlat1=lat1*(PI/180);
       	double dlong1=long1*(PI/180);
      	double dlat2=lat2*(PI/180);
       	double dlong2=long2*(PI/180);
       	double dLong=dlong1-dlong2;
      	double dLat=dlat1-dlat2;
       	double aHarv= pow(sin(dLat/2.0),2.0)+cos(dlat1)*cos(dlat2)*pow(sin(dLong/2),2);
       	double cHarv=2*atan2(sqrt(aHarv),sqrt(1.0-aHarv));
 		/* Radio de la Tierra */
       	const double earth=6378.135;
       	distance=earth*cHarv;
	}

	latitud = lat1;
	longitud = long1;
	km = km + distance;

	sprintf(auxstruct.distance,"%f",distance);
	sprintf(auxstruct.km,"%f",km);
	printf("\n");
	system("printf \"%b\" \"\e[1;35mKM desde la ultima muestra:\e[0m\"");
	printf("\t%s", auxstruct.km);
	/* preotección frente a errores, si la distancia
	* entre dos puntos es mayor que 100 significa que
	* las posiciones recogidas no son válidas.
	*/
	if(distance>100) km = copiakm;
	return auxstruct;

}

	/******************
 	* Métodos públicos *
 	******************/


/* Constructor */
libRPI::libRPI(){

}


        /* Metodo para comprobar la respuesta HTTP
        * enviada. Normalmente seraก utilizado despues del
        * at command AT+HTTPACTION=0 automaticamente
        * se responde con +HTTPACTION:0,200,X donde X son
        * el numero de Bytes que se han recibido y estan disponibles
        * para leer.
        */


int libRPI::httpAnswer(){

        char serial_read[200]="";
        int i = 0, answer = 0;
        unsigned long previous;

		//printf("\n aqui");
        //printf("\n");
        previous = millis();
        //printf("\naqui");
        //printf("\n");
        //while(Serial.read()!=':' && ((millis() - previous)<10000));

        previous = millis();
        do{
        	if(Serial.available()!=0){
            	serial_read[i] = Serial.read();
                i++;
            }
            if(strstr(serial_read,"200")!=NULL) {
            	printf("\n\t");
               	system("printf \"%b\" \"\e[1;32mHTTP code 200: OK\e[0m\"");
		printf("\n");
		answer = 1;
                return (0);
            }
            if(strstr(serial_read, "600")!=NULL){
            	printf("\n\t");
				system("printf \"%b\" \"\e[1;31mHTTP code 600: NOT HTTP PDU\e[0m\"");
                answer = 1;
                return (1);
            }
           	if(strstr(serial_read, "404")!=NULL){
            	printf("\n\tHTTP code 404: NOT FOUND");
                system("printf \"%b\" \"\e[1;31mHTTP code 404: NOT FOUND\e[0m\"");
				answer = 1;
                return (1);
            }
        }while ((millis() - previous) < 10000);
}


	/*Función que comprueba si el puerto serie ttyAMA0
	* esta en uso mediante la comprobación del fichero
	* port.txt, sí en este fichero hay un 1 querra decir
	* que el puerto esta ocupado y por lo tanto esperará
	* dos segundos en volver a preguntar, así hasta que
	* haya un 0.
	* Devuelve un dato tipo int, para confirmar si el acceso
	* ha sido exitoso o no.
	*/


int libRPI::checkport(){

	FILE *fichero;
   	char nombre[50]="/home/ArduPi/blueberry/port.txt";
	int count=0;
	int estado=8;


	printf("\n");
	system("printf \"%b\" \"\e[1;33mComprobando uso puerto ttyAMA0\e[0m\"");
	while(estado!=48){

		fichero = fopen(nombre,"r");
		if(fichero){
	       	estado = fgetc(fichero);
       	}
      	else	return 1;
       	
       	if(!fclose(fichero)){
	   		printf("\n");
		}
        else	return 1;
       	
		if(count>0 && (estado==49 || estado==-1)){
			printf("\n");
			system("printf \"%b\" \"\e[1;33mEsperando 2 segundo a que el puerto quede libre\e[0m\"");
			delay(2000);
		}
		count++;
	}

	printf("\n");
    system("printf \"%b\" \"\e[1;33mPuerto libre!  Ocupando.....\e[0m\"");
	
	fichero = fopen(nombre,"w+");
    if(fichero){
		fputc('1',fichero);
    }
    else	return 1;

	if(!fclose(fichero)){
		printf("\n");
    	system("printf \"%b\" \"\e[1;33mPuerto ocupado!\e[0m\"");
   	}
	else	return 1;
	return 0;
}


	/* función encargada de escribir un 0 en el fichero
	* de configuración del puerto serie ttyAMA0. De tal forma
	* que libera el uso del puerto para que pueda acceder
	* otro programa.
	* Devuelve un dato tipo int, para confirmar si el acceso
	* ha sido exitoso o no.
	*/

int libRPI::setport(){

	FILE *fichero;
   	char nombre[50]="/home/ArduPi/blueberry/port.txt";
	int estado=8;

	printf("\n");
    system("printf \"%b\" \"\e[1;33mLiberando puerto ttyAMA0.....\e[0m\"");
	fichero = fopen(nombre,"w+");
    if(fichero){
    	fputc('0',fichero);
    }
    else	return 1;
   	if(!fclose(fichero)){
		printf("\n");
    		system("printf \"%b\" \"\e[1;33mPuerto liberado!\e[0m\"");
    }
	else	return 1;
	return 0;
}


	/* Funcion que espera a que se reciba el OK tras
	* enviar un at command o la respuesta contraria.
	* Devuelve un 0 si se ha recibido el OK y un 1 en
	* caso contrario.
	*/


int libRPI::wait4OK(){

	char serial_read[1000]="";
    	int i = 0, answer = 0;
	long previous;

	previous=millis();
	while( (Serial.read()!='\n') && ((millis()-previous)<10000) );

	previous=millis();
    	do{
    		if(Serial.available()!=0){
        		serial_read[i] = Serial.read();
            		i++;
        	}
        	if(strstr(serial_read,"OK")!=NULL) {
        		answer = 1;
			return (0);
        	}
        	if(strstr(serial_read, "ERROR")!=NULL){
        		answer = 1;
			return (1);
        	}
	}while (answer == 0 && ((millis()-previous)<5000));

}


	/*Muestra por pantalla el contenido del buffer del Puerto
	 * Serie, el puerto serie se vacia.
	 */
void libRPI::print(){

	char  serial_read[200]="";
	int answer = 0;
	int i = 0;
	int buffer = 0;
	long previous;

	previous=millis();
	while ((Serial.read()!=':') && (millis()-previous)<10000);

	previous=millis();
    do{
		if(Serial.available() != 0){
    		serial_read[i] = Serial.read();
        	i++;
        }
        if(strstr(serial_read, "OK") != NULL){
  			answer=1;
	    }
    }while( answer==0 && ((millis()-previous)<5000));

    printf("\n\n\t\t");
    system("printf \"%b\" \"\e[1;34mEstado.....\e[0m\"");
	printf("\n\n\t\t %s\n",serial_read);
	delay(1000);
}


	/* Recoge el contenido del Puerto serie y lo alamacena en un
	 * array de caracteres, posteriormente se devuelve mediante
	 * paso por función como un puntero tipo char.
	 */

char * libRPI::read(){


	char serial_read[200]="";
	char fullInfo[200]="";
	char *pserial_read = NULL;
 	int i=0, answer=0;
	long previous;

	previous=millis();

	while( (Serial.read()!='\n')  && ((millis()-previous)<10000));
	previous=millis();

	do{
		if(Serial.available()!=0){
			serial_read[i]=Serial.read();
			i++;
		}
		if (strstr(serial_read,"OK")!=NULL){
			answer=1;
		}
	}while(answer==0 && ((millis() - previous)<5000));

	strcpy(fullInfo,serial_read);

	pserial_read=fullInfo;
	return pserial_read;
}


	/*Recibe mediante paso por refeerencia el contenido completo del buffer
	 * para procesarlo, en este caso se dividira el contenido para
	 * almacenar el numero de telefono que ha enviado el sms y el cuerpo
	 * de dicho sms, posteriormente se alamacena en una estructura que
	 * sera devuelta mediante paso por funcion.
	 */

sms libRPI::dividirSMS(char *cadena){

	sms structSmsInfo;

	char buffer[200]="";
	char cuerpo[200]="";
	char auxEstado[20]="";
	char numTel[10]="";
	char auxNumTel[13]="";
	char *ptr=NULL;
	int counter=0;

	strcpy(buffer,cadena);
	ptr = strtok(buffer,"\"");

	while(ptr!=NULL){

		if(counter==1) strcat(auxEstado,ptr);

		if (counter==3)	strcat(auxNumTel,ptr);

		if (counter==8)	strcat(cuerpo,ptr);

		ptr = strtok(NULL,"\"");
		counter++;
	}

	for(counter=3;counter<12;counter++){
		numTel[counter-3]=auxNumTel[counter];
	}

	strcpy(structSmsInfo.estado,auxEstado);

	strcpy(structSmsInfo.telefono,numTel);

	strcpy(structSmsInfo.sms_text,cuerpo);

	return(structSmsInfo);
}


	/*Recibe mediante paso por refeerencia el contenido completo del buffer
	 * para procesarlo, en este caso se dividira el contenido para
	 * almacenar toda la información de GPS en el tipo de dato gprm
	 * definido en libRPI y con los campos que manda el protocolo
	 * NMEA para mensajes gprm. Definido en el manual.
	 */


gprm libRPI::dividirGPRM(char *cadena){

	gprm structGpsInfo;
	gprm structGprm;
	char buffer[200]="";
	char *ptr=NULL;
	int counter=0;
	char acadena[10]="";
	char auxbuffer[200]="";
	numid++;

	strcpy(buffer,cadena);


	/* Introduzco ID a mano */
	//sprintf(auxbuffer,"%d",numid);
	//strcat(auxbuffer," ");
	strcat(auxbuffer,buffer);
	//printf("\nBUFFER:%s",auxbuffer);

	ptr = strtok(auxbuffer," ");

	while(ptr!=NULL){
//		printf("\nCOUNTER:%d --- PTR:%s",counter,ptr);
		switch(counter){
			case 0: strcpy(structGpsInfo.id,ptr); break;
			case 1: strcpy(structGpsInfo.fecha_rpi,ptr); break;
			case 2: strcpy(structGpsInfo.date,ptr); break;;
			case 3:	strcpy(structGpsInfo.UTCtime , ptr);break;
			case 4: strcpy(structGpsInfo.latitude, ptr); break;
			case 5: strcpy(structGpsInfo.N_S_indicator,ptr); break;
			case 6: strcpy(structGpsInfo.longitude , ptr); break;
			case 7: strcpy(structGpsInfo.E_W_indicator, ptr); break;
			case 8: strcpy(structGpsInfo.speed, ptr); break;
			case 9: strcpy(structGpsInfo.Status,ptr); break;
			case 10: strcpy(structGpsInfo.course, ptr); break;
			case 11: strcpy(structGpsInfo.distance, ptr); break;
			case 12: strcpy(structGpsInfo.km, ptr); break;
		}
	//	printf("\nPTR: %s",ptr);
		ptr = strtok(NULL," ");
		counter++;

	}

//	printf("LIBRPI.CPP \nc=%s\nd=%s\ne=%s\nf=%s\ng=%s\nh=%s\ni=%s\nj=%s\nk=%s\nl=%s\nm=%s\nn=%s\no=%s",structGpsInfo.id,structGpsInfo.fecha_rpi,structGpsInfo.date,structGpsInfo.UTCtime,structGpsInfo.latitude,structGpsInfo.N_S_indicator,structGpsInfo.longitude,structGpsInfo.E_W_indicator,structGpsInfo.speed,structGpsInfo.Status,structGpsInfo.course,structGpsInfo.distance,structGpsInfo.km);

	/* A continuación se realizan las conversiones a sistema internacional
	* de los datos obtenidos, así como calculos derivados.
	*/
        //printf("\nLIBRPI FINAL1 %s",structGpsInfo.latitude);
	//strcpy(structGpsInfo.longitude,convert2degrees(structGpsInfo.longitude));
    	//strcpy(structGpsInfo.latitude,convert2degrees(structGpsInfo.latitude));
	//structGpsInfo = calculateKm(structGpsInfo);
	//strcpy(structGpsInfo.speed,convert2kmh(structGpsInfo.speed));
    	//strcpy(structGpsInfo.UTCtime,convert2time(structGpsInfo.UTCtime));
	//printf("\nLIBRPI FINAL2 %s",structGpsInfo.latitude);

	return(structGpsInfo);
}





libRPI RPI = libRPI();
