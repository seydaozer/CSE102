#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct stack{
	struct stack *next;
	int data;
} stack;

typedef struct queue{
	struct queue *next;
	int data;
} queue;

typedef struct bst{
	struct bst *left, *right;
	int data;
} bst;

void fill_structures(stack ** stack_, queue ** queue_, bst ** bst_, int data[20]);
void ordered_print(stack * stack_, queue * queue_, bst * bst_);
void search(stack * stack_, queue * queue_, bst * bst_, int val_to_search);
void special_traverse(stack * stack_, queue * queue_, bst * bst_);

void create_stack(stack **stack_, int data);
void create_queue(queue **queue_, int data);
bst * create_bst(bst ** bst_, int data);
bst * newNode(int data);
void print_bst(bst *bst_);
void sort_stack(stack *stack_);
void sort_queue(queue *queue_);
bst * search_bst(bst *bst_, int data, int *counter);

int main(){

	int data[20]={5, 2, 7, 8, 11, 3, 21, 7, 6, 15, 19, 35, 24, 1, 8, 12, 17, 8, 23, 4};

	bst *bst_;
	queue *queue_;
	stack *stack_;

	fill_structures(&stack_, &queue_, &bst_, data);
	ordered_print(stack_, queue_, bst_);
	search(stack_, queue_, bst_, 12);
	special_traverse(stack_, queue_, bst_);


	return 0;
}

void special_traverse(stack * stack_, queue * queue_, bst * bst_){ // part4
	double begin, end, q_time, s_time, b_time;
	int counter, max, min, c1, c2;
	stack *s_temp, *s_last = NULL;
	queue *q_temp , *q_last = NULL;
	bst *b_temp, *br_last = NULL, *bl_last = NULL;

	begin = clock();
	counter = 0, c1 = 0;
	printf("Stack:\n");
	while(counter < 20){
		s_temp = stack_;
		if(counter % 2 == 0){ // max
			c2 = c1; // I used it to move on to the next one.
			while(c2 > 0){
				s_temp = s_temp->next;
				c2--;
			}
			max = s_temp->data;
			while(s_temp != NULL){
				if(s_temp->data > max)
					max = s_temp->data;
				s_temp = s_temp->next;
			}
			c1++;
			printf("%d ", max);
		}
		else if(counter % 2 == 1){ // min
			min = s_temp->data;
			while(s_temp->next != s_last){
				if(s_temp->data < min)
					min = s_temp->data;
				s_temp = s_temp->next;
			}
			s_last = s_temp; //it goes before the end.
			printf("%d ", min);
		}
		counter++;
	}
	end = clock();
	s_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;

	begin = clock();
	counter = 0, c1 = 0;
	printf("\nQueue:\n");
	while(counter < 20){
		q_temp = queue_;
		if(counter % 2 == 0){ // max
			c2 = c1; // I used it to move on to the next one.
			while(c2 > 0){
				q_temp = q_temp->next;
				c2--;
			}
			max = q_temp->data;
			while(q_temp != NULL){
				if(q_temp->data > max)
					max = q_temp->data;
				q_temp = q_temp->next;
			}
			c1++;
			printf("%d ", max);
		}
		else if(counter % 2 == 1){ // min
			min = q_temp->data;
			while(q_temp->next != q_last){
				if(q_temp->data < min)
					min = q_temp->data;
				q_temp = q_temp->next;
			}
			q_last = q_temp; // it goes before the end.
			printf("%d ", min);
		}
		counter++;
	}
	end = clock();
	s_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;

	printf("\n-------------------------------------\n");
	printf("Structures\tStack\tQueue\tBST\n");
	printf("Exec. Time\t%.3f\t%.3f", s_time, q_time);
	printf("\n-------------------------------------\n");

}

void search(stack * stack_, queue * queue_, bst * bst_, int val_to_search){ // part3
	double begin, end, q_time, s_time, b_time;
	int counter = 1;
	bst *search;

	printf("value to search: %d\n", val_to_search);
	begin = clock();
	printf("Stack: \n");
	while(stack_ != NULL){
		if(val_to_search == stack_->data){
			printf("1 result founded on %d.step\n", counter);
		}
		stack_ = stack_->next;
		counter++;
	}
	end = clock();
	s_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;

	counter = 1;
	begin = clock();
	printf("Queue: \n");
	while(queue_ != NULL){
		if(val_to_search == queue_->data){
			printf("1 result founded on %d.step\n", counter);
		}
		queue_ = queue_->next;
		counter++;
	}
	end = clock();
	q_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;

	counter = 0;
	begin = clock();
	printf("BST: \n");
	search = search_bst(bst_, val_to_search, &counter);
	printf("1 result founded on %d.step\n", counter);
	end = clock();
	b_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;

	printf("-------------------------------------\n");
	printf("Structures\tStack\tQueue\tBST\n");
	printf("Exec. Time\t%.3f\t%.3f\t%.3f", s_time, q_time, b_time);
	printf("\n-------------------------------------\n");

}

bst * search_bst(bst *bst_, int data, int *counter){

	if(bst_ == NULL)
		return;

	*counter = *counter + 1;

	if(data == bst_->data)
		return bst_;

	else if(data < bst_->data)
		search_bst(bst_->left, data, counter);

	else if(data > bst_->data)
		search_bst(bst_->right, data, counter);

	return NULL;
}

void ordered_print(stack * stack_, queue * queue_, bst * bst_){ // part2
	double begin, end, q_time, s_time, b_time;
	bst *b_temp = bst_;
	queue *q_temp = queue_;
	stack *s_temp = stack_;

	begin = clock();
	sort_stack(stack_);
	printf("Stack: \n");
	while(s_temp != NULL){
		printf("%d ", s_temp->data);
		s_temp = s_temp->next;
	}
	end = clock();
	s_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;

	begin = clock();
	sort_queue(queue_);
	printf("\nQueue: \n");
	while(q_temp != NULL){
		printf("%d ", q_temp->data);
		q_temp = q_temp->next;
	}
	end = clock();
	q_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;

	begin = clock();
	printf("\nBST: \n");
	print_bst(b_temp);
	end = clock();
	b_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;

	printf("\n-------------------------------------\n");
	printf("Structures\tStack\tQueue\tBST\n");
	printf("Exec. Time\t%.3f\t%.3f\t%.3f", s_time, q_time, b_time);
	printf("\n-------------------------------------\n\n");

}

void sort_stack(stack *stack_){
	stack *temp, *last;
	int counter = 1, data;

	temp = stack_;
	last = NULL;

	while(temp->next != NULL){
		temp = temp->next;
		counter++; //number of elements
	}

	while(counter > 0){
		temp = stack_; // temp should always point to the stack's head.
		while(temp->next != last){
			if(temp->data < temp->next->data){
				data= temp->data;
				temp->data = temp->next->data;
				temp->next->data = data;
			}
			temp = temp->next;
		}
		last = temp; // the maximum value goes to end each time. so I assign this variable to the last element.
		counter--;
	}

}

void sort_queue(queue *queue_){
	queue *temp, *last;
	int counter = 1, data;

	temp = queue_;
	last = NULL;

	while(temp->next != NULL){
		temp = temp->next;
		counter++; //number of elements
	}

	while(counter > 0){
		temp = queue_; // temp should always point to the queue's head.
		while(temp->next != last){
			if(temp->data < temp->next->data){
				data= temp->data;
				temp->data = temp->next->data;
				temp->next->data = data;
			}
			temp = temp->next;
		}
		last = temp; // the maximum value goes to end each time. so I assign this variable to the last element.
		counter--;
	}

}
void fill_structures(stack ** stack_, queue ** queue_, bst ** bst_, int data[20]){ // part1
	int i;
	double begin, end, q_time, s_time, b_time;
	(*stack_) = NULL;
	(*queue_) = NULL;
	(*bst_) = NULL;

	//stack
	begin = clock();
	for(i=0; i<20; i++)	create_stack(stack_, data[i]);
	end = clock();
	s_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;

	//queue
	begin = clock();
	for(i=0; i<20; i++)	create_queue(queue_, data[i]);
	end = clock();
	q_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;

	//bst
	begin = clock();
	for(i=0; i<20; i++)	*bst_ = create_bst(bst_, data[i]);
	end = clock();
	b_time = (double)(end-begin)/CLOCKS_PER_SEC*1000;
	
	printf("\n-------------------------------------\n");
	printf("Structures\tStack\tQueue\tBST\n");
	printf("Exec. Time\t%.3f\t%.3f\t%.3f", s_time, q_time, b_time);
	printf("\n-------------------------------------\n\n");

}

void create_stack(stack **stack_, int data){
	stack *news;

	news = (stack *)malloc(sizeof(stack));
	news->data = data;
	news->next = (*stack_);
	(*stack_) = news;

}

void create_queue(queue **queue_, int data){
	queue *newq;

	newq = (queue *)malloc(sizeof(queue));
	newq->data = data;
	newq->next = (*queue_);
	(*queue_) = newq;

}

bst * create_bst(bst ** bst_, int data){

	if((*bst_) == NULL)	return newNode(data);
	
	if(data < (*bst_)->data)
		(*bst_)->left = create_bst(&((*bst_)->left), data);

	else if(data > (*bst_)->data)
		(*bst_)->right = create_bst(&((*bst_)->right), data);
	
	return (*bst_);
}

bst * newNode(int data){
	bst *temp;

	temp = (bst*)malloc(sizeof(bst)); 
	temp->data = data; 
    	temp->left = temp->right = NULL; 

    	return temp; 
}

void print_bst(bst *bst_){
	if(bst_ != NULL){

		print_bst(bst_->right);
		printf("%d ", bst_->data);
		print_bst(bst_->left);
	
	}
}
