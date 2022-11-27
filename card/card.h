/******************************************************************************
 *
 * Module: Card
 *
 * File Name: Card.h
 *
 * Description: Header file for the Card module
 *
 * Author: Ola Mohamed
 *
 *******************************************************************************/
#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include	"Types.h"
#include	<stdio.h>
#include    <string.h>
#include	<stdlib.h>

 /*******************************************************************************
  *                      Typedef                                              *
  *******************************************************************************/

typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
	OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;


/*******************************************************************************
 *                       Functions Prototypes                                    *
 *******************************************************************************/

EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);

void getCardHolderNameTest(void);
void getCardExpiryDateTest(void);
void getCardPANTest(void);

#endif // CARD_H_INCLUDED
