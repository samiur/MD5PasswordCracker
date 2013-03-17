#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

char * convert_lalpha(long long,int,int);
char * convert(long long,int);
char alpha_num[96];
void loop(int mode, int temp, int i, int lim, int len, char * salt, char * testhash, pid_t send);

void signal_handler(int signum)
{
	exit(0);
}


struct timeval start;
struct timeval end;

main(int argc, char ** argv)
{
	int i = 0;
	int temp = 0;
	long long j = 0;
	long long k = 0;
	int cnt = 0;
	char salt[33];
	char testhash[128];	
	char * ans;
	char * name;
	FILE * fd;
	int mode = 0;
	if(argc != 3)
	{
		printf("Usage: crack <salt> <MAX length of passwords> %d\n",argc);
		exit(1);
	}
	printf("%s\n",argv[1]);
	strncpy(salt,argv[1],32);
	salt[33] = '\0'; 
	strncpy(testhash,salt,32);
	testhash[33] = '\0';
	pid_t pid;
	pid_t send;
	
	//Ordering the array of printable characters in terms of more important characters
	
	for(i = 0; i < 26; i++)
	{
		alpha_num[i] = 'a' + i;
		alpha_num[26+i] = 'A' + i;
	}
	for(i = 0; i < 10; i++)
	{
		alpha_num[52 +i] = '0' + i;
	}
	for(i = 0; i < 16; i++)
	{
		alpha_num[62+i] = ' ' + i;
	}
	for(i = 0; i < 7; i++)
	{	
		alpha_num[78+i] = ':' + i;
	}
	for(i = 0; i < 6; i++)
	{
		alpha_num[85+i] = '[' + i;
	}
	for(i = 0; i < 5; i++)
	{
		alpha_num[91+i] = '{' + i;
	}
	if(gettimeofday(&start,NULL)==-1)
	{
		perror("Time error");
		exit(1);
	}
	printf("Forking off second thread.\n");
	pid = fork();
	if(pid==-1)
	{
		perror("fork error");
		exit(1);
	}		
	if(pid==0)
	{
		mode = 0;
		send = getppid();
		name = "Thread 1";
	}
	else
	{
		mode = 1;
		send = pid;
		name = "Thread 2";
	}
	signal (SIGUSR1, signal_handler);
	for(i = 1; i <= atoi(argv[2]); i++)
	{
		temp = pow(62,i);	//First 62 characters in my array, meaning alphanumeric characters
		loop(mode, temp, i, 62, 32, salt, testhash, send);
		printf("%s: Got past length %d alpha numeric passwords.\n",name,i);
	}
	printf("Did not find answer with alpha numeric passwords of length %s.\nTrying all possible solutions:\n",argv[2]);
	for(i = 1; i < 10 ; i++)
	{
		temp = (long long)pow(96,i);
		loop(mode, temp, i, 96, 32, salt, testhash, send);
		printf("%s: Got past length %d printable-character passwords.\n",name,i);
	}
	printf("Did not find answer.\n");
	return 0;
}

char * convert(long long num, int length)
{
	static char buffer[128];
	long long temp;
	int i = 0;
	for(temp = num; i < length; temp = temp/96)
	{
		//printf("%llu\n",temp);
		buffer[i] = (32 + temp%96);
		i++;	
	}
	buffer[length] = '\0';
	return buffer;
}

char * convert_lalpha(long long num,int length, int base)
{
	static char buffer[128];
	long long temp;
	int i = 0;
	for(temp = num; i < length; temp = temp/base)
	{
		buffer[length - i - 1] = alpha_num[temp%base];
		i++;	
	}
	buffer[i] = '\0';
	return buffer;
}

void loop(int mode, int temp, int i, int lim, int len, char * salt, char * testhash, pid_t send)
{
	int j = 0;
	char * ans;
	int done = 0;
	double time;
	if(mode == 0)
	{
		for(j=0; j<temp/2; j++)
		{
			ans = convert_lalpha(j,i,lim);
			if(strncmp(crypt(ans,salt),testhash,len)==0)
			{
				done = 1;
				break;
			}	
		}
	}
	if(mode == 1)
	{
		for(j=temp-1; j>=temp/2; j--)
		{
			ans = convert_lalpha(j,i,lim);
			if(strncmp(crypt(ans,salt),testhash,len)==0)
			{
				done = 1;
				break;
			}	
		}
	}
	if(done)
	{
		if(gettimeofday(&end,NULL)==-1)
		{
			perror("Time error");
			exit(1);
		}
		time = (double)end.tv_sec - (double)start.tv_sec + ((double)end.tv_usec - (double)start.tv_usec)*1e-6 ;
		printf("Found answer: %s\nTime taken: %.2fs\n",ans,time);
		kill(send,SIGUSR1);
		exit(0);
	}
}
