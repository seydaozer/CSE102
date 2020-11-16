#include<stdio.h>

#define SIZE_S 50 // size of string -> part5
#define SIZE_N 50 // size of number -> part1
#define SIZE_A 50 // size of integer array -> part2

void menu();
int common_divisor(int number, int *multipliers); // part1
int prime_divisor(int *number1, int size1, int *number2, int size2, int i1, int i2); // part1
void merge(int *array, int first, int last);// part2
void sort(int *array, int first, int mid, int last); // This function sorts the elements of the array according to the entered parameters. I used it for the part2.
void function(int number); // part3
int control_equality(int number, int digit_number); // part4
int find_lenght(char *str, int counter); // This function finds the lenght of the string. I used it for the part5.
void find_capital_letter(char *str, int location, int lenght); //part5
int my_pow(int number, int exponent); // My power function. I used it for the part4.

int main(){
	
	menu();

	return 0;
}

void menu(){

	int choice, // I used it for menu
	number1, number2, result, size1, size2, first_number[SIZE_N], second_number[SIZE_N], // I used these variables for part1
	size, first, last, i, array[SIZE_A], // I used these variables for part2
	number, // I used it for part3 and part4
	temp, digit_number, control, // I used these variables for part4
	lenght; // I used it for part5
	char string[SIZE_S]; // I used it for part5
	
	do{
		printf("\n(1), Find common multipliers for entered numbers.\n");
		printf("(2), Sort entered numbers(increasing order).\n");
		printf("(3), The function for mod 2. (If n=0(mod2) -> n/2, if n=1(mod2) -> 3*n+1).\n");
		printf("(4), Control equality. \n");
		printf("(5), Find first capital letter.\n");
		printf("(0), Quit.\n");
		scanf("%d", &choice);

		switch(choice){

			case 1:
				printf("Please enter two numbers you want to find common multipliers.\n");
				scanf("%d %d", &number1, &number2);
	
				size1 = common_divisor(number1, first_number);
				size2 = common_divisor(number2, second_number);

				if(size1 <= size2)
					result = prime_divisor(first_number, size1, second_number, size2, 0, 0);
				else
					result = prime_divisor(second_number, size2, first_number, size1, 0, 0);

				if(result == 1)
					printf("The entered numbers do not have a common multipliers.\n");
			
				else	printf("The result is: %d\n", result);	
				
				break;

			case 2:
				printf("Please, enter the size of the array\n");
				scanf("%d", &size);

				printf("Please, enter the elements of array:");
				for(i=0; i<size; i++){
					scanf("%d", &array[i]);
				}

				first = 0, last = size-1; // indexes of first and last elements.
				merge(array, first, last);
	
				printf("Sorted array:");
				for(i=0; i<size; i++){
					printf("%d ", array[i]);
				}
				printf("\n");

				break;

			case 3:
				printf("Enter a number, please:\n");
				scanf("%d", &number);

				printf("%d ", number);
				function(number);
				printf("\n");
		
				break;

			case 4:
				digit_number = 1;
				printf("Please enter a number, if you want to check the equality.\n");
				scanf("%d", &number);
	
				temp = number; // I used this variable not to lose the value of the number.
				while(temp > 10){ // I used this while loop to find the number of digits of the number.
					temp = temp / 10;
					digit_number++;
				}

				control = control_equality(number, digit_number);

				if(control == number)
					printf("Equal\n");
				else
					printf("Not equal\n");
			
				break;

			case 5:
				printf("Please enter a string without space.\n");
				scanf("%s", string);
	
				lenght = find_lenght(string, 0);

				find_capital_letter(string, 0, lenght);

				break;

			default:
				if(choice != 0)
					printf("You entered invalid choice. Try again!\n");
		}

	} while(choice != 0);

}

int common_divisor(int number, int *multipliers){ // part1 -> This function finds the prime divisors of the entered number. It also returns the size of the array of these divisors.
	int i = 2, size = 0;

	while(number > 1){
		if(number % i == 0){
			number /= i;
			multipliers[size] = i;
			size++;
			i--;
		}
		i++;
	}

	return size;
}

int prime_divisor(int *number1, int size1, int *number2, int size2, int i1, int i2){ //part1
	int mult = 1, k, counter = 0;

	if(i1 == size1)
		mult = 1;

	else{
		for(k=i2; k<size2; k++){	
			if(number1[i1] == number2[k]){
				mult = number1[i1] * prime_divisor(number1, size1, number2, size2, i1+1, i2+1);
				counter++; // This variable controls the equation. If there is no equality, this program enter the next if.
			}
			if(counter == 0)
				mult = prime_divisor(number1, size1, number2, size2, i1+1, i2);
		}
	}

	return mult;
}

void merge(int *array, int first, int last){ // This function divides the array until it has one element.

	int mid; // I used this variable to divide the array in the middle.
	
	if( first < last ){ // the first index must be less than the last index. Because the number of elements of the array must be greater than zero.
		mid = ( first + last ) / 2; // middle of the array
		
		merge(array, first, mid); // array on the left

		merge(array, mid+1, last); // array on the right
	
		sort(array, first, mid, last); // We call this function to sort the arrays we divides.
	}

}

void sort(int *array, int first, int mid, int last){ // This function sorts the array according to the inputs.

	int left[SIZE_A], right[SIZE_A];
	int size1, size2, i, i1, i2;

	size1 = mid - first + 1; // size of the array on the left
	size2 = last - mid; // size of the array on the right

	for(i=0; i<size1; i++){ // fills the array on the left
		left[i] = array[first+i];
	}
	for(i=0; i<size2; i++){ // fills the array on the right
		right[i] = array[mid+1+i];
	}

	i = first, i1 = 0, i2 = 0;
	while(i1 < size1 && i2 < size2 && i < last){ // Returns to the end of the arrays.
		if(left[i1] < right[i2]){ // Compares left and right arrays. The elements of the array sent to the function are sorted.
			array[i] = left[i1];
			i1++;
		}
		else{
			array[i] = right[i2];
			i2++;
		}
		i++;
	}

	while(i1 < size1){ // if the upper loop has not reached the end of the left array, the remaining elements are placed respectively.
		array[i] = left[i1];
		i1++;
		i++;
	}
	while(i2 < size2){ // if the upper loop has not reached the end of the right array, the remaining elements are placed respectively.
		array[i] = right[i2];
		i2++;
		i++;
	}
	
}

void function(int number){ //part3

	if(number == 1)
		printf("%d", number);

	else if(number > 1){

		if(number % 2 == 0){
			number = number / 2;
		}

		else{
			number = 3 * number + 1;
		}

		if( number != 1)
			printf("%d ", number);

		function(number);
	}

}

int control_equality(int number, int digit_number){ //part4
	
	if(number % 10 == number){ // stop at the last step.
		number = my_pow(number, digit_number);
	}
	else{ // takes the power of the current step and divides the number by 10.
		number = my_pow(number % 10, digit_number) + control_equality(number / 10, digit_number);
	}

	return number;

}

int my_pow(int number, int exponent){ // power function
	int result;
	
	if(exponent == 0)	result = 1;
	
	else	result = number * my_pow(number, exponent-1);
	
	return result;
}

void find_capital_letter(char *str, int location, int lenght){ //part5
	
	if(str[location] >= 'A' && str[location] <= 'Z') // This function stops when there is a capital letter.
		printf("%c\n", str[location]);

	else if(location != lenght) // If the user does not enter a capital letter, I made this check so that there is no error.
		find_capital_letter(str, location+1, lenght);	

	else
		printf("You did not enter capital letters.\n");

}

int find_lenght(char *str, int counter){ //This function find the lenght of string entered.

	if(str[counter] == '\0')
		counter = 0;

	else
		counter = 1 + find_lenght(str, counter+1);

	return counter;
}
