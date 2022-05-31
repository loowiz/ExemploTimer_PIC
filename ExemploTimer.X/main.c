/*=======================================================
					SENAI A. Jacob Lafer
					====================

Nome do projeto:	Timer 0
Versão:				1.0x
Última revisão:		01/06/2020
Cliente:			SENAI
Desenvolvimento:	Luiz Ricardo Bitencourt

Descritivo Funcional:
Teste de funcionamento do Timer 0 com interrupção.

Inputs:
-

Outputs:
- RD1 - LED1
- RB2 - Display RS
- RB3 - Display E
- RB4-RB7 - Display Data
=========================================================*/

//======================================================
// Bibliotecas
//======================================================
#include <xc.h>
#include "lcd_4vias.h"
#include "timer0.h"

// =====================================================
// CONFIGURAÇÕES DO MICROCONTROLADOR
// =====================================================
#pragma config  FOSC    = HS
#pragma config  PLLDIV  = 1
#pragma config  CPUDIV  = OSC1_PLL2
#pragma config  USBDIV  = 1
#pragma config  BORV    = 0
#pragma config  WDTPS   = 1
#pragma config  MCLRE   = ON
#pragma config	PWRT=ON, LVP=OFF, IESO=OFF, WDT=OFF, PBADEN=OFF, BOR=OFF
#pragma config  CCP2MX=ON, LPT1OSC=OFF, ICPRT=OFF, XINST=OFF, VREGEN=OFF
#pragma config  CP0=OFF, CP1=OFF, CP2=OFF, CP3=OFF, CPB=OFF, CPD=OFF, WRTD = OFF
#pragma config  WRT0=OFF, WRT1=OFF, WRT2=OFF, WRT3=OFF, WRTC = OFF, WRTB = OFF
#pragma config  EBTR0=OFF, EBTR1=OFF, EBTR2=OFF, EBTR3=OFF, EBTRB=OFF
// =====================================================

//======================================================
// Definições de Hardware
//======================================================
#define LED1 PORTDbits.RD0
//======================================================

//======================================================
// Declaração de Variáveis
//======================================================
unsigned char	Interrompeu	= 0;
int				Conta		= 0,
		 		Preset		= 0xc2f7;	

//======================================================
// Interrupções
//======================================================
void __interrupt() high_isr(void){  // Tratamento de interrupção de alta prioridade (ISR)
	if(INTCONbits.TMR0IF){  		// Verifica se overflow TMR0IF = 1 (garante que não executará sem ter estourado a contagem)
		INTCONbits.TMR0IF = 0;		// Zera flag de overflow
									// Recarga do preset do Timer 0:
		TMR0H = Preset >> 8;   		// Registrador de contagem do Timer 0 - Parte alta
		TMR0L = Preset;				// Registrador de contagem do Timer 0 - Parte baixa
        
		Conta++;					// Incrementa variável de contagem
		if(Conta==100) Conta = 0;	// Zera variável de contagem a cada 100 interrupções

		Interrompeu = 1;			// Flag de interrupção
    }
}
//======================================================

//======================================================
// Programa Principal
//======================================================
void main (void){
	// -------------------------------------------------
	// Configurações iniciais
	// -------------------------------------------------
	TRISDbits.TRISD0 = 0;						// Determina D1 como saída (LED)
    TRISB = 0b00000011;                         // Todos os pinos do Port B como saídas
	Inicializa_Timer0(Timer0_INT_ON, Preset);	// Inicializa Timer0 com interrupção e Preset (vide variável no início do código)
	IniciaLCD();                                // Inicializa LCD
	LED1 = 1;									// LED Inicialmente ligado
	// -------------------------------------------------

	// -------------------------------------------------
	// Programa principal
	// -------------------------------------------------
	PosicionaLCD(1,1);
    StringLCD("   SENAI  118   ");
	PosicionaLCD(2,1);
    StringLCD("Timer 0:        ");

	while(1){
		if (Interrompeu == 1){							// Verifica se houve interrupção
			LED1 = ~LED1;								// Inverte o estado do LED

            PosicionaLCD(2, 10);
            EscreveLCD((Conta/100)%10 + 48);
            EscreveLCD((Conta/10)%10 + 48);
            EscreveLCD(Conta%10 + 48);

            Interrompeu = 0;
		}
	}
	// -------------------------------------------------
}
//======================================================
