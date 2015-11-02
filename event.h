/* linux/include/linux/eventcalls.h */

#ifndef __EVENT_H
#define __EVENT_H

#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/percpu.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/pid_namespace.h>
#include <linux/notifier.h>
#include <linux/thread_info.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include <linux/posix-timers.h>
#include <linux/cpu.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include <linux/tick.h>
#include <linux/kallsyms.h>

#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/div64.h>
#include <asm/timex.h>
#include <asm/io.h>

#include <linux/kernel.h>

#include <linux/sched.h>   
#include <linux/list.h> 
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/types.h>

struct event
{
    uid_t UID;
    gid_t GID;
    int UIDFlag;
    int GIDFlag;
    /* eventID should be positive integers. */
    int eventID;
    
    /* Implement a kernel double-linked list fo events. */
    struct list_head eventID_list;
    
    /* Implement a wait queue of processes waiting on the event. */
    wait_queue_head_t wait_queue;
};
