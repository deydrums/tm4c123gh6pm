/*Librerias de C*/
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
/*Librerias de TIVAWARE*/
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
/*Variables globales*/
int Contador,Status = 0;
int i = 0;
/*Mapeo de la funcion seno  [255*Sin(2*PI*x) + 255]/2    */
float Sen[176] = {127.5,
                  132.2164237,
                  136.9263913,
                  141.6234557,
                  146.3011873,
                  150.953183,
                  155.573075,
                  160.1545393,
                  164.6913046,
                  169.1771609,
                  173.6059676,
                  177.9716624,
                  182.2682695,
                  186.4899073,
                  190.6307971,
                  194.6852708,
                  198.6477783,
                  202.5128956,
                  206.275332,
                  209.9299372,
                  213.4717087,
                  216.8957984,
                  220.1975192,
                  223.3723516,
                  226.4159497,
                  229.3241473,
                  232.0929636,
                  234.7186084,
                  237.1974877,
                  239.5262082,
                  241.7015824,
                  243.7206325,
                  245.5805946,
                  247.2789229,
                  248.8132925,
                  250.1816032,
                  251.3819819,
                  252.4127855,
                  253.2726031,
                  253.9602575,
                  254.4748077,
                  254.8155491,
                  254.9820155,
                  255,

                  255,
                  254.9127362,
                  254.6510643,
                  254.2153424,
                  253.606167,
                  252.824372,
                  251.8710274,
                  250.7474384,
                  249.4551428,
                  247.9959097,
                  246.3717366,
                  244.5848466,
                  242.6376857,
                  240.5329193,
                  238.2734285,
                  235.8623061,
                  233.3028527,
                  230.5985717,
                  227.7531649,
                  224.7705272,
                  221.6547412,
                  218.4100722,
                  215.0409614,
                  211.5520208,
                  207.9480261,
                  204.2339105,
                  200.4147583,
                  196.4957971,
                  192.4823914,
                  188.380035,
                  184.1943434,
                  179.931046,
                  175.5959788,
                  171.1950756,
                  166.7343607,
                  162.2199401,
                  157.6579933,
                  153.054765,
                  148.4165561,
                  143.7497158,
                  139.0606321,
                  134.3557238,
                  129.641431,
                  127.5,

                  127.5,
                 122.7835763,
                 118.0736087,
                 113.3765443,
                 108.6988127,
                 104.046817,
                 99.42692502,
                 94.84546072,
                 90.30869539,
                 85.82283915,
                 81.39403242,
                 77.02833757,
                 72.73173054,
                 68.51009272,
                 64.36920287,
                 60.31472922,
                 56.35222172,
                 52.48710441,
                 48.72466804,
                 45.0700628,
                 41.52829127,
                 38.10420158,
                 34.80248078,
                 31.6276484,
                 28.5840503,
                 25.67585269,
                 22.90703644,
                 20.28139161,
                 17.80251232,
                 15.47379176,
                 13.29841758,
                 11.27936753,
                 9.419405372,
                 7.721077112,
                 6.18670749,
                 4.81839682,
                 3.618018105,
                 2.587214476,
                 1.727396943,
                 1.039742461,
                 0.525192321,
                 0.184450864,
                 0.01798451,
                 0,

                 0,
                 0.087263794,
                 0.348935725,
                 0.784657606,
                 1.393833001,
                 2.175628045,
                 3.128972581,
                 4.25256163,
                 5.544857175,
                 7.004090264,
                 8.628263435,
                 10.41515345,
                 12.36231433,
                 14.46708073,
                 16.72657153,
                 19.13769387,
                 21.69714727,
                 24.40142826,
                 27.24683509,
                 30.22947284,
                 33.34525876,
                 36.58992782,
                 39.95903856,
                 43.44797921,
                 47.05197395,
                 50.76608947,
                 54.58524173,
                 58.50420291,
                 62.51760856,
                 66.61996496,
                 70.80565662,
                 75.06895397,
                 79.40402124,
                 83.8049244,
                 88.26563929,
                 92.78005989,
                 97.34200667,
                 101.945235,
                 106.5834439,
                 111.2502842,
                 115.9393679,
                 120.6442762,
                 125.358569,
                 127.5,

};
/*Configuraciones*/
void Configuraciones(void){
    SysCtlClockSet(SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_2_5); //Fosc = 80MHz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //Desbloqueo de Pin F0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}

int main(void)
{
Configuraciones();
    while (1){
        Status = GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
        //Senoidal
       if (Status == 1){
            for(i = 0; i<176 ;i++){
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,Sen[i]);
            SysCtlDelay(16); //1k ''' 65
                            //2k      29
                            //3k     16
            }
        //CUADRADA
       }else if (Status == 16){

                   GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,255);
                   SysCtlDelay(2200); // 2k ´´´ 3200
                                      // 3k ´´´ 2200
                                      // 1k ´´´ 6200
                   GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,0);
                   SysCtlDelay(2200); // 2k ´´´ 3200
                                      // 3k ´´´ 2200
                                      // 1k ´´´ 6200
       }
    }
}
