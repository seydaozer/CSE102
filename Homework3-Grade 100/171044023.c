#include<stdio.h>
#include<string.h>

#define SIZE 50

void Menu();
void menu_calculater(); // for part1.
int addition( int, int ); // part1
int substraction( int, int ); // part1
int multiplication( int, int ); // part1
int division( int, int ); // part1
int power( int, int ); // part1
int modulo( int, int ); // part1
int doit( int function(int, int), int, int ); // part1
int convert_number( char [], double ); // part1 -> This function converts the value from char to integer.
void control_operation( char [], int, int, int *, int * ); // part1 -> This function determines which is the operation.
int take_grades( int [] ); // part2
int take_exam_grades( int [] ); // part2
double calculate_homework( int [] ); // part2
double calculate_lab( int [] ); // part2
double calculate_all( int , int [], int ); // part2
void draw_shape( int ); // part3

int main(){
	
	Menu();

	return 0;
}

void Menu(){
	int choice;
	char quit;
	char selection[SIZE], operation[2], num1[25], num2[25], *ptr; // part1
	int number1, number2, result, previous_result = 0, i, count; // part1
	double count_digit;// part1 -> I use this variable because the loop in the convert_number is turning a little extra. I have to use it for control.
	int homework_grades[10], lab_grades[10], exam_grades[2]; // part2
	double average_homework, average_lab, average_all; // part2
	int height; // part3

	
	while(quit != 'q'){
		printf("\n***** MENU *****\n");
		printf("1. Calculater\n");
		printf("2. Calculate Exam Grades\n");
		printf("3. Draw Shape\n");
		printf("4. Exit\n");
		
		printf("Choice: ");
		scanf("%d", &choice);
		printf("\n");

		if(choice>=1 && choice<=4){

			switch(choice){
				case 1:// Part1 - Calculater
					do{
						i = 0;
						count = 0;

						menu_calculater(); // This menu includes operation options.

						scanf("%s", operation); // I assign the entered character to this array(operation).
							

						fgets(selection, SIZE, stdin); // It takes the numbers from the keyboard. I assign the numbers to this array(selection).

						if(operation[0] != 'q'){ // I used if because the program enters control_operation and it runs printf in the else.

							while(selection[i] != '\0'){ // This loop goes to the end of the series and controls the number of spaces.
								if(selection[i] == ' ')	count++;
								i++;
							}
							
							if(count == 2){ // There are 2 numbers.
								ptr = strtok(selection, " ");
								strcpy(num1, ptr); // I copy ptr to num1.
								count_digit = 0.1; // I used it because it is found one more than result in the loop in convert_number.
								number1 = convert_number(num1, count_digit); // to convert the contents of the array
						
								ptr = strtok(NULL, " ");
								strcpy(num2, ptr); 
								count_digit = 0.01; // I used it because it is found two more than result in the loop int convert_number.
								number2 = convert_number(num2, count_digit);
							}
			
							else if(count == 1){ //There is one number.
								ptr = strtok(selection, " ");
								strcpy(num2, ptr); 
								count_digit = 0.01; // I used it because it is found two more than result in the loop.
								number2 = convert_number(num2, count_digit);
								number1 = previous_result; // When a number is entered, the number1 is equal to previous result.
							}
			
							else	printf("This is wrong choice. Please try again!\n");
	
							control_operation(operation, number1, number2, &result, &previous_result);
		
							printf("The result is: %d\n\n", result);

						}

					} while(operation[0] != 'q'); // If the entered character is q, the program stops. 
					
					break;

				case 2: // Part2 - Exam Grades
					printf("Please enter your homework grades:\n");
					take_grades(homework_grades);
			
					printf("Please enter your lab grades:\n");
					take_grades(lab_grades);
			
					printf("Please enter your exam grades(midterm and final):\n");
					take_exam_grades(exam_grades);	
				
					average_homework = calculate_homework(homework_grades);
					average_lab = calculate_lab(lab_grades);
			
					average_all = calculate_all(average_homework, exam_grades, average_lab);
					printf("The average (all notes) is: %.2lf \n", average_all);	
					break;
				
				case 3: // Part3 - Draw Shape
					printf("Please enter a number for height of the shape:\n");
					scanf("%d", &height);	
					draw_shape(height);
					break;
						
				case 4: //Exit
					quit = 'q';
					printf("You are exiting.\n");

					break;
			} // end of switch
		} // end of if
		
		else
			printf("This is an invalid choice. Try again!\n\n");
	} // end of while
} // end of Menu


void menu_calculater(){
	printf("CALCULATER MENU\n");
	printf("(+), Addition\n");
	printf("(-), Substraction\n");
	printf("(*), Multiplication\n");
	printf("(/), Division\n");
	printf("(**), Power\n");
	printf("(%%), Modulo\n");
	printf("(q), Exit\n\n");
}

int convert_number(char num[], double count){ //This function determines which is the operation.
	int array[25], number = 0, i = 0, j;

	while(num[i] != '\0'){ // This loop goes to the end of the series.
		if(num[i] == '0')	array[i] = 0;
		else if(num[i] == '1')	array[i] = 1;
		else if(num[i] == '2')	array[i] = 2;
		else if(num[i] == '3')	array[i] = 3;
		else if(num[i] == '4')	array[i] = 4;
		else if(num[i] == '5')	array[i] = 5;
		else if(num[i] == '6')	array[i] = 6;
		else if(num[i] == '7')	array[i] = 7;
		else if(num[i] == '8')	array[i] = 8;
		else if(num[i] == '9')	array[i] = 9;
		count *= 10.0; // I used it to find the value of the digits.
		i++;
	}
	
	for(j=0; j<i; j++){ // The actual value of the numbers is calculated.
		array[j] *= count;
		count /= 10.0;
	}

	for(j=0; j<i; j++){ // I find the real value of the number by collecting the numbers I find.
		number += array[j];
	}

	return number;
}

void control_operation(char operation[], int number1, int number2, int *result, int *previous_result){
	int i = 0, j = 0, count = 0;

	while(operation[i] != '\0'){ // This loop is used to find out how many digits the number is.
		count++; // Number of digits.
		i++;
	}

	if(count == 2){
		if(operation[0] == '*' && operation[1] == '*'){ // This operation is power.
			*previous_result = doit(power, number1 ,number2);
			*result = *previous_result;
		}
		else
			printf("Your choice is wrong. Please try again!\n");
	}

	else if(count == 1){
		if(operation[0] == '+'){ // This operation is addition.
			*previous_result = doit(addition, number1, number2);
			*result = *previous_result;
		}
		else if(operation[0] == '-'){ // This operation is substraction.
			*previous_result = doit(substraction, number1 ,number2);
			*result = *previous_result;
		}
		else if(operation[0] == '*'){ // This operation is multiplication.
			*previous_result = doit(multiplication, number1 ,number2);
			*result = *previous_result;
		}
		else if(operation[0] == '/'){ // This operation is division.
			*previous_result = doit(division, number1 ,number2);
			*result = *previous_result;
		}
		else if(operation[0] == '%'){ // This operation is modulo.
			*previous_result = doit(modulo, number1 ,number2);
			*result = *previous_result;
		}
		else
			printf("Your choice is wrong. Please try again!\n");
	}
}

int addition(int number1, int number2){
	return number1 + number2;
}

int substraction(int number1, int number2){
	return number1 - number2;
}

int multiplication(int number1, int number2){
	return number1 * number2;	
}

int division(int number1, int number2){
	int result;

	if(number2 != 0)
		result = number1 / number2;
	else{
		printf("The division error. Please you do not enter zero.\n");
		result = 0;	
	}

	return result;
}

int power(int number1, int number2){
	int i, temp;

	if(number2 == 0)
		number1 = 1;
		
	else{
		temp = number1;
		for(i=1; i<number2; i++)
			number1 *= temp;
	}
	
	return number1;
}

int modulo(int number1, int number2){
	int result, count;

	count = number1 / number2;
	result = number1 - (count * number2);	

	return result;
}

int doit(int function(int number1, int number2), int first_number, int second_number){
	int result;

	if(function == addition)
		result = addition(first_number, second_number);
	else if(function == substraction)
		result = substraction(first_number, second_number);
	else if(function == multiplication)
		result = multiplication(first_number, second_number);
	else if(function == division)
		result = division(first_number, second_number);
	else if(function == power)
		result = power(first_number, second_number);
	else if(function == modulo)
		result = modulo(first_number, second_number);
	
	return result;
}

int take_grades( int grades[] ){
	int i, grade;

	for(i=0; i<10; i++){
		scanf("%d", &grade);
		grades[i] = grade;
	}

	return 0;
}

int take_exam_grades( int exam_grades[] ){
	int i, grade;

	for(i=0; i<2; i++){
		scanf("%d", &grade);
		exam_grades[i] = grade;
	}

	return 0;
}

double calculate_homework( int homework_grades[] ){
	int i;
	double sum = 0.0;

	for(i=0; i<10; i++){
		sum += homework_grades[i];
	}

	return (sum / 10.0);
}


double calculate_lab( int lab_grades[] ){
	int i;
	double sum = 0.0;

	for(i=0; i<10; i++){
		sum += lab_grades[i];
	}

	return (sum / 10.0);
}

double calculate_all(int average_homework, int exam_grades[], int average_lab){
	double all;
	all = (average_homework * 10 / 100) + (average_lab * 20 / 100) + (exam_grades[0] * 30 / 100) + (exam_grades[1] * 40 / 100);
	return all;
}

void draw_shape(int height){
	int i, j, k, star_number = 2;
	
	for(k=0; k<height; k++){
		if(k == 0){
			for(i=1; i<height; i++)	printf(" ");
			printf("/\\\n");
		}
		else{
			for(i=1; i<height-k; i++)	printf(" ");
			printf("/");
			for(i=0; i<star_number; i++)	printf("*");
			printf("\\\n");
			star_number += 2;
		}
	}

	star_number -= 2;

	for(k=0; k<height; k++){
		if(k == height-1){
			for(i=1; i<height; i++)	printf(" ");
			printf("\\/\n");
		}
		else{
			for(i=0; i<k; i++)	printf(" ");
			printf("\\");
			for(i=0; i<star_number; i++)	printf("*");
			printf("/\n");
			star_number -= 2;
		}
	}
}
