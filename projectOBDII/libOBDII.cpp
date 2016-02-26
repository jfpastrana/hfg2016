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
#include <string.h>
#include <math.h>
#include "arduPi.h"
#include "libOBDII.h"
#include "libPORT.h"




/* Se definen como variables globales para que
* no se pierda el valor cuando se sale de la funcion.
*/

int auxvelocidad=0;
int velocidad = 300;
int numpeticiones=0;
int nummuestras=0;
int pposdiesel=0, pposgas=0;
int pnegdiesel=0, pneggas=0;
int velocidadmedia=0;
int engine=6;
int maf=99;
int rail=99;
int rpm=99;
int ctemperature=0;
int pid0b = 0;
float porcentaje=0;
float mediadiesel=0;
float mediagas=0;
int throttle=300;

libOBDII::libOBDII(){

}


    /* convierte de hexadecimal a decimal */
long int hexToDec(char *pid){


    char auxpid[20]="";
    strcpy(auxpid,pid);
    int i=0,j=0,len=0,num=0,dec=0;
    len = strlen(auxpid);
    for(i=len-1; i>= 0; i--){
        if(auxpid[i] >= '0' && auxpid[i] <= '9'){
            num = auxpid[i]-'0';
        }
        if(auxpid[i] >= 'A' && auxpid[i] <= 'F'){
            num = pid[i]-55;
        }
        dec = dec + pow(16,j)*num;
        j++;
    }
    return dec;
}


    /*Codigo de decodificacion de datos para el DTC */
char * decode(char num){

    char * puntero = NULL;
    char code[5]="";

    switch(num){
        case '0': strcpy(code,"P0"); break;
        case '1': strcpy(code,"P1"); break;
        case '2': strcpy(code,"P2"); break;
        case '3': strcpy(code,"P3"); break;
        case '4': strcpy(code,"C0"); break;
        case '5': strcpy(code,"C1"); break;
        case '6': strcpy(code,"C2"); break;
        case '7': strcpy(code,"C3"); break;
        case '8': strcpy(code,"B0"); break;
        case '9': strcpy(code,"B1"); break;
    }
    puntero = code;
    return puntero;
}


	/* Funcion encargada de separar la informacion delimitida
	* en tokens, la informacion se recibe como una cadena y
	* se devuelve almacenada en una estructura. Sera utilizada
	* principalmente cuando se lea informacion proviniente de
	* mysql.
	*/


carInfo libOBDII::dividirInfoOBD(char *cadena){


        carInfo structOBD;
        char buffer[200]="";
        char *ptr=NULL;
        int counter=0;
        char acadena[10]="";


        strcpy(buffer,cadena);
        ptr = strtok(buffer," ");

    while(ptr!=NULL){

        counter++;
        switch(counter){
            case 1: strcpy(structOBD.id,ptr);break;
            case 2: strcpy(structOBD.fecha_rpi,ptr); break;
            case 3: strcpy(structOBD.speed, ptr); break;
            case 4: strcpy(structOBD.aspeed, ptr); break;
            case 5: strcpy(structOBD.rpm,ptr); break;
            case 6: strcpy(structOBD.ctemp,ptr); break;
            case 7: strcpy(structOBD.engine , ptr); break;
            case 8: strcpy(structOBD.maf, ptr); break;
            case 9: strcpy(structOBD.mil, ptr); break;
            case 10: strcpy(structOBD.rail, ptr); break;
            case 11: strcpy(structOBD.inyeccion, ptr); break;
            case 12: strcpy(structOBD.consumo, ptr); break;
            case 13: strcpy(structOBD.eficdiesel, ptr); break;
            case 14: strcpy(structOBD.eficgas, ptr); break;
            case 15: strcpy(structOBD.dtc,ptr);break;
            case 16: strcpy(structOBD.throttle,ptr); break;
            case 17: strcpy(structOBD.intake_air,ptr); break;
            case 18: strcpy(structOBD.lambda_o2s1_v, ptr); break;
            case 19: strcpy(structOBD.lambda_o2s8_v, ptr); break;
            case 20: strcpy(structOBD.egr, ptr); break;
            case 21: strcpy(structOBD.lambda_o2s3_c, ptr); break;
            case 22: strcpy(structOBD.run_time, ptr); break;
            case 23: strcpy(structOBD.fuel_level, ptr); break;
        }
        ptr = strtok(NULL," ");
        }
    return(structOBD);
}


        /*Funcion que extrae la informacion relevante de todo
        * lo recogido por el puerte serie, la trata y la transforma
        * para devolverlo como un array de char.
        */

char * libOBDII::dividirPID(char *infoPID, char *pid, int bytes){

	char auxchar[20]="";
	char auxnumA[20]="";
	char auxnumB[20]="";
	char auxnumC[20]="";
	char auxnumD[20]="";
	char auxnumE[20]="";
	char auxnumF[20]="";
	char auxinfo[200]="";
	char auxInfoPID[200]="";
	char * pinfo = NULL;
	int i=0, j=0, counter=0;

	strcpy(auxInfoPID,infoPID);
	strcpy(auxchar,pid);

	/* PID Disponible informacion */
	if(strstr(auxInfoPID,auxchar)!=NULL){
		/* Eliminamos los espacios en blanco entre caracteres */
       	do{
          	if(auxInfoPID[j] != ' '){
               	auxinfo[i] = auxInfoPID[j];
               	i++;
           	}
            j++;
      	}while(auxInfoPID[j] != '\0');

		i=0;
		j=0;
		counter=0;
		/* Extraemos solo la verdadera informacion */
		do{
			if( (auxinfo[i] == auxchar[0]) && (auxinfo[i+1] == auxchar[1])){
				counter = 1;
 				do{
					if(j<2)
						auxnumA[j]=auxinfo[i+2];
					if(j>2 &&  j<4)
						auxnumB[j]=auxinfo[i+2];
					if(j<4 && j<6)
						auxnumC[j]=auxinfo[i+2];
					if(j>6 && j< 8)
						auxnumD[j]=auxinfo[i+2];
                    if(j>8 && j< 10)
                        auxnumE[j]=auxinfo[i+2];
                    if(j>10 && j< 12)
                        auxnumF[j]=auxinfo[i+2];
					i++;
					j++;
				}while( j< (2*bytes) );
			}

			i++;
		}while(counter != 1);

		/* Convertimos a decimal todos los numeros */
		/* Si solo es un Byte */
		sprintf(auxchar,"%ld",hexToDec(auxnumA));

		/* Si son 2 Bytes */
		if(bytes>1 && bytes>=2){
			strcat(auxchar," ");
			sprintf(auxinfo,"%ld",hexToDec(auxnumB));
			strcat(auxchar,auxinfo);
		}

		/* Si son 3 Bytes */
        if(bytes>2 && bytes>=3){
           	strcat(auxchar," ");
			sprintf(auxinfo,"%ld",hexToDec(auxnumC));
           	strcat(auxchar,auxinfo);
        }

		/* Si son 4 Bytes */
        if(bytes>3 && bytes>=4){
           	strcat(auxchar," ");
			sprintf(auxinfo,"%ld",hexToDec(auxnumD));
           	strcat(auxchar,auxinfo);
        }
        /* Si son 5 Bytes */
        if(bytes>4 && bytes>=5){
            strcat(auxchar," ");
            sprintf(auxinfo,"%ld",hexToDec(auxnumE));
            strcat(auxchar,auxinfo);
        }
        /* Si son 6 Bytes */
        if(bytes>5 && bytes>=6){
            strcat(auxchar," ");
            sprintf(auxinfo,"%ld",hexToDec(auxnumF));
            strcat(auxchar,auxinfo);
        }
		i=0;
	}

	/* PID no encontrado, no hay información disponible */
	else{
		printf("\nPID no encontrado");
		strcpy(auxchar,"****");
	}

	pinfo = auxchar;
    return pinfo;
}


        /* Funcion donde se trata la informacion extraida anteriormente
        * aqui es donde se realizan las operaciones especificas para cada
        * PID para devolver unicamente el resultado como tipo CHAR.
        */

char * libOBDII::tratarPID(char *infoPID, char *pid, int bytes){


	char charpid[20]="";
	char resultado[20]="";
	char aux[20]="";
	char * presultado=NULL;
	char * ptr = NULL;
	int num1=0,num2=0,num3=0,num4=0,num5=0,num6=0;
	int i=0,j=0,len=0,dec=0,anum=0, k=0;
	float auxnum=0;
	float consumo=0;
	char auxresultado[20]="";

	strcpy(charpid,pid);
	strcpy(resultado,infoPID);

	/* Solo si se recibe la informacion deseada*/
	if(strstr(resultado,"****") == NULL){
        /* Solo si hay algun dato que obtener*/
		if (bytes!=0){
			ptr= strtok(resultado," ");
   	        while(ptr != NULL){
                strcpy(aux,ptr);
               	i=0;
               	j=0;
               	len = strlen(aux);
               	dec=0;
               	for(i=len-1; i>= 0; i--){
                  	if(aux[i] >= '0' && aux[i] <= '9'){
                   		anum = aux[i]-'0';
                    }
                    dec = dec + pow(10,j)*anum;
                    j++;
                }
				if(k==0) num1=dec;
				if(k==1) num2=dec;
                if(k==2) num3=dec;
				if(k==3) num4=dec;
                if(k==4) num5=dec;
                if(k==5) num6=dec;
                ptr= strtok(NULL," ");
           		k++;
			}
		}

        /* ENGINE LOAD VALUE - MYSQL*/
        if(strstr(charpid,"04")!=NULL){
            num1=num1*100;
            num1=num1/255;
            engine = num1;
            sprintf(resultado,"%d",num1);
        }

        /* ENGINE COOLANT TEMPERATURE - MYSQL */
        if(strstr(charpid,"05")!=NULL){
            num1=num1-40;
            ctemperature=num1;
            sprintf(resultado,"%d",num1);
        }
        /* INTAKE MANIFOLD ABSOLUTE PRESSURE- NO MYSQL */
        if(strstr(charpid,"0B")!=NULL){
            pid0b = num1;
            sprintf(resultado,"%d",num1);
        }
		/* RPM - MYSQL */
		if(strstr(charpid,"0C")!=NULL){
    		num1=num1*256;
			num1=num1+num2;
			num1=num1/4;
			rpm = num1;
			sprintf(resultado,"%d",num1);
		}
        /* VELOCIDAD MEDIA . MYSQL */
        if(strstr(charpid,"0D")!=NULL){
            auxvelocidad = auxvelocidad+num1;
            velocidad = num1;
            numpeticiones++;
            velocidadmedia=auxvelocidad/numpeticiones;
            sprintf(resultado,"%d",velocidadmedia);
        }
        /* INTAKE AIR TEMPERATURE - MYSQL */
        if(strstr(charpid,"0F")!=NULL){
            num1=num1 -40;
            sprintf(resultado,"%d",num1);
        }
        /* MAF AIR FLOW RATE - MYSQL */
        if(strstr(charpid,"10")!=NULL){
            num1=num1*256;
            num1=num1 + num2;
            num1=num1/100;
            maf = num1;
            sprintf(resultado,"%d",num1);
        }
        /* THROTTLE - MYSQL */
        if(strstr(charpid,"11")!=NULL){
            num1=num1*100;
            num1=num1/255;
	 	    throttle = num1;
            sprintf(resultado,"%d",num1);
        }
        /* DISTANCE TRAVELLED WITH "MILL" ON . MYSQL*/
		if(strstr(charpid,"21")!=NULL){
            num1=num1*256;
	 	    num1=num1=+num2;
			sprintf(resultado,"%d",num1);
        }
        /* FUEL RAIL PRESSURE - MYSQL*/
        if(strstr(charpid,"23")!=NULL){
            num1=num1*256;
            num1=num1 + num2;
            num1=num1*10;
			rail = num1;
            sprintf(resultado,"%d",num1);
        }
        /* 02S1_WR__LAMBDA EQUIVALENCE RATIO VOLTAGE - 2 RESULTADOS*/
        if(strstr(charpid,"24")!=NULL){
            num1=num1*256;
            num1=num1 + num2;
            num1=num1*2;
			num1=num1/65535;
            num3=num3*256;
            num3=num3 + num4;
            num3=num3*8;
            num3=num3/65535;
            sprintf(resultado,"%d",num1);
			sprintf(auxresultado,"%d",num3);
			strcat(resultado,"----");
			strcat(resultado,auxresultado);
        }
        /* 02S8_WR__LAMBDA EQUIVALENCE RATIO VOLTAGE - 2 RESULTADOS*/
        if(strstr(charpid,"2B")!=NULL){
            num1=num1*256;
            num1=num1 + num2;
            num1=num1*2;
            num1=num1/65535;
            num3=num3*256;
            num3=num3 + num4;
            num3=num3*8;
            num3=num3/65535;
            sprintf(resultado,"%d",num1);
            sprintf(auxresultado,"%d",num3);
            strcat(resultado,"----");
            strcat(resultado,auxresultado);
        }
        /* 02S1_WR__LAMBDA EQUIVALENCE RATIO VOLTAGE - 2 RESULTADOS*/
        if(strstr(charpid,"2C")!=NULL){
            num1=num1*100;
            num1=num1/255;
            sprintf(resultado,"%d",num1);
        }
        /* 02S3_WR__LAMBDA EQUIVALENCE RATIO CURRENT - 2 RESULTADOS*/
        if(strstr(charpid,"36")!=NULL){
            num1=num1*256;
            num1=num1 + num2;
            num1=num1/32768;
            num3=num3*256;
            num3=num3 + num4;
            num3=num3/256;
			num3=num3 - 128;
            sprintf(resultado,"%d",num1);
            sprintf(auxresultado,"%d",num3);
            strcat(resultado,"----");
            strcat(resultado,auxresultado);
        }
    	/* CALCULO DE CONSUMO DE COMBUSTIBLE A LOS 100KM */
		if(strstr(charpid,"FF")!=NULL){
			strcpy(resultado,"0");
			if(rpm != 0){
				printf("\n\nRPM:%d\n\n",rpm);
				if(engine!=0){
					printf("\n\nENGINE:%d\n\n",engine);
					if(maf!=99){
						if(velocidad==0){
							consumo = (maf*engine*0.0021) + 0.1;
							sprintf(resultado,"%f",consumo);
						}
						else{
							consumo = (maf*engine*0.0021) + (10/velocidad);
							sprintf(resultado,"%f",consumo);
						}
					}
		  		    else printf("\nNo disponible PID 10: MAF");
				}
				else printf("\nSin carga en el motor");
			}
			else printf("\nAutomovil no arrancado RPM=0");

		}

		/* Algoritmo que calcula el porcentaje de conduccion efeciciente de una manera
		* aproximada, sobre todo teneindo en cuenta velocidad y revoluciones.
		*/

		if(strstr(charpid,"FE")!=NULL){
			/*Cuando el motor este a mas de 50 grados.*/
			if(ctemperature>=50){
				/*Para una velocidad permitida en ciudad y circumbalaciones */
				if( ((velocidad >= 3) && (velocidad <=80)) && velocidad !=300 ){
                    /* Vehiculo con velocidad y embragado, penalizacion */
                    if(rpm<=1000){
                        pnegdiesel++;
                        pneggas++;
                    }
					/* Revoluciones correctas para diesel*/
					if(rpm>1199 && rpm <= 1600){
						pposdiesel++;
					}
					/*Revoluciones correctas para gasolina*/
					if(rpm>1399 && rpm <= 1800){
						pposgas++;
					}
					/* Para mas de 1600 revoluciones*/
					if(rpm>1600){
						/* El vehículo está reduciendo*/
						if( (pid0b<=100) || (rail==0) || (throttle == 0)){
							pposdiesel++;
							pposgas++;
						}
						/* El vehículo esta acelarando y supera las 1600 rpm */
						if( (pid0b > 100) || (rail!=0) || (throttle!=0 && throttle!=300)) {
							pnegdiesel++;
							pneggas++;
						}
					}
					nummuestras++;
				}
    			/* Coche parado practicamente y conductor acelerando*/
				if( (velocidad<3) && (rpm>=1000)){
					pnegdiesel++;
					pneggas++;
					nummuestras++;
				}
			}
			else printf("\nTemperatura del motor por debajo de 50 Grados\nNo empieza el Juego!");

			if(nummuestras!=0){
				mediadiesel=((pposdiesel)*100)/nummuestras;
			}
			sprintf(resultado,"%f",mediadiesel);
		}

		/* Calculo eficiencia en automoviles gasolina*/
		if(strstr(charpid,"FD")!=NULL){

			if(nummuestras!=0){
				mediagas=((pposgas)*100)/nummuestras;
			}
			sprintf(resultado,"%f",mediagas);
		}
    }
   /* No contiene la informacion */
    else printf("\nPID sin informacion");

    presultado = resultado;
	return presultado;

}




char * libOBDII::showDTC(char * info){

    char entrada [50] ="";
    strcpy(entrada,info);
    int len = strlen(entrada);
    char final[20]="";
	char * puntero = NULL;
    if(len>12){
        char * ptr = NULL;
        ptr = strtok(entrada,"43");
        ptr = strtok(NULL,"43");
        printf("\nCadena analizar:%s",ptr);
        strcpy(entrada,ptr);
        char aux[50] = "";
        int i = 0, j = 0;
        do{
            if(entrada[i] != ' '){
               aux[j] = entrada[i];
               j++;
            }
            i++;
        }while(entrada[i] != '\0');
        printf("\nCadena sin espacios:%s",aux);
        len = strlen (aux);
        char dtc1[10]="";
        char dtc2[10]="";
        char dtc3[10]="";
        char code[5]="";
        char aux2[5]="";
        j=0;
        do{
            if(j<4) dtc1[j] = aux[j];
            if(j>=4 && j<8) dtc2[j-4] = aux[j];
            if(j>=8 && j<12) dtc3[j-8] = aux[j];
            j++;
        }while(j<len);
        printf("\ndtc1:%s\ndtc2:%s\ndtc3:%s",dtc1,dtc2,dtc3);
        if(strcmp(dtc1,"0000")!=0){
            strcpy(code,decode(dtc1[0]));
            strcpy(final,code);
            for(i=0;i<3;i++)
                aux2[i]=dtc1[i+1];
            strcat(final,aux2);
            strcat(final,"-");
            if(strcmp(dtc2,"0000")!=0){
                strcpy(code,decode(dtc2[0]));
                strcat(final,code);
                for(i=0;i<3;i++)
                    aux2[i]=dtc2[i+1];
                strcat(final,aux2);
                strcat(final,"-");
                if(strcmp(dtc3,"0000")!=0){
                    strcpy(code,decode(dtc3[0]));
                    strcat(final,code);
                    for(i=0;i<3;i++)
                        aux2[i]=dtc3[i+1];
                    strcat(final,aux2);
                }
                else strcat(final,"P0000");
            }
            else strcat(final,"P000");
        }
        else strcpy(final,"P0000");
    }
    else strcpy(final,"P0000");
    printf("\nFINAL=%s",final);

	puntero = final;
	return puntero;
}




libOBDII OBD = libOBDII();
