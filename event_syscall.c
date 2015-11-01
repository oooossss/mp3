#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/sched.h>


#include <linux/event.h>


struct event global_event;

rwlock_t eventID_list_lock;

void doevent_init()
{
	eventID_list_lock = RW_LOCK_UNLOCKED;

	INIT_LIST_HEAD(&global_event.eventID_list);

	global_event.id = 0;
    
	init_waitqueue_head(&global_event.wait_queue);
	event_initialized = true;
}

asmlinkage long doeventopen()
{
	//set a index of lock
	unsigned long flags;
	int last_eventid;
	//create a new event
	struct event* new_event = kmalloc(sizeof(struct event), GFP_KERNEL);
	if (new_event == NULL){
		printk("Event cannot be initialized\n");
		return -1;
	}
	// Initialize attributes of new_event. 
	new_event->UID = current->cred->euid;  
	new_event->GID = current->cred->egid;
	new_event->UIDFlag = 1;
	new_event->GIDFlag = 1;
	//init event list entry
	INIT_LIST_HEAD(&(new_event->eventID_list));
	// put lock on the event list
	write_lock_irqsave(&eventID_list_lock, flags);
	// add newevent to the tail of the event list
	list_add_tail(&(new_event->eventID_list), &global_event.eventID_list);
	// find the last event id 
	last_eventid = list_entry((new_event->eventID_list).prev, struct event, eventID_list)->eventID;
	new_event->eventID = last_eventid + 1;
	//init the wait queue
	init_waitqueue_head(&(new_event->wait_queue)); 
	// unlock the event list
	write_unlock_irqrestore(&eventID_list_lock, flags);
	return new_event->id;
}

asmlinkage long doeventclose(int eventID)
{
	if(eventId < 0)
	{
		printk("Event cannot be initialized\n");
		return -1;
	}
	unsigned long flags;
	//set read lock
	read_lock_irqsave(&eventID_list_lock, flags);
	// Search for event in event list. 
	struct event * this_event = get_event(eventID);
	// read unlock 
	read_unlock_irqrestore(&eventID_list_lock, flags);
	if (this_event == NULL)
	{
		printk("event not found!");
		return -1;
	}
	else{
	int processes_signaled = sys_doeventsig(eventID);
	// set write lock 
	write_lock_irqsave(&eventID_list_lock, flags);
	//delete from event list
	list_del(&(this_event->eventID_list));
	//write unlock
	write_unlock_irqrestore(&eventID_list_lock, flags);}
	kfree(this_event);
	return processes_signaled;
}

asmlinkage long doeventwait(int eventID)
{
	if(eventId < 0)
	{
		printk("event not found!");
		return -1;
	}
	
	unsigned long flags;
	//set read lock
	read_lock_irqsave(&eventID_list_lock, flags);
	// Search for event in event list. 
	struct event * this_event = get_event(eventID);
	// read unlock 
    read_unlock_irqrestore(&eventID_list_lock, flags);
    if (this_event == NULL)
    {
	printk("event not found!");
	return -1;
    }
    DEFINE_WAIT(wait);
   ///////////////////////////////
    prepare_to_wait(&(this_event->wait_queue), &wait, TASK_INTERRUPTIBLE);
    // start wait
    schedule();
    finish_wait(&(this_event->wait_queue), &wait);


    return 0;
}

asmlinkage long doeventsig(int eventID)
{
	if(eventId < 0)
		{
			printk("event not found!");
			return -1;
		}
		
		unsigned long flags;
		//set read lock
		read_lock_irqsave(&eventID_list_lock, flags);
		// Search for event in event list. 
		struct event * this_event = get_event(eventID);
		// read unlock 
	    read_unlock_irqrestore(&eventID_list_lock, flags);
	    if (this_event == NULL)
	    {
		printk("event not found!");
		return -1;
	    }
	//lock wait queue
	spin_lock_irqsave(&(this_event->wait_queue.lock), flags);
	
	int length=0 ;
	struct list_head * i;
	list_for_each(i,&(this_event->wait_queue.task_list))
	{
		length++;
	}
	int processes_signaled = length;
	//unlock wait queue
	spin_unlock_irqrestore(&(this_event->wait_queue.lock), flags);
	//wake up tasks in the wait queue
	wake_up(&(this_event->wait_queue));
	
	return processes_signaled;
}

asmlinkage long doeventinfo(int eventID)
{
    if(eventId < 0)
		{
			printk("event not found!");
			return -1;
		}
		
		unsigned long flags;
		//set read lock
		read_lock_irqsave(&eventID_list_lock, flags);
		int length=0 ;
		struct list_head * i;
		//count the event number 
		list_for_each(i,&(this_event->wait_queue.task_list))
		{
			length++;
		}
		int event_count = length;
		
		// create a new array to shore IDs
		int * storeIDs;
		if ((storeIDs = kmalloc(event_count * sizeof(int), GFP_KERNEL)) == NULL) {
			printk("error \n");
			return -1;
		    }
		
		//Insert all IDs to array 
		int i = 0;
		struct event * pos; 
		 
		list_for_each_entry(pos, &global_event.eventID_list, eventID_list)          
		{    
				
			Do not insert or count global_event whose eventID is 0. 
			if ((pos->eventID) != 0) {  
				*(storeIDs + i++) = pos->eventID;   
			}
		}
        //unlock read
		read_unlock_irqrestore(&eventID_list_lock, flags);
		//check 
		if (eventIDs == NULL) {
			kfree(storeIDs);
			return event_count;
		}

		if (num < event_count) {
			kfree(storeIDs;
			return -1;

		}
        if (copy_to_user(eventIDs, storeIDs, event_count * sizeof(int)) != 0) {
	    printk("error sys_doeventinfo(): copy_to_user()\n");
	    return -1;
        }

        kfree(storeIDs);
	
        return event_count;
		
}

asmlinkage long doeventchown(int num, int * eventIDs)
{
	if(eventId < 0)
	{
		printk("event not found!");
		return -1;
	}
	if (UIDFlag < 0 || UIDFlag > 1 || GIDFlag < 0 || GIDFlag > 1) 
	{
		printk("invalid arguments\n");
		return -1;
	}
	unsigned long flags;
	read_lock_irqsave(&eventID_list_lock, flags);
	//Search for the event in event list. 
	struct event * this_event = get_event(eventID);
	read_unlock_irqrestore(&eventID_list_lock, flags);
}

asmlinkage long doeventchmod(int eventID, uid_t UID, gid_t GID)
{
	
}

asmlinkage long doeventstat (int eventID, uid_t * UID, gid_t * GID, int * UIDFlag, int * GIDFlag )
{
	
}
