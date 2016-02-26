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


	/* Comunicación con el puerto serie rfcomm0 */


#include <stdio.h>
#include "libPORT.h"
#include "arduPi.h"

libPORT::libPORT(){

}

	/*Muestra por pantalla el contenido del buffer del Puerto
	 * Serie, el puerto serie se vacia.
	 */
void libPORT::print(){

	char  serial_read[200]="";
	int answer = 0, i = 0, buffer = 0, timer = 0;
	long previous;
	previous = millis();
    	do{
		if(Serial.available() != 0){
            		serial_read[i] = Serial.read();
          		i++;
        	}
        	if (strstr(serial_read,">") != NULL){
        		answer=1;
		}
    	}while(answer==0 && ((millis()-previous)<5000));
    	printf("\n---------------------------------------------",serial_read);

	if( strstr(serial_read,"OK")!=NULL){
		printf("\nComando recibido correctamente");
		printf("\nRespuesta: %s",serial_read);
	}
	if( strstr(serial_read,"?") != NULL){
		printf("\nComando NO reconocido");
	}
	delay(1000);
}

	/*Recoge el contenido del Puerto serie y lo alamacena en un
	 * array de caracteres, posteriormente se devuelve mediante
	 * paso por funcion como un puntero tipo char.
	 */

char * libPORT::read(){

	char serial_read[200]="";
	char fullInfo[200]="";
	char *pserial_read = NULL;
	int i=0, answer=0, timer = 0;
	long previous;

	previous = millis();
	while(Serial.read()!='\n' && ((millis()-previous)<10000));

	previous = millis();
	do{
		if(Serial.available()!=0){
			serial_read[i]=Serial.read();
			i++;
		}
		if ((serial_read[i] == '?')){
			printf("\nComando NO reconocido");
			answer=1;
		}
		if(strstr(serial_read, ">") !=NULL){
			answer = 1;
		}
	}while(answer==0 && ((millis()-previous)<5000));
	strcpy(fullInfo,serial_read);
	pserial_read=fullInfo;
	return pserial_read;
}


void libPORT::waitforOK(){


	char serial_read[200]="";
	int i = 0, answer = 0;
	long previous;

	previous = millis();
	do{
		if(Serial.available()!=0){
			serial_read[i] = Serial.read();
			i++;
		}
		if(strstr(serial_read,"OK")!=NULL) {
			printf("\nPeticion enviada correctamente");
			answer = 1;
		}
		if(strstr(serial_read, "?")!=NULL){
			printf("\nPeticion enviada INcorrectamente");
			answer = 1;
		}
	}while (answer == 0 && ((millis()-previous)<5000));

}

libPORT PORT = libPORT();

