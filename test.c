#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>


int test_createwait(void);
int test_chown(void);


int main(int argc, char *argv[])
{
	int number;
	printf("type in your test number: ");
	scanf("%d",&number);
	
	switch (number) {
		case 1:  test_createwait(); break;
		case 2:  ;
		case 3:  ;
		case 4:  ;
		default: printf("Wrong input test number");
	}
}

int test_createwait(void)
{
	//create new event 
	int newID = syscall(181);
	printf("the new event ID is %d\n", newID);
	// do wait 
	int wait =syscall(183,newID);
	if(wait == -1)
	{
		printf("waiting error\n");
		return 0;
	}
	printf("wait end\n");
	return 0;
}

int test_chown(void)
{
	uid_t *newuid = malloc(sizeof(uid_t));
	gid_t *newgid = malloc(sizeof(gid_t));
	int *newuidFlag = malloc(sizeof(int));
	int *newgidFlag = malloc(sizeof(int));
	
	int newID = syscall(181, NULL);
	printf("type in the event uid and the gid you want change \n");
	int new_uid, new_gid;
	printf("type in the uid ");
	scanf(%d,&new_uid);
	printf("\ntype in the gid ");
	scanf(%d,&new_gid);
	printf("\nChange the event's uid to to %d and the gid to %d.\n", new_uid, new_gid);
	
	int change = syscall(205, newID, newuid, newgid);
	if (change == -1){
	printf("Failure in changing\n");}
	else {
		int stat = syscall(214, newID, newuid, newgid, newuidFlag, newgidFlag);
		printf("Thie event's id is %d\n", newID);
		printf("This event's uid and gid are %d %d \n", *newuid, *newgid);
		printf("This event's uidflag and gidflag are %d %d \n", *newuidFlag, *newgidFlag);
		return 0;
	}
}

int test_chmod(void)
{
	uid_t *newuid = malloc(sizeof(uid_t));
	gid_t *newgid = malloc(sizeof(gid_t));
	int *newuidFlag = malloc(sizeof(int));
	int *newgidFlag = malloc(sizeof(int));
}
