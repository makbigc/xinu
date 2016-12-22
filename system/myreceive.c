/* myreceive.c - myreceive */

#include <xinu.h>

umsg32	myreceive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/
	struct	cirbuf	*msgbuf;

	mask = disable();
	prptr = &proctab[currpid];
	msgbuf = &(prptr->prmsgbuf);

	/* If the message buffer is empty */

	/*if (msgbuf->tail == msgbuf->head && msgbuf->tail == 0) {
		prptr->prstate = PR_MYRECV;
		resched();
	}*/

	if (msgbuf->vacancy == NMSG) {
		prptr->prstate = PR_MYRECV;
		resched();
	}

	msg = *(msgbuf->tail);		/* Retrieve message		*/
	*(msgbuf->tail++) = 0;		

	msgbuf->vacancy++;

	if (msgbuf->tail - msgbuf->buffer >= NMSG)
		msgbuf->tail = msgbuf->buffer;	// Rewind the tail

	restore(mask);
	return msg;
}

syscall	myreceiven(
	  umsg32*	msgs,
	  uint32	msg_count
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	struct	cirbuf	*msgbuf;

	mask = disable();
	prptr = &proctab[currpid];
	msgbuf = &(prptr->prmsgbuf);

	/* If the message buffer is empty */

	/*if (msgbuf->vacancy == NMSG) {
		prptr->prstate = PR_MYRECV;
		resched();
	}*/

	/* If the message in the buffer isn't long enough */

	if ( NMSG - msgbuf->vacancy < msg_count) {
		prptr->prstate = PR_MYRECV;
		resched();
	}

	/* Retrieve message */

	for (uint32 i=0; i<msg_count; i++) {

		if (msgbuf->tail - msgbuf->buffer >= NMSG)
			msgbuf->tail = msgbuf->buffer;	// Rewind the tail

		*(msgs++) = *(msgbuf->tail);
		*(msgbuf->tail++) = 0;		

		msgbuf->vacancy++;
	}
	restore(mask);
	return OK;
}
