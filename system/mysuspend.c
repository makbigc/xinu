#include <xinu.h>

syscall	mysuspend(
	  pid32		pid		/* ID of process to suspend	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	pri16	prio;			/* Priority to return		*/

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];

	/*if ((prptr->prstate != PR_CURR) && (prptr->prstate != PR_READY)) {
		restore(mask);
		return SYSERR;
	}*/

	if ((prptr->prstate == PR_SUSP)
	    ||(prptr->delayed_suspend_flag == TRUE))
		return SYSERR;

	if ((prptr->prstate == PR_WAIT)
	    ||(prptr->prstate == PR_SLEEP)
	    ||(prptr->prstate == PR_RECV)
	    ||(prptr->prstate == PR_RECTIM))
		prptr->delayed_suspend_flag = TRUE;
	else if (prptr->prstate == PR_READY) {
		getitem(pid);		    /* Remove a ready process	*/
					    /*   from the ready list	*/
		prptr->prstate = PR_SUSP;
	} else {
		prptr->prstate = PR_SUSP;   /* Mark the current process	*/
		resched();		    /*   suspended and resched.	*/
	}


	prio = prptr->prprio;
	restore(mask);
	return prio;
}
