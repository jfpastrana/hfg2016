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
#include "libPORT.h"
#include <my_global.h>
#include <mysql.h>
#include "libmysqlOBDII.h"
#include <string.h>


int counter = 1;


void finish_with_error(MYSQL *con){
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
}


mysqlOBDII::mysqlOBDII(){

}


void mysqlOBDII::insertarCarInfo(carInfo structCarInfo){


        /* Se consulta la hora local a la RPI, para introducirlo
        * como identificador en la tabla mySQL.
        */

        time_t tiempo;
        char fecha_rpi[80];

        struct tm *tmPtr;

        tiempo = time(NULL);
        tmPtr = localtime(&tiempo);
        strftime( fecha_rpi, 80, "%Y%m%d%H%M%S", tmPtr );


        MYSQL *con = mysql_init(NULL);

        char insert[500] = "";
        const char * pinsert = NULL ;
        char empty[10]= "";

        if (con == NULL){
                fprintf(stderr, "%s\n", mysql_error(con));
                exit(1);
        }

        if (mysql_real_connect(con, "localhost", "javi", "1234","localDB", 0, NULL, 0) == NULL){
                finish_with_error(con);
        }

	printf("\nCOUNTER:%d",counter);

        sprintf(insert,"INSERT INTO obdTable (id_obd,fecha_rpi,velocidad,velocidad_media,rpm,temperatura_motor,motor,MAF,MIL,inyeccion_directa,inyeccion,consumo,eficiencia_diesel,eficiencia_gasolina,DTC,throttle,intake_air,lambda_o2s1_v,lambda_o2s8_v,egr,lambda_o2s3_c,run_time,fuel_level) VALUES ('%d','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",counter,fecha_rpi,structCarInfo.speed, structCarInfo.aspeed,structCarInfo.rpm,structCarInfo.ctemp,structCarInfo.engine,structCarInfo.maf,structCarInfo.mil,structCarInfo.rail,structCarInfo.inyeccion,structCarInfo.consumo,structCarInfo.eficdiesel,structCarInfo.eficgas,structCarInfo.dtc,structCarInfo.throttle,structCarInfo.intake_air,structCarInfo.lambda_o2s1_v,structCarInfo.lambda_o2s8_v,structCarInfo.egr,structCarInfo.lambda_o2s3_c,structCarInfo.run_time,structCarInfo.fuel_level);
        counter++;

        pinsert = insert;


        if (mysql_query(con,pinsert)) {
                finish_with_error(con);
        }


        mysql_close(con);

}





mysqlOBDII SQLOBD = mysqlOBDII();
