#include "mbed.h"
#include "Serial.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <LIS331.h>
#define limX 0.1
#define limY 0.1
#define LIMARRET 1000
#define GPSX 0.001
#define GPSY 0.001
#include "WakeUp.h"




Serial gps(D1, D0);
LIS331 accelerometer(D4, D5);
DigitalOut mosgps(D9);DigitalOut mossig(D10);//DigitalOut mosacc(D11);
DigitalOut led(LED3);
//Serial pc(USBTX, USBRX);









float lat = 0, lng = 0;
typedef union float_bits {
    unsigned int i;
    float f;
} bits;


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
    lat = (float)((int)(atof(part1)/100));
    lat = lat + (atof(part1) - 100 * lat)/60;

    split = strtok(NULL,",");
    
    strcpy(part2,split);
    if(*part2=='N') {}   
                         
    else{               
        
        lat *= -1;
    }

    split = strtok(NULL,",");
    
    strcpy(part3,split);
    lng = (float)((int)(atof(part3)/100));
    lng = lng + (atof(part3) - 100 * lng)/60;
    
    split = strtok(NULL,",");
    
    strcpy(part4,split);
    if(*part4=='E'){}
   
    else{
        
        lng *= -1;
    }

    split = strtok(NULL,",");

}



int accego(){
    float x=0;

    float y=0;
    
    int go = 0;
    int arretBoucle = 0;
        while(!accelerometer.getAccelStatus()){}
            
            while ((go == 0) && (arretBoucle < LIMARRET ))
            {
                
                
                x = accelerometer.getAccelX();
                y = accelerometer.getAccelY();
                
            
                if ((x>limX)||(y>limY)) 
                go = 1;
            
                
                arretBoucle += 1;
            }
            
        
    return go;
    
    }
    

int main()
{
    bits LAT, LNG;
    mosgps = 1;
    wait(2);
    
    Gps();
    LAT.f = lat;
    LNG.f = lng;
    wait(5);
    mosgps = 0;
 
    while(1){
            //mos2 = 1;
            //wait(2);
            //accego();
           // if(accego()){ 
            //mos2 = 0;
            //wait(2);
            mosgps = 1;
            wait(46);
                Gps();
                if(((lat-LAT.f)> GPSX)||((lat-LAT.f)<(-GPSX))||((lng-LNG.f)>GPSX)||((lng-LNG.f)<(-GPSX))){
                    //myled3 = 1;
                    LAT.f = lat;
                    LNG.f = lng;
                    wait(2);
                    mosgps = 0;
                    mossig = 1;wait(2);
                    gps.printf("AT$SF=%08x%08x\r\n", LAT.i, LNG.i);wait(10);
                    mossig = 0;
                    //for(int i = 0;i<3;i++){
                       // led = 1;
                       // wait(2);
                        //led = 0;
                       // wait(2);
                       // }
                    //myled3 = 0;
                }
           }
            
            
           //mos = 0;
           //WakeUp::set(5);
           //sleep();
         //wait(5);
          
          //}
        
 
        
       
  
}