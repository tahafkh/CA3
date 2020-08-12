#include"functions.h"
int main() {
	int exit = 0,log=0;
	char username[20], password[20];
	user* head = readFromFile();//starting the program from reading database file
	printf("Welcome to UT Bank!\nFirst you should log in either as admin or as a regular user!\n");
	while (exit == 0) {
		login(username, password, head, &log);//login either as admin or regular user
		exit=commandOperations(head, log, username);//operations in login statue and check  if user wants to exit or not
		if (exit == 1)
			exit = 0;
	}
	printf("Hope you had a convenient experience! :)\n");
	writeToFile(head);//writing datas to database file and exit the program
}