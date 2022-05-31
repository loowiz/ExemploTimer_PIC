#ifndef	TIMER0_H_
#define TIMER0_H_

// ============================================================================================ //
//						BIBLIOTECA PARA USO DO TIMER 0											//
//																								//
//	ESCOLA SENAI "A. JACOB LAFER																//
//	CURSO T�CNICO DE ELETROELETR�NICA															//
//	DISCIPLINA: DESET																			//
//	DATA: 23/09/2016																			//
//	REVIS�O: A																					//
// ============================================================================================	//
#include <xc.h>

// ============================================================================================	//
//										DEFINI��ES												//
// ============================================================================================	//
#define	Timer0_INT_ON	1		
#define	Timer0_INT_OFF	0		


// ============================================================================================	//
//									SUB-ROTINA DE INICIALIZA��O DO TIMER 0						//
// ============================================================================================	//
void Inicializa_Timer0(int Interrupcao, unsigned int Preset_t0){
	if(Interrupcao == Timer0_INT_ON){
		INTCON = 0x20;			// Habilita interrup��o de overflow do Timer 0 (p. 99 do datasheet)
		INTCON2bits.TMR0IP = 1; // Interrup��o do Timer 0 com prioridade alta (p. 100 do datasheet)
		RCONbits.IPEN = 1;		// Habilita prioridade de interrup��es (p. 108 do datasheet) 
	}							// Obs.: Interrup��o configurada, por�m n�o habilitada

	T0CON = 0x85;				// Liga Timer 0 em 16 bits (clock interno) com prescaler 1:64 (p. 125 do datasheet)
	TMR0H = Preset_t0 >> 8;		// Registrador de contagem do Timer 0 - Parte alta
	TMR0L = Preset_t0;			// Registrador de contagem do Timer 0 - Parte baixa
	if(Interrupcao == Timer0_INT_ON) INTCONbits.GIEH = 1;		// Habilita interrup��es (p. 99 do datasheet)
}

// ============================================================================================	//
#endif