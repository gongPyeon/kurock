#include <semaphore.h>

typedef struct _seqlock_t {
    int count;
    sem_t lock; // count 전역변수를 증가시킬 때 필요한 lock
    sem_t writelock;
} seqlock_t;

void seqlock_init(seqlock_t *seq);
void seqlock_write_lock(seqlock_t *seq);
void seqlock_write_unlock(seqlock_t *seq);
unsigned seqlock_read_begin(seqlock_t *seq);
unsigned seqlock_read_retry(seqlock_t *seq, unsigned cnt);

void seqlock_init(seqlock_t *seq){
    seq->count = 0;
    sem_init(&seq->writelock, 0, 1);
    sem_init(&seq->lock, 0, 1);
}
void seqlock_write_lock(seqlock_t *seq){
    sem_wait(&seq->lock);

    sem_wait(&seq->writelock);
    seq->count++;

    sem_post(&seq->lock);

}
void seqlock_write_unlock(seqlock_t *seq){
    sem_wait(&seq->lock);

    sem_post(&seq->writelock);
    seq->count++;

    sem_post(&seq->lock);
}
unsigned seqlock_read_begin(seqlock_t *seq){
    return seq->count; // count 값 리턴
}
unsigned seqlock_read_retry(seqlock_t *seq, unsigned cnt){
    if(seq->count != cnt || seq->count % 2 == 1) // diff or odd
        return 1;

    // same
    return 0;
}