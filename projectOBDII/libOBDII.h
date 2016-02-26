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



#ifndef LIBOBDII_H_
#define LIBOBDII_H_

#include "arduPi.h"
#include "libPORT.h"

typedef struct{

	char id[5];
	char rpm[10];
	char speed[10];
	char aspeed[10];
	char ctemp[10];
	char mil[10];
	char engine[10];
	char maf[10];
	char rail[10];
	char consumo[10];
	char eficdiesel[10];
	char eficgas[10];
	char inyeccion[10];
	char fecha_rpi[80];
	char dtc[10];
	char intake_air[10];
	char lambda_o2s1_v[10];
	char lambda_o2s8_v[10];
	char egr[10];
	char lambda_o2s3_c[10];
	char run_time[10];
	char throttle[10];
	char fuel_level[10];

} carInfo;



class libOBDII{

public:
	libOBDII();
	char * dividirPID(char *infoPID, char *pid, int bytes);
	char * tratarPID(char *infoPID, char *pid, int bytes);
	carInfo dividirInfoOBD(char * cadena);
	char * showDTC(char * info);
};

extern libOBDII OBD;

#endif /* LIBOBDII_H_ */
