/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/
	pri16		prprio;

	/* Decrement the ms counter, and see if a second has passed */

	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */

		clktime++;

		/* Reset the local ms counter for the next second */

		count1000 = 1000;
	}

	/* Increment the cpu time of the current process */

	proctab[getpid()].pr_cputime++;

	/* Handle sleeping processes if any exist */

	if(!isempty(sleepq)) {

		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/

		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */

	if((--preempt) <= 0) {
		//preempt = QUANTUM;

		/* As the current process consumes the whole time slice */
		/* the process class is CPU-bound */
		prprio = chprcls(currpid, PRCLS_CPUB);

		if (prprio == NETPRIO){
			preempt = 10;
		} else {
			preempt = quantum[prprio];
		}
		//proctab[getpid()].pr_class = PRCLS_CPUB;

		resched();
	}
}
