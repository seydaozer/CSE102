#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define SIZE 20

struct person{
	char name[SIZE];
	char surname[SIZE];
	char musical_Work[SIZE];
	int age;
	struct person *next;
} *top;

void addNode(char name[], char surname[], char creation[], int age);
void deleteNode();
void Sort_Alphabetically();
void Sort_Increasingly();
void print_stack();
int menu();
void swap(struct person *p1, struct person *p2);

int main(){
	int age, choice;
	char name[SIZE], surname[SIZE], creation[SIZE];
	
	do{
		choice = menu();
		switch(choice){
			case 1:
				printf("Name: ");
				scanf(" %[^\n]s", name);
				printf("Surname: ");
				scanf(" %[^\n]s", surname);
				printf("Creation: ");
				scanf(" %[^\n]s", creation);
				printf("Age: ");
				scanf("%d", &age);
				addNode(name, surname, creation, age);	
				//print_stack();
				break;

			case 2:
				deleteNode();
				//print_stack();
				break;

			case 3:
				Sort_Alphabetically();
				print_stack();
				break;

			case 4:
				Sort_Increasingly();
				print_stack();
				break;
			case 5:
				printf("You are exiting!\n");
				break;

			default: printf("This is invalid choice!\n");

		} // end of switch
	} while(choice != 5); // end of do-while

	return 0;
}

void addNode(char name[], char surname[], char creation[], int age){ // part1
	
	struct person *newp;

	newp = (struct person *)malloc(sizeof(struct person));
	strcpy(newp->name, name);
	strcpy(newp->surname, surname);
	strcpy(newp->musical_Work, creation);
	newp->age = age;

	newp->next = top;
	top = newp;

}

void deleteNode(){ // part2
	
	struct person *freep;

	if(top == NULL)	printf("Stack is empty! You can not delete.\n");

	else{ 
		freep = top;
		top = freep->next;
		free(freep);
	}

}

void Sort_Alphabetically(){ // part3
	struct person *temp, *last;
	int counter = 1;

	temp = top;
	last = NULL;

	while(temp->next != NULL){ 
		temp = temp->next;
		counter++; //number of elements
	}

	while(counter > 0){
		temp = top; // temp should always point to the top(head).
		while(temp->next != last){
			if( strcmp(temp->name, temp->next->name) > 0 )
				swap(temp, temp->next);
			temp = temp->next;
		}
		last = temp; // the maximum value goes to end each time. so I assign this variable to the last element.
		counter--;
	}
	
}

void Sort_Increasingly(){ // part4
	struct person *temp, *last;
	int counter = 1;

	temp = top;
	last = NULL;

	while(temp->next != NULL){
		temp = temp->next;
		counter++; //number of elements
	}
	

	while(counter > 0){
		temp = top; // temp should always point to the top(head).
		while(temp->next != last){
			if(temp->age > temp->next->age)
				swap(temp, temp->next);
			temp = temp->next;
		}
		last = temp; // the maximum value goes to end each time. so I assign this variable to the last element.
		counter--;
	}
	
}

void swap(struct person *p1, struct person *p2){
	char temp_name[SIZE], temp_surname[SIZE], temp_creation[SIZE];;
	int temp_age;

	// temp = p1		
	strcpy(temp_name, p1->name);
	strcpy(temp_surname, p1->surname);
	strcpy(temp_creation, p1->musical_Work);
	temp_age = p1->age;
	// p1 = p2
	strcpy(p1->name, p2->name);
	strcpy(p1->surname, p2->surname);
	strcpy(p1->musical_Work, p2->musical_Work);
	p1->age = p2->age;
	// p2 = temp
	strcpy(p2->name, temp_name);
	strcpy(p2->surname, temp_surname);
	strcpy(p2->musical_Work, temp_creation);
	p2->age = temp_age;

}

void print_stack(){

	struct person *temp;
	int i= 1;

	if(top == NULL)	return;

	else{
		temp = top;
		while(temp != NULL){
			printf("%d) %s\n", i, temp->name);
			printf("%s\n", temp->surname);
			printf("%s\n", temp->musical_Work);
			printf("%d\n\n", temp->age);
			temp = temp->next;
			i++;
		}
	}

}

int menu(){
	int choice;
	printf("************* MENU *************\n");
	printf("1- Add a Person to the Stack\n");
	printf("2- Pop a Person from the Stack\n");
	printf("3- Sort Alphabetical Order\n");
	printf("4- Sort Age in Increasing Order\n");
	printf("5- Exit menu\n");
	printf("********************************\n");
	printf("Select your Choise: ");
	scanf("%d", &choice);

	return choice;
}
