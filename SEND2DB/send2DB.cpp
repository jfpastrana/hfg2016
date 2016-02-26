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
#include "libRPI.h"
#include "libOBDII.h"
#include "libmysqlRPI.h"
#include <stdio.h>
#include <stdlib.h>

	/* Envio de datos median HTTP, utlizando formulario,
	* de datos previamente leidos de una tabla mySQL local.
	*/
void send2DB(){

	carInfo structCar;
	char str[1000]="";
	char aux_str[1000]="";
	char buffer[200]="";
	char command[500];
	char empty[5]="";
	strcpy(empty,"0000");
	int timer=0;
	int numfilasobd=0;

	do{
  		numfilasobd=SQL.numFilasTablaOBD();
		if(numfilasobd!=0){
			/* --OBD-- Recogemos los datos de la tabla SQL */
			structCar = SQL.leerObdSQL();

			strcpy(command,"curl -H \"Content-Type: application/json\" -X POST -d '{\"eficiencia\":\"");
			strcat(command, structCar.eficdiesel);
			strcat(command,",\"codigo\":");
			strcat(command, structCar.eficgas);
			strcat(command,"}' http:/");
			strcat(command, "/efficiency.pagekite.me/efficiency/api/sample.php");
			system(command);

/* ANTIGUO
			printf("\ntelefono=%s\nid_viaje_obd=%s\nfecha_rpi_obd=%s\nvelocidad_obd=%s\nvelocidad_media=%s\nrpm=%s\ntmotor=%s\nmotor=%s\nMAF=%s\nMIL=%s\ninyeccion_directa=%s\ninyeccion=%s\nconsumo=%s\neficdiesel=%s\neficgas=%s\nDTC=%s\nthrottle=%s\nintake_air=%s\nlambda_o2s1_v=%s\nlambda_o2s8_v=%s\negr=%s\nlambda_o2s3_c=%s\nrun_time=%s\nfuel_level=%s",telefono,structCar.id,structCar.fecha_rpi,structCar.speed,structCar.aspeed,structCar.rpm,structCar.ctemp,structCar.engine,structCar.maf,structCar.mil,structCar.rail,structCar.inyeccion,structCar.consumo,structCar.eficdiesel,structCar.eficgas,structCar.dtc,structCar.throttle,structCar.intake_air,structCar.lambda_o2s1_v,structCar.lambda_o2s8_v,structCar.egr,structCar.lambda_o2s3_c,structCar.run_time,structCar.fuel_level);
			printf("\n");
			system("printf \"%b\" \"\e[1;34mEnviando datos de OBD\e[0m\"");
			strcpy(str,"AT+HTTPPARA=\"URL\",\"http://jfpastrana.com/blueberry/insertobd.php?");
			sprintf(aux_str,"a=%s&b=%s&c=%s&d=%s&e=%s&f=%s&g=%s&h=%s&i=%s&j=%s&k=%s&l=%s&m=%s&n=%s&o=%s&p=%s&q=%s&r=%s&s=%s&t=%s&u=%s&v=%s&w=%s&x=%s&z=%d",telefono,structCar.id,structCar.fecha_rpi,structCar.speed,structCar.aspeed,structCar.rpm,structCar.ctemp,structCar.engine,structCar.maf,structCar.mil,structCar.rail,structCar.inyeccion,structCar.consumo,structCar.eficdiesel,structCar.eficgas,structCar.dtc,structCar.throttle,structCar.intake_air,structCar.lambda_o2s1_v,structCar.lambda_o2s8_v,structCar.egr,structCar.lambda_o2s3_c,structCar.run_time,structCar.fuel_level,pin);
			strcat(str,aux_str);
			strcat(str,"\"");
			delay(500);
			Serial.println(str);
*/

			/* Eliminamos de la tabla SQL GPS la fila enviada */
			 SQL.eliminarFilaOBD(structCar.fecha_rpi);

		}

		/* Tabla mySQL esta vacia! */
		else{
			printf("\n");
			system("printf \"%b\" \"\e[1;34mBase de dato de OBD vacia.\e[0m\"");
			printf("\n");
			delay(100);
		}
		delay(2500);
	}while(timer!=5);
}


int main(){

	printf("\n");
	system("printf \"%b\" \"\e[1;34mIniciando aplicacion para enviar la informacion mediante CURL.\e[0m\"");
	send2DB();
	return (0);
}

