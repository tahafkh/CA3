#include "functions.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
transaction* headTransaction() {//first transaction which is not important
	transaction* headtrans = (transaction*)malloc(sizeof(transaction));
	if (headtrans == NULL)
		printf("error in allocating memory!\n");
	else {
		headtrans->next = NULL;
		return headtrans;
	}
}
user* search(char *username,user*head) {//find a user's node by searching the names
	user*current = head;
	while (current != NULL) {
		if (strcmp(username, current->name) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}
transaction* newTrans(transaction* head_trans) {//creating a new transaction
	transaction* newtrans = (transaction*)malloc(sizeof(transaction));
	transaction*temp_trans = head_trans;
	if (newtrans == NULL)
		printf("error in allocating memory\n");
	else {
		while (temp_trans->next != NULL)
			temp_trans = temp_trans->next;
		temp_trans->next = newtrans;
		newtrans->next = NULL;
		return newtrans;
	}
}
user* headUser() {//admin user
	user* head= (user*)malloc(sizeof(user));
	if (head  == NULL)
		printf("error in allocating memory!\n");
	else {
		head->balance = 0.00000;
		head->first = NULL;
		head->id = 0;
		strcpy(head->name, "admin");
		strcpy(head->password, "123");
		strcpy(head->phoneNumber, "00000000000");
		head->numsoftransactions = 0;
		head->next = NULL;
		return head;
	}
}
user* newUser(user*head_user) {//creating a new user
	user* new_user=(user*)malloc(sizeof(user));
	if (new_user == NULL)
		printf("error in allocating memory!\n");
	else {
		user* temp_user = head_user;
		while (temp_user->next != NULL)
			temp_user = temp_user->next;
		temp_user->next = new_user;
		new_user->next = NULL;
		return new_user;
	}
}
user * readFromFile() {//reading from file and creating linked list for users
	FILE *file = fopen("UT Bank.txt", "r");
	user* head = headUser();
	user* new_user;
	transaction* new_trans;
	char name[20];
	char type[20];
	if (file == NULL)
		printf("Error in opening file!");
	else {
		while (1) {
			if ((fscanf(file, "%s", name)) == EOF)
				break;
			else {
				new_user=newUser(head);
				strcpy(new_user->name, name);
				fscanf(file, " %s %ld %s %lf %d", new_user->password, &(new_user->id), new_user->phoneNumber, &(new_user->balance), &(new_user->numsoftransactions));
				if (new_user->numsoftransactions) {
					new_user->first = headTransaction();
				}
				for (int i = 0; i < new_user->numsoftransactions; i++) {
					new_trans=newTrans(new_user->first);
					fscanf(file, "%s %lf", new_trans->type, &(new_trans->value));
				}
			}
		}
	}
	fclose(file);
	return head;
}
void login(char *username, char* password,user *head,int *log) {//logging in
	user*current = head->next;
	*log = 0;
	printf("login ");
	scanf("%s %s", username, password);
	getchar();
	if (strcmp("admin",username)==0) {
		if (strcmp("123", password) == 0) {
			printf("admin has logged in successfully!\n");
			(*log) = 2;
		}
	}
	else {
		while (current != NULL) {
			if (strcmp(username, current->name) == 0 && strcmp(password, current->password) == 0) {
				*log = 1;
				printf("%s has logged in successfully!\n",username);
				break;
			}
			current = current->next;
		}
	}
	if (*log == 0) {
		printf("error in login!please try again!\n");
		login(username, password, head,log);
	}
}
void add_user(user *head,int log) {//creating a new user
	char name[20], password[20],phoneNumber[20];
	user*current = head;
	int available = 0,max=0;
	scanf(" %s %s %s", name, password, phoneNumber);
	getchar();
	if (log == 1)
		printf("Sorry but you can't do that! :)\n");
	else {
		while (current != NULL) {
			if (strcmp(current->name, name) == 0 || strcmp(current->phoneNumber, phoneNumber) == 0) {
				available = 1;
				break;
			}
			if (current->id > max)
				max = current->id;
			current = current->next;
		}
		if (available == 1)
			printf("same user is available!\n");
		else {
			current=newUser(head);
			current->balance = 0.000000;
			if (max == 0)
				current->id = 810198000;
			else
				current->id = max + 1;
			current->numsoftransactions = 0;
			strcpy(current->phoneNumber,phoneNumber);
			strcpy(current->name, name);
			strcpy(current->password, password);
			printf("add user is done successfully!\n");
		}
	}
}
void deposit(user*head, int log) {//deposit value to users
	user* current;
	transaction*new_trans;
	char name[20];
	double value;
	scanf(" %s %lf", name, &value);
	getchar();
	if (log == 1) {
		printf("Sorry but you can't do that! :)\n");
	}
	else {
		if ((current = search(name, head)) != NULL) {
			if (current->numsoftransactions == 0) {
				current->first = headTransaction();
			}
			new_trans = newTrans(current->first);
			new_trans->value = value;
			current->balance += value;
			strcpy(new_trans->type, "Deposit");
			current->numsoftransactions++;
			printf("deposit is done successfully!\n");
		}
		else
			printf("No such user found!\n");
	}
}
void withdraw(user*head, int log) {//withdraw value from users
	user* current;
	transaction* new_trans;
	char name[20];
	double value;
	scanf(" %s %lf", name, &value);
	getchar();
	if (log == 1) {
		printf("Sorry but you can't do that! :)\n");
	}
	else {
		if ((current = search(name, head)) != NULL) {
			if (value <= current->balance) {
				if (current->numsoftransactions == 0) {
					current->first = headTransaction();
				}
				new_trans = newTrans(current->first);
				current->balance -= value;
				new_trans->value = value;
				strcpy(new_trans->type, "Withdraw");
				current->numsoftransactions++;
				printf("withdraw is done successfully!\n");
			}
			else
				printf("Not enough balance for withdraw!\n");
		}
		else
			printf("No such user found\n");
	}
}
void transfer(char*username, int log, user* head) {//transfer value from a user to another one
	char destname[20];
	double value;
	int available = 0;
	user *current = head->next;
	user *current2 = head->next;
	transaction*  new_trans1;
	transaction* new_trans2;
	scanf(" %s %lf", destname, &value);
	getchar();
	if (log == 2) {
		printf("Sorry but you can't do that! :)\n");
	}
	else {
		if ((current = search(username, head)) != NULL && (current2 = search(destname, head)) != NULL) {
			if (value <= current->balance) {
				if (current->numsoftransactions == 0)
					current->first = headTransaction();
				if (current2->numsoftransactions == 0)
					current2->first = headTransaction();
				new_trans1 = newTrans(current->first);
				new_trans2 = newTrans(current2->first);
				current->balance -= value;
				current2->balance += value;
				new_trans1->value = value;
				new_trans2->value = value;
				strcpy(new_trans1->type, "TransferOut");
				strcpy(new_trans2->type, "TransferIn");
				current->numsoftransactions++;
				current2->numsoftransactions++;
				printf("transfer is done successfully!\n");
			}
			else
				printf("Not enough balance for transfer!\n");
		}
		else
			printf("No such user found!\n");
	}
}
void delete(user* head, int log) {//delete a user
	user*current = head->next;
	user*previous = head;
	transaction*temp;
	char username[20];
	int available = 0;
	scanf(" %s", username);
	getchar();
	if (log == 1) {
		printf("Sorry but you can't do that! :)\n");
	}
	else {
		while (current != NULL) {
			if (strcmp(username, current->name) == 0) {
				available = 1;
				if (current->numsoftransactions > 0) {
					while (current->first->next != NULL) {
						temp = current->first->next->next;
						free(current->first->next);
						current->first->next = temp;
					}
					free(current->first);
				}
				previous->next = current->next;
				free(current);
				printf("delete is done successfully!\n");
				break;
			}
			previous = current;
			current = current->next;
		}
	}
	if (available == 0 && log == 2)
		printf("No such user found!\n");
}
void view(user* head, int log, char*username) {//view informations for a user
	user*current = head->next;
	transaction* current2;
	char username2[20];
	if (log == 1) {
		current = search(username, head);
		printf("ID:%ld Name:%s PhoneNumber:%s Balance:%lf\n", current->id, current->name, current->phoneNumber, current->balance);
		if (current->numsoftransactions) {
			current2 = current->first->next;
			while (current2 != NULL) {
				printf("%s : %lf\n", current2->type, current2->value);
				current2 = current2->next;
			}
		}
	}
	else if (log == 2) {
		scanf(" %s", username2);
		getchar();
		if ((current = search(username2, head)) != NULL) {
			printf("ID:%ld Name:%s PhoneNumber:%s Balance:%lf\n", current->id, current->name, current->phoneNumber, current->balance);
			if (current->numsoftransactions) {
				current2 = current->first->next;
				while (current2 != NULL) {
					printf("%s : %lf\n", current2->type, current2->value);
					current2 = current2->next;
				}
			}
		}
		else
			printf("No such user found!\n");
	}
}
int commandOperations(user*head, int log,char *username) {//know which operation user wants to do
	char operation[20],trashInput;
	int exit = 0;
	while (exit == 0) {
		scanf("%s", operation);
		if (strcmp(operation, "add_user") == 0)
			add_user(head,log);
		else if (strcmp(operation, "deposit") == 0)
			deposit(head, log);
		else if (strcmp(operation, "withdraw") == 0)
			withdraw(head, log);
		else if (strcmp(operation, "view") == 0)
			view(head, log, username);
		else if (strcmp(operation, "delete") == 0)
			delete(head, log);
		else if (strcmp(operation, "transfer") == 0)
			transfer(username, log, head);
		else if (strcmp(operation,"logout")==0) {
			getchar();
			exit = 1;
		}
		else if (strcmp(operation, "exit") == 0) {
			getchar();
			exit = 2;
		}
		else {
			scanf("%c", &trashInput);
			while (trashInput != '\n') {
				scanf("%c", &trashInput);
			}
			printf("not defined in operations!\n");
		}
	}
	return exit;
}
void writeToFile(user*head) {//write datas to file and free the linked list
	FILE *file = fopen("UT Bank.txt", "w");
	user* current = head->next;
	transaction*current2;
	transaction*temp;
	if (file == NULL)
		printf("error in opening file!\n");
	else {
		while (current != NULL) {
			fprintf(file, "%s %s %ld %s %lf %d\n", current->name, current->password, current->id, current->phoneNumber, current->balance, current->numsoftransactions);
			if (current->numsoftransactions) {
				current2 = current->first->next;
				while (current2 != NULL) {
					fprintf(file,"%s %lf\n", current2->type, current2->value);
					current2 = current2->next;
				}
			}
			current = current->next;
		}
		current = head->next;
		while (current != NULL) {
			if (current->numsoftransactions) {
				while (current->first->next != NULL) {
					temp = current->first->next->next;
					free(current->first->next);
					current->first->next = temp;
				}
				free(current->first);
			}
			head->next = current->next;
			free(current);
			current = head->next;
		}
		free(head);
	}
	fclose(file);
}