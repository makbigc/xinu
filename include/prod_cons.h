#define BUFFERSIZE 10
#define NPRODUCERS 3
#define NCONSUMERS 0

struct cirbuf {
    char buffer[BUFFERSIZE];
    char *head;
    char *tail;
};

struct cirbuf buffer1;

char producer_tags[NPRODUCERS];
int32 producer_sleep_times[NPRODUCERS];
int32 producer_counts[NPRODUCERS];
int32 producer_insert_counts[NPRODUCERS];
pid32 producer_id[NPRODUCERS];


char consumer_tags[NCONSUMERS];
int32 consumer_sleep_times[NCONSUMERS];
int32 consumer_counts[NCONSUMERS];
int32 consumer_extract_counts[NCONSUMERS][NPRODUCERS];
pid32 consumer_id[NCONSUMERS];



sid32 head_mutex;
sid32 tail_mutex;
sid32 fillcount;
sid32 emptycount;


void producer(struct cirbuf *, int32);
void consumer(struct cirbuf *, int32);

void init_cirbuf(struct cirbuf *buffer);
void additem(struct cirbuf *buffer, char);
char removeitem(struct cirbuf *buffer);

int tag2number(char);

void start_prod_con(void);
void stop_prod_con(void);
void print_report(void);
