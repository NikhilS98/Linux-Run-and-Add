#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

//Resizabale array that stores the numbers once they have been read i.e
//after enter is pressed. The elements are then added when input is a ";" 
typedef struct {
  int *array;
  size_t used;
  size_t size;
} Array;

void initArray(Array *a, size_t initialSize) {
  a->array = (int *)malloc(initialSize * sizeof(int));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Array *a, int element) {
  // a->used is the number of used entries, because a->array[a->used++] 
  // updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (int *)realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}

int main(int argc, char **argv)
{
	char welcome_msg[] = "Welcome!\nTo add type n1 n2 n3; ...";
	
	write(STDOUT_FILENO, welcome_msg, sizeof(welcome_msg)-1);
	
	int sum, read_ret = 0, sprintf_ret;
	
	Array num_list;
	int size = 50, last_index = 0;
	initArray(&num_list, size);
	
	char read_buff[size], write_buff[15];
	
	do {
		//prompt symbol
		write(STDOUT_FILENO, "\n>", 2);
		
		//input from user
		read_ret = read(STDIN_FILENO, read_buff, size - 1);
		read_buff[read_ret] = '\0';
		
		char *token = strtok(read_buff, " \n");
		
		while(token != NULL) {
			if(strcmp(token, "bye") == 0) {
				exit(0);
			}
			
			//check if semi colon has been inputed
			int *has_colon = strstr(token, ";");
			
			if(has_colon == NULL) {
				insertArray(&num_list, atoi(token));
			}
			else {
				//don't add semi colon in num_list when for example numbers
				//are written in one line and semi colon in the next line
				if(strcmp(token, ";") != 0) {
					insertArray(&num_list, atoi(token));
				}
				sum = 0;
				
				/*
					last_index indicates the index from which to start adding.
					since num_list is never freed and keeps on growing as numbers
					are inserted, maintaining index is required to not add numbers
					before the last ";"
				*/
				while (last_index < num_list.used) {
					sum += num_list.array[last_index++];
				}
				
				sprintf_ret = sprintf(write_buff, "Sum: %d\n", sum);
				write(STDOUT_FILENO, write_buff, sprintf_ret);
			}
			
			token = strtok(NULL, " \n");
		}
		
	} while (1);
	
	return 0;
}