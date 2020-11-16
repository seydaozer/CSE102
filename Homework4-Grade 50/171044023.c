#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef enum
	{ Charmander, Pikachu, Squirtle, Bulbasaur, Pidgeotto, Ratata, Mug, Caterpie, Zubat, Krabby, empty } 
pokemon;

typedef enum
	{ quadratic, linear }
attack_type;

void Menu();
void pokedex(char Pokemon_name[10][11], int range[], attack_type type[], int attack_power[], int stamina[]); //part1
int compare_name(char Pokemon_name[11], char name[11]); //part1

void oaks_laboratory(char Pokemon_name[10][11], pokemon Pokemons[], pokemon *my_Pokemons); //part2
void show_Pokemons(char Pokemon_name[10][11], pokemon Pokemons[], int pokemon_count); //part2
void catch_a_pokemon(char Pokemon_name[10][11], pokemon Pokemons[], pokemon *my_pocket); //part2
void release_a_pokemon(char Pokemon_name[10][11], pokemon Pokemons[], pokemon *my_pocket); //part2

void battle(char Pokemon_name[10][11], int range[], attack_type type[], int attack_power[], int stamina[], pokemon user_Pokemons[]); //part3
void show_area (char Pokemon_name[10][11], int area[8][8], int pokemon_staminas[8][8]); //part3

int main(){
	srand(time(NULL));
	
	Menu();

	return 0;
}

void Menu(){

	char Pokemon_name[10][11] = { "Charmander", "Pikachu", "Squirtle", "Bulbasaur", "Pidgeotto", "Ratata", "Mug", "Caterpie", "Zubat", "Krabby" };
	int range[] = {1, 3, 4, 3, 2, 2, 1, 2, 3, 2};
	int attack_power[] = {500, 350, 300, 400, 250, 250, 350, 200, 350, 300};
	attack_type type[10] = {quadratic, linear, linear, linear, quadratic, linear, quadratic, quadratic, linear, linear}; 
	int stamina[] = {2200, 1500, 1700, 2500, 1900, 2500, 3000, 1200, 1250, 2600};
	
	pokemon Pokemons[10] = { Charmander, Pikachu, Squirtle, Bulbasaur, Pidgeotto, Ratata, Mug, Caterpie, Zubat, Krabby};
	pokemon my_pocket[4] = {empty, empty, empty, empty};

	int choice, i, control_pocket;	
	
	printf("\n1) Open Pokedex\n");
	printf("2) Go to Oak’s Laboratory\n");
	printf("3) Enter the tournament\n");
	printf("4) Exit\n");
	scanf("%d", &choice);

	do{
		switch(choice){

		case 1:
			pokedex(Pokemon_name, range, type, attack_power, stamina);	
			break;
	
		case 2:
			oaks_laboratory(Pokemon_name, Pokemons, my_pocket);
			break;

		case 3:
			for(i=0; i<4; i++){
				if(my_pocket[i] == empty)
					control_pocket = 0;
				else
					control_pocket = 1;
					
			}

			if(control_pocket == 1)
				battle(Pokemon_name, range, type, attack_power, stamina, my_pocket);
			
			else	printf("There are not enough pokémons in your pocket.\n");

			break;

		default:	printf("This is an invalid choice. Try again!\n\n");
		
		}
	}while(choice != 4);
	
}

void oaks_laboratory(char Pokemon_name[10][11], pokemon Pokemons[], pokemon *my_Pokemons){
	int choice, pokemon_count;	
	
	do{
	
		printf("\nWelcome to Laboratory of Profeessor Oak. How can I help you?\n");
		printf("1) Show Pokémons\n");
		printf("2) Catch a Pokémon\n");
		printf("3) Release a Pokémon\n");	
		printf("4) Show my pocket\n");
		printf("5) Back\n");

		scanf("%d", &choice);
	

			switch(choice){
				case 1:
					pokemon_count = 10;
					show_Pokemons(Pokemon_name, Pokemons, pokemon_count);
					break;

				case 2:
					pokemon_count = 10;
					show_Pokemons(Pokemon_name, Pokemons, pokemon_count);	
					catch_a_pokemon(Pokemon_name, Pokemons, my_Pokemons);
					
					break;

				case 3:
					pokemon_count = 4;
					show_Pokemons(Pokemon_name, my_Pokemons, pokemon_count);
					release_a_pokemon(Pokemon_name, Pokemons, my_Pokemons);
					show_Pokemons(Pokemon_name, my_Pokemons, pokemon_count);

					break;

				case 4:
					pokemon_count = 4;
					show_Pokemons(Pokemon_name, my_Pokemons, pokemon_count);

					break;
				
			}
		
	}while(choice != 5);
}
	
void pokedex(char Pokemon_name[10][11], int range[], attack_type type[], int attack_power[], int stamina[]){
	int i;
	char name[11];
	pokemon poke = empty;

	printf("Please type name of the Pokémon (type exit to close Pokedex):\n");
	scanf("%s", name);

	
	for(i=0; i<10; i++){
	
		if(compare_name(Pokemon_name[i], name) == 1)
			poke = i;
	}	

	if(poke == empty)
		printf("You have entered Pokémon's name incorrectly. Please enter as shown in the menu.\n");
		
	else{
		printf("Name         : %s\n", Pokemon_name[poke]);

		printf("Attack Type  : ");
		if(poke == 0 || poke == 4 || poke == 6 || poke == 7)	printf("Quadratic\n");
		else	printf("Linear\n");	

		printf("Range        : %d block\n", range[poke]);
		printf("Attack Power : %d\n", attack_power[poke]);
		printf("Stamina      : %d\n", stamina[poke]);
	}
}

int compare_name(char Pokemon_name[11], char name[11]){ // compare name entered
	int j, temp;

	for(j=0; name[j] == Pokemon_name[j] && name[j] != '\0'; j++);
		if(name[j] == Pokemon_name[j])
			temp = 1;
		else
			temp = 0;

	return temp;
}

void show_Pokemons(char Pokemon_name[10][11], pokemon Pokemons[], int pokemon_count){
	int i;

	if(pokemon_count == 4)	printf("Pokémon in the user's pocket:\n");
	else	printf("Pokémon List:\n");

	for(i=0; i<pokemon_count; ++i){
		if(Pokemons[i] == empty)	printf(" "); 
		else	printf("%d - %s\n", Pokemons[i], Pokemon_name[Pokemons[i]]);
	}
}

void catch_a_pokemon(char Pokemon_name[10][11], pokemon Pokemons[], pokemon *my_pocket){
	int pokemon;

	printf("Please enter the name of the Pokémon you want to catch.(You can catch up to four Pokémon.)\n");

	scanf("%d", &pokemon);

	if(pokemon == my_pocket[0] || pokemon == my_pocket[1] || pokemon == my_pocket[2] || pokemon == my_pocket[3])
		 printf("You have entered a previously selected name. Please enter a new Pokémon.\n");
	
	else{
		if(my_pocket[0] == empty)
			my_pocket[0] = Pokemons[pokemon];
		else if(my_pocket[1] == empty)
			my_pocket[1] = Pokemons[pokemon];
		else if(my_pocket[2] == empty)
			my_pocket[2] = Pokemons[pokemon];
		else if(my_pocket[3] == empty)
			my_pocket[3] = Pokemons[pokemon];
	}
}

void release_a_pokemon(char Pokemon_name[10][11], pokemon Pokemons[], pokemon *my_pocket){
	int pokemon, i;
	
	printf("Please enter the name of the Pokémon you want to remove.\n");
		
	scanf("%d", &pokemon);		

	for(i=0; i<4; i++){
		if(my_pocket[i] == pokemon)
			my_pocket[i] = empty;
	}
}

void battle(char Pokemon_name[10][11], int range[10], attack_type type[10], int attack_power[10], int stamina[10], pokemon user_Pokemons[10]){
	int i, j, k = 0, row, column;
	int area[8][8];	
	int pokemon_staminas[8][8];
	int computer_pokemon[4];

	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			area[i][j] = -1;
			pokemon_staminas[i][j] =  -1;
		}
	}

	for(i=0; i<4; i++){
		printf("Please enter the row and column numbers for your Pokémon(%s). The row must be between 1 and 2, the column must be between 1 and 8:\n", Pokemon_name[user_Pokemons[i]]);
		scanf("%d %d", &row, &column);
		area[row-1][column-1] = user_Pokemons[i];
	}
	

	while(i<8){
		computer_pokemon[k] = rand() % 9;
		row = rand() % 2 + 6; //random row
		column = rand() % 8; //random column
		area[row][column] = computer_pokemon[k]; //the are for computer
		pokemon_staminas[row][column] = stamina[computer_pokemon[k]]; // staminas for computer
		i++;
		k++;
	}

	show_area(Pokemon_name, area, pokemon_staminas);

}

void show_area(char Pokemon_name[10][11], int area[8][8], int pokemon_staminas[8][8]){
	int i, j, k;

	for(i=0; i<11*8+1; i++){
		printf("-");
	}
	printf("\n");

	for(i=0; i<8; i++){ // for rows
			
		for(j=0; j<8; j++){ // for first column
			if(area[i][j] == -1){
				printf("|          ");
			}
			else{
				printf("|   ");
				for(k=0; k<3; k++){ // print the first three letters of pokémon
					printf("%c", Pokemon_name[area[i][j]][k]);
				}
				printf("    ");
			}
		}
		
		printf("|\n");
		
		for(j=0; j<8; j++){ // for second column
			if(pokemon_staminas[i][j] == -1){
				printf("|          ");
			}
			else{
				printf("|   ");
				printf("%d", pokemon_staminas[i][j]);
				printf("   ");
			}
		}
		
				
		printf("|\n");
	
		for(j=0; j<11*8+1; j++){ // for hyphen
			printf("-");
		}
		printf("\n");
	}

}
