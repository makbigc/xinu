#include <xinu.h>

void producer(struct cirbuf_char *buffer, int32 i)
{
    while(1) {
        for (int j=0; j < producer_counts[i]; j++) {
            wait(emptycount);
                    additem(buffer, producer_tags[i]);
            signal(fillcount);
            producer_insert_counts[i]++;
        }
        sleepms(producer_sleep_times[i]);
    }
}

void consumer(struct cirbuf_char *buffer, int32 i)
{
    char tag;
    int k;

    while(1) {
        for (int j=0; j<consumer_counts[i]; j++) {
            wait(fillcount);
                    tag = removeitem(buffer);
            signal(emptycount);
            k = tag2number(tag);
            consumer_extract_counts[i][k]++;
        }
        sleepms(consumer_sleep_times[i]);
    }
}

void init_cirbuf_char(struct cirbuf_char *buffer)
{
    buffer->head = buffer->buffer;
    buffer->tail = buffer->buffer;
}

void additem(struct cirbuf_char *buffer, char item)
{
    wait(head_mutex);

        if (buffer->head - buffer->buffer >= BUFFERSIZE)
            buffer->head = buffer->buffer;

        *(buffer->head++) = item;

    signal(head_mutex);
}

char removeitem(struct cirbuf_char *buffer)
{
    char item;

    wait(tail_mutex);

        if (buffer->tail - buffer->buffer >= BUFFERSIZE)
            buffer->tail = buffer->buffer;

        item = *(buffer->tail);
        *(buffer->tail++) = 0;

    signal(tail_mutex);

    return item;
}

int tag2number(char tag)
{
    for (int i=0; i < BUFFERSIZE; i++) {
        if ( tag == producer_tags[i])
            return i;
    }
}

void start_prod_con(void)
{
    //Initialization of semaphores
    head_mutex = semcreate(1);
    tail_mutex = semcreate(1);
    fillcount = semcreate(0);
    emptycount = semcreate(BUFFERSIZE);

    //Initialization of producers
    producer_tags[0]='A';
    producer_tags[1]='B';
    producer_tags[2]='C';

    producer_sleep_times[0]=100;
    producer_sleep_times[1]=200;
    producer_sleep_times[2]=300;

    producer_counts[0]=10;
    producer_counts[1]=5;
    producer_counts[2]=2;

    //Initialization of consumers
/*    consumer_tags[0]='a';
    consumer_sleep_times[0]=100;
    consumer_counts[0]=10;
*/

    //Intialization of the circular buffer
    init_cirbuf_char(&buffer1);

    for (int i=0; i<NPRODUCERS ; i++) {
        producer_id[i]=create(producer, 1024, 20, producer_tags[i], 2, &buffer1, i);
        resume(producer_id[i]);
    }
/*
    for (int j=0; j<NCONSUMERS ; j++) {
        consumer_id[j]=create(consumer, 1024, 20, consumer_tags[j], 2, &buffer1, j);
        resume(consumer_id[j]);
    }*/
}

void stop_prod_con(void)
{
    for (int i=0; i<NPRODUCERS ; i++)
        kill(producer_id[i]);
/*
    for (int j=0; j<NCONSUMERS ; j++)
        kill(consumer_id[j]);
  */
}

void print_report(void)
{

    int remain[NPRODUCERS]={0};

    for (int i=0; i<NPRODUCERS ; i++)
        printf("Producer %c: created %d items\n",
                producer_tags[i], producer_insert_counts[i]);

/*
    for (int j=0; j<NCONSUMERS ; j++) {
        printf("Consumer %c: deleted ", consumer_tags[j]);
        for (int k=0; k<NPRODUCERS ; k++)
            printf("%d items from producer %c ",
                    consumer_extract_counts[j][k], producer_tags[k]);
        printf("\n");
    }
*/
    printf("The shared buffer contains:");

    for (int p=0; p<NPRODUCERS ; p++) {
        for (int q=0; q<BUFFERSIZE; q++) {
            if (buffer1.buffer[q]==producer_tags[p])
                remain[p]++;
        }
        printf(" %d items from producer %c",remain[p], producer_tags[p]);
    }

}
