/*
 * File:		platinum.c
 * Purpose:		Main process
 *
 */
//This is the main portion of the code. It includes all features of the project.
//This code was modified from the platinum code you were working on and provided to the class.
//Modifications written by Tobin Dewey, Matt Amoroso, and Rafael Oliveira.
 
//There is a strange error on first time importing code. 
//Something along the lines mw32 file lacking target to [platinum.c directory] 
//restarting the computer and rebuilding the code seems to solve this error.

//A lot of this code was included with the progress you made on platinum. 
//The main changes we made were setting c1Mod and c3Mod, changing the pwm signals,and wrote parser code to affect these values. 


#include "common.h"
#include "uart.h"
#include "ddefs2.h"
#include "mcg.h"
#include "stdio.h"
//#include "derivative.h"

#ifdef CMSIS
#include "start.h"
#endif

static int k, waittime = 0x0A05;
static unsigned char redButtonDown = FALSE, blueButtonDown = FALSE;
static long absoluteTime = 0;
static char buf[40];
static int dim, nvals, nstrs, lp, cmdflg = 0;
static elem_t el;
static struct pars_list *pars = NULL;
static char* p[10];
static char command[35];
int c1Mod = 12000;  //Mod values of pulses at startup. Sets servo to 0 degrees in both directions.
int c3Mod = 12000;

/********************************************************************/

int strfrac(char *s, char *p[]); //Something to do with teraterm.

 struct pars_list
	prstbl[] =
	{
	"SET",0,1,1,1,
	"GET",1,1,1,1,
	"STATUS",2,1,1,0
	}; 

static void WriteToUART0(const char *text){ //UART0 Module used to write to teraterm.
	while (*text != '\0'){
		uart0_putchar(UART0_BASE_PTR, *text);
		text++;
	}
}

void UART0_get_cmd(void){ //UART0 Module to establish communication with teraterm.
	static unsigned char i=0; 
	unsigned char rc;
	rc = in_char();
	if (i >= sizeof(command)-1 || rc == 0x0D) {
		command [i] = '\0';
		i = 0;
		cmdflg = TRUE;
	} else {
		command[i] = rc;
		i++;
	}
	out_char(rc);
}


int pulse(int c1Mod, int c3Mod){ //This section of code it used to create the two pulses.
	unsigned int count;
	pll_init(8000000, HIGH_GAIN, CRYSTAL, 4, 24, MCGOUT);
	
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_ADC0_MASK | SIM_SCGC6_DAC0_MASK;
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);
	SIM_SCGC5 |= (SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK);
	
	PORTD_PCR1 |= PORT_PCR_MUX(4);
	PORTD_PCR3 |= PORT_PCR_MUX(4);
	PORTB_PCR3 |= PORT_PCR_MUX(4);
	PORTE_PCR30 |= PORT_PCR_MUX(0);
	PORTE_PCR20 |= PORT_PCR_MUX(0);
	
	TPM0_SC |= (TPM_SC_PS(1) | TPM_SC_CMOD(1));
	TPM0_C1SC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
	TPM0_C3SC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
	
	TPM0_MOD = TPM_MOD_MOD(54000);
	TPM0_C1V = TPM_CnV_VAL(c1Mod);
	TPM0_C3V = TPM_CnV_VAL(c3Mod); //6000 gives pulse width of 500us, 12000 1us.
	
	}

void ADC0_IRQHandler(){ 
	unsigned int local_throttle_flag;
	int temp = ADC0_RA;
}

int main (void)
{
    	char ch;

        
#ifdef CMSIS  // If we are conforming to CMSIS, we need to call start here
    start();
#endif
    
    
    //The following section of code represents our parsing.
  	printf("\n\rRunning the platinum project.\n\r");
  	printf("\n\rUse the '1' and '2' key to adjust the vertical axis.\n\r");
  	printf("\n\rUse the 'q' and 'w' key to adjust the horizontal axis.\n\r");
  	printf("\n\rUse the '?' key to to get a current readout of the positions of the horizontal and vertical axis.\n\r");  	
    int degrees = 0;
    int degrees2 = 0;
	while(1)
	{
		out_char('>'); //Begins each line with a '>' symbol, this is simply for aesthetic purposes.
		ch = in_char();
		    if (ch == '1'){ //Command to move vertical axis positive three degrees.
		    	if (c1Mod < 24000){
				out_char(ch);
				c1Mod += 400;
				degrees = degrees + 3;
				pulse(c1Mod, c3Mod);
				printf("The vertical axis is %d degrees\n\r", degrees);
		    	}
		    	else {printf ("Servo cannot go higher than 90 degrees. Out of bounds.\n\r");}
		    	}
			else if (ch == '2') //Command to move vertical axis negative three degrees.
			{
				out_char(ch);
				if(c1Mod > 12000){
				c1Mod -= 400;
				degrees = degrees - 3;
				pulse(c1Mod, c3Mod);
				printf("The vertical axis is %d \n\r", degrees);	
				}
				else{printf ("Servo cannot go lower than 0 degrees. Out of bounds.\n\r");}
			}
			else if (ch == '?') //Provides readout of current servo position.
			{
				out_char(ch);
				float pw1 = (1 +(c1Mod - 12000)*.000083333333333333);
				float pw2 = (1 +(c3Mod - 12000)*.000083333333333333);
				printf(" Vertical Axis: %d degrees. Pulse width %f msec \n\r", degrees, pw1);
				printf(" Horizontal Axis: %d degrees. Pulse width %f msec \n\r", degrees2, pw2);
			}
			
		    else if (ch == 'q') //Command to move horizontal axis positive three degrees.
		    {
		    	if (c1Mod < 24000){
				out_char(ch);
				c3Mod += 400;
				degrees2 = degrees2 + 3;
				pulse(c1Mod, c3Mod);
				printf("The horizontal axis is %d \n\r", degrees2);
		    	}
		    	else {printf ("Servo cannot go higher than 90 degrees. Out of bounds.\n\r");}
			}
			else if (ch == 'w') //Command to move horizontal axis negative three degrees.
			{
				if(c1Mod > 12000){
				out_char(ch);
				degrees2 = degrees2 - 3;
				c3Mod -= 400;
				pulse(c1Mod, c3Mod);
				printf("The horizontal axis is %d \n\r", degrees2);		
				}
				else{printf ("Servo cannot go lower than 0 degrees. Out of bounds.\n\r");}
			}
			else{
				printf("Invalid command, Please enter 1, 2, q, e, or ?. \n\r");
				}
				}
	
	//Next section has to do with pulse modulation.
		unsigned int temp;
		unsigned int count;
		count++;
				
				ADC0_SC1A ^= ADC_SC1_ADCH(8);
				while (( ADC0_SC2 & ADC_SC2_ADACT_MASK) == ADC_SC2_ADACT_MASK){
				}
				temp = ADC0_RA;
				temp = temp >> 4;
				DAC0_DAT0L = (temp);
				DAC0_DAT0H = (temp >> 8);
				
				ADC0_SC1A |= ADC_SC1_ADCH(8);
				while(( ADC0_SC2 & ADC_SC2_ADACT_MASK) == ADC_SC2_ADACT_MASK){
				}
				temp = ADC0_RA;
				temp = temp >> 1;
				TPM0_C0V = TPM_CnV_VAL(temp);
	}  
/********************************************************************/
