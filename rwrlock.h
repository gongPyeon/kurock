#include <semaphore.h>

typedef struct _rwrlock_t {
    sem_t lock; // reader들의 lock
    sem_t writelock; // write lock
    int readers; // 현재 critical section에 readr가 몇명있는지
} rwrlock_t;

void rwrlock_init(rwrlock_t *rwr);
void rwrlock_acquire_writelock(rwrlock_t *rwr);
void rwrlock_release_writelock(rwrlock_t *rwr);
void rwrlock_acquire_readlock(rwrlock_t *rwr);
void rwrlock_release_readlock(rwrlock_t *rwr);

void rwrlock_init(rwrlock_t *rwr){
    rwr->readers = 0;
    sem_init(&rwr->lock, 0, 1); // lock이므로 1로 세팅한다
    sem_init(&rwr->writelock, 0, 1); 
}
void rwrlock_acquire_writelock(rwrlock_t *rwr){
    sem_wait(&rwr->writelock); // 획득하면 -1
}
void rwrlock_release_writelock(rwrlock_t *rwr){
    sem_post(&rwr->writelock); // 해제하면 +1
}
void rwrlock_acquire_readlock(rwrlock_t *rwr){
    sem_wait(&rwr->lock);
    rwr->readers++;
    if(rwr->readers == 1)
        sem_wait(&rwr->writelock); //reader가 진입하면 writelock을 재운다
    sem_post(&rwr->lock);
}
void rwrlock_release_readlock(rwrlock_t *rwr){
    sem_wait(&rwr->lock);
    rwr->readers--;
    if(rwr->readers == 0)
        sem_post(&rwr->writelock);
    sem_post(&rwr->lock);

}