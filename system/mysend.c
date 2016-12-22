/* mysend.c - mysend */

#include <xinu.h>

syscall	mysend(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	struct	cirbuf	*msgbuf;

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	msgbuf = &(prptr->prmsgbuf);

	/* If the circular buffer is full */

	/*if (msgbuf->head == msgbuf->tail && *(msgbuf->head) != 0)
		return SYSERR;*/

	if (msgbuf->vacancy == 0 )
		return SYSERR;

	*(msgbuf->head++) = msg;	/* Deliver message		*/

	msgbuf->vacancy--;

	if (msgbuf->head - msgbuf->buffer >= NMSG)
		msgbuf->head = msgbuf->buffer; /* Rewind the head */

	if (prptr->prstate == PR_MYRECV) {
		ready(pid);
	} /*else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}*/
	restore(mask);		/* Restore interrupts */
	return OK;
}

uint32	mysendn(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32*	msgs,		/* Contents of message		*/
	  uint32	msg_count
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	struct	cirbuf	*msgbuf;
	uint32	count;			/* Number of message delivered	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	msgbuf = &(prptr->prmsgbuf);
	count = 0;

	/* If the circular buffer is full */

	if (msgbuf->vacancy == 0)
		return SYSERR;

	/* Deliver message */

	while (msgbuf->vacancy-- > 0 && count++ < msg_count) {

		if (msgbuf->head - msgbuf->buffer >= NMSG)
			msgbuf->head = msgbuf->buffer; /* Rewind the head */

		*(msgbuf->head++) = *(msgs++);

	}

	if (prptr->prstate == PR_MYRECV) {
		ready(pid);
	} /*else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}*/
	restore(mask);		/* Restore interrupts */
	return count;
}

