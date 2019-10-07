#include "mbed.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


Serial pc(USBTX, USBRX);
Serial gps(D1, D0);

//    GPS

void Gps(){
    
    char buffer[128];       //   NMEA data, Example:  $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47  
    char * split;
    char part1[12] = "";    //   4807.038     Latitude 48 deg 07.038' 
    char part2[12] = "";    //   N            Direction - North
    char part3[12] = "";    //   01131.000    Longitude 11 deg 31.000' 
    char part4[12] = "";    //   E            Direction - East

    while(gps.readable()==0){}   //    attendre jusqua on peut lire le GPS
    
    gps.gets(buffer, 70);    //   on met tous les information dans le buffer
    
    while((buffer[0] != '$') || (buffer[1] != 'G')|| (buffer[2] != 'P')|| (buffer[3] != 'G')|| (buffer[4] != 'G')|| (buffer[5] != 'A')){
             gps.gets(buffer, 70);
             }    //   on met tous les informations dans buffer et si il est just on va commencer
             
    split = strtok(buffer, ",");    //   on met le pointeur sur le toisieme element
    split = strtok(NULL,",");
    
    split = strtok(NULL,",");    //   on commence
    
    strcpy(part1,split);
    pc.printf("%f\n",atof(part1));
    split = strtok(NULL,",");
    
    strcpy(part2,split);
    if(*part2=='N')    //   on definit N->1 E->2 S->3 W->4                  N(1)
    *part2='1';        //                      comme ca ->             W(4)      E(2)
    else               //                                                   S(3)
    *part2='3';
    pc.printf("%d\n", atoi(part2));
    split = strtok(NULL,",");
    
    strcpy(part3,split);
    pc.printf("%f\n",atof(part3));
    split = strtok(NULL,",");
    
    strcpy(part4,split);
    if(*part4=='E')
    *part4='2';
    else
    *part4='4';
    pc.printf("%d\n",atoi(part4));
    split = strtok(NULL,",");

    }
    



int main()
{
    pc.baud(9600);
    gps.baud(9600);
    
    while(1){
        Gps();
        
        wait(1);
        }
    
    
    }   