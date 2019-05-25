#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int add(char *token);

int run(char *token);

int newProcess(char *path);

int main(int argc, char **argv)
{
	char welcome_msg[] = "Welcome!\nTo add type add n1 n2 n3 ... \
	\nTo open a new process type run name\nTo exit type bye";
	
	write(STDOUT_FILENO, welcome_msg, sizeof(welcome_msg)-1);
	
	int size_buf = 200, ret;
	char read_buf[size_buf], write_buf[20];
	char *token_ptr;
	
	do {
		//prompt symbol
		write(STDOUT_FILENO, "\n>", 2);
		
		//read input into buf
		ret = read(STDIN_FILENO, read_buf, size_buf);
		if(ret == -1) {
			perror("read");
		}
		
		//assigning null character at end (replacing enter)
		//so that buf can be safely used in string functions
		read_buf[ret - 1] = '\0';
		
		//delimitter = space
		token_ptr = strtok(read_buf, " ");
		
		if(token_ptr != NULL) {
			//if command is run
			if(strcmp(token_ptr, "run") == 0) {
				//to get the string after run command which should be the path
				token_ptr = strtok(NULL, " ");
				ret = run(token_ptr);
				if (ret > -1) {
					ret = sprintf(write_buf, "Child pid: %d", ret);
					write(STDOUT_FILENO, write_buf, ret);
				}
			}
			//if command is add
			else if(strcmp(token_ptr, "add") == 0) {
				token_ptr = strtok(NULL, " ");
				ret = sprintf(write_buf, "Sum: %d", add(token_ptr));
				write(STDOUT_FILENO, write_buf, ret);
			}
		}
		
	} while (strcmp(read_buf, "bye"));
}

int add(char *token) {
	int result = 0;
	while(token != NULL) {
		result += atoi(token);
		token = strtok(NULL, " ");
	}
	return result;
	
}

int run(char *token) {
	char path[200];
	int i = 0;
	while(token != NULL) {
		//so that only first "argument" after run is saved in path array
		//in future implementation, will handle args list to be used in exec
		if(i++ == 0) {
			strcpy(path, token);
		}
		
		token = strtok(NULL, " ");
	}
	//if strtok returned null after run was found
	if(i == 0) {
		char errMsg[] = "Not enough arguments";
		write(STDOUT_FILENO, errMsg, sizeof(errMsg) - 1);
		return -1;
	}
		
	return newProcess(path);
}

int newProcess(char *path) {
	int pid = fork();
	
	if(pid == 0) {
		int exec_ret = execlp(path, NULL);
		
		//terminate the child process if exec is unsuccessful
		exit(1);
	}
	
	if(pid == -1)
		perror("fork");
	
	return pid;
}
