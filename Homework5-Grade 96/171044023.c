#include<stdio.h>
#include<stdlib.h>

#define DICT_SIZE 15
#define WORD_LEN 10
#define LINE_LEN 18

void print_board(char board[DICT_SIZE][DICT_SIZE]); //print my game board
void word_placement(char *dict[DICT_SIZE], int coord[DICT_SIZE][4], char board[DICT_SIZE][DICT_SIZE]); //word placement
void find_words(char *dict[DICT_SIZE], int coord[DICT_SIZE][4], char board[DICT_SIZE][DICT_SIZE]); //control words entered
void find_direction(char word[WORD_LEN], int c1, int c2, char board [DICT_SIZE][DICT_SIZE]);

int get_line_size(char *line) {
	char *ch_iter = line; // so as not to lose beginning of line
	int counter = 0;
	// go until you see new line or null char
	while(*ch_iter != '\n' && *ch_iter != '\0') {
		ch_iter++; // next char
		counter++; // increment counter
	}
	
	return counter;
}

void copy_string(char *source, char *destination) {
	// get iterators over original pointers
	char *src_iter = source;
	char *dst_iter = destination;
	// until null char
	while (*src_iter != '\0') {
		// copy pointers
		*dst_iter = *src_iter;
		// advance to next char
		src_iter++;
		dst_iter++;
   }
   // terminate string
   *dst_iter = '\0';
}

void remove_newline(char *line) {
	char *ch_iter = line;
	// go until you see new line
	while(*ch_iter != '\n' && *ch_iter != 13) {
		ch_iter++; // next char
	}
	*ch_iter = '\0'; // overwrite new line

}

void print_dictionary(char *dict[]) {
	int i;
	for(i = 0 ; i < DICT_SIZE ; i++) {
		printf("%s\n", dict[i]);
	}
}

void print_coord(int coord[DICT_SIZE][4]) {
	int i, j;
	for(i = 0 ; i < DICT_SIZE ; i++) {
		for(j = 0 ; j < 4 ; j++) {
			printf("%d", coord[i][j]);
		}
		printf("\n");
	}
}

int main(){
	char *dict[DICT_SIZE];
	int coord[DICT_SIZE][4];    
	char line[LINE_LEN];
	FILE *fp = fopen("word_hunter.dat", "r");
	
	int line_counter = 0;
	int dict_counter = 0;
	int i, j;
	char board[DICT_SIZE][DICT_SIZE]; //my game board
	

	srand(time(NULL));

	while(fgets(line, LINE_LEN, fp) != NULL) {
		if(line_counter%5 == 0) {
			dict[dict_counter] = (char*) malloc(sizeof(char) * get_line_size(line));
			remove_newline(line);
			copy_string(line, dict[dict_counter]);
		} else if (line_counter%5 == 1){
			coord[dict_counter][0] = atoi(line);
		} else if (line_counter%5 == 2){			
			coord[dict_counter][1] = atoi(line);		
		} else if (line_counter%5 == 3){
			coord[dict_counter][2] = atoi(line);
		} else if (line_counter%5 == 4){
			coord[dict_counter][3] = atoi(line);
			dict_counter++;
		}
		line_counter++;
	}
	
	fclose(fp);
	//print_dictionary(dict);
	//print_coord(coord);
	
	// WRITE HERE...

	for(i=0; i<DICT_SIZE; i++){
		for(j=0; j<DICT_SIZE; j++){
			board[i][j] = ' ';
		}
	}
	//print_board(board);
	word_placement(dict, coord, board);

	find_words(dict, coord, board);	
	
	return 0;
}

void print_board(char board[DICT_SIZE][DICT_SIZE]){
	int i, j;

	for(i=0; i<DICT_SIZE; i++){
		for(j=0; j<DICT_SIZE; j++){
			if(board[i][j] == ' '){
				board[i][j] = rand() % 25 + 97;
				printf("%c ", board[i][j]);
			}
			else	printf("%c ", board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void word_placement(char *dict[DICT_SIZE], int coord[DICT_SIZE][4], char board[DICT_SIZE][DICT_SIZE]) {
	int i, j, lenght;
	
	for(i=0; i<DICT_SIZE; i++){
		lenght = get_line_size(dict[i]);
		//printf("%d ", lenght);
		//printf("\n");
		for(j=0; j<lenght; j++){
			if(coord[i][0] == coord[i][2]){ //x eşit
				if(coord[i][1] == coord[i][3]){ //y eşit
					board [coord[i][0]] [coord[i][1]] = dict[i][j];
				} else if(coord[i][1] < coord[i][3]){ //y küçük
					board [coord[i][0]] [coord[i][1]+j] = dict[i][j];
				} else if(coord[i][1] > coord[i][3]){ //y büyük
					board [coord[i][0]] [coord[i][1]-j] = dict[i][j];
				}
			}
			else if(coord[i][0] < coord[i][2]){ //x küçük
				if(coord[i][1] == coord[i][3]){ //y eşit
					board [coord[i][0]+j] [coord[i][1]] = dict[i][j];
				} else if(coord[i][1] < coord[i][3]){ //y küçük
					board [coord[i][0]+j] [coord[i][1]+j] = dict[i][j];
				} else if(coord[i][1] > coord[i][3]){ //y büyük
					board [coord[i][0]+j] [coord[i][1]-j] = dict[i][j];
				} 
			}
			else if(coord[i][0] > coord[i][2]){ //x büyük
				if(coord[i][1] == coord[i][3]){ //y eşit
					board [coord[i][0]-j] [coord[i][1]] = dict[i][j];
				} else if(coord[i][1] < coord[i][3]){ //y küçük
					board [coord[i][0]-j] [coord[i][1]+j] = dict[i][j];
				} else if(coord[i][1] > coord[i][3]){ //y büyük
					board [coord[i][0]-j] [coord[i][1]-j] = dict[i][j];
				} 
			}
		}
	}
	print_board(board);
}

void find_words(char *dict[DICT_SIZE], int coord[DICT_SIZE][4], char board[DICT_SIZE][DICT_SIZE]){
	int i, control_word, lenght, counter = 0;
	char word[WORD_LEN]; // entered word
	int c1, c2; //entered coordinates
	
	do{
	printf("Please enter the word you found and start or end coordinates of the word.\n");
	scanf("%s %d %d", word, &c1, &c2);
	
	lenght = get_line_size(word);
	control_word = compare(dict, word);

	if(control_word != 0 && control_word != -1){

		if(board[c1][c2] == word[0] || board[c1][c2] == word[lenght-1]){
			find_direction(word, c1, c2, board);	
			counter++;
		}	
	}
	}while(counter != 15);	
	
}

int compare(char *dict[DICT_SIZE], char word[WORD_LEN]){
	int i, j, temp, lenght; 
	char exit[WORD_LEN] = "exit";

	lenght = get_line_size(word);

	for(i=0; i<DICT_SIZE && temp != 1; i++){
		for(j=0; j<lenght; j++){

			if(word[j] == dict[i][j])
				temp = 1;
			else if(word[j] == exit[j])
				temp = -1;
			else	
				temp = 0;
		}
		
	}

	return temp;
}

void find_direction(char word[WORD_LEN], int c1, int c2, char board [DICT_SIZE][DICT_SIZE]){
	int k, i, j, l, lenght;
	char diff;

	diff = 'a' - 'A';
	lenght = get_line_size(word);

	k = 1;
	if(board[c1][c2] == word[0]){ //baslangıc aynı
		board[c1][c2] -= diff;
			if(board[c1-k][c2] == word[k]){ //north

				for(k=2; k<lenght && board[c1-k][c2] == word[k]; k++)
					board[c1-k-1][c2] -= diff;

				for(i=2; i<lenght; i++){
					if(board[c1-i][c2] == word[i])				
						board[c1-i][c2] -= diff;
				}

			}
			if(board[c1-k][c2+k] == word[k]){ //nort_east

				for(k=2; k<lenght && board[c1-k][c2+k] == word[k]; k++)
					board[c1-k-1][c2+k-1] -= diff;

				for(i=2; i<lenght; i++){
					if(board[c1-i][c2+i] == word[i])				
						board[c1-i][c2+i] -= diff;
				}

			}
			if(board[c1][c2+k] == word[k]){ //east

				for(k=2; k<lenght && board[c1][c2+k] == word[k]; k++)
					board[c1][c2+k-1] -= diff;

				for(i=2; i<lenght; i++){
					if(board[c1][c2+i] == word[i])				
						board[c1][c2+i] -= diff;
				}

			}
			if(board[c1+k][c2+k] == word[k]){ //south_east

				for(k=2; k<lenght && board[c1+k][c2+k] == word[k]; k++)
					board[c1+k-1][c2+k-1] -= diff;

				for(i=2; i<lenght; i++){
					if(board[c1+i][c2+i] == word[i])				
						board[c1+i][c2+i] -= diff;
				}

			}
			if(board[c1+k][c2] == word[k]){ //south

				for(k=2; k<lenght && board[c1+k][c2] == word[k]; k++)
					board[c1+k-1][c2] -= diff;

				for(i=2; i<lenght; i++){ 
					if(board[c1+i][c2] == word[i])
					board[c1+i][c2] -= diff;
				}

				
			}
			if(board[c1+k][c2-k] == word[k]){ //south_west

				for(k=2; k<lenght && board[c1+k][c2-k] == word[k]; k++)
					board[c1+k-1][c2-k-1] -= diff;

				for(i=2; i<lenght; i++){
					if(board[c1+i][c2-i] == word[i])				
						board[c1+i][c2-i] -= diff;
				}

			}
			if(board[c1][c2-k] == word[k]){ //west

				for(k=2; k<lenght && board[c1][c2-k] == word[k]; k++)
					board[c1][c2-k-1] -= diff;

				for(i=2; i<lenght; i++){
					if(board[c1][c2-i] == word[i])				
						board[c1][c2-i] -= diff;
				}

			}
			if(board[c1-k][c2-k] == word[k]){ //north_west

				for(k=2; k<lenght && board[c1-k][c2-k] == word[k]; k++)
					board[c1-k-1][c2-k-1] -= diff;

				for(i=2; i<lenght; i++){
					if(board[c1-i][c2-i] == word[i])				
						board[c1-i][c2-i] -= diff;
				}

			}
	}

	else if(board[c1][c2] == word[lenght-1]){ //bitis aynı
		j = 1;
		board[c1][c2] -= diff;
			if(board[c1-1][c2] == word[lenght-2]){ //north
				k = 2;
				for(l=lenght-3; l>=0; l--){
					if(board[c1-k][c2] == word[l] && board[c1-k-1][c2] == word[l-1]){
						for(i=lenght-2; i>=0; i--){			
							board[c1-j][c2] -= diff;
							j++;
						}
					}
					k++;
				}
			}
			if(board[c1-1][c2+1] == word[lenght-2]){ //nort_east
				k = 2;
				for(l=lenght-3; l>=0; l--){
					if(board[c1-k][c2+k] == word[l] && board[c1-k-1][c2+k+1] == word[l-1]){
						for(i=lenght-2; i>=0; i--){
							board[c1-j][c2+j] -= diff;
							j++;
						}
					}
					k++;
				}
			}
			if(board[c1][c2+1] == word[lenght-2]){ //east
				k = 2;
				for(l=lenght-3; l>=0; l--){
					if(board[c1][c2+k] == word[l] && board[c1][c2+k+1] == word[l-1]){
						for(i=lenght-2; i>=0; i--){
							board[c1][c2+j] -= diff;
							j++;
						}
					}
					k++;
				}
			}
			if(board[c1+1][c2+1] == word[lenght-2]){ //south_east
				k = 2;
				for(l=lenght-3; l>=0; l--){
					if(board[c1+k][c2+k] == word[l] && board[c1+k+1][c2+k+1] == word[l-1]){
						for(i=lenght-2; i>=0; i--){
							board[c1+j][c2+j] -= diff;
							j++;
						}
					}
					k++;
				}
			}
			if(board[c1+1][c2] == word[lenght-2]){ //south
				k = 2;
				for(l=lenght-3; l>=0; l--){
					if(board[c1+k][c2] == word[l] && board[c1+k+1][c2] == word[l-1]){
						for(i=lenght-2; i>=0; i--){
							board[c1+j][c2] -= diff;
							j++;
						}
					}
					k++;
				}
			}
			if(board[c1+1][c2-1] == word[lenght-2]){ //south_west
				k = 2;
				for(l=lenght-3; l>=0; l--){
					if(board[c1+k][c2-k] == word[l] && board[c1+k+1][c2-k-1] == word[l-1]){
						for(i=lenght-2; i>=0; i--){
							board[c1+j][c2-j] -= diff;
							j++;
						}
					}
					k++;
				}
			}
			if(board[c1][c2-1] == word[lenght-2]){ //west
				k = 2;
				for(l = lenght - 3; l>=0; l--){
					if(board[c1][c2-k] == word[l] && board[c1][c2-k-1] == word[l-1]){
						for(i=lenght-2; i>=0; i--){
							board[c1][c2-j] -= diff;
							j++;
						}
					}
					k++;
				}
			}
			if(board[c1-1][c2-1] == word[lenght-2]){ //north_west
				k = 2;
				for(l=lenght-3; l>=0; l--){
					if(board[c1-k][c2-k] == word[l] && board[c1-k-1][c2-k-1] == word[l-1]){
						for(i=lenght-2; i>=0; i--){
							board[c1-j][c2-j] -= diff;
							j++;
						}
					}
					k++;
				}
			}
	}
	print_board(board);
}
