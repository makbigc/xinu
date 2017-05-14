#include <xinu.h>

pri16	chprcls(
		pid32	pid,
		bool8	newprcls
		)
{
	struct procent *prptr;
	bool8		oldprcls;
	pri16		delta;


	prptr = &proctab[pid];
	oldprcls = prptr->pr_class;

	delta = delta_prprio[oldprcls][newprcls];

	prptr->pr_class = newprcls;

	if (prptr->prprio == 0
		||prptr->prprio == 8
		||prptr->prprio == NETPRIO){

		return prptr->prprio;
	}

	prptr->prprio += delta;

	return prptr->prprio;
}

