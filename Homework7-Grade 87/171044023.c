#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

typedef enum{ noone, cap, car } player_type;
typedef enum{ start, property, tax, punish } block_type;

typedef struct{
	player_type type;
	int current_block_id;
	int owned_block_ids[12];
	int account;
	int turn_to_wait;
	char name[5];
} player;

typedef struct{
	int block_id;
	char name[15];
	int price;
	int rent;
	int rent_1;
	int rent_2;
	int rent_3;
	int house_price;
	int house_count;
	player owner;
	block_type type;
} block;


void init_the_board(block board[20]);
void show_board(block board[20], player player_one, player player_two);
void show_properties(block board[20]);
void buy_property(block *current_block, player *current_player);
void sell_property(block board[20], player *current_player);
void gameplay (block board[20], player player_one, player player_two);
void buy_house(block *current_block, player *current_player);

int main(){
	block board[20];
	player player_one, player_two;
	int i;

	srand(time(NULL));

	player_one.type = cap;
	player_two.type = car;
	player_one.current_block_id = 0;
	player_two.current_block_id = 0;
	player_one.account = 100000;
	player_two.account = 100000;
	strcpy(player_one.name, "Cap");
	strcpy(player_two.name, "Car");
	player_one.turn_to_wait = 0;
	player_two.turn_to_wait = 0;

	for(i=0; i<12; i++){ // I fill the array with 0 because then I am going to put the ids in the array.
		player_one.owned_block_ids[i] = 0;
	}
	for(i=0; i<12; i++){ // I fill the array with 0 because then I am going to put the ids in the array.
		player_two.owned_block_ids[i] = 0;
	}

	init_the_board(board);
	
	gameplay(board, player_one, player_two);	

	return 0;
}

void gameplay (block board[20], player player_one, player player_two){
	int choice, dice, player_id_temp, i, debt;
	player current_player;
	char option;
	
	current_player = player_one; // I used this variable to play the current player.

	do{
		
		if(current_player.type == player_one.type)	show_board(board, current_player, player_two);
		else if(current_player.type == player_two.type)	show_board(board, player_one, current_player);

		printf("it's %s's turn!\n\n",current_player.name);
	
		printf("1- Roll the dice\n");
		printf("2- Show my account\n");
		printf("3- Show my properties\n");
		printf("4- Show property deeds\n");
		printf("5- Buy property\n");
		printf("6- Buy house\n");
		printf("7- Sell property\n");
		printf("Please select an option to continue: ");
		scanf("%d", &choice);

		switch(choice){
			case 1: // Roll the dice
				if(current_player.turn_to_wait == 0){ // If the player does not have a punishment, it will continue the game.
					dice = rand() % 6 + 1;
					player_id_temp = current_player.current_block_id; // I used this variable to save the previous id.
					current_player.current_block_id += dice;
					current_player.current_block_id = current_player.current_block_id % 20;
				
					if(current_player.type == player_one.type)	show_board(board, current_player, player_two);
					else if(current_player.type == player_two.type)	show_board(board, player_one, current_player);
					printf("it's %s's turn!\n\n",current_player.name);				

					if(board[current_player.current_block_id].type == start || current_player.current_block_id - player_id_temp < 0)
						current_player.account += 10000;

					else if(board[current_player.current_block_id].type == property){

						if(board[current_player.current_block_id].owner.type == noone){ // No owner of the property.
							printf("Do you want to buy the property? Yes(y or Y) - No (n or N):  ");
							scanf(" %c", &option);
							if(option == 'Y' || option == 'y')	buy_property(board, &current_player);

						}
						else if(board[current_player.current_block_id].owner.type == board[current_player.current_block_id].type){ // Property's owner is current player.
							printf("Do you want to build one or more house on the property. Yes(y or Y) or No(n or N):  ");
							scanf(" %c", &option);
							if(option == 'Y' || option == 'y')	buy_house(board, &current_player);

						}
						else if(board[current_player.current_block_id].owner.type != noone &&
							board[current_player.current_block_id].owner.type != board[current_player.current_block_id].type){ // Property's owner is other player
							switch(board[current_player.current_block_id].house_count){ // rent for the house count
								case 0:	debt = board[current_player.current_block_id].rent;
									break;
								case 1:	debt = board[current_player.current_block_id].rent_1;
									break;
								case 2:	debt = board[current_player.current_block_id].rent_2;
									break;
								case 3:	debt = board[current_player.current_block_id].rent_3;
									break;
							}

							if(current_player.account <= debt){ // if the player do not have enough money
								printf("You do not have enough money. Do you want to sell your property? Yes(y or Y) - No(n or N):  ");
								scanf(" %c", &option);
								if(option == 'Y' || option == 'y'){
									sell_property(board, &current_player);
									current_player.account -= debt;
									if(current_player.type == player_one.type)	player_two.account += debt;
									else if(current_player.type == player_two.type)	player_one.account += debt;
								}
								else if(option == 'N' || option == 'n') current_player.account = 0;	
							}
							else	current_player.account -= debt;
						}
					}
					else if(board[current_player.current_block_id].type == tax){
						debt = board[current_player.current_block_id].rent; 
						if(current_player.account <= debt){ // if the player do not have enough money
							printf("You do not have enough money. Do you want to sell your property? Yes(y or Y) - No(n or N):  ");
							scanf(" %c", &option);
							if(option == 'Y' || option == 'y'){
								sell_property(board, &current_player);
								current_player.account -= debt;
							}
							else if(option == 'N' || option == 'n') current_player.account = 0;	
						}
						else	current_player.account -= debt;
					}

					else if(board[current_player.current_block_id].type == punish)
						current_player.turn_to_wait = board[current_player.current_block_id].rent;

					// player exchange
					if(current_player.type == player_one.type){
						player_one = current_player;
						current_player = player_two;
					}
					else if(current_player.type == player_two.type){
						player_two = current_player;
						current_player = player_one;
					}
				}
				else{ // if the player has a punishment, the player can not make a move.
					current_player.turn_to_wait -= 1; // the punishment decreases.
					// the turn passes to the other player
					if(current_player.type == player_one.type){
						player_one = current_player;
						current_player = player_two;
					}
					else if(current_player.type == player_two.type){
						player_two = current_player;
						current_player = player_one;
					}
				}
				break;

			case 2: // Show my account
				printf("Your account is: %d$\n\n", current_player.account);
				break;

			case 3: // Show my propoerties
				printf("%s's properties:\n", current_player.name);
				for(i=0; i<12; i++){
					if(current_player.owned_block_ids[i] != 0)
						printf("%d - \t%s\n", current_player.owned_block_ids[i], board[current_player.owned_block_ids[i]].name);
				}
				break;

			case 4: // Show property deeds
				show_properties(board);
				break;

			case 5: // Buy property
				buy_property(board, &current_player);
				break;

			case 6: // Buy house
				buy_house(board, &current_player);
				break;

			case 7: // Sell property
				sell_property(board, &current_player);
				break;

			default:	printf("This is invalid choice!\n");
		}

	} while(player_one.account != 0 || player_two.account != 0);
}

void sell_property(block board[20], player *current_player){ //part5
	int id, i = 0;

	printf("Please enter the id for the properties you want to sell:  ");
	scanf("%d", &id);

	if(board[id].type == property){
		if(board[id].owner.type == current_player->type){
			current_player->account += board[id].price / 2;
			board[id].owner.type = noone;
			for(i=0; i<12 ; i++){ // The property is deleted from the array.
				if(current_player->owned_block_ids[i] == id)	current_player->owned_block_ids[i] = 0; 
			}
			if(board[id].house_count > 0) // if the current player have houses in the property, these houses are sold.
				current_player->account += board[id].house_count * board[id].house_price / 2;
			board[id].house_count = 0;
		}
		else printf("The property is not your property!\n");
	} 
	else	printf("This block is not a property.\n");
}

void buy_property(block *current_block, player *current_player){ //part4
	char choice;
	int i = 0, stop = 0;

	if(current_block[current_player->current_block_id].type == property){
		if(current_block[current_player->current_block_id].owner.type == noone && current_player->account > current_block[current_player->current_block_id].price){
			current_player->account -= current_block[current_player->current_block_id].price;
			current_block[current_player->current_block_id].owner.type = current_player->type;

			while(stop != 1){ // The property is added to the array. 
				if(current_player->owned_block_ids[i] == 0){
				current_player->owned_block_ids[i] = current_player->current_block_id;
				stop = 1;
				}
				i++;
			}
			
			printf("Do you want to build one or more house on the property. Yes(y or Y) - No(n or N):  ");
			scanf(" %c", &choice);
			if(choice == 'Y' || choice == 'y')	buy_house(current_block, current_player);
		}
		else if(current_block[current_player->current_block_id].owner.type != noone)	printf("This property is already taken.\n");
		else if(current_player->account < current_block[current_player->current_block_id].price)	printf("You do not have enough money.\n");
	} else	printf("This block is not a property.\n");

}

void buy_house(block *current_block, player *current_player){
	int house_number;

	printf("Enter the number of the house you want to buy: ");
	scanf("%d", &house_number);
	if(current_block[ current_player->current_block_id ].house_count + house_number < 4){

		if(current_player->account > house_number * current_block[ current_player->current_block_id ].house_price){
			current_player->account -= house_number * current_block[ current_player->current_block_id ].house_price;
			current_block[ current_player->current_block_id ].house_count += house_number;
		}
		else	printf("You do not have enough money.\n");
	}

	else	printf("You can not build more than 3 houses.\n");
}

void show_properties(block board[20]){ //part3
	int i, choice;
	
	do{
		printf("Please select a property to see details:\n");
		for(i=0; i<20; i++){
			if(board[i].type == property)
				printf("%d - \t%s\n", board[i].block_id, board[i].name);
		}
		printf("0 - \tExit\n");

		scanf("%d", &choice);
		if(board[choice].type == property){
			for(i=0; i<33; i++)	printf("-");
			printf("\n|           %-20s|\n", board[choice].name);
			for(i=0; i<33; i++)	printf("-");
			printf("\n|\tRent\t\t%-d$\t|\n", board[choice].rent);
			printf("|\tRent 1 H\t%-d$\t|\n", board[choice].rent_1);
			printf("|\tRent 2 H\t%-d$\t|\n", board[choice].rent_2);
			printf("|\tRent 3 H\t%-d$\t|\n", board[choice].rent_3);
			for(i=0; i<33; i++)	printf("-");
			printf("\n|\tHouse Price\t%-d$\t|\n", board[choice].house_price);
			for(i=0; i<33; i++)	printf("-");
			printf("\n");
		}

	} while(choice != 0);
}

void show_board(block board[20], player player_one, player player_two){ //part2
	int i, j, k, t;

	for(i=0; i<24*6+1; i++)	printf("-");
	printf("\n");

	for(i=0; i<3; i++){ // first part
		if(i == 0){ // first line
			printf("|");
			for(k=0; k<6; k++){
				printf("\t%-8s\t|", board[k].name);
			}		
			printf("\n");
		}

		else if(i == 1){ // second line
			printf("|");
			for(k=0; k<6; k++){
				if(board[k].type == tax)
					printf("\t$%-8d\t|", board[k].rent);
				else if(board[k].type == property)
					printf("\t$%-8d\t|", board[k].price);
				else
					printf("\t\t\t|");
			}		
			printf("\n");
		}

		else if(i == 2){ // third line
			printf("|");
			for(k=0; k<6; k++){
				if(player_one.current_block_id == board[k].block_id || player_two.current_block_id == board[k].block_id){

					if(player_one.current_block_id == player_two.current_block_id)	printf("\tCap Car\t\t|");
					else if(player_one.current_block_id == board[k].block_id && player_two.current_block_id != board[k].block_id)	printf("\tCap\t\t|");
					else if(player_two.current_block_id == board[k].block_id && player_one.current_block_id != board[k].block_id)	printf("\tCar\t\t|");
						
				}
				else	printf("\t\t\t|");
			}
			printf("\n");
		}

	} // finish first part

	for(i=0; i<24*6+1; i++)	printf("-");
	printf("\n");
	j = 6;

	for(t=19; t>15; t-- && j++){ // middle part

		for(i=0; i<3; i++){
			if(i == 0){ // first line
				printf("|");
				for(k=0; k<6; k++){
					if(k == 0)	printf("\t%-8s\t|", board[t].name);
					else if(k == 5)	printf("|\t%-8s\t|", board[j].name);
					else	printf("\t\t\t");
				}		
				printf("\n");
			}
			else if(i == 1){ // second line
				printf("|");
				for(k=0; k<6; k++){
					if(k == 0){
						if(board[t].type == tax)
							printf("\t$%-8d\t|", board[t].rent);
						else if(board[t].type == property)
							printf("\t$%-8d\t|", board[t].price);
						else	printf("\t\t\t");
					}
					else if(k == 5){
						if(board[j].type == tax)
							printf("|\t$%-8d\t|", board[j].rent);
						else if(board[j].type == property)
							printf("|\t$%-8d\t|", board[j].price);
						else	printf("\t\t\t");
					}
					else	printf("\t\t\t");
				}	
				printf("\n");
			}
			else if(i == 2){ // third line
				printf("|");
				for(k=0; k<6; k++){
				if(k == 0){
					if(player_one.current_block_id == board[t].block_id || player_two.current_block_id == board[t].block_id){

						if(player_one.current_block_id == player_two.current_block_id)	printf("\tCap Car\t\t|");
						else if(player_one.current_block_id == board[t].block_id && player_two.current_block_id != board[t].block_id)	printf("\tCap\t\t|");
						else if(player_two.current_block_id == board[t].block_id && player_one.current_block_id != board[t].block_id)	printf("\tCar\t\t|");
						
					}
					else	printf("\t\t\t|");
				}
				else if(k == 5){
					if(player_one.current_block_id == board[j].block_id || player_two.current_block_id == board[j].block_id){

						if(player_one.current_block_id == player_two.current_block_id)	printf("\tCap Car\t\t|");
						else if(player_one.current_block_id == board[j].block_id && player_two.current_block_id != board[j].block_id)	printf("|\tCap\t\t|");
						else if(player_two.current_block_id == board[j].block_id && player_one.current_block_id != board[j].block_id)	printf("|\tCar\t\t|");
						
					}
					else	printf("|\t\t\t|");
				}
				else	printf("\t\t\t");
			}
			printf("\n");

			}
						
		}

		for(i=0; i<25; i++)	printf("-");

		if(t == 16){
			for(i=0; i<95; i++)	printf("-");
		}
		else{
			for(i=0; i<95; i++)	printf(" ");
		}

		for(i=0; i<25; i++)	printf("-");
		
		printf("\n");		
		
	} // finish middle part

	for(i=0; i<3; i++){ // last part
		if(i == 0){// first line
			printf("|");
			for(k=15; k>9; k--){
				printf("\t%-8s\t|", board[k].name);
			}		
			printf("\n");
		}

		else if(i == 1){ // second line
			printf("|");
			for(k=15; k>9; k--){
				if(board[k].type == tax)
					printf("\t$%-8d\t|", board[k].rent);
				else if(board[k].type == property)
					printf("\t$%-8d\t|", board[k].price);
				else
					printf("\t\t\t|");
			}		
			printf("\n");
		}

		else if(i == 2){// third line
			printf("|");
			for(k=15; k>9; k--){
				if(player_one.current_block_id == board[k].block_id || player_two.current_block_id == board[k].block_id){

					if(player_one.current_block_id == player_two.current_block_id)	printf("\tCap Car\t\t|");
					else if(player_one.current_block_id == board[k].block_id && player_two.current_block_id != board[k].block_id)	printf("\tCap\t\t|");
					else if(player_two.current_block_id == board[k].block_id && player_one.current_block_id != board[k].block_id)	printf("\tCar\t\t|");

				}
				else	printf("\t\t\t|");
			}
			printf("\n");
		}
	}
	
	for(i=0; i<24*6+1; i++)	printf("-");
	printf("\n");

}

void init_the_board(block board[20]){ //part1

	board[0].block_id = 0;
	strcpy(board[0].name, "Start");
	board[0].price = 0;
	board[0].rent = 0;
	board[0].rent_1 = 0;
	board[0].rent_2 = 0;
	board[0].rent_3 = 0;
	board[0].house_price = 0;
	board[0].house_count = 0;
	board[0].owner.type = noone;	
	board[0].type = start;

	board[1].block_id = 1;
	strcpy(board[1].name, "Esenyurt");
	board[1].price = 16000;
	board[1].rent = 800;
	board[1].rent_1 = 4000;
	board[1].rent_2 = 9000;
	board[1].rent_3 = 25000;
	board[1].house_price = 10000;
	board[1].house_count = 0;
	board[1].owner.type = noone;
	board[1].type = property;

	board[2].block_id = 2;
	strcpy(board[2].name, "Car Park");
	board[2].price = 0;
	board[2].rent = 1500;
	board[2].rent_1 = 0;
	board[2].rent_2 = 0;
	board[2].rent_3 = 0;
	board[2].house_price = 0;
	board[2].house_count = 0;
	board[2].owner.type = noone;
	board[2].type = tax;

	board[3].block_id = 3;
	strcpy(board[3].name, "Tuzla");
	board[3].price = 16500;
	board[3].rent = 850;
	board[3].rent_1 = 4250;
	board[3].rent_2 = 9500;
	board[3].rent_3 = 26000;
	board[3].house_price = 12000;
	board[3].house_count = 0;
	board[3].owner.type = noone;
	board[3].type = property;

	board[4].block_id = 4;
	strcpy(board[4].name, "Arnavutkoy");
	board[4].price = 17000;
	board[4].rent = 875;
	board[4].rent_1 = 4500;
	board[4].rent_2 = 10000;
	board[4].rent_3 = 28000;
	board[4].house_price = 1200;
	board[4].house_count = 0;
	board[4].owner.type = noone;
	board[4].type = property;

	board[5].block_id = 5;
	strcpy(board[5].name, "Wait 2 Turn");
	board[5].price = 0;
	board[5].rent = 2;
	board[5].rent_1 = 0;
	board[5].rent_2 = 0;
	board[5].rent_3 = 0;
	board[5].house_price = 0;
	board[5].house_count = 0;
	board[5].owner.type = noone;
	board[5].type = punish;

	board[6].block_id = 6;
	strcpy(board[6].name, "Catalca");
	board[6].price = 20000;
	board[6].rent = 1000;
	board[6].rent_1 = 5000;
	board[6].rent_2 = 12000;
	board[6].rent_3 = 30000;
	board[6].house_price = 13000;
	board[6].house_count = 0;
	board[6].owner.type = noone;
	board[6].type = property;

	board[7].block_id = 7;
	strcpy(board[7].name, "Beykoz");
	board[7].price = 23000;
	board[7].rent = 1100;
	board[7].rent_1 = 5500;
	board[7].rent_2 = 12500;
	board[7].rent_3 = 33000;
	board[7].house_price = 13000;
	board[7].house_count = 0;
	board[7].owner.type = noone;
	board[7].type = property;

	board[8].block_id = 8;
	strcpy(board[8].name, "Car Fix");
	board[8].price = 0;
	board[8].rent = 1750;
	board[8].rent_1 = 0;
	board[8].rent_2 = 0;
	board[8].rent_3 = 0;
	board[8].house_price = 0;
	board[8].house_count = 0;
	board[8].owner.type = noone;
	board[8].type = tax;

	board[9].block_id = 9;
	strcpy(board[9].name, "Maltepe");
	board[9].price = 30000;
	board[9].rent = 1350;
	board[9].rent_1 = 7000;
	board[9].rent_2 = 15000;
	board[9].rent_3 = 40000;
	board[9].house_price = 15000;
	board[9].house_count = 0;
	board[9].owner.type = noone;
	board[9].type = property;

	board[10].block_id = 10;
	strcpy(board[10].name, "Bills");
	board[10].price = 0;
	board[10].rent = 2000;
	board[10].rent_1 = 0;
	board[10].rent_2 = 0;
	board[10].rent_3 = 0;
	board[10].house_price = 0;
	board[10].house_count = 0;
	board[10].owner.type = noone;
	board[10].type = tax;

	board[11].block_id = 11;
	strcpy(board[11].name, "Sisli");
	board[11].price = 33000;
	board[11].rent = 1500;
	board[11].rent_1 = 8000;
	board[11].rent_2 = 16000;
	board[11].rent_3 = 42000;
	board[11].house_price = 16000;
	board[11].house_count = 0;
	board[11].owner.type = noone;
	board[11].type = property;

	board[12].block_id = 12;
	strcpy(board[12].name, "Oil");
	board[12].price = 0;
	board[12].rent = 2250;
	board[12].rent_1 = 0;
	board[12].rent_2 = 0;
	board[12].rent_3 = 0;
	board[12].house_price = 0;
	board[12].house_count = 0;
	board[12].owner.type = noone;
	board[12].type = tax;

	board[13].block_id = 13;
	strcpy(board[13].name, "Atasehir");
	board[13].price = 35000;
	board[13].rent = 1600;
	board[13].rent_1 = 8500;
	board[13].rent_2 = 17000;
	board[13].rent_3 = 45000;
	board[13].house_price = 17000;
	board[13].house_count = 0;
	board[13].owner.type = noone;
	board[13].type = property;

	board[14].block_id = 14;
	strcpy(board[14].name, "Sariyer");
	board[14].price = 40000;
	board[14].rent = 1750;
	board[14].rent_1 = 9500;
	board[14].rent_2 = 19000;
	board[14].rent_3 = 48000;
	board[14].house_price = 19000;
	board[14].house_count = 0;
	board[14].owner.type = noone;
	board[14].type = property;

	board[15].block_id = 15;
	strcpy(board[15].name, "Wait 1 Turn");
	board[15].price = 0;
	board[15].rent = 1;
	board[15].rent_1 = 0;
	board[15].rent_2 = 0;
	board[15].rent_3 = 0;
	board[15].house_price = 0;
	board[15].house_count = 0;
	board[15].owner.type = noone;
	board[15].type = punish;

	board[16].block_id = 16;
	strcpy(board[16].name, "Kadikoy");
	board[16].price = 43000;
	board[16].rent = 1900;
	board[16].rent_1 = 11000;
	board[16].rent_2 = 21500;
	board[16].rent_3 = 55000;
	board[16].house_price = 23000;
	board[16].house_count = 0;
	board[16].owner.type = noone;
	board[16].type = property;

	board[17].block_id = 17;
	strcpy(board[17].name, "Besiktas");
	board[17].price = 60000;
	board[17].rent = 2500;
	board[17].rent_1 = 15000;
	board[17].rent_2 = 28000;
	board[17].rent_3 = 60000;
	board[17].house_price = 30000;
	board[17].house_count = 0;
	board[17].owner.type = noone;
	board[17].type = property;

	board[18].block_id = 18;
	strcpy(board[18].name, "Vocation");
	board[18].price = 0;
	board[18].rent = 5000;
	board[18].rent_1 = 0;
	board[18].rent_2 = 0;
	board[18].rent_3 = 0;
	board[18].house_price = 0;
	board[18].house_count = 0;
	board[18].owner.type = noone;
	board[18].type = tax;

	board[19].block_id = 19;
	strcpy(board[19].name, "Bebek");
	board[19].price = 70000;
	board[19].rent = 3500;
	board[19].rent_1 = 20000;
	board[19].rent_2 = 35500;
	board[19].rent_3 = 65000;
	board[19].house_price = 35000;
	board[19].house_count = 0;
	board[19].owner.type = noone;
	board[19].type = property;

}
