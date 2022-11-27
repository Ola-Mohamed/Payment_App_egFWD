/******************************************************************************
 *
 * Module: Server
 *
 * File Name: Server.h
 *
 * Description: Header file for the Server  module
 *
 * Author: Ola Mohamed
 *
 *******************************************************************************/

#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED
#include    "../Termnial/Termnial.h"
#include    "card.h"
#include	<stdio.h>
#include    <string.h>
#include    <stdlib.h>

 /*******************************************************************************
  *                      Typedef                                              *
  *******************************************************************************/
typedef enum EN_transState_t
{
     APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD, INTERNAL_SERVER_ERROR
}EN_transStat_t;

typedef struct ST_transaction_t
{
    ST_cardData_t cardHolderData;
    ST_terminalData_t terminalData;
    EN_transStat_t transState;
    uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef enum EN_serverError_t
{
    SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE, BLOCKED_ACCOUNT
}EN_serverError_t;

typedef enum EN_accountState_t
{
    RUNNING,
    BLOCKED
}EN_accountState_t;

typedef struct ST_accountsDB_t
{
    float balance;
    EN_accountState_t state;
    uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;


/*******************************************************************************
 *                       Functions Prototypes                                   *
 *******************************************************************************/
EN_transStat_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence);
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence);
EN_serverError_t saveTransaction(ST_transaction_t* transData);

//7
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);


void listSavedTransactions(void);
void recieveTransactionDataTest(void);
void isValidAccountTest(void);
void isBlockedAccountTest(void);
void isAmountAvailableTest(void);
void saveTransactionTest(void);



#endif // SERVER_H_INCLUDED
