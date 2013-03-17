#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

char alpha_num[62];
char * random_string(int,int);
void test(char * pass, char * md5_salt, int guess);

main(int argc, char ** argv)
{
	char * salt;
	char * pass;
	char md5_salt[42] = "$1$";
	int i = 0;
	signal (SIGUSR1, SIG_IGN);
	for(i = 0; i < 26; i++)
	{
		alpha_num[i] = 'a' + i;
		alpha_num[26+i] = 'A' + i;
	}
	for(i = 0; i < 10; i++)
	{
		alpha_num[52 +i] = '0' + i;
	}
	srand(time(NULL));
	salt = random_string(2,rand()%16);
	strcat(md5_salt, salt);
	strcat(md5_salt,"$");
	strcat(md5_salt, "AAAAAAAAAAAAAAAAAAAAAA");
	pass = random_string(2,3);
	test(pass,md5_salt,4);
	salt = random_string(2,rand()%16);
	strcat(md5_salt, salt);
	strcat(md5_salt,"$");
	strcat(md5_salt, "AAAAAAAAAAAAAAAAAAAAAA");
	pass = random_string(3,3);
	test(pass,md5_salt,4);
	salt = random_string(2,rand()%16);
	strcat(md5_salt, salt);
	strcat(md5_salt,"$");
	strcat(md5_salt, "AAAAAAAAAAAAAAAAAAAAAA");
	pass = random_string(3,4);
	test(pass,md5_salt,4);
	return 0;	
}

void test(char * pass, char * md5_salt, int guess)
{
	char *args[4];
	char num[1];
	pid_t pid;
	num[0] = '0' + guess;
	printf("Password is: %s\nTesting cracker:\n",pass);
	args[0] = "./crack";
	args[1] = crypt(pass,md5_salt);
	args[2] = num;
	args[3] = '\0';
	pid = fork();
	if(pid == -1)
	{
		perror("fork error");
		exit(1);
	}
	if(pid == 0)
	{
		if(execvp("./crack",args)==-1)
			perror("execve error");
	}
	else
	{
		wait(0);
	}
	
}

char * random_string(int c, int length)
{
	static char ret[128];
	int i = 0;
	
	
	if(c==1)
	{
		for(i = 0; i<length; i++)
		{
			ret[i] = alpha_num[rand()%26];
		}
	}
	else if(c==2)
	{
		for(i = 0; i<length; i++)
		{
			ret[i] = alpha_num[rand()%52];
		}
	}
	else if(c==3)
	{
		for(i = 0; i<length; i++)
		{
			ret[i] = alpha_num[rand()%62];
		}
	}
	else if(c==4)
	{
		for(i = 0; i<length; i++)
		{
			ret[i] = (char)(rand()%96 + 32);
		}
	}
	ret[length] = '\0';
	return ret;	
}
