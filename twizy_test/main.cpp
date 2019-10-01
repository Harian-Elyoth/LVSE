#include "mbed.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "Sht31.h"

//GPS
Serial pc(USBTX, USBRX);
Serial gps(D1, D0);

//DUST
InterruptIn input(D5); 
Timer t;    // measure amount of time signal is low

//TH
Sht31 th(D4,D5);



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
    pc.printf("%f-",atof(part1));
    split = strtok(NULL,",");
    
    strcpy(part2,split);
    if(*part2=='N')    //   on definit N->1 E->2 S->3 W->4                  N(1)
    *part2='1';        //                      comme ca ->             W(4)      E(2)
    else               //                                                   S(3)
    *part2='3';
    pc.printf("%d\n",atoi(part2));
    split = strtok(NULL,",");
    
    strcpy(part3,split);
    pc.printf("%f-",atof(part3));
    split = strtok(NULL,",");
    
    strcpy(part4,split);
    if(*part4=='E')
    *part4='2';
    else
    *part4='4';
    pc.printf("%d\n",atoi(part4));
    split = strtok(NULL,",");
    
    }
    
    
//DUST

// Start Timer
void start(void){
    t.start();
}

// Stop TImer
void stop(void){
    t.stop();
}

// calculate sensor value
void calculate(){
    // run measurements
    int lpo = t.read_us();
    double ratio = t.read_us()/(30000*10.0);
    double concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
    pc.printf("c = %f\n",concentration);
    
    // reset and run sensors
    t.reset(); // reset mesurement every 30 seconds
    input.fall(&start); // start measuring when signal is low
    input.rise(&stop);  // stop measuring when signal is high
}

//TH
void temhumi(){
    pc.printf("tem=%f ",th.readTemperature());
    pc.printf("humi=%f\n",th.readHumidity());
    }
    



int main()
{
    pc.baud(9600);
    gps.baud(9600);
    
    t.reset(); 
    input.fall(&start); 
    input.rise(&stop); 
    
    wait(30);
    
    while(1){
        Gps();
        
        calculate();
        
        temhumi();
        
        wait(30);
        }
    
    
    }   