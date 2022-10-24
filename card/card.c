/******************************************************************************
 *
 * Module: Card
 *
 * File Name: Card.c
 *
 * Description: Source file for the Card module
 *
 * Author: Ola Mohamed
 *
 *******************************************************************************/

#include    "card.h"
#include	<stdio.h>
#include    <string.h>

 /*******************************************************************************
  *                      Functions                                              *
  *******************************************************************************/

  /*******************************************************************************
   * [Function Name] : getCardHolderName
   *
   * [Description]   : A function to ask for card holder name and check its length
   *            and you only have 2 trials
   *
   * [Args in]       : ST_cardData_t *cardData
   * [Args out]      : WRONG_NAME , OK
   *******************************************************************************/

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	uint8_t i;
	uint8_t buff[35];


	printf("\nPlease Enter The Card Holder Name:");
	gets(buff);

	if ((buff[0] == NULL) ||  (strlen(buff)< 20) || (strlen(buff) >24))
	{

		return WRONG_NAME;
	}
	else {
		for (i = 0; i <= strlen(buff); i++)
		{
			cardData->cardHolderName[i] = buff[i];
		}
		return OK;
	}
}
/*******************************************************************************
 * [Function Name] : getCardExpiryDate
 *
 * [Description]   : A function to ask for card expiry date and check its length
 *            and format and you only have 2 trials
 *
 * [Args in]       : ST_cardData_t *cardData
 * [Args out]      : WRONG_NAME , OK
 *******************************************************************************/
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {

	uint8_t buff[10];
	uint8_t i;

	printf("\nPlease enter card Expiry Date (MM/YY): ");
	gets(buff);
	//fflush(stdin);
	int month = (buff[0] - 48) * 10 + (buff[1] - 48);

	if ((buff == NULL) || (strlen(buff) < 5) || (strlen(buff) > 5)
		|| month > 12 || month < 0 || (buff[2] != '/'))
	{
		return WRONG_EXP_DATE;
	}
	else
	{
		for (i = 0; i <= strlen(buff); i++)
		{
			cardData->cardExpirationDate[i] = buff[i];
		}
		return OK;
	}
}
/*******************************************************************************
 * [Function Name] : getCardPAN
 *
 * [Description]   : A function to ask for card PAN number and check its length
 *            and you only have 2 trials
 *
 * [Args in]       : ST_cardData_t *cardData
 * [Args out]      : WRONG_PAN OR CARD_OK
 *******************************************************************************/
EN_cardError_t getCardPAN(ST_cardData_t* cardData) {

	
	uint8_t buff[25];
	uint8_t	i;
	printf("\nPlease enter account number: ");
	fflush(stdin);
	gets(buff);

	if (strlen(buff) < 16 || strlen(buff) > 20)
	{
	return WRONG_PAN;
	}
	else
	{
		for (i = 0; i < strlen(buff); i++)
		{
			cardData->primaryAccountNumber[i] = buff[i];
		}
		cardData->primaryAccountNumber[strlen(buff)] = '\0';

		return OK;
	}
}
/*******************************************************************************
 *                         Test Cases                                           *
 *******************************************************************************/
void getCardHolderNameTest(void) {

	ST_cardData_t cardData;


	printf("Tester Name :Ola Mohamed  \n");
	printf("Function Name:getCardHolderName  \n");
	printf("Test Case 1: less than 20 characters \n");
	printf("Input Data: ola \n");
	printf("Expected Result: 1 (WRONG_NAME) \n");
	printf("Actual Result: %d \n", getCardHolderName(&cardData));
	//printf("\n");

	printf("Tester Name: Ola Mohamed \n");
	printf("Function Name: getCardHolderName\n");
	printf("Test Case 2: more than 20 characters \n");
	printf("Input Data: Ola Mohamed Ahmed Mohamed Metwaly \n");
	printf("Expected Result: 1 (WRONG_NAME) \n");
	printf("Actual Result: %d \n", getCardHolderName(&cardData));
	//printf("\n");

	printf("Tester Name:  Ola Mohamed\n");
	printf("Function Name: getCardHolderName\n");
	printf("Test Case 3: not less than 20 characters or more than 24 characters \n");
	printf("Input Data: ola mohamed ahmed moha\n");
	printf("Expected Result: 0 (OK) \n");
	printf("Actual Result: %d \n", getCardHolderName(&cardData));

}
void getCardExpiryDateTest(void)
{
	ST_cardData_t cardData;

	printf("Tester Name: Ola Mohamad \n");
	printf("Function Name: getCardExpiryDate\n");
	printf("Test Case 1: bad format yy/mm \n");
	printf("Input Data: 22/10 \n");
	printf("Expected Result: 2 (WRONG_EXP_DATE) \n");
	printf("Actual Result: %d \n", getCardExpiryDate(&cardData));
	printf("\n");

	printf("Tester Name: Ola Mohamed\n");
	printf("Function Name: getCardExpiryDate\n");
	printf("Test Case 2: good format but month bigger than 12 \n");
	printf("Input Data: 15/24 \n");
	printf("Expected Result: 2 (WRONG_EXP_DATE) \n");
	printf("Actual Result: %d \n", getCardExpiryDate(&cardData));
	printf("\n");

	printf("Tester Name: Ola Mohamad \n");
	printf("Function Name: getCardExpiryDate\n");
	printf("Test Case 3: good format and good input \n");
	printf("Input Data: 10/23 \n");
	printf("Expected Result: 0 (CARD_OK) \n");
	printf("Actual Result: %d \n", getCardExpiryDate(&cardData));
	printf("\n");

	printf("Tester Name: Ola Mohamad \n");
	printf("Function Name: getCardExpiryDate\n");
	printf("Test Case 6:changing slash  \n");
	printf("Input Data: 10-23\n");
	printf("Expected Result: 2 (WRONG_EXP_DATE) \n");
	printf("Actual Result: %d \n", getCardExpiryDate(&cardData));

}

void getCardPANTest(void)
{
	ST_cardData_t cardData;

	printf("Tester Name: Ola Mohamad \n");
	printf("Function Name: getCardPAN\n");
	printf("Test Case 1: less than 16 (eg. 14 numbers) \n");
	printf("Input Data: 15423652301747 \n");
	printf("Expected Result: 3 (WRONG_PAN) \n");
	printf("Actual Result: %d \n", getCardPAN(&cardData));
	printf("\n");

	printf("Tester Name:Ola Mohamad \n");
	printf("Function Name: getCardPAN\n");
	printf("Test Case 2: more than 20 (eg. 22 numbers)\n");
	printf("Input Data: 5417912032741589632014 \n");
	printf("Expected Result: 3 (WRONG_PAN) \n");
	printf("Actual Result: %d \n", getCardPAN(&cardData));
	printf("\n");

	printf("Tester Name:Ola Mohamad \n");
	printf("Function Name: getCardPAN\n");
	printf("Test Case 3: not more than 20 nor less than 16\n");
	printf("Input Data: 1745963210878996325 \n");
	printf("Expected Result: 0 (OK) \n");
	printf("Actual Result: %d \n", getCardPAN(&cardData));
}