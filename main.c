#include <stdio.h>
#include "APP/app.h"

int main(void) {
	static char ch[1] = "1";
	printf("1- New Transaction\n");
	printf("2- Show previous Transactions\n");
	printf("3- Exit (Saved data will be erased!!)\n");
	while (1)
	{
		printf("choice: ");
		gets(ch);
		switch (ch[0])
		{
		case '1':
			appstart();
			break;
		case '2':
			listSavedTransactions();
			break;
		case '3':
			printf("GoodBye :)\n");
			break;
		}
	}

}
