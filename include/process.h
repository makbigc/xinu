/* process.h - isbadpid */

/* Maximum number of processes in the system */

#ifndef NPROC
#define	NPROC		8
#endif		

/* Process state constants */

#define	PR_FREE		0	/* Process table entry is unused	*/
#define	PR_CURR		1	/* Process is currently running		*/
#define	PR_READY	2	/* Process is on ready queue		*/
#define	PR_RECV		3	/* Process waiting for message		*/
#define	PR_SLEEP	4	/* Process is sleeping			*/
#define	PR_SUSP		5	/* Process is suspended			*/
#define	PR_WAIT		6	/* Process is on semaphore queue	*/
#define	PR_RECTIM	7	/* Process is receiving with timeout	*/
#define PR_MYRECV	8

/* Process classification based on sources */

#define PRCLS_CPUB	0	/* Process is bound to CPU */
#define PRCLS_IOB	1	/* Process is bound to I/O */

/* Miscellaneous process definitions */

#define	PNMLEN		16	/* Length of process "name"		*/
#define	NULLPROC	0	/* ID of the null process		*/

/* Process initialization constants */

#define	INITSTK		65536	/* Initial process stack size		*/
#define	INITPRIO	4	/* Initial process priority		*/
#define	INITRET		userret	/* Address to which process returns	*/

/* Inline code to check process ID (assumes interrupts are disabled)	*/

#define	isbadpid(x)	( ((pid32)(x) < 0) || \
			  ((pid32)(x) >= NPROC) || \
			  (proctab[(x)].prstate == PR_FREE))

/* Number of device descriptors a process can have open */

#define NDESC		5	/* must be odd to make procent 4N bytes	*/

/* Maximum number of messages pending in the process's queue */

#define NMSG		20

struct cirbuf {
	umsg32	buffer[NMSG];
	umsg32	*head;
	umsg32	*tail;
	uint32	vacancy;
};


/* Definition of the process table (multiple of 32 bits) */

struct procent {		/* Entry in the process table		*/
	uint16		prstate;	/* Process state: PR_CURR, etc.	*/
	pri16		prprio;		/* Process priority		*/
	char		*prstkptr;	/* Saved stack pointer		*/
	char		*prstkbase;	/* Base of run time stack	*/
	uint32		prstklen;	/* Stack length in bytes	*/
	char		prname[PNMLEN];	/* Process name			*/
	sid32		prsem;		/* Semaphore on which process waits*/
	pid32		prparent;	/* ID of the creating process	*/
	umsg32		prmsg;		/* Message sent to this process	*/
	struct cirbuf	prmsgbuf;
	bool8		prhasmsg;	/* Nonzero iff msg is valid	*/
	int16		prdesc[NDESC];	/* Device descriptors for process*/
	bool8		delayed_suspend_flag;
	uint32		pr_cputime;	/* in millisecond*/
	uint32		pr_tsready;	/* timestamp when set to ready state*/
	bool8		pr_class;	/* bound to which resource*/
};

/* Marker for the top of a process stack (used to help detect overflow)	*/
#define	STACKMAGIC	0x0A0AAAA9

extern	struct	procent proctab[];
extern	int32	prcount;	/* Currently active processes		*/
extern	pid32	currpid;	/* Currently executing process		*/

int32	delta_prprio[2][2];
int32	quantum[9];
