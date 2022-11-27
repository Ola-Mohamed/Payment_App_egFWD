/******************************************************************************
 *
 * Module: Terminal
 *
 * File Name: terminal.c
 *
 * Description: Source file for the Terminal module
 *
 * Author: Ola Mohamed
 *
 *******************************************************************************/
#include   "Termnial.h"

 /*******************************************************************************
  *                      Functions                                              *
  *******************************************************************************/

  /*******************************************************************************
   * [Function Name] : getTransactionDate
   *
   * [Description]   : A function to ask for transaction date(the date of the day)
   *            and check its length and you only have 2 trials
   *
   * [Args in]       : ST_terminalData_t *termData
   * [Args out]      : WRONG_DATE OR TERMINAL_OK
   *******************************************************************************/

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {

    printf("Please Enter The Transaction Date as DD/MM/YYYY : ");
	scanf_s("%s", termData->transactionDate, 12);
	if (strlen(termData->transactionDate) != 10 || (termData->transactionDate)[2] != '/' || (termData->transactionDate)[5] != '/')
	{
		printf("Wrong TransactionDate. \n");
		return WRONG_DATE;
	}
	return TERMINAL_OK;
}
/*******************************************************************************
 * [Function Name] : isCardExpired
 *
 * [Description]   : A function to check if the card is expired or not and print
 *            card and transaction date on screen
 *
 * [Args in]       : ST_cardData_t *cardData , ST_terminalData_t *termData
 * [Args out]      : EXPIRED_CARD OR TERMINAL_OK
 *******************************************************************************/
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
	uint8_t trans_year_val;
	uint8_t card_year_val;
	uint8_t trans_month_val;
	uint8_t card_month_val;
	uint8_t trans_day_val;
	uint8_t trans_year[3];

	trans_year[0] = termData.transactionDate[8];
	trans_year[1] = termData.transactionDate[9];

	uint8_t card_year[3];
	card_year[0] = cardData.cardExpirationDate[3];
	card_year[1] = cardData.cardExpirationDate[4];

	uint8_t trans_month[3];
	trans_month[0] = termData.transactionDate[3];
	trans_month[1] = termData.transactionDate[4];

	uint8_t card_month[3];
	card_month[0] = cardData.cardExpirationDate[0];
	card_month[1] = cardData.cardExpirationDate[1];

	uint8_t trans_day[3];
	trans_day[0] = termData.transactionDate[0];
	trans_day[1] = termData.transactionDate[1];

	trans_year_val = atoi(trans_year);
	card_year_val = atoi(card_year);
	trans_month_val = atoi(trans_month);
	card_month_val = atoi(card_month);
	trans_day_val = atoi(trans_day);

	if (card_year_val > trans_year_val || (card_month_val - trans_month_val) == 0 && (card_month_val - trans_month_val) > 0)
	{
		return TERMINAL_OK;
	}
	else
	{
		printf("\nExpired Card");
		return EXPIRED_CARD;
	}
}
/*******************************************************************************
 * [Function Name] : getTransactionAmount
 *
 * [Description]   : A function to ask for the client transaction amount which
 *            must be more than 0
 *
 * [Args in]       : ST_terminalData_t *termData
 * [Args out]      : INVALID_AMOUNT OR TERMINAL_OK
 *******************************************************************************/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {

		printf("Enter the transaction amount : ");
		scanf_s("%f", &(termData->transAmount));

		if (termData->transAmount <= 0) {
			printf("Invalid Amount of Transaction. \n");
			return INVALID_AMOUNT;
		}
		printf("Okay\n");
		return TERMINAL_OK;
}
/*******************************************************************************
 * [Function Name] : isBelowMaxAmount
 *
 * [Description]   : A function to check if the transaction amount is below the
 *            maximum amount or not
 *
 * [Args in]       : ST_terminalData_t *termData
 * [Args out]      : EXCEED_MAX_AMOUNT OR TERMINAL_OK
 *******************************************************************************/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData){
	if (termData->transAmount > termData->maxTransAmount)
	{
		//7
		printf("\nCannot make transaction becouse balance is low");
		return EXCEED_MAX_AMOUNT;
	}
	printf("\nYou can make a transaction");
	return TERMINAL_OK;
}
/*******************************************************************************
 * [Function Name] : setMaxAmount
 *
 * [Description]   : A function to set the maximum amount
 *
 * [Args in]       : ST_terminalData_t *termData , float maxAmount
 * [Args out]      : INVALID_MAX_AMOUNT OR TERMINAL_OK
 *******************************************************************************/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData){
	printf("Enter the transaction max amount : ");
	scanf_s("%f", &(termData->maxTransAmount));
	if (termData->maxTransAmount <= 0)
	{
		printf("Invalid Max Amount of Transaction. \n");
		return INVALID_MAX_AMOUNT;
	}
	return TERMINAL_OK;
}


void getTransactionDateTest(void)
{
	ST_terminalData_t termData;
	getTransactionDate(&termData);
	printf("%s", termData.transactionDate);
	printf("\n");
}

void isCardExpriedTest(void)
{
	ST_cardData_t cardData1 = { "Ola Mohamed","7452132574141354","02/21" };
	ST_cardData_t cardData2 = { "Ola Mohamed","7452132574141354","10/27" };
	ST_terminalData_t termData;
	getTransactionDate(&termData);
	printf("%s\n\n", termData.transactionDate);
	printf("Tester Name: Ola Mohamed \n");
	printf("Function Name: isCardExpired\n");
	printf("Test Case 1: expired card \n");
	printf("Input Data: 10/21 \n");
	printf("Expected Result:2 (EXPIRED_CARD) \n");
	printf("Actual Result: %d \n", isCardExpired(cardData1, termData));
	printf("\n");
	printf("Tester Name: Ola Mohamed\n");
	printf("Function Name: isCardExpired\n");
	printf("Test Case 2: not expired card \n");
	printf("Input Data: 05/23 \n");
	printf("Expected Result:0 (TERMINAL_OK) \n");
	printf("Actual Result: %d \n", isCardExpired(cardData2, termData));

}
void getTransactionAmountTest(void)
{
	ST_terminalData_t termData;

	printf("Tester Name: Ola Mohamed \n");
	printf("Function Name: getTransactionAmount\n");
	printf("Test Case 1: -ve value \n");
	printf("Input Data: -5 \n");
	printf("Expected Result:4 (INVALID_AMOUNT) \n");
	printf("Actual Result: %d \n", getTransactionAmount(&termData));
	printf("\n");
	printf("Tester Name: Ola Mohamed\n");
	printf("Function Name: getTransactionAmount\n");
	printf("Test Case 2: +ve value \n");
	printf("Input Data: 5.1 \n");
	printf("Expected Result:0 (TEMINAL_OK) \n");
	printf("Actual Result: %d \n", getTransactionAmount(&termData));
}


void isBelowMaxAmountTest(void)
{
	ST_terminalData_t termData;
	//setting terminal max amount 
	termData.maxTransAmount = 1500.0;

	printf("Tester Name: Ola Mohamed \n");
	printf("Function Name: isBelowMaxAmount\n");
	printf("Test Case 1: more than max amount (>1500)\n");
	printf("Input Data: 1650 \n");
	termData.transAmount = 1650;
	printf("Expected Result:5  (EXCEED_MAX_AMOUNT) \n");
	printf("Actual Result: %d \n", isBelowMaxAmount(&termData));

	printf("Tester Name: Ola Mohamed \n");
	printf("Function Name: isBelowMaxAmount\n");
	printf("Test Case 2: less than max amount (<1500)\n");
	printf("Input Data: 1250 \n");
	termData.transAmount = 1250;
	printf("Expected Result:0  (TERMINAL_OK) \n");
	printf("Actual Result: %d \n", isBelowMaxAmount(&termData));

	printf("Tester Name: Ola Mohamed \n");
	printf("Function Name: isBelowMaxAmount\n");
	printf("Test Case 3: equal to max amount (=1500)\n");
	printf("Input Data: 1500 \n");
	termData.transAmount = 1500;
	printf("Expected Result:0  (TERMINAL_OK) \n");
	printf("Actual Result: %d \n", isBelowMaxAmount(&termData));

}

void setMaxAmountTest(void)
{

	ST_terminalData_t termData;


	printf("Tester Name: Ola Mohamed \n");
	printf("Function Name: setMaxAmount\n");
	printf("Test Case 1: -ve number (-150)\n");
	printf("Input Data: -150 \n");
	printf("Expected Result: 6  (INVALID_MAX_AMOUNT) \n");
	printf("Actual Result: %d \n", setMaxAmount(&termData,1500));
	printf(" \n");
	printf("Tester Name: Ola Mohamed \n");
	printf("Function Name: setMaxAmount\n");
	printf("Test Case 2: zero (0)\n");
	printf("Input Data: 0 \n");
	printf("Expected Result: 6  (INVALID_MAX_AMOUNT) \n");
	printf("Actual Result: %d \n", setMaxAmount(&termData,1500));
	printf(" \n");
	printf("Tester Name:Ola Mohamed \n");
	printf("Function Name: setMaxAmount\n");
	printf("Test Case 3: +ve number  (2300)\n");
	printf("Input Data: 2300 \n");
	printf("Expected Result: 0  (TERMINAL_OK) \n");
	printf("Actual Result: %d \n", setMaxAmount(&termData,1500));
	printf(" \n");

}
