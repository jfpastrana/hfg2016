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



#ifndef LIBMYSQLRPI_H__
#define LIBMYSQLRPI_H_

#include "arduPi.h"
#include "libRPI.h"
#include "libOBDII.h"


class mysqlRPI{

public:

        mysqlRPI();
        gprm leerGpsSQL();
	carInfo leerObdSQL();;
	void eliminarFilaGPS(char * fecha_rpi);
	void eliminarFilaOBD(char * fecha_rpi);
	int numFilasTablaGPS();
	int numFilasTablaOBD();
};

extern mysqlRPI SQL;



#endif /* LIBMYSQLRPI_H_*/

