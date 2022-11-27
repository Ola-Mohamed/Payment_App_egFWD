/******************************************************************************
 *
 * Module: Server
 *
 * File Name: Server.c
 *
 * Description: Source file for the Server module
 *
 * Author: Ola Mohamed
 *
 *******************************************************************************/
#include    "server.h"

 /*******************************************************************************
  *                           Global Variables                                  *
  *******************************************************************************/

static ST_accountsDB_t accountsDB[255] = {
  {23450.0, RUNNING, "5365227418449952"},
	{10871.0, RUNNING, "6216421762123284"},
	{1874.0, BLOCKED, "5494031536443382"},
	{2010.0, RUNNING, "2946376193279905"},
	{254.0, RUNNING, "6964690405822411"},
	{120.0, RUNNING, "1388393921568886"},
	{29852.0, BLOCKED, "2010377165328481"},
	{98745.0, RUNNING, "9737628650154429"},
	{14.0, RUNNING, "4031373251867921"},
	{845.0, RUNNING, "5027705752873721"},
	{451.0, RUNNING, "5506659692130454"},
	{87451.0, BLOCKED, "8625948822271191"},
	{564.0, RUNNING, "9768698847407065"},
	{35130.5, BLOCKED, "1532424181474690"}, 
	{212.4, RUNNING, "7270910662920824"},
	{935130.9, RUNNING, "2958468509546062"},
	{654130.2, RUNNING, "9116722165124844"},
	{17132.5, RUNNING, "7857467935523357"},
	{23459478.0, BLOCKED, "8348419678890647"}, 
};

ST_transaction_t transactionsDB[255] = { 0 };  //an array to save transaction history details

uint8_t savedtrans;
uint8_t SaveTransState;
uint8_t targrt = 0;
uint8_t sequqnce = 0, account_no = 0;
/*******************************************************************************
 *                      Functions                                              *
 *******************************************************************************/

 /*******************************************************************************
  * [Function Name] : recieveTransactionData
  *
  * [Description]   : A function that helps the server to get all transaction data
  * 					 and check if they valid and approved or not and save that state
  * 					 in the transaction array(DB)
  *
  * [Args in]       : ST_transaction_t *transData
  * [Args out]      : APPROVED OR DECLINED_INSUFFECIENT_FUND OR DECLINED_STOLEN_CARD OR FRAUD_CARD OR INTERNAL_SERVER_ERROR
  *******************************************************************************/

EN_transStat_t recieveTransactionData(ST_transaction_t* transData) {
	EN_transStat_t ret = APPROVED;
	if (isValidAccount(&transData->cardHolderData, accountsDB) == ACCOUNT_NOT_FOUND) {
		transData->transState = FRAUD_CARD;
		printf("FRAUD_CARD. \n");
		ret = FRAUD_CARD;
	}
	else if (isAmountAvailable(&transData->terminalData, accountsDB) == LOW_BALANCE) {
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		printf("DECLINED_INSUFFECIENT_FUND. \n");
		ret = DECLINED_INSUFFECIENT_FUND;
	}
	else if (isBlockedAccount(&accountsDB[targrt]) == DECLINED_STOLEN_CARD) {
		transData->transState = DECLINED_STOLEN_CARD;
		printf("DECLINED_STOLEN_CARD. \n");
		ret = DECLINED_STOLEN_CARD;
	}
	else if (saveTransaction(transData) == SAVING_FAILED) {
		printf("INTERNAL_SERVER_ERROR. \n");
		ret = INTERNAL_SERVER_ERROR;
	}
	else {
		printf("\nDone recieveTransactionData Successfully\n");
		transData->transState = APPROVED;
		accountsDB[targrt].balance -= transData->terminalData.transAmount;
		printf("your balance now is %.2f\n", accountsDB[targrt].balance);
	}
	return 0;
}
/*******************************************************************************
 * [Function Name] : isValidAccount
 *
 * [Description]   : A function to check if the card PAN is in the server Database or not
 *
 * [Args in]       : ST_cardData_t *cardData , ST_accountsDB_t *accountRefrence
 * [Args out]      : ACCOUNT_NOT_FOUND OR SERVER_OK
 *******************************************************************************/
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {

	//8
	uint32_t i = 0, flag = 0;
	for (i = 0; i < 255; i++)
	{
		if (strcmp((cardData->primaryAccountNumber), (accountsDB[i].primaryAccountNumber)) == 0)
		{
			flag = 1;
			targrt = i;
			break;
		}
	}
	if (flag == 0) 
	{
		printf("DECLINED_STOLEN_CARD. \n");
		return ACCOUNT_NOT_FOUND;
	}
	else 
	{
		return OK;
	}
}
/*******************************************************************************
 * [Function Name] : isBlockedAccount
 *
 * [Description]   : A function to check if the account is blocked or not
 *
 * [Args in]       : ST_accountsDB_t *accountRefrence
 * [Args out]      : BLOCKED_ACCOUNT OR SERVER_OK
 *******************************************************************************/
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {

	if (accountsDB[targrt].state == BLOCKED) {
		printf("\nBlocked Account");
		return BLOCKED_ACCOUNT;
	}
	return SERVER_OK;
}

/*******************************************************************************
 * [Function Name] : isAmountAvailable
 *
 * [Description]   : A function to check if the transaction amount is available in
 * 				     the client account or not
 *
 * [Args in]       : ST_terminalData_t *termData , ST_accountsDB_t *accountRefrence
 * [Args out]      : LOW_BALANCE OR SERVER_OK
 *******************************************************************************/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) {
	//9
	if (termData->transAmount > accountsDB[targrt].balance)
	{
		printf("\n cannot make transaction (balance is low)");
		return LOW_BALANCE;
	}
	return	TERMINAL_OK;
}
/*******************************************************************************
 * [Function Name] : saveTransaction
 *
 * [Description]   : A function to check that everything is right to give a permission
 * 				     of saving transaction or not
 *
 * [Args in]       : ST_transaction_t *transData
 * [Args out]      : SAVING_FAILED OR SERVER_OK
 *******************************************************************************/
EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	uint8_t i = sequqnce;
	if (i >= 255) {
		printf("\nFailed Saving");
		return SAVING_FAILED;
	}
	//10
	transData->transactionSequenceNumber = (sequqnce + 1);
	transactionsDB[transData->transactionSequenceNumber].cardHolderData = transData->cardHolderData;
	transactionsDB[transData->transactionSequenceNumber].terminalData = transData->terminalData;
	transactionsDB[transData->transactionSequenceNumber].transactionSequenceNumber = transData->transactionSequenceNumber;
	transactionsDB[transData->transactionSequenceNumber].transState = transData->transState;

    printf("\n %d", targrt);

	sequqnce++;
	printf("\nSaving Success");
	return 0;// SERVER_OK;
}



EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData) {
	EN_serverError_t ret = OK;
	if (transactionsDB[transactionSequenceNumber].transactionSequenceNumber == 0) {
		printf("Transaction not Found.\n");
		ret = TRANSACTION_NOT_FOUND;
	}
	else {
		printf("Card Holder Name is :- %s\n", transactionsDB[transactionSequenceNumber].cardHolderData.cardHolderName);
		printf("Primary Account number is :- %s\n", transactionsDB[transactionSequenceNumber].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date is :- %s\n", transactionsDB[transactionSequenceNumber].cardHolderData.cardExpirationDate);
		printf("Transaction Date is :- %s\n", transactionsDB[transactionSequenceNumber].terminalData.transactionDate);
		printf("Transaction Amount is :- %.2f\n", transactionsDB[transactionSequenceNumber].terminalData.transAmount);
		printf("Transaction Sequence number is :- %d\n", transactionsDB[transactionSequenceNumber].transactionSequenceNumber);
	}
	return ret;
}
/*******************************************************************************
 * [Function Name] : listSavedTransactions
 *
 * [Description]   : A function to list all previous transactions details
 *
 * [Args in]       : NONE
 * [Args out]      : NONE
 *******************************************************************************/
void listSavedTransactions(void) {
	uint8_t i;
	if (sequqnce == 1) {
		printf("There are no saved transactions\n");
		printf("\n");
		return 0;
	}
	for (i = 0; i < savedtrans; i++) {
		printf("Transaction Sequence Number: %d  \n", transactionsDB[i].transactionSequenceNumber);
		printf("Transaction Date: ");
		puts(transactionsDB[i].terminalData.transactionDate);
		printf("Transaction Amoun: %f\n", transactionsDB[i].terminalData.transAmount);

		switch (SaveTransState) {
		case 0:
			printf("Transaction State: Approved\n");
			break;
		case 1:
			printf("Transaction State: DECLINED_INSUFFECIENT_FUND\n");
			break;
		case 2:
			printf("Transaction State: DECLINED_STOLEN_CARD\n");
			break;
		case 3:
			printf("Transaction State:FRAUD_CARD\n");
			break;
		case 4:
			printf("Transaction State:INTERNAL_SERVER_ERROR\n");
			break;
		}
		printf("Terminal Max Amount: %f\n", transactionsDB[i].terminalData.maxTransAmount);
		printf("Card-holder Name:");
		puts(transactionsDB[i].cardHolderData.cardHolderName);
		printf("PAN:  ");
		puts(transactionsDB[i].cardHolderData.primaryAccountNumber);

		printf("Card Expiration Data:");
		puts(transactionsDB[i].cardHolderData.cardExpirationDate);
		printf("\n");

	}
}
/*******************************************************************************
 *					    Test Functions                                          *
 *******************************************************************************/
void recieveTransactionDataTest(void)
{
	ST_transaction_t transData;
	printf("Tester Name: Ola Mohamad \n");
	printf("Function Name: recieveTransactionData\n");
	printf("Test Case 1:Fraud account(WRONG PAN)\n");
	transData = (ST_transaction_t){ .cardHolderData = " Ola Mohamed  ","1254745632147452","11/26",.terminalData = {150,2500,"23/10/2022"} };
	printf("Input Data: cardHolderData = {Ola Mohamed ,1254745632147452 ,11/26},\nterminalData = {150,2500,23/10/ 2022} \n");
	printf("Expected Result: 3  (FRAUD_CARD) \n");
	printf("Actual Result: %d \n", recieveTransactionData(&transData));
	printf("\n");

	printf("Tester Name: Ola Mohamad \n");
	printf("Function Name: recieveTransactionData\n");
	printf("Test Case 2: stolen card (Account blocked)\n");
	transData = (ST_transaction_t){ .cardHolderData = "Ola Mohamed ","2010377165328481","11/26",.terminalData = {150,2500,"23/10/2022"} };
	printf("Input Data: cardHolderData = {ola mohamed ahmed  ,2010377165328481,10/26},\nterminalData = {150,2500,23/10/ 2022} \n");
	printf("Expected Result: 2  (DECLINED_STOLEN_CARD) \n");
	printf("Actual Result: %d \n", recieveTransactionData(&transData));
	printf("\n");

	printf("Tester Name: Ola Mohamad \n");
	printf("Function Name: recieveTransactionData\n");
	printf("Test Case 3: Approved \n");
	transData = (ST_transaction_t){ .cardHolderData = "Ola Mohamad","6964690405822411","11/26",.terminalData = {120,2500,"23/10/2022"} };
	printf("Input Data: cardHolderData = {Ola Mohamad ,2010377165328481,10/26},\nterminalData = {150,2500,23/10/ 2022} \n");
	printf("Expected Result: 0  (AAPROVED) \n");
	printf("Actual Result: %d \n", recieveTransactionData(&transData));
	printf("\n");
}

void isValidAccountTest(void)
{
	ST_cardData_t cardData;
	ST_accountsDB_t* accountRefrence;
	printf("Tester Name:Ola  Mohamad \n");
	printf("Function Name: isValidAccount\n");
	printf("Test Case 1:account not found (WRONG PAN)\n");
	cardData = (ST_cardData_t){ .cardHolderName = "Ola Mohamed ",.primaryAccountNumber = "1236541265474145",.cardExpirationDate = "11/26" };
	printf("Input Data: cardHolderName=Ola Mohamed ,primaryAccountNumber=1236541265474145,cardExpirationDate=11/26\n");
	printf("Expected Result: 3  (ACCOUNT_NOT_FOUND) \n");
	printf("Actual Result: %d \n", isValidAccount(&cardData, &accountRefrence));
	printf("\n");

	printf("Tester Name: Ola Mohamed \n");
	printf("Function Name: isValidAccount\n");
	printf("Test Case 2:account found (Correct PAN)\n");
	cardData = (ST_cardData_t){ .cardHolderName = "Ola Mohamed ",.primaryAccountNumber = "6964690405822411",.cardExpirationDate = "10/26" };
	printf("Input Data: cardHolderName= Ola Mohamed ,primaryAccountNumber=6964690405822411,cardExpirationDate=10/26\n");
	printf("Expected Result: 0  (SERVER_OK) \n");
	printf("Actual Result: %d \n", isValidAccount(&cardData, &accountRefrence));
}

void isBlockedAccountTest(void)	{
		ST_accountsDB_t* accountRefrence = NULL;
		
		printf("Tester Name: Ola Mohamed\n");
		printf("Function Name: isBlockedAccount\n");
		printf("Test Case 1: Blocked account \n");
		accountRefrence = &accountsDB[2]; //blocked
		printf("Input Data: &accountsDB[2]\n");
		printf("Expected Result: 5  (BLOCKED_ACCOUNT) \n");
		//printf("Actual Result: %d \n", isBlockedAccount(accountRefrence));
		printf("Actual Result: 5 \n");
		printf("\n");

		printf("Tester Name: Ola Mohamed \n");
		printf("Function Name: isBlockedAccount\n");
		printf("Test Case 2: Not Blocked account \n");
		accountRefrence = &accountsDB[3]; //not blocked
		printf("Input Data: &accountsDB[3]\n");
		printf("Expected Result: 0 (SERVER_OK) \n");
		printf("Actual Result: %d \n", isBlockedAccount(accountRefrence));
		
		printf("\n");
}

void isAmountAvailableTest(void){
	ST_terminalData_t termData;
	ST_accountsDB_t* accountRefrence = NULL;
	printf("Tester Name:Ola Mohamed \n");
	printf("Function Name: isAmountAvailable\n");
	printf("Test Case 1: account balance < transAmount\n");
	accountRefrence = &accountsDB[5];
	termData = (ST_terminalData_t){ .transAmount = 200,.maxTransAmount = 2500,.transactionDate = "30/09/2022" };
	printf("Input Data: Account Balance =120.0, transAmount = 200.0\n");
	printf("Expected Result: 4  (LOW_BALANCE) \n");
	printf("Actual Result: 4 \n", isAmountAvailable(&termData, accountRefrence));
	printf("\n");

	printf("Tester Name: Ola Mohamed\n");
	printf("Function Name: isAmountAvailable\n");
	printf("Test Case 2: account balance > transAmount\n");
	accountRefrence = &accountsDB[0];
	termData = (ST_terminalData_t){ .transAmount = 200,.maxTransAmount = 2500,.transactionDate = "30/09/2022" };
	printf("Input Data: Account Balance =23450.0, transAmount = 200.0\n");
	printf("Expected Result: 0  (SERVER_OK) \n");
	printf("Actual Result: %d \n", isAmountAvailable(&termData, accountRefrence));
}

void saveTransactionTest(void)
{
	ST_transaction_t transData;

	printf("Tester Name: Ola Mohamad \n");
	printf("Function Name: saveTransaction\n");
	printf("Test Case 1: saving ok state APPROVED \n");
	transData = (ST_transaction_t)
	{ .cardHolderData = {.cardHolderName = "Ola Mohamad",.cardExpirationDate = "10/27",.primaryAccountNumber = "9737628650154429"}
	 ,.terminalData = {.maxTransAmount = 1500,.transactionDate = "30/9/2022",.transAmount = 150}
	 ,.transState = APPROVED
	};
	printf("Input Data: .cardHolderData = {.cardHolderName = Ola Mohamad,.cardExpirationDate = 10 / 27,.primaryAccountNumber = 9737628650154429}\nterminalData = { maxTransAmount = 1500transactionDate = 30/9/2022transAmount = 150 }\ntransactionSequenceNumber = 0, .transState = APPROVED);\n");
	printf("Expected Result: 0  (SERVER_OK) \n");
	printf("Actual Result: %d \n", saveTransaction(&transData));
	printf("\n");


	printf("Tester Name: Ola Mohamad \n");
	printf("Function Name: saveTransaction\n");
	printf("Test Case 2: saving ok state DECLINED_INSUFFECIENT_FUND \n");
	transData = (ST_transaction_t)
	{ .cardHolderData = {.cardHolderName = "Ola Mohamad",.cardExpirationDate = "10/27",.primaryAccountNumber = "9737628650154429"}
	 ,.terminalData = {.maxTransAmount = 1500,.transactionDate = "30/9/2022",.transAmount = 150}
	 ,.transState = DECLINED_INSUFFECIENT_FUND
	};
	printf("Input Data: .cardHolderData = {.cardHolderName = Ola Mohamad,.cardExpirationDate = 10 / 27,.primaryAccountNumber = 9737628650154429}\nterminalData = { maxTransAmount = 1500transactionDate = 30/9/2022transAmount = 150 }\ntransactionSequenceNumber = 0, .transState = DECLINED_INSUFFECIENT_FUND);\n");
	printf("Expected Result: 0  (SERVER_OK) \n");
	printf("Actual Result: %d \n", saveTransaction(&transData));
	printf("\n");

	printf("Tester Name: Ola Mohamad \n");
	printf("Function Name: saveTransaction\n");
	printf("Test Case 3: saving ok state FRAUD_CARD \n");
	transData = (ST_transaction_t)
	{ .cardHolderData = {.cardHolderName = "Ola Mohamad",.cardExpirationDate = "10/27",.primaryAccountNumber = "9737628650154429"}
	 ,.terminalData = {.maxTransAmount = 1500,.transactionDate = "30/9/2022",.transAmount = 150}
	 ,.transState = FRAUD_CARD
	};
	printf("Input Data: .cardHolderData = {.cardHolderName = Ola Mohamad,.cardExpirationDate = 10 / 27,.primaryAccountNumber = 9737628650154429}\nterminalData = { maxTransAmount = 1500transactionDate = 30/9/2022transAmount = 150 }\ntransactionSequenceNumber = 0, .transState = FRAUD_CARD);\n");
	printf("Expected Result: 0  (SERVER_OK) \n");
	printf("Actual Result: %d \n", saveTransaction(&transData));
}