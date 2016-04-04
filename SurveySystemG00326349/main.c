//Survey system implemented using linked lists
//Ryan Gordon - G00326349
#include<stdio.h>
#include<stdlib.h>

struct payload {
	int ppsNo;
	char fName[20];
	char lName[20];
	char email[40];
	char address[50];
	int age;
	//survey details
	int ciggiesSmoked;
	int timesExercised;
	int income;
	int unitsTaken;

	//possible to insert int defining the various brackets for surveys

};//using a struct which will hold the survey details within the node
  //a node containing a next pointer and a payload of details for the person
struct survey
{

	struct payload surveyDetails;
	struct survey* next;
};
int login();
void openFile();
void closeFile();
void addSurvey(struct survey** head_ptr);
void insertionSort(struct survey **head_ref);
void sortedInsert(struct survey** head_ref, struct survey* new_node);
void printElements(struct survey* head_ptr);
void searchList(struct survey *head_ptr, int searchPPS, char fName[20], char lName[20]);
void updateSurvey(struct survey *head_ptr, int searchPPS, char fName[20], char lName[20]);
void deletedSpecifiedSurvey(struct survey **head_ptr, int searchPPS);
void generateSurvey(int input, int surveyType);
void compileSurvey();
void openReportFile();
void printReports(struct survey* head_ptr);
void importReports(struct survey** head_ptr);

FILE *fptr;
char searchFName[20] = " ", searchLName[20] = " ";
int searchNum = 0, searchChoice = 0;
char approvedUsrnames[3][7];
char approvedPassword[3][7];
int totalSurveys; //total amount of surveys taken to be used for generating statistics
int smokerBrackets[4], drinkerBrackets[4], exerciseBrackets[4];
int incomeBrackets[7], ageBrackets[5];//array where [0] is >20 cigs; [1] more than >40;[2] is <40 cigs [3] is any smoker
float smokerStats[4], drinkerStats[4], exerciseStats[4];
float incomeStats[7], ageStats[5];
char smokerPhrases[4][25] = { "any amount", "less than 20","less than 40","more than 40" };
char drinkerPhrases[4][25] = { "no amount at all", "less than 2 units","less than 4 units","more than 4 units" };
char exercisePhrases[4][30] = { "never exercise during", "exercise less than 3 times","exercise less than 5 times","exercise more than 5 times" };
char incomePhrases[7][25] = { "no income", "less than 20,000","less than 40,000","less than 60,000","less than 80,000", "less than 100,000", "more than 100,000" };
char agePhrases[5][25] = { "18-20", "20-30","30-50","50-65","65+" };


void main()
{
	struct survey *top;
	int choice;
	top = NULL;

	while (login() == 1) {

		importReports(&top);
		//import surveys if any
		do
		{

			printf("Please Enter your choice!\n");
			printf(" 1: To add an element!\n");
			printf(" 2: To display all the surveys!\n");
			printf(" 3: To display a survey!\n");
			printf(" 4: To update a survey!\n");
			printf(" 5: To delete a survey!\n");
			printf(" 6: Generate statistics!\n");
			printf(" 7: To generate report files!\n");


			//printf(" 5: Disaplay Length of list\n");

			printf(" -1: To terminate\n");

			scanf("%d", &choice);

			if (choice == 1) {

				//check PPS
				addSurvey(&top);
				insertionSort(&top);//upon adding element to node list is sorted
									//sort done here to ensure a sort is done upon every addition 

			}
			else if (choice == 2) {
				printElements(top);
			}
			else if (choice == 3) {
				printf("Would you like to search by name or PPS Number\n");
				printf("1. Search by PPS \n2. Search by name\n");
				scanf("%d", &searchChoice);
				if (searchChoice == 1) {
					printf("\nEnter PPSNo to search");
					scanf("%d", &searchNum);
					searchList(top, searchNum, searchFName, searchLName);

				}
				else if (searchChoice == 2) {
					printf("\nEnter Name to search");
					scanf("%s %s", searchFName, searchLName);
					searchList(top, searchNum, searchFName, searchLName);

				}

			}
			else if (choice == 4) {
				printf("Would you like to search by name or PPS Number\n");
				printf("1. Search by PPS \n2. Search by name\n");
				scanf("%d", &searchChoice);
				if (searchChoice == 1) {
					printf("\nEnter PPSNo to search");
					scanf("%d", &searchNum);
					updateSurvey(top, searchNum, searchFName, searchLName);

				}
				else if (searchChoice == 2) {
					printf("\nEnter Name to search");
					scanf("%s %s", searchFName, searchLName);
					updateSurvey(top, searchNum, searchFName, searchLName);

				}

			}
			else if (choice == 5) {
				printf("\nEnter PPSNo to delete");
				scanf("%d", &searchNum);
				deletedSpecifiedSurvey(&top, searchNum);
			}
			else if (choice == 6) {
				printf("\nGenerating Statistics....\n\n");
				compileSurvey();
			}
			else if (choice == 7) {
				printf("\nGenerating Statistics first in the event you haven't done this prior\n\n");
				compileSurvey();
				printReports(top);
			}





			//printElements(top);



			//check node and display rather than ++ a var
			//searchList();


		} while (choice != -1);


	}

}
//login() is a function which grants users access to the systems on the premise they have the login details
//3 logins are approved by default found in the approvedLogins.txt file. A user is prompted for the Username
//and then the password. During password input, the users input is obfuscated on screen showing only an * symbol
//keeping with tradition password systems. another option would be to show nothing being entered like linux however I feel * is better to show user 
int login() {

	char userPassword[7] = "", usrname[7], inputChar;
	int i;
	openFile();

	int numofLogins = 3;


	if (fptr != NULL)
	{
		printf("\nChecking for approved logins\n");
		while (!feof(fptr))
		{
			for (i = 0; i < 3; i++) {
				fscanf(fptr, "%s", approvedUsrnames[i]);
				fscanf(fptr, "%s", approvedPassword[i]);
			}

		}
		closeFile();
	}


	printf("Enter User name: ");
	scanf("%s", usrname);
	printf("Enter the password <any 6 characters>: ");

	for (i = 0; i<6; i++)
	{

		inputChar = _getch();
		userPassword[i] = inputChar;
		inputChar = '*';
		printf("%c", inputChar);
	}//obfuscate the input to the user
	for (i = 0; i < 3; i++) {

		if (strcmp(approvedUsrnames[i], usrname) == 0) {//if user name equals one of the approved compare password
			if (strcmp(userPassword, approvedPassword[i]) == 0) {//if user password equals one of the approved grant access
				printf("\n\n\nLogin Successful!\n");
				return 1;
				break;
			}//end nested if

		}//end if
		
	}//end for
	printf("Unsuccessful login, quitting application\n");
	getch();
	return 0;
}
void openFile()
{
	fptr = fopen("approvedLogins.txt", "r");
	if (fptr == NULL)
	{
		printf("Error opening file ! Creating a file \n");
		fptr = fopen("approvedLogins.txt", "w");
		closeFile();
		openFile();
	}
	else {
		printf("Login file read successfully ! \n");
	}
}
void openReportFile()
{
	fptr = fopen("statisticsReport.txt", "w");
	if (fptr == NULL)
	{
		printf("Error opening file ! Creating a file \n");

	}
	else {
		printf("Report file opened successfully printing reports! ! \n");
	}
}
void openSurveyFile() {
	fptr = fopen("surveys.txt", "a+");
	if (fptr == NULL)
	{
		printf("Error opening file ! Creating a file \n");

	}
	else {
		printf("Survey file opened successfully!\n");
	}

}
void closeFile()
{
	fclose(fptr);
}
//addSurvey() is the function which takes in the parameters for the survey (name, address etc.) and puts it into the linked list
//a new node is created which is filled with info. Some vaidation is done such as PPS number uniqueness and the node is added to the list
//in this function the node is added to the very front of the list. Note: the sorting does not happen here without sorting it would be a list of the most recently enetered items
//a second function will carry out the sort
void addSurvey(struct survey** head_ptr)
{

	int inputPPS;
	struct survey *temp;
	struct survey *newNode;


	temp = head_ptr;

	newNode = (struct survey*)malloc(sizeof(struct survey));
	printf("\nPlease enter your PPS number (Number must be unique)\n");
	scanf("%d", &inputPPS);
	while (temp != NULL)
	{

		if (inputPPS == temp->surveyDetails.ppsNo)
		{
			printf("\nPPS you have entered is not unique. \n\n");
			free(newNode); free(temp);
			return;
		}
		temp = temp->next;




	}
	newNode->surveyDetails.ppsNo = inputPPS;
	//if (checkUnique(&head_ptr, inputPPS) == 1) {

	newNode->surveyDetails.ppsNo = inputPPS;
	printf("\nPlease enter your first name:");
	scanf("%s", newNode->surveyDetails.fName);
	printf("\nPlease enter your last name:");
	scanf("%s", newNode->surveyDetails.lName);
	printf("\nEnter email address: ");
	scanf("%s", newNode->surveyDetails.email);

	printf("\nEnter current address: (Do not enter spaces with your address. Use _ or - or no divider at all)");

	scanf("%s", newNode->surveyDetails.address);//takes in the next 99 characters until a newline is found

	printf("\nPlease enter your age:");
	scanf("%d", &newNode->surveyDetails.age);
	generateSurvey(newNode->surveyDetails.age, 5);

	printf("\nPlease enter your yearly salary (as whole number):");
	scanf("%d", &newNode->surveyDetails.income);
	generateSurvey(newNode->surveyDetails.income, 4);

	printf("\nHow many cigarrettes do you smoke a day? :");
	scanf("%d", &newNode->surveyDetails.ciggiesSmoked);
	generateSurvey(newNode->surveyDetails.ciggiesSmoked, 1);

	printf("\nHow many units of alcohol do you drink in a day? :");
	scanf("%d", &newNode->surveyDetails.unitsTaken);
	generateSurvey(newNode->surveyDetails.unitsTaken, 2);

	printf("\nHow many time do you exercise every week? :");
	scanf("%d", &newNode->surveyDetails.timesExercised);
	generateSurvey(newNode->surveyDetails.timesExercised, 3);



	while (temp != NULL)
	{

		temp = temp->next;

	}

	newNode->next = *head_ptr;

	*head_ptr = newNode;
	totalSurveys++;

}
//the insertionSort() function is one that is called after the addSurvey function. It was build as a separate function as 
//it gives users the ability to have a sorting option in the menu and as it is separate however called from the same if statement in main
//it will sort the list after every new survey is added
void insertionSort(struct survey **head_ref)
{
	// Initialize sorted linked list
	struct survey *sorted = NULL;

	// Traverse the given linked list and insert every
	// node to sorted
	struct survey *current = *head_ref;
	while (current != NULL)
	{

		struct survey *next = NULL;
		// Store next for next iteration
		if (current->next != NULL) {
			next = current->next;
		}


		// insert current in sorted linked list
		sortedInsert(&sorted, current);

		// Update current
		current = next;
	}

	// Update head_ref to point to sorted linked list
	*head_ref = sorted;
}
//sortedInsert() takes the current list and the newNode that has been created within it
//the survey is compared using the PPS number whereby if the number is bigger than the current pps number thats being pointed at
//then move to the next survey and compare these once the survey has found its correct position the list is mended ensuring no links lost
void sortedInsert(struct survey** head_ref, struct survey* new_node)
{
	struct survey* current;
	/* Special case for the head end */
	if (*head_ref == NULL || (*head_ref)->surveyDetails.ppsNo >= new_node->surveyDetails.ppsNo)
	{
		new_node->next = *head_ref;
		*head_ref = new_node;
	}
	else
	{
		/* Locate the node before the point of insertion */
		current = *head_ref;
		while (current->next != NULL &&
			current->next->surveyDetails.ppsNo < new_node->surveyDetails.ppsNo)
		{
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
}
void printElements(struct survey* head_ptr)
{

	struct survey *temp;

	temp = head_ptr;

	while (temp != NULL)
	{


		printf("\n\n\nSurvey Details...");
		printf("\n\nPPS number: %d", temp->surveyDetails.ppsNo);
		printf("\nFirst name: %s", temp->surveyDetails.fName);
		printf("\n\nLast name: %s", temp->surveyDetails.lName);
		printf("\nAddress: %s", temp->surveyDetails.address);
		printf("\nEmail: %s", temp->surveyDetails.email);
		printf("\nAge: %d", temp->surveyDetails.age);
		printf("\n Salary: %d", temp->surveyDetails.income);
		printf("\nCigarettes a day: %d", temp->surveyDetails.ciggiesSmoked);
		printf("\n Units of alcohol a day: %d", temp->surveyDetails.unitsTaken);
		printf("\n Times Exercised: %d", temp->surveyDetails.timesExercised);

		temp = temp->next;
	}
}
//search list allows a survey to be searched either by the name of the survey or its pps num
//in the main a distinction is made by the user however the name and pps variables are carried into the method together
//within the method if the first name is found the last name is also compared if it is found the survey will print to screen
void searchList(struct survey *head_ptr, int searchPPS, char fName[20], char lName[20])
{
	int nodeNum = 0;
	int choice = 0;
	struct survey *temp;
	temp = head_ptr;

	while (temp != NULL)
	{
		//run if statement to check for name
		//if searchName is == to temp name then show details
		//separate if for pps. Sentinal values to be used so that these can be in the same method
		if (searchFName == temp->surveyDetails.fName)//compares the pps given by user with the survey
		{
			if (searchLName == temp->surveyDetails.lName)//compares the pps given by user with the survey
			{
				//if found display
				printf("\n Survey found with PPS:%d! \nPrinting details....\n\n", searchPPS);
				printf("\n\n\nSurvey Details...");
				printf("\n\nPPS number: %d", temp->surveyDetails.ppsNo);
				printf("\nFirst name: %s", temp->surveyDetails.fName);
				printf("\n\nLast name: %s", temp->surveyDetails.lName);
				printf("\nAddress: %s", temp->surveyDetails.address);
				printf("\nAge: %d", temp->surveyDetails.age);
				printf("\n Salary: %d", temp->surveyDetails.income);
				printf("\nCigarettes a day: %d", temp->surveyDetails.ciggiesSmoked);
				printf("\n Units of alcohol a day: %d", temp->surveyDetails.unitsTaken);
				printf("\n Times Exercised: %d", temp->surveyDetails.timesExercised);

				return;//stops iterations
			}
		}
		if (searchPPS == temp->surveyDetails.ppsNo)//compares the pps given by user with the survey
		{
			//if found display
			printf("\n Survey found with PPS:%d! \n Printing details....\n\n", searchPPS);
			printf("\n\n\nSurvey Details...");
			printf("\n\nPPS number: %d", temp->surveyDetails.ppsNo);
			printf("\nFirst name: %s", temp->surveyDetails.fName);
			printf("\n\nLast name: %s", temp->surveyDetails.lName);
			printf("\nAddress: %s", temp->surveyDetails.address);
			printf("\nAge: %d", temp->surveyDetails.age);
			printf("\n Salary: %d", temp->surveyDetails.income);
			printf("\nCigarettes a day: %d", temp->surveyDetails.ciggiesSmoked);
			printf("\n Units of alcohol a day: %d", temp->surveyDetails.unitsTaken);
			printf("\n Times Exercised: %d", temp->surveyDetails.timesExercised);

			return;//stops iterations
		}
		temp = temp->next;
	}//shown if no surveys found
	printf("\n No surveys matching have been found ");
}
//updateSurvey() allows a survey to be searched either by the name of the survey or its pps num
//in the main a distinction is made by the user however the name and pps variables are carried into the method together
//within the method if the first name is found the last name is also compared if it is also found
//the user may then change details for the survey. The PPS number is unchangeable however could be.
void updateSurvey(struct survey *head_ptr, int searchPPS, char fName[20], char lName[20])
{
	int nodeNum = 0;
	struct survey *temp;
	temp = head_ptr;

	while (temp != NULL)
	{
		if (searchFName == temp->surveyDetails.fName)//compares the pps given by user with the survey
		{
			if (searchLName == temp->surveyDetails.lName)//compares the pps given by user with the survey
			{
				printf("\n Survey found with PPS:%d! \n Printing details....\n\n", searchPPS);

				printf("\nPlease enter your first name:");
				scanf("%s", temp->surveyDetails.fName);
				printf("\nPlease enter your last name:");
				scanf("%s", temp->surveyDetails.lName);
				printf("\nEnter email address: ");
				scanf("%s", temp->surveyDetails.email);
				printf("\nEnter current address: ");
				scanf(" %s", temp->surveyDetails.address);//takes in the next 99 characters until a newline is found
				printf("\nPlease enter your age:");
				scanf("%d", &temp->surveyDetails.age);
				printf("\nPlease enter your yearly salary (as whole number):");
				scanf("%d", &temp->surveyDetails.income);
				printf("\nHow many cigarrettes do you smoke a day? :");
				scanf("%d", &temp->surveyDetails.ciggiesSmoked);
				printf("\nHow many units of alcohol do you drink in a day? :");
				scanf("%d", &temp->surveyDetails.unitsTaken);
				printf("\nHow many time do you exercise every week? :");
				scanf("%d", &temp->surveyDetails.timesExercised);

				return;
			}
		}
		if (searchPPS == temp->surveyDetails.ppsNo)
		{
			printf("\nSurvey found with PPS:%d! \nPrinting details....\n\n", searchPPS);

			printf("\nPlease enter your first name:");
			scanf("%s", temp->surveyDetails.fName);
			printf("\nPlease enter your last name:");
			scanf("%s", temp->surveyDetails.lName);
			printf("\nEnter email address: ");
			scanf("%s", temp->surveyDetails.email);
			printf("\nEnter current address: ");
			scanf(" %s", temp->surveyDetails.address);//takes in the next 99 characters until a newline is found
			printf("\nPlease enter your age:");
			scanf("%d", &temp->surveyDetails.age);
			printf("\nPlease enter your yearly salary (as whole number):");
			scanf("%d", &temp->surveyDetails.income);
			printf("\nHow many cigarrettes do you smoke a day? :");
			scanf("%d", &temp->surveyDetails.ciggiesSmoked);
			printf("\nHow many units of alcohol do you drink in a day? :");
			scanf("%d", &temp->surveyDetails.unitsTaken);
			printf("\nHow many time do you exercise every week? :");
			scanf("%d", &temp->surveyDetails.timesExercised);

			return;
		}
		nodeNum++;
		temp = temp->next;
	}
	printf("\n No surveys matching have been found ");
}
//it then iterates through the linked list looking for a node (survey) with the same pps 
//if one is found the survey is deleted however its link is kept so that the other surveys in the list are not lost
//there is a special case for the head which preserves all the surveys after it
void deletedSpecifiedSurvey(struct survey **head_ptr, int searchPPS) {
	struct survey *temp;
	struct survey *newNode;
	struct survey *old_temp;

	if (*head_ptr == NULL)

	{
		printf("Nothing to delete\n"); //if list is empty prompt user 
	}
	temp = (struct survey*)malloc(sizeof(struct survey));
	temp = *head_ptr;
	old_temp = (struct survey*)malloc(sizeof(struct survey));
	
	if (searchPPS == temp->surveyDetails.ppsNo && (*head_ptr)->surveyDetails.ppsNo == searchPPS)
	{
		temp = *head_ptr; //temp pointer is equal to the current head
		*head_ptr = temp->next;
		free(temp);
		totalSurveys--;
		printf("\nSurvey with PPS No: %d has been deleted\n\n\n", searchPPS);
		return;
	}
	while (temp != NULL) 
	{
		if (searchPPS == temp->surveyDetails.ppsNo) {
			old_temp->next = temp->next; //the old pointer is linked to the next node to not lose any nodes thereafter

											//Shows value of element being deleted
			free(temp); //de allocates the pointer 
			totalSurveys--;
			printf("\nSurvey with PPS No: %d has been deleted\n\n\n", searchPPS);
			return;
		}
		old_temp = temp; //make the old pointer point to temp pointer
		temp = temp->next; //temp pointer is equal to next value from temp
	}//end while 

}
//generateSurvey is a function which handles all of the statistical data for the report
//each of the statistics have a int type associated i.e smokers=1 drinkers=2
//using this a swtich statment is put in place where each case is one of the stat topics
//the input is then compared to see which bracket the survey falls under
//using numbers for each means it is one method which can be called in multiple places
void generateSurvey(int input, int surveyType) {
	switch (surveyType) {
	case 1: //smokers survey
		if (input == 0) {
			break;
		}
		if (input >= 1) {
			smokerBrackets[0]++;//indicates a person smokes and evaluates by how many below
		}//two separate if here rather than else if as below due to us wanting to know if there and smoke && how many
		if (input < 20) {
			smokerBrackets[1]++;
		}
		else if (input < 40) {
			smokerBrackets[2]++;
		}
		else if (input > 40) {
			smokerBrackets[3]++;
		}
		break;//end case
	case 2://drinker survey
		if (input == 0) {
			drinkerBrackets[0]++;//indicates a person does not drink
		}

		else if (input < 2) {
			drinkerBrackets[1]++;
		}
		else if (input < 4) {
			drinkerBrackets[2]++;
		}
		else if (input > 4) {
			drinkerBrackets[3]++;
		}//we are working with if elses here as we only want to know whos sober,
		 //and how many does the other have
		break;
	case 3://case for amount of exercise
		if (input == 0) {
			exerciseBrackets[0]++;//indicates a person does not exercise
		}
		else if (input < 2) {
			exerciseBrackets[1]++;
		}
		else if (input < 4) {
			exerciseBrackets[2]++;
		}
		else if (input > 4) {
			exerciseBrackets[3]++;
		}

		break;
	case 4://case for different income brackets
		if (input == 0) {
			incomeBrackets[0]++;
		}
		if (input < 20000) {
			incomeBrackets[1]++;
		}
		else if (input < 40000) {
			incomeBrackets[2]++;
		}
		else if (input < 60000) {
			incomeBrackets[3]++;
		}
		else if (input < 80000) {
			incomeBrackets[4]++;
		}
		else if (input < 100000) {
			incomeBrackets[5]++;
		}
		else if (input > 100000) {
			incomeBrackets[6]++;
		}
		break;
	case 5://case for different age brackets
		if (input > 18 && input < 20) {
			ageBrackets[0]++;
		}
		else if (input > 20 && input < 30) {
			ageBrackets[1]++;
		}
		else if (input > 30 && input < 50) {
			ageBrackets[2]++;
		}
		else if (input < 65 && input > 50) {
			ageBrackets[3]++;
		}
		else if (input > 65) {
			ageBrackets[4]++;
		}
		break;
	}//end swtich

}
//compileSurvey takes no parameters as it makes use of global variables which have been filled using the generateSurvey function
//compileSurvey can be run after 1 survey has been taken in either from file or user. While normally 1+ files will be imported,
//a condition is set in the event there is none there are many lines to this method due to it calcuating an actual percentage for all 
//24 brackets that were required in the specifications
void compileSurvey() {
	int i;
	if (totalSurveys == 0) {
		printf("Sorry but you must have a least one survey to generate statistics. We cannot analyse what is not there.");
	}
	else {
		//calc percentages
		for (i = 0; i < 4; i++) {//this loop fills the first 3 statistic groups with float numbers to give decimal
			smokerStats[i] = (smokerBrackets[i] * 100.0f) / totalSurveys;
			drinkerStats[i] = (drinkerBrackets[i] * 100.0f) / totalSurveys;
			exerciseStats[i] = (exerciseBrackets[i] * 100.0f) / totalSurveys;

		}
		for (i = 0; i < 7; i++) {//seperate for loop needed due to their being more brackets
			incomeStats[i] = (incomeBrackets[i] * 100.0f) / totalSurveys;
		}
		for (i = 0; i < 5; i++) {//seperate for loop needed due to their being more brackets
			ageStats[i] = (ageBrackets[i] * 100.0f) / totalSurveys;
		}
		for (i = 0; i < 4; i++) {
			printf("Percentage of people who smoke %s from survey size of %d: %.2f \n", smokerPhrases[i], totalSurveys, smokerStats[i]);
		}
		for (i = 0; i < 4; i++) {
			printf("Percentage of people who drink %s from survey size of %d: %.2f \n", drinkerPhrases[i], totalSurveys, drinkerStats[i]);
		}
		for (i = 0; i < 4; i++) {
			printf("Percentage of people who %s in a week from survey size of %d: %.2f \n", exercisePhrases[i], totalSurveys, exerciseStats[i]);
		}
		for (i = 0; i < 7; i++) {
			printf("Percentage of people who earn %s from survey size of %d: %.2f \n", incomePhrases[i], totalSurveys, incomeStats[i]);

		}
		for (i = 0; i < 5; i++) {
			printf("Percentage of people in the %s age bracket from survey size of %d: %.2f \n", agePhrases[i], totalSurveys, ageStats[i]);

		}
	}//end else
}//end compile survey
//print reports opens a file survey.txt and until reaching the end of the list inserts the survey details using a loop
//once this is done the file is closed
void printReports(struct survey* head_ptr) {
	int i;
	openSurveyFile();

	struct survey *temp;

	temp = head_ptr;

	while (temp != NULL)
	{
		fprintf(fptr, "%d %s %s %s %s %d %d %d %d %d", temp->surveyDetails.ppsNo, temp->surveyDetails.fName, temp->surveyDetails.lName,
			temp->surveyDetails.address, temp->surveyDetails.email, temp->surveyDetails.age, temp->surveyDetails.income, temp->surveyDetails.ciggiesSmoked,
			temp->surveyDetails.unitsTaken, temp->surveyDetails.timesExercised);

		temp = temp->next;
	}
	closeFile();


	openReportFile();
	//reused code from the compile survey function to print the statistics to a file in the same format as the users see
	for (i = 0; i < 4; i++) {
		fprintf(fptr, "Percentage of people who smoke %s from survey size of %d: %.2f \n", smokerPhrases[i], totalSurveys, smokerStats[i]);
	}
	for (i = 0; i < 4; i++) {
		fprintf(fptr, "Percentage of people who drink %s from survey size of %d: %.2f \n", drinkerPhrases[i], totalSurveys, drinkerStats[i]);
	}
	for (i = 0; i < 4; i++) {
		fprintf(fptr, "Percentage of people who %s in a week from survey size of %d: %.2f \n", exercisePhrases[i], totalSurveys, exerciseStats[i]);
	}
	for (i = 0; i < 7; i++) {
		fprintf(fptr, "Percentage of people who earn %s from survey size of %d: %.2f \n", incomePhrases[i], totalSurveys, incomeStats[i]);

	}
	for (i = 0; i < 5; i++) {
		fprintf(fptr, "Percentage of people in the %s age bracket from survey size of %d: %.2f \n", agePhrases[i], totalSurveys, ageStats[i]);

	}
	closeFile();
}
//in this method the file surveys.txt is opened and if any surveys are in there it will read in all the details of the survey
//store it into a node with is inserted, once this happens the normal program begins where the user can add more surveys
void importReports(struct survey** head_ptr) {
	struct survey *temp;
	struct survey *newNode;

	openSurveyFile();

	temp = head_ptr;

	newNode = (struct survey*)malloc(sizeof(struct survey));
	while (!feof(fptr))
	{

		fscanf(fptr, "%d %s %s %s %s %d %d %d %d %d", &newNode->surveyDetails.ppsNo, newNode->surveyDetails.fName, newNode->surveyDetails.lName,
			newNode->surveyDetails.address, newNode->surveyDetails.email, &newNode->surveyDetails.age, &newNode->surveyDetails.income, &newNode->surveyDetails.ciggiesSmoked,
			&newNode->surveyDetails.unitsTaken, &newNode->surveyDetails.timesExercised);



	} /* end while */

	while (temp != NULL)
	{

		temp = temp->next;

	}


	newNode->next = *head_ptr;

	*head_ptr = newNode;
	totalSurveys++;

	closeFile();
}

