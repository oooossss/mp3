#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/sched.h>

struct event 
{
	int id;
	uid_t uid;
	gid_t gid;
	int uidflag;
	int gidflag;
	struct list_head eventID_list;
	wait_queue_head_t wait_queue;
}
