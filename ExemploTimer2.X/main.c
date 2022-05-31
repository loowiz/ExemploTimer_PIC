/*=======================================================
					SENAI A. Jacob Lafer
					====================

Nome do projeto:	Timer 0 (exemplo 2)
Versão:				1.0x
Última revisão:		01/06/2020
Cliente:			SENAI
Desenvolvimento:	Luiz Ricardo Bitencourt

Descritivo Funcional:
Relógio digital com Timer 0.

Outputs:
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
// Declaração de Variáveis
//======================================================
int				Segundos	= 0,
                Minutos     = 0,
                Horas       = 0,
		 		Preset		= 0xc2f7;  // Período de 1s

//======================================================
// Interrupções
//======================================================
void __interrupt() high_isr(void) {     // Tratamento de interrupção de alta prioridade (ISR)
    if (INTCONbits.TMR0IF) {            // Verifica se overflow TMR0IF = 1 (garante que não executará sem ter estourado a contagem)
        INTCONbits.TMR0IF = 0;          // Zera flag de overflow
                                        // Recarga do preset do Timer 0:
        TMR0H = Preset >> 8;            // Registrador de contagem do Timer 0 - Parte alta
        TMR0L = Preset;                 // Registrador de contagem do Timer 0 - Parte baixa

        Segundos++;
        if (Segundos == 60) {
            Segundos = 0;
            Minutos++;
            if (Minutos == 60) {
                Minutos = 0;
                Horas++;
                if (Horas == 24) {
                    Horas = 0;
                }
            }
        }
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
	TRISB = 0b00000011;                         // Todos os pinos do Port B como saídas
	Inicializa_Timer0(Timer0_INT_ON, Preset);	// Inicializa Timer0 com interrupção e Preset (vide variável no início do código)
	IniciaLCD();                                // Inicializa LCD
	// -------------------------------------------------

	// -------------------------------------------------
	// Programa principal
	// -------------------------------------------------
	PosicionaLCD(1,1);
    StringLCD("   SENAI  118   ");
    PosicionaLCD(2,1);
    StringLCD("Horario:        ");

    while (1) {
        PosicionaLCD(2, 9);
        EscreveLCD((Horas / 10) % 10 + 48);
        EscreveLCD(Horas % 10 + 48);
        EscreveLCD(':');
        EscreveLCD((Minutos / 10) % 10 + 48);
        EscreveLCD(Minutos % 10 + 48);
        EscreveLCD(':');
        EscreveLCD((Segundos / 10) % 10 + 48);
        EscreveLCD(Segundos % 10 + 48);
    }
	// -------------------------------------------------
}
//======================================================
