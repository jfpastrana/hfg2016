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


#include "arduPi.h"
#include "libPORT.h"
#include "libmysqlOBDII.h"
#include "libOBDII.h"

#include <stdio.h>
#include <string.h>

carInfo auxStructCar;

void setup(){

	Serial.begin(115200);
	delay(2000);
}

void setupOBD(){


	printf("\nInciando configuración OBDII.....\n");
	printf("\nReseteando.....");
	Serial.flush();
	Serial.println("ATZ");
	delay(1000);
	printf("\nLinefeeds ON.....");
    	Serial.flush();
    	Serial.println("ATL1");
    	delay(1000);
    	printf("\nCabeceras ON.....");
    	Serial.flush();
    	Serial.println("ATH1");
    	delay(1000);
	printf("\nImprimir espacios.....");
    	Serial.flush();
    	Serial.println("ATS1");
    	delay(1000);
    	printf("\nConfigurando protocolo.....");
    	Serial.flush();
    	Serial.println("ATSP0");
    	delay(1000);
}

/* Metodo que configura el tiempo de espera de respuesta
* del Dongle OBD, y lo establece en el maximo tiempo, 1 segundo.
*/

void setupTimeOBD(){

	printf("\nAumentando el tiempo de espera de respuesta.....");
	Serial.flush();
	Serial.println("ATJTM5");
	delay(1000);
	PORT.waitforOK();
	printf("\nAl máximo tiempo permitido.....");
	Serial.flush();
	Serial.println("ATSTFF");
	delay(1000);
	PORT.waitforOK();
}



void storePID(){

	char buffer[200]="";
	char auxbuffer[10]="";
	char resultado[20]="";
	int i=0, timer=1, j=0, k=0;
	char pid[3]="";
	char idsql[5] = "";
	char aux[20]="";
	int t=0;

	do{
	        /* Pido PID 0104 DRIVER'S DEMAND ENGINE- */
        	printf("\nPediendo PID 1.....");
        	Serial.flush();
        	Serial.println("0104");
        	strcpy(buffer,PORT.read());
        	strcpy(auxbuffer,"04");
        	strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,1));
        	strcpy(aux,OBD.tratarPID(resultado,auxbuffer,1));
		if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
		strcpy(auxStructCar.engine,aux);

        	/* ENGINE COOLANT TEMPERATURE */
        	printf("\nPediendo PID 2.....");
        	Serial.flush();
        	Serial.println("0105");
        	strcpy(buffer,PORT.read());
        	strcpy(auxbuffer,"05");
        	strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,1));
        	strcpy(aux,OBD.tratarPID(resultado,auxbuffer,1));
        	if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
        	strcpy(auxStructCar.ctemp,aux);

        	/* INTAKE MANIFOLD PREASSURE */
        	printf("\nPediendo PID 3.....");
        	Serial.flush();
        	Serial.println("010B");
        	strcpy(buffer,PORT.read());
        	strcpy(auxbuffer,"0B");
        	strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,1));
        	strcpy(aux,OBD.tratarPID(resultado,auxbuffer,1));
        	if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
        	strcpy(auxStructCar.inyeccion,aux);

		/* Pido el primer PID 010C RPM */
        	printf("\nPediendo PID 4.....");
        	Serial.flush();
        	Serial.println("010C");
        	strcpy(buffer,PORT.read());
        	strcpy(auxbuffer,"0C");
        	strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,2));
        	strcpy(aux,OBD.tratarPID(resultado,auxbuffer,2));
        	if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
        	strcpy(auxStructCar.rpm,aux);

      		/* Pido PID 010D VEHICLE SPEED */
		/* Tambien se pide la velocidad media, sin PID */
		printf("\nPidiendo PID 5.....");
		Serial.flush();
		Serial.println("010D");
		strcpy(buffer,PORT.read());
		strcpy(auxbuffer,"0D");
    
	    	/* Velociad se almacena directamente */
   		strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,1));	
		if(strcmp(resultado,"0")==0) strcpy(resultado,"0.00");
		strcpy(auxStructCar.speed,resultado);
	
    		/* Velocidad Media */
		strcpy(aux,OBD.tratarPID(resultado,auxbuffer,1));
        	if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
        	strcpy(auxStructCar.aspeed,aux);

		/* Pido PID 0110 MAF AIR FLOW RATE */
        	printf("\nPediendo PID 6.....");
        	Serial.flush();
        	Serial.println("0110");
        	strcpy(buffer,PORT.read());
        	strcpy(auxbuffer,"10");
        	strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,2));
        	strcpy(aux,OBD.tratarPID(resultado,auxbuffer,2));
        	if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
        	strcpy(auxStructCar.maf,aux);

        	/* Pido PID 0111 THROTTLE POSITION */
        	printf("\nPediendo PID 7.....");
        	Serial.flush();
        	Serial.println("0111");
        	strcpy(buffer,PORT.read());
        	strcpy(auxbuffer,"11");
        	strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,1));
        	strcpy(aux,OBD.tratarPID(resultado,auxbuffer,1));
		if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
		strcpy(auxStructCar.throttle,aux);

        	/* Pido PID 0123 FUEL RAIL PRESSURE */
        	printf("\nPediendo PID 8.....");
        	Serial.flush();
        	Serial.println("0123");
        	strcpy(buffer,PORT.read());
        	strcpy(auxbuffer,"23");
        	strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,2));
        	strcpy(aux,OBD.tratarPID(resultado,auxbuffer,2));
        	if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
        	strcpy(auxStructCar.rail,aux);

        	/* CALCULO DEL PORCENTAJE DE CONDUCCION EFICIENTE - DIESEL, PID PROPIO FE */
        	printf("\nCalculando porcentaje de conduccion eficiente DIESEL%.....");
        	strcpy(auxbuffer,"FE");
        	strcpy(resultado,"FE");
        	strcpy(aux,OBD.tratarPID(resultado,auxbuffer,0));
        	if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
        	strcpy(auxStructCar.eficdiesel,aux);

		/*CALCULO DEL PORCENTAJE DE CONDUCCION EFICIENTE - GASOLINA, PID PROPIO FD */
      		printf("\nCalculando porcentaje de conduccion eficiente GASOLINA%.....");
        	strcpy(auxbuffer,"FD");
        	strcpy(resultado,"FD");
        	strcpy(aux,OBD.tratarPID(resultado,auxbuffer,0));
        	if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
        	strcpy(auxStructCar.eficgas,aux);

		if(t==4){
			/* Pido PID 010F INTAKE AIR TEMPERATURE */
            		printf("\nPediendo PID 9.....");
            		Serial.flush();
            		Serial.println("010F");
            		strcpy(buffer,PORT.read());
            		strcpy(auxbuffer,"0F");
            		strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,1));
            		strcpy(aux,OBD.tratarPID(resultado,auxbuffer,1));
            		if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
            		strcpy(auxStructCar.intake_air,aux);

           		/* RUN TIME SINCE ENGINE START*/
           		printf("\nPediendo PID 10.....");
           		Serial.flush();
           		Serial.println("011F");
           		strcpy(buffer,PORT.read());
           		strcpy(auxbuffer,"011F");
           		strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,2));
           		strcpy(aux,OBD.tratarPID(resultado,auxbuffer,2));
           		if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
           		strcpy(auxStructCar.run_time,aux);

			/* DISTANCE TRAVLLED WITH "MIL"*/
			printf("\nPediendo PID 11.....");
        		Serial.flush();
        		Serial.println("0121");
        		strcpy(buffer,PORT.read());
        		strcpy(auxbuffer,"021");
        		strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,2));
			strcpy(aux,OBD.tratarPID(resultado,auxbuffer,2));
           		if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
           		strcpy(auxStructCar.mil,aux);

           		/* Pido PID 0124 02S1_WR_LAMBDA EQUIVALENCE RAIO VOLTAGE */
           		printf("\nPediendo PID 12.....");
           		Serial.flush();
           		Serial.println("0124");
           		strcpy(buffer,PORT.read());
           		strcpy(auxbuffer,"24");
           		strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,4));
           		strcpy(aux,OBD.tratarPID(resultado,auxbuffer,4));
           		if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
           		strcpy(auxStructCar.lambda_o2s1_v,aux);

          		/* Pido PID 012B 02S8_WR_LAMBDA EQUIVALENCE RAIO VOLTAGE */
           		printf("\nPediendo PID 13.....");
           		Serial.flush();
           		Serial.println("012B");
           		strcpy(buffer,PORT.read());
           		strcpy(auxbuffer,"2B");
           		strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,4));
           		strcpy(aux,OBD.tratarPID(resultado,auxbuffer,4));
           		if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
           		strcpy(auxStructCar.lambda_o2s8_v,aux);

           		/* Pido PID 012C COMMANDED EGR */
           		printf("\nPediendo PID 14.....");
           		Serial.flush();
           		Serial.println("012C");
           		strcpy(buffer,PORT.read());
           		strcpy(auxbuffer,"2C");
           		strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,1));
           		strcpy(aux,OBD.tratarPID(resultado,auxbuffer,1));
           		if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
           		strcpy(auxStructCar.egr,aux);

           		/* FUEL LEVEL INPUT*/
           		printf("\nPediendo PID 15.....");
           		Serial.flush();
           		Serial.println("012F");
          		strcpy(buffer,PORT.read());
           		strcpy(auxbuffer,"02F");
           		strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,1));
           		strcpy(aux,OBD.tratarPID(resultado,auxbuffer,1));
           		if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
           		strcpy(auxStructCar.fuel_level,aux);

       			/* Pido PID 0136 02S3_WR_LAMBDA EQUIVALENCE RAIO CURRENT */
           		printf("\nPediendo PID 16.....");
           		Serial.flush();
           		Serial.println("0136");
           		strcpy(buffer,PORT.read());
           		strcpy(auxbuffer,"36");
           		strcpy(resultado,OBD.dividirPID(buffer,auxbuffer,4));
           		strcpy(aux,OBD.tratarPID(resultado,auxbuffer,4));
           		if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
          		strcpy(auxStructCar.lambda_o2s3_c,aux);

           		/* CALCULO DE CONSUMO A LOS 100KM, PID PROPIO FF */
          		printf("\nCalculando consumo a los 100km.....");
           		strcpy(auxbuffer,"FF");
			strcpy(resultado,"FF");
           		strcpy(aux,OBD.tratarPID(resultado,auxbuffer,0));
    			if(strcmp(aux,"0")==0) strcpy(aux,"0.00");
			strcpy(auxStructCar.consumo,aux);

			/* GUARDANDO DTCs */
			printf("\nConsultando DTCs.....");
			Serial.flush();
			Serial.println("03");
			strcpy(buffer,PORT.read());
			strcpy(auxStructCar.dtc,OBD.showDTC(buffer));

			/* Insertamos ID para mySQL */
			sprintf(idsql,"%d",timer);
			strcpy(auxStructCar.id,idsql);
			printf("\nID: %s",auxStructCar.id);
			timer++;
	
			/* Insertamos la informacion en la BBDD SQL */
			SQLOBD.insertarCarInfo(auxStructCar);
			printf("\nInsertados datos SQL\n\n");
			t=0;
			delay(1000);
		}
	t++;
	}while(t!=5);
}

int main(){

	setup();
	delay (1000);
	setupOBD();
	delay(1000);
	storePID();

	return (0);
}
