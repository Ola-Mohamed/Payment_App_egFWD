/******************************************************************************
 *
 * Module: APP
 *
 * File Name: APP.c
 *
 * Description: Header file for the APP  module
 *
 * Author: Ola Mohamed
 *
 *******************************************************************************/

#include "App.h"
#include "server.h"
 /*******************************************************************************
   *                      Global Variables                                        *
  *******************************************************************************/
ST_cardData_t card;       
ST_terminalData_t term;
ST_transaction_t trans;
EN_transStat_t transState;
ST_accountsDB_t accref;
extern uint8_t savedtrans;
	
	
/*******************************************************************************
 *                      Functions                                              *
 *******************************************************************************/

 /*******************************************************************************
  * [Function Name] : appStart
  *
  * [Description]   : A function to start the application and proceed in the transaction process
  *
  * [Args in]       : NONE
  * [Args out]      : NONE
  *******************************************************************************/
void appstart(void)
{
	while (getCardHolderName(&card) == WRONG_NAME)
	{
		printf("Unvalid name, Try again \n");
	}
	printf("**valid Name**\n");

	while (getCardExpiryDate(&card) == WRONG_EXP_DATE)
	{
		printf("Unvalid expiry date, Try again \n");
	}
	printf("**valid Expiration Date**\n");

	while (getCardPAN(&card) == WRONG_PAN)
	{
		printf("Unvalid PAN, Try again \n");
	}
	printf("**valid Card PAN Format **\n");

	trans.cardHolderData = card;

	if (getTransactionDate(&term) != TERMINAL_OK) {
		printf("Wrong Transaction Date format\n");
		return;
	}
	else {
		printf("**valid Transaction Date**\n");
	}

	if (isCardExpired(card, term) != TERMINAL_OK) {
		printf("expired card\n");
		return;
	}
	else {
		printf("**valid card Exp Date**\n");
	}

	if (setMaxAmount(&term, 2500) != TERMINAL_OK) {
		printf("Invalid Maximum Transaction Amount!!\n");
		return;
	}
	else {
		printf("MAX amount is set\n");
	}

	if (getTransactionAmount(&term) != TERMINAL_OK) {
		printf("Invalid_Amount!!\n");
		return;
	}
	trans.terminalData = term;

	if (isBelowMaxAmount(&term) != TERMINAL_OK) {
		printf("You have exceeded the Maximum Transaction Amount!!");
		return;
	}
	else {
		
	//	printf("transaction accepted\n");
		printf("Transaction Not received!!\n");
	
	}

	if (recieveTransactionData(&trans.cardHolderData) != SERVER_OK) {
		printf("Transaction Not received!!\n");
		return;
	}
	else {
		printf("Transaction received\n");
		savedtrans++;
	}
}

