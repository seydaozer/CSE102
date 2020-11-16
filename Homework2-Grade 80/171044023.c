#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define TRIAL 10

void menu();
int make_a_guess(int trial, int min, int max);
void show_scores(int score_human, int score_program);
void draw_hourglass(int height);
void draw_mountain_road(int lenght, int max_radius);
void control_distance(int distance); //This function calculates the distance between LN and GN and prints it.
void control_scores(int trial, int *score_human, int *score_program); // This fuction determines who takes the score.

int main(){
	srand(time(NULL));

	menu();

	return 0;
}

void menu(){
	int choice;
	int trial, min, max, LN, GN, distance, score_human = 0, score_program = 0; // I used these variables in part2.
	int height; // I used these variables in part3.
	int lenght, max_radius; // I used these variables in part4.
	char quit; 
	
	while(quit != 'q'){
		printf("\n***** MENU *****\n");
		printf("1. Play Lucky Number\n");
		printf("2. Draw Hourglass\n");
		printf("3. Draw Mountain Road\n");
		printf("4. Exit\n");
		
		printf("Choice: ");
		scanf("%d", &choice);

		if(choice>=1 && choice<=4){

			switch(choice){

				case 1: //Lucky number game
					min = 1; // min value for lucky number
					max = 1024; // max value for lucky number
					LN = rand() % 1023 + 1; // a random value
					GN = 0; // The guess number
					trial = 0; // The trial number

					printf("(with having T = %d & LN = %d)\n", TRIAL, LN); // The hint for users
					
					while(GN != LN && trial < TRIAL){ // This loop checks for equality and trial number.
						
						GN = make_a_guess(trial, min, max);

						if(GN >= min && GN <= max){ // The guess have to be between min and max.
						
							if(LN > GN){
								distance = LN - GN;
								min = GN;
							}

							else{
								distance = GN - LN; 
								max = GN;						
							}

							control_distance(distance);
						}
						trial++;					
					} 

					if(GN != LN)
						printf("Program wins!\n");

					control_scores(trial, &score_human, &score_program);
					show_scores(score_human, score_program);

					break;

				case 2: //Print a hourglass
					
					height = 0;

					while(height % 2 == 0){ // only odd numbers
						printf("Please enter a odd number for height of the hourglass.\n");
						scanf("%d", &height);
					}

					draw_hourglass(height);

					break;

				case 3: //Print a mountain road
					
					printf("Please enter a lenght and max radius for mountain road.\n");
					scanf("%d %d", &lenght, &max_radius);

					draw_mountain_road(lenght, max_radius);

					break;

				case 4: //Exit
					
					quit = 'q';
					printf("You are exiting.\n");

					break;
			}
		}

		else
			printf("This is an invalid choice. Try again!\n\n");
	}
}  

int make_a_guess(int trial, int min, int max){
	int guess;

	printf("(Trial: %d) ", trial+1);
	printf("Make a guess between %d and %d, inclusively:", min, max);
	scanf("%d", &guess);

	return guess;
}

void show_scores(int score_human, int score_program){
	
	printf("Your Score: %d  Program Score: %d \n", score_human, score_program);
}

void draw_hourglass(int height){
	int i, j; 

	for(i=0; i<(height/2)+1; i++){ // top of the hourglass
		for(j=0; j<i; j++)
			printf(" ");
		for(j=0; j<height-(2*i); j++)
			printf("*");
		printf("\n");	
	}

	for(i=1; i<(height/2)+1; i++){ // bottom of the hourglass
		for(j=0; j<(height/2)-i; j++)
			printf(" ");
		for(j=0; j<(2*i)+1; j++)
			printf("*");
		printf("\n");
	}
		
}

void draw_mountain_road(int lenght, int max_radius){
	int i, j, k, radius;
	int count = 0;

	for(k=1; k<=lenght; k++){

		radius = rand() % max_radius;

		if(k%2 == 1){ // for half circles on the left
			if(k == 1){ // for first half circle
				for(i=0; i<radius; i++){
					for(j=0; j<=max_radius+radius-i; j++)
						printf(" ");
					printf("/\n");
				}

				for(i=0; i<=max_radius; i++)
					printf(" ");
				printf("|\n");

				for(i=1; i<=radius; i++){
					for(j=0; j<=max_radius+i; j++){
						printf(" ");
					}
					printf("\\ \n");
					count = radius;
				}
			}

			else{ //other half circles
				for(i=0; i<radius; i++){
					for(j=0; j<=max_radius+count-i; j++)
						printf(" ");
					printf("/\n");
				}

				for(i=0; i<=max_radius+count-radius; i++)
					printf(" ");
				printf("|\n");

				for(i=1; i<=radius; i++){
					for(j=0; j<=max_radius+count-radius+i; j++)
						printf(" ");
					printf("\\ \n");
				}	
			}
		} //end of if

		else{ // for half circles on the right
			for(i=1; i<=radius; i++){
				for(j=0; j<=max_radius+count+i; j++)
					printf(" ");
				printf("\\ \n");
			}

			for(i=0; i<=max_radius+count+radius+1; i++)
				printf(" ");
			printf("|\n");

			for(i=0; i<radius; i++){
				for(j=0; j<=max_radius+count+radius-i; j++)
					printf(" ");
				printf("/\n");
			}
		} //end of else
	} //end of for
}

void control_distance(int distance){
	int count = -1;

	if( distance == 0) // This situaiton is LN = GN.
		printf("Distance: 0. You win!\n");

	while(distance >= 1){ // It determines the power of 2.
		distance = distance / 2;
		count++;
	}
	//Print distance
	if(count == 9)
		printf("Distance: 10.\n");
	else if(count == 8)
		printf("Distance: 9.\n");
	else if(count == 7)
		printf("Distance: 8.\n");
	else if(count == 6)
		printf("Distance: 7.\n");
	else if(count == 5)
		printf("Distance: 6.\n");
	else if(count == 4)
		printf("Distance: 5.\n");
	else if(count == 3)
		printf("Distance: 4.\n");
	else if(count == 2)
		printf("Distance: 3.\n");
	else if(count == 1)
		printf("Distance: 2.\n");
	else if(count == 0)
		printf("Distance: 1.\n");

}
void control_scores(int trial, int *score_human, int *score_program){
		if(trial < TRIAL) // The user win.
			*score_human += 1;

		else // The program win. 
			*score_program += 1;
}
