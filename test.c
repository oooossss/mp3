#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

void test_create(void);
void test_close(void);
void test_createwait(void);
void test_sig(void);
void test_info(void);
void test_chown(void);
void test_chmod(void);
void test_stat(void);

int main()
{
	while(1){
	int number;
	printf("type in your test number: ");
	scanf("%d",&number);
	
	switch (number) {
		case 1:  test_create(); break;
		case 2:  test_close(); break;
		case 3:  test_createwait();break;
		case 4:  test_sig();break;
		case 5:  test_info();break;
		case 6:  test_chown();break;
		case 7:  test_chmod();break;
		case 8:  test_stat();break;
		default: printf("Wrong input test number");
	}
  }
}

void test_create()
{
	int number;
	printf("\nThe number of event you want to create : ");
	scanf("%d",&number);
	for(int i=0;i<number;i++)
	{
		int j = syscall(181);
		printf("\nevent %d has been created\n",j);
	}
	printf("\n %d events has been created",number);
    
}

void test_close()
{
	int ID;
	printf("\nType the eventID you want to close : ");
	scanf("%d", &ID);
	int i = syscall(182, ID);
	if (i == -1)
	{
		printf("\n Event close failure \n");
	}
}


void test_createwait()
{
	//create new event 
	int newID = syscall(181);
	printf("\n the new event ID is %d\n", newID);
	// do wait 
    int typeID;
    printf("\nchoose the event ID you want to wait : ");
    scanf("%d", &typeID);
    if(fork() == 0)
    {
	    int wait =syscall(183,typeID);
	    if(wait == -1)
	    {
		   printf("\nwaiting error\n");
		   //return 0;
	    }
	    else {
		printf("\nWaiting successful\n");
	    }
	    exit(0);
    }
    else{
	   printf("\nfork failure and parents is here\n"); 
    }
	//printf("\nwait end\n");
	//return 0;
}

void test_sig()
{
	int ID;
	printf("\ntype the event ID you want to signal : \n");
	scanf("%d",&ID);
	int i = syscall(184, ID);
	if(i == -1)
	{
		printf("\nsignal failure!\n");
	}
	else {
		printf("\nevent %d signaled ", ID);
	}
}

void test_info()
{
	int *p;
	int number;
	int i;
	printf("\ntype the number of event (0 for NULL) :");
	scanf("%d", &number);
	if(number != 0)
	{
		p=malloc(number * sizeof(int));
	    i = syscall(185, number,p);
		if (i == -1 )
		{
			printf ("\nfailure in info \n");
		}
		else {
			printf("\nevents are : ");
			for (i = 0; i < number; i++) {
				printf ("%d ", p[i]);
			}
		}
	}
	else {
		i = syscall(185,1,NULL);
		if (i == -1 )
		{
			printf ("\nfailure in info \n");
		}
		else {
			printf("\n the number of active events is : %d\n", i);
		}
	}
	//return i;
}


void test_chown()
{
	uid_t *newuid = malloc(sizeof(uid_t));
	gid_t *newgid = malloc(sizeof(gid_t));
	int *newuidFlag = malloc(sizeof(int));
	int *newgidFlag = malloc(sizeof(int));
	int newID;
	//int newID = syscall(181, NULL);
	printf("\ntype in the event number : ");
	scanf("%d",&newID);
	printf("\n");
	printf("type in the event uid and the gid you want change \n");
	int new_uid, new_gid;
	printf("type in the uid ");
	scanf("%d",&new_uid);
	printf("\ntype in the gid ");
	scanf("%d",&new_gid);
	printf("\nChange the event's uid to to %d and the gid to %d.\n", new_uid, new_gid);
	
	int change = syscall(205, newID, new_uid, new_gid);
	if (change != -1){
	printf("Failure in change\n");}
	else {
		int stat = syscall(214, newID, newuid, newgid, newuidFlag, newgidFlag);
		printf("Thie event's id is %d\n", newID);
		printf("This event's uid and gid are %d %d \n", *newuid, *newgid);
		printf("This event's uidflag and gidflag are %d %d \n", *newuidFlag, *newgidFlag);
		
	}
	free(newuid);
	free(newgid);
	free(newuidFlag);
	free(newgidFlag);
	//return 0;
}

void test_chmod()
{
	uid_t *newuid = malloc(sizeof(uid_t));
	gid_t *newgid = malloc(sizeof(gid_t));
	int *newuidFlag = malloc(sizeof(int));
	int *newgidFlag = malloc(sizeof(int));
	int newID;
	//int newID = syscall(181, NULL);
	printf("\ntype in the event number : ");
	scanf("%d",&newID);
	printf("\n");
	printf("type in the event uidflag and the gidflag you want change \n");
	int new_uidflag, new_gidflag;
	printf("type in the uidflag ");
	scanf("%d",&new_uidflag);
	printf("\ntype in the gidflag ");
	scanf("%d",&new_gidflag);
	printf("\nChange the event's uidflag to to %d and the gidflag to %d.\n", new_uidflag, new_gidflag);
	
	int change = syscall(211, newID, new_uidflag, new_gidflag);
	if (change != -1) {
		int stat = syscall(214, newID, newuid, newgid, newuidFlag, newgidFlag);
		printf("Thie event's id is %d\n", newID);
		printf("This event's uid and gid are %d %d \n", *newuid, *newgid);
		printf("This event's uidflag and gidflag are %d %d \n", *newuidFlag, *newgidFlag);
		
	}	
	free(newuid);
	free(newgid);
	free(newuidFlag);
	free(newgidFlag);
	//return 0;
}

void test_stat()
{   
	int ID;
	uid_t *newuid = malloc(sizeof(uid_t));
	gid_t *newgid = malloc(sizeof(gid_t));
	int *newuidFlag = malloc(sizeof(int));
	int *newgidFlag = malloc(sizeof(int));
	printf("\ntype in the event ID : ");
	scanf("%d",&ID);
	int i = syscall(214,ID,newuid, newgid, newuidFlag, newgidFlag);
	if (i != -1)
	{
		printf("Thie event's id is %d\n", ID);
		printf("This event's uid and gid are %d %d \n", *newuid, *newgid);
		printf("This event's uidflag and gidflag are %d %d \n", *newuidFlag, *newgidFlag);

	}
	else {
		printf("\n failure in stat\n");
	}
	free(newuid);
	free(newgid);
	free(newuidFlag);
	free(newgidFlag);
	
}













void test_notasks()
{
	printf("bounary condition : no tasks waiting when doeventsig() is called!\n");
	int newid = syscall(181, NULL);
	//create a event and close it
	int c = syscall(182, newid);
	if (c == -1 )
	{
		printf("close failure");
	}
	
}
