/* 
 * File:   Nmain.c
 * Author: tpeea
 *
 * Created on 16 octobre 2019, 16:47
 */
// DSPIC30F6014 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FPR = XT_PLL16           // Primary Oscillator Mode (XT w/PLL 16x)
#pragma config FOS = PRI                // Oscillator Source (Primary Oscillator)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Enabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = NONE            // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


#define RESET       0x80
#define CLEAR       0x81
#define HOMECLEAR   0x82



#define LCDHomeClear()  LCDWriteCmd(HOMECLEAR)
#define LCDHomeReset()  LCDWriteCmd(RESET)
#define LCDClear()      LCDWriteCmd(CLEAR)



/*
 * 
 */
//Commutation de LED1/2/3/4 (RD0/1/2/3) sur appui SW1/2/3/4(INT1/2/3)
void __attribute__((interrupt,auto_psv)) _INT1Interrupt(void)
{
PORTDbits.RD0=~PORTDbits.RD0;
IFS1bits.INT1IF=0; // acquittement FLAG
}
void __attribute__((interrupt,auto_psv)) _INT2Interrupt(void)
{
PORTDbits.RD1=~PORTDbits.RD1;
IFS1bits.INT2IF=0; // acquittement FLAG
}
void __attribute__((interrupt,auto_psv)) _INT3Interrupt(void)
{
PORTDbits.RD2=~PORTDbits.RD2;
_INT3IF=0; // acquittement FLAG
}
void __attribute__((interrupt,auto_psv)) _INT4Interrupt(void)
{
PORTDbits.RD3=~PORTDbits.RD3;
_INT4IF=0; // acquittement FLAG
}

/**
 * 
 * 
 * 
 * */
void LCDinit(void){
    //config SPIxCON
    _LATG9 = 0;
    TRISGbits.TRISG9=0;
    
    SPI2CONbits.PPRE=1;
    SPI2CONbits.SPRE=7;
    SPI2CONbits.CKE=0;
    SPI2CONbits.CKP=0;
    SPI2CONbits.MSTEN=1;
    
    SPI2STATbits.SPIEN=1;
}

static void LCDWriteCmd(char c)
{   SPI2BUF = c;
    while(SPI2STATbits.SPITBF);
}



void LCDgoto(int l,int c){
  
}

void LCDWriteChar(char c){
    LCDWriteCmd(0xA8);
    LCDWriteCmd(c);
}

void LCDWriteString(char * str){
    int k = 0;
while(str[k] != 0){
    LCDWriteChar(str[k++]);
}    
}


//SPITBF
int main ( void )
{
TRISDbits.TRISD0=0; // configuration LED1 en sortie
// initialisation INT1 sur SW1
INTCON2bits.INT1EP=1; // interruption sur front descendant.
IPC4bits.INT1IP=1; // priorit�e 1
IFS1bits.INT1IF=0; // raz FLAG
IEC1bits.INT1IE=1; // autorisation interruption INT1

TRISDbits.TRISD1=0; // configuration LED2 en sortie
// initialisation INT2 sur SW2
INTCON2bits.INT2EP=1; // interruption sur front descendant.
_INT2IP=1; // priorit�e 1
IFS1bits.INT2IF=0; // raz FLAG
IEC1bits.INT2IE=1; // autorisation interruption INT2

TRISDbits.TRISD2=0; // configuration LED3 en sortie
// initialisation INT3 sur SW3
INTCON2bits.INT3EP=1; // interruption sur front descendant.
_INT3IP=1; // priorit�e 1
_INT3IF=0; // raz FLAG
_INT3IE=1; // autorisation interruption INT3

TRISDbits.TRISD3=0; // configuration LED4 en sortie
// initialisation INT4 sur SW4
INTCON2bits.INT4EP=1; // interruption sur front descendant.
_INT4IP=1; // priorit�e 1
_INT4IF=0; // raz FLAG
_INT4IE=1; // autorisation interruption INT4






LCDinit();
LCDHomeClear();
LCDWriteString("coucou !");

while(1);
}