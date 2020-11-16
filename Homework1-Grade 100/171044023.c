#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define PI 3
#define SQUARE 1
#define RECTANGULAR 2
#define CIRCULAR 3
#define RED 0
#define YELLOW 1
#define BLUE 2
#define BLACK 3
#define WHITE 4

double CreateBody(int shape);
int SetColor(int color);
double LoadMoves(int shape, double body_size);
int SetAttackPower(int lower_bound, int upper_bound);
void ShowPokemon(int shape, double size_of_body, int color, double move_lenght, int attack_power);

int main(){
	int shape, color, attack_power;
	double size_of_body, move_lenght;
	shape = CIRCULAR; // SQUARE , RECTANGULAR
	size_of_body = CreateBody(shape);
	color = 798;
	color = SetColor(color);
	move_lenght = LoadMoves(shape, size_of_body);
	attack_power = SetAttackPower(0, 150);
	ShowPokemon(shape, size_of_body, color, move_lenght, attack_power);

	return 0;
}

double CreateBody(int shape){
	double radius, side1, side2;
	double size_of_body;
	if(shape == SQUARE){
		printf("Please enter a positive number for the side of square:\n");
		scanf("%lf", &side1);
		size_of_body = side1 * side1;
	}
	if(shape == RECTANGULAR){
		printf("Please enter two positive numbers for the sides of rectangular:\n");
		scanf("%lf %lf", &side1, &side2);
		size_of_body = side1 * side2;
	}
	if(shape == CIRCULAR){
		printf("Please enter a positive number for the radius of circular:\n");
		scanf("%lf", &radius);
		size_of_body = PI * pow(radius,2);
	}

	return size_of_body;
}

int SetColor(int color){
	int mod;
	if(color<1000 && color>0)
		mod = color % 5;
	else
		mod = 1;	

	return mod;
}

double LoadMoves(int shape, double body_size){
	double radius, side;
	double move_lenght;	
	if(shape == SQUARE){
		side = sqrt(body_size);
		move_lenght = 4 * side;
	}
	if(shape == RECTANGULAR){
		side = body_size / 5;   // We assumed that the second side is 5.
		move_lenght = 2 * (side + 5);
	}
	if(shape == CIRCULAR){
		radius = sqrt(body_size / PI);
		move_lenght = 2 * PI * radius;
	}

	return move_lenght;
}

int SetAttackPower(int lower_bound, int upper_bound){
	int attack_power;
	srand(time(NULL));	
	attack_power = lower_bound + rand() % (upper_bound - lower_bound);

	return attack_power;
}

void ShowPokemon(int shape, double size_of_body, int color, double move_lenght, int attack_power){
	int i, j, k;
	double radius, side;

	//Print shape
	if(shape == SQUARE){
		side = sqrt(size_of_body);
		for(i=0; i<side; i++){
			for(j=0; j<side; j++){
				printf("X ");
			}
			printf("\n");
		}
	}
	if(shape == RECTANGULAR){
		side = (int)size_of_body / 5;   // We asssumed that the second side is 5.
		for(i=0; i<5; i++){
			for(j=0; j<side; j++){
				printf("X");
			}
			printf("\n");
		}
	}
	if(shape == CIRCULAR){
		radius = sqrt(size_of_body / PI);
		k = 1;   // I used this variable(k) to determine the number of tab between two "X".
		if((int)radius % 2 == 0){   // I used this if for circles with even numbers.  
			for(i=0; i<(radius/2)+1; i++){   // number of the top rows
				if(i == 0){   // the first row
					for(j=0; j<(radius/2)-1; j++)
						printf("\t");
					printf("    X");
				}
				else{   // the other rows
					for(j=0; j<(radius/2)-i ;j++)
						printf("\t");
					printf("X");
					for(j=0; j<k; j++)
						printf("\t");
					printf("X");
					k = k+2;
				}
				printf("\n");
			}
			for(i=1; i<(radius/2)+1; i++){   // number of the bottom rows
				if(i == radius/2){   // the last row
					for(j=0 ;j<(radius/2)-1 ;j++)
						printf("\t");
					printf("    X");
				}
				else{   // the other rows
					for(j=0; j<i; j++)
						printf("\t");
					printf("X");
					for(j=1; j<radius-(i*2); j++)
						printf("\t");
					printf("X");
				}
				printf("\n");
			}
		}
		else{ // odd numbers
			for(i=0; i<(radius/2); i++){   // number of the top rows	
				if(i == 0){   // the first row
					for(j=1; j<(radius/2); j++)
						printf("\t");
					printf("X");
				}
				else{   // the other rows
					for(j=1; j<(radius/2)-i; j++)
						printf("\t");
					printf("X");
					for(j=0; j<(i*2); j++)
						printf("\t");
					printf("X");
				}
				printf("\n");
				}
			for(i=1; i<(radius/2); i++){   // number of the bottom rows
				if(i == radius/2-0.5){   // the last row
					for(j=1; j<(radius/2); j++)
						printf("\t");
					printf("X");
				}
				else{   // the other rows
					for(j=0; j<i; j++)
						printf("\t");
					printf("X");
					for(j=1; j<radius-(i*2); j++)
						printf("\t");
					printf("X");
				}
				printf("\n");
			}
		}
	}

	//Print name
	if(shape == SQUARE)
		printf("Name: Square Pokémon \n");
	if(shape == RECTANGULAR)
		printf("Name: Rectangular Pokémon \n");
	if(shape == CIRCULAR)
		printf("Name: Circular Pokémon \n");

	//Print size
	printf("Size: %.2f\n",size_of_body);
	
	//Print color
	if(color == RED)
		printf("Color: Red\n");
	if(color == YELLOW)
		printf("Color: Yellow\n");
	if(color == BLUE)
		printf("Color: Blue\n");
	if(color == BLACK)
		printf("Color: Black\n");
	if(color == WHITE)
		printf("Color: White\n");
	
	//Print move lenght
	printf("Move: %.2f\n",move_lenght);

	//Print attack power
	printf("Attack Power: %d\n",attack_power);
}

