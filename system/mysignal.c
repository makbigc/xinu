#include <xinu.h>

syscall	mysignal(
	  sid32		sem		/* ID of semaphore to signal	*/
	)
{
	intmask mask;			/* Saved interrupt mask		*/
	struct	sentry *semptr;		/* Ptr to sempahore table entry	*/
	pid32	pid;
	struct 	procent	*prptr;

	mask = disable();
	if (isbadsem(sem)) {
		restore(mask);
		return SYSERR;
	}
	semptr= &semtab[sem];
	if (semptr->sstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}
	if ((semptr->scount++) < 0) {	/* Release a waiting process */
		pid = dequeue(semptr->squeue);
		prptr = &proctab[pid];
		if (prptr->delayed_suspend_flag == TRUE) {
			prptr->prstate == PR_SUSP;
			prptr->delayed_suspend_flag == FALSE;
		} else {
			ready(pid);
		}
	}
	restore(mask);
	return OK;
}
