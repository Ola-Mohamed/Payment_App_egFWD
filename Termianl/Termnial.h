/******************************************************************************
 *
 * Module: Terminal
 *
 * File Name: terminal.h
 *
 * Description: Header file for the Terminal module
 *
 * Author: Ola Mohamed
 *
 *******************************************************************************/

#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include	"Types.h"
#include	"../Card/card.h"
#include	<stdio.h>
#include    <string.h>
#include    <stdlib.h>

 /*******************************************************************************
  *                      Typedef                                              *
  *******************************************************************************/

typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[11];
}ST_terminalData_t;

typedef enum EN_terminalError_t
{
	TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t;

/*******************************************************************************
 *                       Functions Prototypes                                    *
 *******************************************************************************/

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
/*EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);*/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);


void getTransactionDateTest(void);
void isCardExpriedTest(void);
void getTransactionAmountTest(void);
void isBelowMaxAmountTest(void);
void setMaxAmountTest(void);

#endif // TERMINAL_H_INCLUDED
