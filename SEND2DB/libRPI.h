/*
*
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


	/*Librería donde se definen las estrucuturas utilizadas para almacenar
	* la información de GPS y SMS. Así como dondee estan definidos los
	* métodos públicos implementados en libRPI.cpp.
	* El objeto definido es RPI.
	*/


#ifndef LIBRPI_H_
#define LIBRPI_H_

#include "arduPi.h"


typedef struct{
	char id[5];
	char UTCtime[10];
	char Status[2];
	char latitude[10];
	char N_S_indicator[2];
	char longitude[10];
	char E_W_indicator[2];
	char speed[10];
	char course[10];
	char date[10];
	char mode[10];
	char distance[10];
	char km[10];
	char fecha_rpi[80];
} gprm;

typedef struct {

	char estado[50];
	char telefono[10];
	char sms_text[200];
}sms;


class libRPI{

public:

	libRPI();
	int checkport();
	int setport();
	void print();
	char * read();
	int wait4OK();
	sms dividirSMS(char *cadena);
	gprm dividirGPRM(char *cadena);
        int httpAnswer();

};

extern libRPI RPI;

#endif /* LIBRPI_H_ */

