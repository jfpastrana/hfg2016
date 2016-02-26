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



#include <stdio.h>
#include <stdlib.h>
#include "arduPi.h"
#include "libRPI.h"
#include <my_global.h>
#include <mysql.h>
#include "libmysqlRPI.h"
#include <string.h>
#include "libOBDII.h"
#include <math.h>


int counter = 1;
int ultimafilagps=0;
int ultimafilaobd=0;


	/* Convierte un numero en una cadena char en
	* una variable tipo int.
	*/


int char2dec(char *numero){

        int i=0, j=0, len=0,num=0,dec=0;
        char cadena[20]="";

        strcpy(cadena,numero);
        len=strlen(cadena);
        for(i=len-1; i>=0;i--){
                if(cadena[i] >= '0' && cadena[i] <= '9'){
                        num = cadena[i] - '0';
                }
                dec = dec + pow(10,j)*num;
                j++;
        }
	return dec;
}


	/* Metodo local para mostrar mensajes de error mySQL */

void finish_with_error(MYSQL *con){
        fprintf(stderr, "ERROR:%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
}


mysqlRPI::mysqlRPI(){

}

	/* Funcion que lee de la tabla gprmTable que contiene la informacion
	* alamacenada por el modulo GPS, para su posterior tratamiento, se
	* enviará para dividir la informacion y almacenarla cada campo en
	* su respectiva variable para que se devuelva una estructura con todos
	* los campos.
	*/


gprm	mysqlRPI::leerGpsSQL(){

	gprm structGps;
        char buffer[200]="";
	char auxbuffer[200]="";
	char idfila[5]="";

	int i = 0;

        MYSQL *con = mysql_init(NULL);

        if (con == NULL){
                fprintf(stderr, "mysql_init() failed\n");
                exit(1);
        }

        if (mysql_real_connect(con, "localhost", "javi", "1234","localDB", 0, NULL, 0) == NULL){
                finish_with_error(con);
        }


        if (mysql_query(con, "SELECT * FROM gprmTable")){
                finish_with_error(con);
        }

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL){
                finish_with_error(con);
        }

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;


        row = mysql_fetch_row(result);


	for(i = 0; i < num_fields; i++){

		/* A cada vuelta del FOR se almacena un campo de la tabla */
                sprintf(buffer,"%s ", row[i] ? row[i] : "NULL");
		/* Se concatena cada campo en una cadena */
//		printf("\nBUFFER MYSQLLIB %s",buffer);
		strcat(auxbuffer,buffer);

	}

	/* Cerramos la conexion con mySQL local */
        mysql_free_result(result);
        mysql_close(con);

	/* Separamos la informacion en campos y la almacenamos */
	structGps = RPI.dividirGPRM(auxbuffer);

//	printf("\n*********************************************************");
//	printf("&\nc=%s&\nd=%s&\ne=%s&\nf=%s&\ng=%s&\nh=%s&\ni=%s&\nj=%s&\nk=%s&\nl=%s&\nm=%s&\nn=%s&\no=%s",structGps.id,structGps.fecha_rpi,structGps.date,structGps.UTCtime,structGps.latitude,structGps.N_S_indicator,structGps.longitude,structGps.E_W_indicator,structGps.speed,structGps.Status,structGps.course,structGps.distance,structGps.km);

	//printf("\n\nKM1:%s",structGps.dtc);

	/* Ultimo ID leido */
	strcpy(idfila,structGps.id);
	ultimafilagps = char2dec(idfila);

	printf("\nLATITUD LIBMYSQL %s",structGps.latitude);

	return structGps;
}


carInfo    mysqlRPI::leerObdSQL(){

        carInfo structCar;
        char buffer[200]="";
        char insert[200]="";
        char * pinsert=NULL;
	char auxbuffer[200]="";
	char idfila[5]="";

        MYSQL *con = mysql_init(NULL);

        if (con == NULL){
                fprintf(stderr, "mysql_init() failed\n");
                exit(1);
        }

        if (mysql_real_connect(con, "localhost", "javi", "1234","localDB", 0, NULL, 0) == NULL){
                finish_with_error(con);
        }

        if (mysql_query(con, "SELECT * FROM obdTable")){
                finish_with_error(con);
        }

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL){
                finish_with_error(con);
        }

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;

        row = mysql_fetch_row(result);

        for(int i = 0; i < num_fields; i++){

                /* A cada vuelta del FOR se almacena un campo de la tabala */
                sprintf(buffer,"%s ", row[i] ? row[i] : "NULL");
                /* Se concatena cada campo en una cadena */
                strcat(auxbuffer,buffer);

        }
        mysql_free_result(result);
        mysql_close(con);


        /* Separamos la informacion en campos y la almacenamos */
        structCar = OBD.dividirInfoOBD(auxbuffer);


//	printf("\n\nDTC: %s",structCar.dtc);


        /* Ultimo ID leido */
        strcpy(idfila,structCar.id);
        ultimafilaobd = char2dec(idfila);

	return structCar;


}

int mysqlRPI::numFilasTablaGPS(){

        char buffer[200]="";
	int numfilas=0;

        MYSQL *con = mysql_init(NULL);

        if (con == NULL){
                fprintf(stderr, "mysql_init() failed\n");
                exit(1);
        }

        if (mysql_real_connect(con, "localhost", "javi", "1234","localDB", 0, NULL, 0) == NULL){
                finish_with_error(con);
        }


        if (mysql_query(con, "SELECT COUNT(*) FROM gprmTable")){
                finish_with_error(con);
        }

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL){
                finish_with_error(con);
        }

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;

        while ((row = mysql_fetch_row(result))){
                for(int i = 0; i < num_fields; i++){
                        sprintf(buffer,"%s ", row[i] ? row[i] : "NULL");
                  }
        }


	numfilas = char2dec(buffer);


	mysql_free_result(result);
        mysql_close(con);

	return numfilas;
}



int mysqlRPI::numFilasTablaOBD(){

        char buffer[200]="";
        int numfilas=0;

        MYSQL *con = mysql_init(NULL);

        if (con == NULL){
                fprintf(stderr, "mysql_init() failed\n");
                exit(1);
        }

        if (mysql_real_connect(con, "localhost", "javi", "1234","localDB", 0, NULL, 0) == NULL){
                finish_with_error(con);
        }


        if (mysql_query(con, "SELECT COUNT(*) FROM obdTable")){
                finish_with_error(con);
        }

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL){
                finish_with_error(con);
        }

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;

        while ((row = mysql_fetch_row(result))){
                for(int i = 0; i < num_fields; i++){
                        sprintf(buffer,"%s ", row[i] ? row[i] : "NULL");
                  }
        }


        numfilas = char2dec(buffer);

        mysql_free_result(result);
        mysql_close(con);

        return numfilas;
}





	/* Elimina la ultima fila leida de la tabla mySQL GPS
	* el valor de la fecha_rpi enviada como parametro.
	*/

void mysqlRPI::eliminarFilaGPS(char * fecha_rpi){

        char insert[200]="";
        char * pinsert=NULL;
	char fila_borrar[80]="";
	strcpy(fila_borrar,fecha_rpi);

        MYSQL *con = mysql_init(NULL);

        if (con == NULL){
                fprintf(stderr, "mysql_init() failed\n");
                exit(1);
        }

        if (mysql_real_connect(con, "localhost", "javi", "1234","localDB", 0, NULL, 0) ==NULL){
                finish_with_error(con);
        }

        if (mysql_query(con, "SELECT * FROM gprmTable")){
                finish_with_error(con);
        }

	MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL){
                finish_with_error(con);
        }

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;

        sprintf(insert,"DELETE FROM gprmTable WHERE fecha_rpi='%s'",fila_borrar);
        pinsert = insert;

        if (mysql_query(con,pinsert)){
                finish_with_error(con);
        }
     	mysql_free_result(result);
        mysql_close(con);
}


        /* Elimina la ultima fila leida de la tabla mySQL OBD
        * el valor de la fecha_rpi enviada com parametro.
        */

void mysqlRPI::eliminarFilaOBD(char * fecha_rpi){



        char insert[200]="";
        char * pinsert=NULL;
	char fila_borrar[80]="";
	strcpy(fila_borrar,fecha_rpi);

        MYSQL *con = mysql_init(NULL);

        if (con == NULL){
                fprintf(stderr, "mysql_init() failed\n");
                exit(1);
        }

        if (mysql_real_connect(con, "localhost", "javi", "1234","localDB", 0, NULL, 0) ==NULL){
                finish_with_error(con);
        }

        if (mysql_query(con, "SELECT * FROM obdTable")){
                finish_with_error(con);
        }

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL){
                finish_with_error(con);
        }

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;

        sprintf(insert,"DELETE FROM obdTable WHERE fecha_rpi='%s'",fila_borrar);
        pinsert = insert;

        if (mysql_query(con,pinsert)){
                finish_with_error(con);
        }

        mysql_free_result(result);
        mysql_close(con);
}



mysqlRPI SQL = mysqlRPI();

