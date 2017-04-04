#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>




	int pid[4]={0};
	int dzialanie=1;
	
void zakoncz(int sygnal)
{
	printf("zakoncz %d\n", getpid());
	fflush(stdout);
	kill(0,sygnal);
	
		if(getpid()==pid[0])
	{
		remove("plik.txt");
	}
	exit(0);
}
void wstrzymaj(int sygnal)
{
	if(dzialanie==1)
	{
	printf("wstrzymaj %d\n", getpid());
	fflush(stdout);
	
	dzialanie=0;

	kill(0, sygnal);
	}
	
}
void wznow(int sygnal)
{
	if(dzialanie==0)
	{
	printf("wznow %d\n", getpid());
	fflush(stdout);
	
	dzialanie=1;
	
	kill(0, sygnal);
	}
	
}
FILE *pobierzplik()
{
	char nazwapliku[20];
	FILE *plikwej;	
	printf("\npodaj nazwę pliku:");
	scanf("%s", &nazwapliku);
	
	plikwej=fopen(nazwapliku, "r");
	if(plikwej==NULL)
	{
		printf("Blad otwarcia pliku wejsciowego wpisz inny plik\n");
		plikwej=pobierzplik();
	}
	return plikwej;
}

int main()
{
	char bufor[200];
	char bufor1[200];
	int czym=1;
	
	FILE *plik1;
	FILE *plikwej;

	int o=0;
	char o1[10];
	int pipe1[2];
	pipe(pipe1);
	int do15=0;
	
	pid[0]=getpid();
	


	if(fork()==0) //proces 1
	{	
		pid[1]=getpid();
		printf("\nproces 1 pid %d\n", pid[1]);
		fflush(stdout);
		
			signal(4,zakoncz);
			signal(5, wstrzymaj);
			signal(6, wznow);
			
		sleep(1);
		printf("Obsluga sygnalow:\n");
		printf("S1 - sygnal zakonczenia: 4 SIGILL\n");
		printf("S2 - sygnal wstrzymania: 5 SIGTRAP\n");
		printf("S3 - sygnal zakonczenia: 6 SIGABORT\n\n");
		
		printf("jakim sposobem chcesz podać dane?\n");
		printf("1. Ręcznie z konsoli\n");
		printf("2. Z pliku \n");
		printf("3. /dev/urandom \n");
		fflush(stdout);
		
		scanf("%d", &czym);
		
		getchar();
		fflush(stdin);

		
			
			
				if(czym==2)
				{
					plikwej=pobierzplik();
				}
				if(czym==3)
				{
					plikwej=fopen("/dev/urandom", "r");
						if(plikwej==NULL)
						{
							perror("Blad otwarcia /dev/urandom\n");
						}		
				}

			while(1)
			{
			while(dzialanie)
			{
				
				
				if(czym==2||czym==3)
				{	
					int i;
					for(i=0;i<200; i++)
						bufor[i]=0;
						
					if(fgets(bufor, 200, plikwej)==0)
					{
						printf("koniec pliku, nic wiecej do pracy");
						fflush(stdout);
						while(1)
						{
						}
					}
			
				}
				else
				{
						printf("\nwpisz tekst:");
						fflush(stdout);
						fgets(bufor, 200, stdin);
				}
				
				
			plik1=fopen("/home/student/plik.txt", "w+");
			if(plik1==NULL)
			{
				perror("Blad otwarcia plik do zapisu\n");
			}
			
			int i=0;
			while(i<200)
			{
					
				fprintf(plik1,"%c", bufor[i]);
				if(bufor[i]=='\n')
				{
					break;
				}
				i++;
			}
			
			fclose(plik1);
			
		while(access("/home/student/plik.txt",F_OK)==0)
		{
		}
		


			}
			}	
	}
	else 
	{
			if(fork()==0)// proces 2
			{
				pid[2]=getpid();
				printf("proces 2 pid %d\n", pid[2]);
				fflush(stdout);
				
						signal(4,zakoncz);
						signal(5, wstrzymaj);
						signal(6, wznow);
						
				close(pipe1[0]);
				
				while(1)
				{
				while(dzialanie)
				{
					
					plik1=fopen("/home/student/plik.txt", "r");
					
					int i;
					for(i=0;i<200; i++)
					{
						bufor1[i]=0;
					}
					
					if(plik1!=NULL)
					{	
					fgets(bufor1, 200, plik1);

					int n=0;
					
					while(bufor1[n]!='\n')
					{	
						if(n<200)
						{
							if(bufor1[n]=='o')
							{
								o++;
							}
							n++;
						}
						else
						{
							break;
						}
					}
					if(bufor1[n]=='\n')
					{
					sprintf(o1,"%i",o);// int na chara(stringa)
					
					
					write(pipe1[1], o1, 10);
					o=0;
					}
					fclose(plik1);
					remove("plik.txt");
					}			
				}
				}

			}
		else
		{
			if(fork()==0)// proces 3
			{
				pid[3]=getpid();
				printf("proces 3 pid %d\n", pid[3]);
				fflush(stdout);
				
					signal(4,zakoncz);
					signal(5, wstrzymaj);
					signal(6, wznow);
					
				close(pipe1[1]);
				
				while(1)
				{	
				while(dzialanie)
				{
					
					
					read(pipe1[0], o1, 10);
					
					if(do15<14)
					{
						
					fprintf(stderr,"%s ", o1);
					do15++;
					}
					else
					{
						fprintf(stderr,"%s \n", o1);
						do15=0;
					}
					
				}
				}

			}
			else 
			{
				
				signal(4,zakoncz);
				signal(5, SIG_IGN);
				signal(6, SIG_IGN);
				printf("proces maciezysty pid %d\n\n", pid[0]);
				
				while(1)
				{}
			}
		}

	}
	return 0;
}
