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
#include "app.h"
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

	getCardHolderName(&card);
	getCardExpiryDate(&card);
	getCardPAN(&card);

	getTransactionDate(&term);
	isCardExpired(card, term);
	setMaxAmount(&term);
	getTransactionAmount(&term);

	isBelowMaxAmount(&term);

	trans.cardHolderData = card; // UPDATE CARD_HOLDER_DATA
	trans.terminalData = term;
	saveTransaction(&trans);

	//getTransaction(transData_1.transactionSequenceNumber, &transData_1);

	recieveTransactionData(&trans);

	getTransaction(1, &trans);
	
	savedtrans++;

}

