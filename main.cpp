#include "mbed.h"
#include "LIS331.h"
#define limX 0.05
#define limY 0.05
#define LIMARRET 10000

LIS331 accelerometer(D4, D5); // entree I2C de l'accelerometre   *Format(sda, scl)*
Serial pc(USBTX, USBRX);

 

//variables du code

float x_prec;
float x;
float y_prec;
float y;
int stop = 0;
int arretBoucle = 0;

//comportement
int main(void){
    while(1){
        if (accelerometer.getAccelStatus()){
            x = accelerometer.getAccelX();
            y = accelerometer.getAccelY();
            while ((stop == 0) && (arretBoucle < LIMARRET ))
            {
                //wait(1);
                x_prec = x;
                y_prec = y;
                
                x = accelerometer.getAccelX();
                y = accelerometer.getAccelY();
                //pc.printf("X_prec vaut %f et x vaut:%f\n\r",x_prec, x);
                //pc.printf("Y_prec vaut %f et y vaut:%f\n\r",y_prec, y);
                if ((abs(x) - abs(x_prec))>limX) {stop = 1; } 
                if ((abs(y) - abs(y_prec))>limY) {stop = 1; } 
                
                if (stop == 0) {pc.printf("LE SYSTEME N A PAS TROP BOUGE\n\r");}
                else {pc.printf("     LE SYSTEME BOUGEEEE \n\r");}
                arretBoucle ++;
                pc.printf("\n\r");
            }
            wait(5);
            pc.printf("FIN DE LA BOUCLE\n\r");
        }
    }
}
    
    