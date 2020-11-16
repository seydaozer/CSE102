#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

typedef enum{ noone, cap, car } player_type;
typedef enum{ start, property, tax, punish, fortune } block_type;

typedef struct{
	player_type type;
	int current_block_id;
	int owned_block_ids[12];
	int account;
	int turn_to_wait;
	char *name;
} player;

typedef struct block{
	struct block *next;
	int block_id;
	char *name;
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

typedef struct fortune_card{
	struct fortune_card *next;
	char *name;
	int no;
} fortune_card;

void init_the_board(block *board);
void show_board(block *board, player player_one, player computer);
void compare(block *board);
void print_cap_car(block *temp, player player_one, player computer);
void show_properties(block *board);
void buy_property(block *current_block, player *current_player);
void sell_property(block *board, player *current_player);
void gameplay (block *board, player player_one, player computer);
void buy_house(block *current_block, player *current_player);
int roll_dice();
void play_computer(block *board, fortune_card *fortune_cards, player *computer, player *player_one);
void computer_buy_property(block *board, player *computer);
int property_counter(player *computer);
void computer_sell_property(block *board, player *computer);
void fill_fortune_cards(fortune_card *fortune_cards); // fill the fortune cards
void fortune_part(block *board, fortune_card *fortune_cards, player *current_player, player *other_player);

int main(){
	block *head, *board;
	player player_one, computer;
	int i;

	srand(time(NULL));

	head = (block *)malloc(sizeof(block));
	board = head;

	player_one.type = car;
	player_one.current_block_id = 0;
	player_one.account = 100000;
	player_one.name = (char*)malloc(3 * sizeof(char));
	strcpy(player_one.name, "Car");
	player_one.turn_to_wait = 0;

	computer.type = cap;
	computer.current_block_id = 0;
	computer.account = 100000;
	computer.name = (char*)malloc(3 * sizeof(char));
	strcpy(computer.name, "Cap");
	computer.turn_to_wait = 0;

	for(i=0; i<12; i++){ // I fill the array with 0 because then I am going to put the ids in the array.
		player_one.owned_block_ids[i] = 0;
	}
	for(i=0; i<12; i++){ // I fill the array with 0 because then I am going to put the ids in the array.
		computer.owned_block_ids[i] = 0;
	}

	init_the_board(board);	
	board = head;

	show_board(board, player_one, computer);
	gameplay(board, player_one, computer);

	return 0;
}

void gameplay (block *board, player player_one, player computer){
	int choice, next_block, option, debt, i, j, selection;
	player current_player;
	block *temp, *temp1;
	fortune_card *fortune_cards, *head_f, *temp_f;

	head_f = (fortune_card *)malloc(sizeof(fortune_card));
	fortune_cards = head_f;

	fill_fortune_cards(fortune_cards); // the function fill the fortune_cards.
	fortune_cards = head_f;

	current_player = player_one; // I used this variable to play the current player.

	while(player_one.account >= 0 && computer.account >= 0){
		temp_f = fortune_cards;
		temp = board;
		//if(current_player.type == player_one.type)	show_board(board, current_player, computer);
		//else if(current_player.type == computer.type)	show_board(board, player_one, current_player);
		
		printf("%s :%d$ , %s:%d$\n\n", player_one.name, player_one.account, computer.name, computer.account);
	
		printf("1- Roll the dice\n");
		printf("2- Show my account\n");
		printf("3- Show my properties\n");
		printf("4- Show property deeds\n");
		printf("5- Buy property\n");
		printf("6- Buy house\n");
		printf("7- Sell property\n");
		printf("Please select an option to continue( %s ): ", current_player.name);

		scanf("%d", &choice);
		
		switch(choice){
			case 1: // roll the dice
				if(current_player.turn_to_wait == 0){ // If the player does not have a punishment, it will continue the game.
					if(current_player.type == player_one.type){ // its player_one's turn.
						next_block = (current_player.current_block_id + roll_dice());
		    			        if (next_block >= 24)	current_player.account += 10000;
		        			current_player.current_block_id = next_block % 24;				
						
						for(i=0; i<current_player.current_block_id; i++){
							temp = temp->next;
						}

						switch(temp->type){
							case property:
								printf("%s has arrived %s \n", current_player.name, temp->name);
								if(temp->owner.type == noone){ // No owner of the property.
									printf("Do you want to buy %s ?\n", temp->name);
									printf("1- Yes\n");
									printf("2- No\n");
									scanf(" %d", &option);

									if(option == 1){
										buy_property(temp, &current_player);
										printf("You left %d in your account.\n", current_player.account);
										printf("Do you want to build one or more house on the property?\n");
										printf("1- Yes\n");
										printf("2- No\n");
										scanf("%d", &selection);	
										if(selection == 1)	buy_house(temp, &current_player);
									}

								}
								else{ // Property's owner is other player
									switch(temp->house_count){ // the rent for the house count
										case 0:	debt = temp->rent;
											break;
										case 1:	debt = temp->rent_1;
											break;
										case 2:	debt = temp->rent_2;
											break;
										case 3:	debt = temp->rent_3;
											break;
									}
									if(current_player.account <= debt){ // if the player do not have enough money
										printf("You do not have enough money. Do you want to sell your property?\n");
										printf("1- Yes\n");
										printf("2- No\n");
										scanf("%d", &option);
										if(option == 1){
											sell_property(temp, &current_player);
											current_player.account -= debt;
											computer.account += debt;	
										}
										else if(option == 2) current_player.account -= debt; // player_one lost the game.
									}
									else{
										current_player.account -= debt;
										computer.account += debt;
									}
								}
								break;

							case tax:
								printf("%s has arrived tax block(%s).\n", current_player.name, temp->name);
								debt = temp->rent; 
								if(current_player.account <= debt){ // if the player do not have enough money
									printf("You do not have enough money. Do you want to sell your property?\n");
									printf("1- Yes\n");
									printf("2- No\n");
									scanf("%d", &option);
									if(option == 1){
										sell_property(temp, &current_player);
										current_player.account -= debt;
						
									}
									else if(option == 2) current_player.account -= debt; // player_one lost the game.
								}
								else	current_player.account -= debt;
								break;

							case punish:
								current_player.turn_to_wait = temp->rent;
								printf("%s has punished for %d turn.\n", current_player.name, temp->rent);
								break;

							case fortune:
								printf("%s has arrived fortune block.\n", current_player.name);
								temp_f = fortune_cards;
								temp1 = board;
								fortune_part(temp1, temp_f, &current_player, &computer);
								break;
						}
						// player exchange
						if(current_player.type == player_one.type){
							player_one = current_player;
							current_player = computer;
						}

					}
					else{ // its computer's turn.
						play_computer(board, fortune_cards, &current_player, &player_one);
						// player exchange
						if(current_player.type == computer.type){
							computer = current_player;
							current_player = player_one;
						}
					}
					// show the board
					if(current_player.type == player_one.type)	show_board(board, current_player, computer);
					else if(current_player.type == computer.type)	show_board(board, player_one, current_player);
		
				}
				else{ // current_player has a punishment.
     					printf("You need to wait %d turn to continue.\n",current_player.turn_to_wait);
					current_player.turn_to_wait-=1;
					// player exchange
					if(current_player.type == player_one.type){
						player_one = current_player;
						current_player = computer;
					}
					else if(current_player.type == computer.type){
						computer = current_player;
						current_player = player_one;
					}
               			}
				break;

			case 2: // Show my account
				printf("Your account is: %d$\n\n", current_player.account);
				break;

			case 3: // Show my poperties
				printf("%s's properties:\n", current_player.name);
				printf("----------------------------------------\n");
				for(i=0; i<12; i++){
					temp1 = board;
					if(current_player.owned_block_ids[i] != 0){
						for(j=1; j<=current_player.owned_block_ids[i]; j++)	temp1 = temp1->next;
						printf("%d - \t%s\n", current_player.owned_block_ids[i], temp1->name);
					}
				}
				printf("----------------------------------------\n");
				break;

			case 4: // Show property deeds
				show_properties(board);
				break;

			case 5: // Buy property
				buy_property(temp, &current_player);
				break;

			case 6: // Buy house
				buy_house(temp, &current_player);
				break;

			case 7: // Sell property
				sell_property(board, &current_player);
				break;

			default:	printf("This is invalid choice!\n");
		}
	} 
}

void play_computer(block *board, fortune_card *fortune_cards, player *computer, player *player_one){
	int next_block, i, average = 0, stop = 0, dice1, dice2, num_properties, debt;
	block *temp, *temp1;
	fortune_card *temp_f;

	temp_f = fortune_cards;
	temp = board, temp1 = board;

	dice1 = roll_dice();
	next_block = (computer->current_block_id + dice1);
	if (next_block >= 24)	computer->account += 10000;
	computer->current_block_id = next_block % 24;				

	for(i=0; i<computer->current_block_id; i++){
		temp = temp->next;
	}
	printf("Computer rolled the dice: %d\n\n", dice1);

	switch(temp->type){
		case property:
			printf("%s(computer) has arrived %s.\n", computer->name, temp->name);
			if(temp->owner.type == noone){ // No owner of the property.
				if(computer->account > temp->price){
					for(i=0; i<24; i++){
						average += temp1->price;
						temp1 = temp1->next;
					}
					average /= 24;
					if(temp->price < average)	computer_buy_property(temp, computer);
					else{
						dice2 = roll_dice();
						if(dice2 >= 1 && dice2 <= 3)	computer_buy_property(temp, computer);
					}
				}
			}
			else if(temp->owner.type == computer->type){ // Property's owner is computer.
				num_properties = property_counter(computer);
				if(num_properties <= 4){
					dice2 = roll_dice();
					if(dice2 >= 1 && dice2 <= 3){
						if(temp->house_count < 4){
							if(computer->account > temp->house_price){
								computer->account -= temp->house_price;
								temp->house_count += 1;
							}
						}
					}
				}
			}
			else{ // Property's owner is player_one.
				switch(temp->house_count){ // rent for the house count
					case 0:	debt = temp->rent;
						break;
					case 1:	debt = temp->rent_1;
						break;
					case 2:	debt = temp->rent_2;
						break;
					case 3:	debt = temp->rent_3;
						break;
				}
				if(computer->account <= debt){ // if the computer do not have enough money
					computer_sell_property(board, computer);
					computer->account -= debt;
					player_one->account += debt;
				}
				else{
					computer->account -= debt;
					player_one->account += debt;
				}				
			}
			break;

		case tax:
			printf("Computer has arrived tax block(%s).\n", temp->name);
			debt = temp->rent; 
			if(computer->account <= debt){ // if the computer do not have enough money		
				computer_sell_property(board, computer);
				computer->account -= debt;
			}
			else	computer->account -= debt;

			break;

		case punish:
			computer->turn_to_wait = temp->rent;
			printf("Computer has punished for %d turn.\n", temp->rent);
			break;

		case fortune:
			printf("Computer has arrived fortune block.\n");
			temp = board;
			fortune_part(temp, temp_f, computer, player_one);
			break;	
	}

}

void fortune_part(block *board, fortune_card *fortune_cards, player *current_player, player *other_player){
	int fortune_no, stop = 0, i = 1, free_house_id, dice, next_block, num_property;
	
	fortune_no = rand() % 5 + 1;

	while(stop != 1){ // I am finding the fortune card which the program is giving randomly.
		if(fortune_cards->no == fortune_no)	stop = 1;
		if(stop != 1)	fortune_cards = fortune_cards->next;
		i++;
	}
	printf("Fortune card is: %s!!\n", fortune_cards->name);
	
	switch(fortune_no){
		case 1: // Free house
			num_property = property_counter(current_player); // the number of properties the current player owns.
			if(num_property != 0){
				free_house_id = rand() % num_property; // I did a random selection in these properties.
				for(i=0; i<current_player->owned_block_ids[free_house_id]; i++)
					board = board->next;
				if(board->house_count < 4){
					if(current_player->account > board->house_price){
						current_player->account -= board->house_price;
						board->house_count += 1;
					}
					else printf("You do not enough money!\n");
				}
				else	printf("There are enough houses on this property(the program is giving randomly).\n");
			}
			break;

		case 2: // Time Travel
			dice = roll_dice();
			if(dice >= 1 && dice <=3){
				next_block = (current_player->current_block_id + 2);
				if (next_block >= 24)	current_player->account += 10000;
				current_player->current_block_id = next_block % 24;
			}
			else	current_player->current_block_id -= 2;
			break;

		case 3: // Garnishment
			current_player->account -= 5000;
			break;

		case 4: // Generosity
			current_player->account -= 10000;
			other_player->account += 10000;
			break;

		case 5: // Treasure Hunter
			current_player->account += 20000;
			break;
									
	}

}

void computer_sell_property(block *board, player *computer){
	int i, j, max = 0, stop = 0;
	block *temp;
	temp = board;

	for(i=0; i<12; i++){// I am finding the maximum price.
		temp = board;
		if(computer->owned_block_ids[i] != 0){
			for(j=1; j<=computer->owned_block_ids[i]; j++)	temp = temp->next;
			if(temp->price > max)	max = temp->price;
		}
	}

	temp = board;
	i = 0;
	while(stop != 1){ // I am finding the property with the maximum value.
		if(temp->price == max)	stop = 1;
		if(stop != 1)	temp = temp->next;
		i++;
	}
	
	computer->account += max / 2;
	temp->owner.type = noone;
	for(i=0; i<12 ; i++){ // The property is deleted from the array.
		if(computer->owned_block_ids[i] == temp->block_id)	computer->owned_block_ids[i] = 0; 
	}
	if(temp->house_count > 0) // if the current player have houses in the property, these houses are sold.
		computer->account += temp->house_count * temp->house_price / 2;
	temp->house_count = 0;

	printf("Computer has sold %s.\n", temp->name);
}

int property_counter(player *computer){ // I find number of the player's properties.
	int i, counter = 0;

	for(i=0; i<12; i++){
		if(computer->owned_block_ids[i] != 0)
			counter++;
	}

	return counter;
}
void computer_buy_property(block *board, player *computer){
	int stop = 0, i = 0;

	computer->account -= board->price;
	board->owner.type = computer->type;

	while(stop != 1){ // The property is added to the array. 
		if(computer->owned_block_ids[i] == 0){
			computer->owned_block_ids[i] = computer->current_block_id;
			stop = 1;
		}	
		i++;	
	}

	printf("Computer has bought %s.\n", board->name);

}

void sell_property(block *board, player *current_player){ //part5
	int id, i = 0, j;

	printf("Please enter the id for the properties you want to sell:  ");
	scanf("%d", &id);

	for(j=0; j<=id; j++){
		board = board->next;
	}

	if(board->type == property){
		if(board->owner.type == current_player->type){
			current_player->account += board->price / 2;
			board->owner.type = noone;
			for(i=0; i<12 ; i++){ // The property is deleted from the array.
				if(current_player->owned_block_ids[i] == id)	current_player->owned_block_ids[i] = 0; 
			}
			if(board->house_count > 0) // if the current player have houses in the property, these houses are sold.
				current_player->account += board->house_count * board->house_price / 2;
			board->house_count = 0;
			printf("You have sold %s.\n", board->name);
		}
		else printf("The property is not your property!\n");
	} 
	else	printf("This block is not a property.\n");

}

void buy_property(block *current_block, player *current_player){ //part4
	int i = 0, stop = 0;

	if(current_block->type == property){
		if(current_block->owner.type == noone && current_player->account > current_block->price){
			current_player->account -= current_block->price;
			current_block->owner.type = current_player->type;
			printf("Congratiulations! You have bought %s.\n", current_block->name);
			while(stop != 1){ // The property is added to the array. 
				if(current_player->owned_block_ids[i] == 0){
					current_player->owned_block_ids[i] = current_player->current_block_id;
					stop = 1;
				}
				i++;
			}
		}
		else if(current_block->owner.type != noone)	printf("This property is already taken.\n");
		else if(current_player->account <= current_block->price)	printf("You do not have enough money.\n");
	} else	printf("This block is not a property.\n");

}

void buy_house(block *current_block, player *current_player){
	int house_number;

	printf("Enter the number of the house you want to buy: ");
	scanf("%d", &house_number);
	if(current_block->house_count + house_number < 4){

		if(current_player->account > house_number * current_block->house_price){
			current_player->account -= house_number * current_block->house_price;
			current_block->house_count += house_number;
			printf("%d houses were built. And there are now %d houses.\n", house_number, current_block->house_count);
		}
		else	printf("You do not have enough money.\n");
	}
	else	printf("You can not build more than 3 houses.\n");
}

int roll_dice(){
	int dice;
	dice = rand() % 6 + 1;
	return dice;
}

void show_properties(block *board){
	int i, choice;
	block * temp;
	
	do{
		temp = board;
		printf("Please select a property to see details:\n");
		for(i=0; i<24; i++){
			if(temp->type == property)
				printf("%d - \t%s\n", temp->block_id, temp->name);
			temp = temp->next;
		}
		printf("0 - \tExit\n");

		scanf("%d", &choice);

		temp = board;
		for(i=0; i<choice; i++)	temp = temp->next;
		
		if(temp->type == property){
			for(i=0; i<33; i++)	printf("-");
			printf("\n|           %-20s|\n", temp->name);
			for(i=0; i<33; i++)	printf("-");
			printf("\n|\tRent\t\t%-d$\t|\n", temp->rent);
			printf("|\tRent 1 H\t%-d$\t|\n", temp->rent_1);
			printf("|\tRent 2 H\t%-d$\t|\n", temp->rent_2);
			printf("|\tRent 3 H\t%-d$\t|\n", temp->rent_3);
			for(i=0; i<33; i++)	printf("-");
			printf("\n|\tHouse Price\t%-d$\t|\n", temp->house_price);
			for(i=0; i<33; i++)	printf("-");
			printf("\n");
		}
		
	} while(choice != 0);
}

void show_board(block *board, player player_one, player computer){
	int i, j, k, up1, up2, up3, down1, down2, down3;
	block *temp;
	temp = board;
	
	for(i=0; i<24*7+1; i++)	printf("-");
	printf("\n");

	for(i=0; i<3; i++){ // first part
		if(i == 0){ // first line
			printf("|");
			for(j=0; j<7; j++){
				printf("\t%-8s\t|", temp->name);
				temp = temp->next;
			}	
			printf("\n");
		}
		else if(i == 1){ // second line
			temp = board;
			printf("|");
			for(j=0; j<7; j++){
				compare(temp);
				temp = temp->next;
			}		
			printf("\n");
		}
		else if(i == 2){ // third line
			temp = board;
			printf("|");
			for(j=0; j<7; j++){
				print_cap_car(temp, player_one, computer);
				temp = temp->next;
			}
			printf("\n");
		}
	} // finish first part

	for(i=0; i<24*7+1; i++)	printf("-");
	printf("\n");		

	//second part
	up1 = 23, up2 = 23, up3 = 23;
	down1 = 7, down2 = 7, down3 = 7; 
	for(j=0; j<5; j++){
		//first line
		temp = board;
		for(i=0; i<up1; i++)	temp = temp->next;
		printf("|\t%-8s\t|", temp->name);
		printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
		temp = board;
		for(i=0; i<down1; i++)	temp = temp->next;
		printf("|\t%-8s\t|", temp->name);
		printf("\n");
		up1--;
		down1++;

		// second line
		temp = board;
		for(i=0; i<up2; i++)	temp = temp->next;
		printf("|");
		compare(temp);
		printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t|");
		temp = board;
		for(i=0; i<down2; i++)	temp = temp->next;
		compare(temp);
		printf("\n");
		up2--;
		down2++;

		//third line
		temp = board;
		for(i=0; i<up3; i++)	temp = temp->next;
		printf("|");
		print_cap_car(temp, player_one, computer);
		printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t|");
		temp = board;
		for(i=0; i<down3; i++)	temp = temp->next;
		print_cap_car(temp, player_one, computer);		
		up3--;
		down3++;
		printf("\n");
		if(j == 4){
			for(i=0; i<24*7+1; i++)	printf("-");
			printf("\n");
		}
		else{
			printf("-------------------------");
			printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
			printf("-------------------------\n");
		}
		//finish third line	
	} // finish second part
	
	for(i=0; i<3; i++){ // third part
		if(i == 0){ // first line
			up1 = 18;
			printf("|");
			for(j=0; j<7; j++){
				temp = board;
				for(k=0; k<up1; k++)	temp = temp->next;
				printf("\t%-8s\t|", temp->name);
				up1--;
			}	
			printf("\n");
		}
		if(i == 1){ // second line
			up1 = 18;
			printf("|");
			for(j=0; j<7; j++){
				temp = board;
				for(k=0; k<up1; k++)	temp = temp->next;
				compare(temp);
				up1--;
			}		
			printf("\n");
		}
		if(i == 2){ // third line
			up1 = 18;
			printf("|");
			for(j=0; j<7; j++){
				temp = board;
				for(k=0; k<up1; k++)	temp = temp->next;
				print_cap_car(temp, player_one, computer);
				up1--;
			}
			printf("\n");
		}
	} // finish third part
	for(i=0; i<24*7+1; i++)	printf("-");
	printf("\n");
}

void print_cap_car(block *temp, player player_one, player computer){

	if(player_one.current_block_id == temp->block_id || computer.current_block_id == temp->block_id){
		if(player_one.current_block_id == computer.current_block_id)	printf("\tCar Cap\t\t|");
		else if(player_one.current_block_id == temp->block_id)	printf("\tCar\t\t|");
		else if(computer.current_block_id == temp->block_id)	printf("\tCap\t\t|");					
	}
	else	printf("\t\t\t|");

}

void compare(block *board){
	if(board->type == tax)
		printf("\t$%-8d\t|", board->rent);
	else if(board->type == property)
		printf("\t$%-8d\t|", board->price);
	else	printf("\t\t\t|");
}

void fill_fortune_cards(fortune_card *fortune_cards){

	fortune_cards->no = 1;
	fortune_cards->name = (char*)malloc( (strlen("Free House")+1) * sizeof(char) );
	strcpy(fortune_cards->name, "Free House");

	fortune_cards->next = (fortune_card *)malloc(sizeof(fortune_card));
	fortune_cards = fortune_cards->next;
	fortune_cards->no = 2;
	fortune_cards->name = (char*)malloc( (strlen("Time Travel")+1) * sizeof(char) );
	strcpy(fortune_cards->name, "Time Travel");

	fortune_cards->next = (fortune_card *)malloc(sizeof(fortune_card));
	fortune_cards = fortune_cards->next;
	fortune_cards->no = 3;
	fortune_cards->name = (char*)malloc( (strlen("Garnishment")+1) * sizeof(char) );
	strcpy(fortune_cards->name, "Garnishment");

	fortune_cards->next = (fortune_card *)malloc(sizeof(fortune_card));
	fortune_cards = fortune_cards->next;
	fortune_cards->no = 4;
	fortune_cards->name = (char*)malloc( (strlen("Generosity")+1) * sizeof(char) );
	strcpy(fortune_cards->name, "Generosity");

	fortune_cards->next = (fortune_card *)malloc(sizeof(fortune_card));
	fortune_cards = fortune_cards->next;
	fortune_cards->no = 5;
	fortune_cards->name = (char*)malloc( (strlen("Treasure Hunter")+1) * sizeof(char) );
	strcpy(fortune_cards->name, "Treasure Hunter");
	
	fortune_cards->next = NULL;	
}

void init_the_board(block *board){
	
	board->name = (char *)malloc(strlen("Start") * sizeof(char));
	board->block_id = 0;
	strcpy(board->name, "Start");
	board->price = 0;
	board->rent = 0;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;	
	board->type = start;
	
	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Esenyurt") * sizeof(char));
	board->block_id = 1;
	strcpy(board->name, "Esenyurt");
	board->price = 16000;
	board->rent = 800;
	board->rent_1 = 4000;
	board->rent_2 = 9000;
	board->rent_3 = 25000;
	board->house_price = 10000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Car Park") * sizeof(char));
	board->block_id = 2;
	strcpy(board->name, "Car Park");
	board->price = 0;
	board->rent = 1500;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = tax;
	
	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc( (strlen("Fortune Card")+1) * sizeof(char));
	board->block_id = 3;
	strcpy(board->name, "Fortune Card");
	board->price = 0;
	board->rent = 0;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = fortune;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Tuzla") * sizeof(char));
	board->block_id = 4;
	strcpy(board->name, "Tuzla");
	board->price = 16500;
	board->rent = 850;
	board->rent_1 = 4250;
	board->rent_2 = 9500;
	board->rent_3 = 26000;
	board->house_price = 12000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Arnavutkoy") * sizeof(char));
	board->block_id = 5;
	strcpy(board->name, "Arnavutkoy");
	board->price = 17000;
	board->rent = 875;
	board->rent_1 = 4500;
	board->rent_2 = 10000;
	board->rent_3 = 28000;
	board->house_price = 1200;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Wait 2 Turn") * sizeof(char));
	board->block_id = 6;
	strcpy(board->name, "Wait 2 Turn");
	board->price = 0;
	board->rent = 2;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = punish;
	
	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Catalca") * sizeof(char));
	board->block_id = 7;
	strcpy(board->name, "Catalca");
	board->price = 20000;
	board->rent = 1000;
	board->rent_1 = 5000;
	board->rent_2 = 12000;
	board->rent_3 = 30000;
	board->house_price = 13000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Beykoz") * sizeof(char));
	board->block_id = 8;
	strcpy(board->name, "Beykoz");
	board->price = 23000;
	board->rent = 1100;
	board->rent_1 = 5500;
	board->rent_2 = 12500;
	board->rent_3 = 33000;
	board->house_price = 13000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc((strlen("Fortune Card")+1) * sizeof(char));
	board->block_id = 9;
	strcpy(board->name, "Fortune Card");
	board->price = 0;
	board->rent = 0;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = fortune;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Car Fix") * sizeof(char));
	board->block_id = 10;
	strcpy(board->name, "Car Fix");
	board->price = 0;
	board->rent = 1750;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = tax;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Maltepe") * sizeof(char));
	board->block_id = 11;
	strcpy(board->name, "Maltepe");
	board->price = 30000;
	board->rent = 1350;
	board->rent_1 = 7000;
	board->rent_2 = 15000;
	board->rent_3 = 40000;
	board->house_price = 15000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Bills") * sizeof(char));
	board->block_id = 12;
	strcpy(board->name, "Bills");
	board->price = 0;
	board->rent = 2000;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = tax;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Sisli") * sizeof(char));
	board->block_id = 13;
	strcpy(board->name, "Sisli");
	board->price = 33000;
	board->rent = 1500;
	board->rent_1 = 8000;
	board->rent_2 = 16000;
	board->rent_3 = 42000;
	board->house_price = 16000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Oil") * sizeof(char));
	board->block_id = 14;
	strcpy(board->name, "Oil");
	board->price = 0;
	board->rent = 2250;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = tax;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc((strlen("Fortune Card")+1) * sizeof(char));
	board->block_id = 15;
	strcpy(board->name, "Fortune Card");
	board->price = 0;
	board->rent = 0;
	board->rent_1 =0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = fortune;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Atasehir") * sizeof(char));
	board->block_id = 16;
	strcpy(board->name, "Atasehir");
	board->price = 35000;
	board->rent = 1600;
	board->rent_1 = 8500;
	board->rent_2 = 17000;
	board->rent_3 = 45000;
	board->house_price = 17000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Sariyer") * sizeof(char));
	board->block_id = 17;
	strcpy(board->name, "Sariyer");
	board->price = 40000;
	board->rent = 1750;
	board->rent_1 = 9500;
	board->rent_2 = 19000;
	board->rent_3 = 48000;
	board->house_price = 19000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Wait 1 Turn") * sizeof(char));
	board->block_id = 18;
	strcpy(board->name, "Wait 1 Turn");
	board->price = 0;
	board->rent = 1;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = punish;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Kadikoy") * sizeof(char));
	board->block_id = 19;
	strcpy(board->name, "Kadikoy");
	board->price = 43000;
	board->rent = 1900;
	board->rent_1 = 11000;
	board->rent_2 = 21500;
	board->rent_3 = 55000;
	board->house_price = 23000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Besiktas") * sizeof(char));
	board->block_id = 20;
	strcpy(board->name, "Besiktas");
	board->price = 60000;
	board->rent = 2500;
	board->rent_1 = 15000;
	board->rent_2 = 28000;
	board->rent_3 = 60000;
	board->house_price = 30000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc((strlen("Fortune Card")+1) * sizeof(char));
	board->block_id = 21;
	strcpy(board->name, "Fortune Card");
	board->price = 0;
	board->rent = 0;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = fortune;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Vocation") * sizeof(char));
	board->block_id = 22;
	strcpy(board->name, "Vocation");
	board->price = 0;
	board->rent = 5000;
	board->rent_1 = 0;
	board->rent_2 = 0;
	board->rent_3 = 0;
	board->house_price = 0;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = tax;

	board->next = (block *)malloc(sizeof(block));
	board = board->next;
	board->name = (char *)malloc(strlen("Bebek") * sizeof(char));
	board->block_id = 23;
	strcpy(board->name, "Bebek");
	board->price = 70000;
	board->rent = 3500;
	board->rent_1 = 20000;
	board->rent_2 = 35500;
	board->rent_3 = 65000;
	board->house_price = 35000;
	board->house_count = 0;
	board->owner.type = noone;
	board->type = property;

	board->next = NULL;
}
