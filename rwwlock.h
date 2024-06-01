#include <semaphore.h>

typedef struct _rwwlock_t {
    sem_t lock; // 두개의 lock을 걸기 위함
    sem_t readlock; // reader들의 lock
    sem_t writelock; // write의 lock
    int readers; // 현재 critical section에 readr가 몇명있는지
} rwwlock_t;

void rwwlock_init(rwwlock_t *rww);
void rwwlock_acquire_writelock(rwwlock_t *rww);
void rwwlock_release_writelock(rwwlock_t *rww);
void rwwlock_acquire_readlock(rwwlock_t *rww);
void rwwlock_release_readlock(rwwlock_t *rww);

void rwwlock_init(rwwlock_t *rww){
    rww->readers = 0;
    sem_init(&rww->lock, 0, 1);
    sem_init(&rww->readlock, 0, 1);
    sem_init(&rww->writelock, 0, 1);
}

void rwwlock_acquire_writelock(rwwlock_t *rww){ // write를 한번에 하나씩 실행시키면 문제가 되지 않는다 => 확인해보기
    sem_wait(&rww->lock); // 두개의 lock을 성공시키기 위함 => deadlock이 발생하는가?

    sem_wait(&rww->writelock);
    sem_wait(&rww->readlock); // writer가 진입하는 순간 reader를 재운다
    
    sem_post(&rww->lock);
}

/*
    sem_wait(&rww->lock); // 두개의 lock을 성공시키기 위함 => deadlock?
    sem_wait(&rww->writelock);
    rww->writers++;
    if(rww->writers == 1) // 매번 write가 한번만 불린다면 이 조건이 필요없다
        sem_wait(&rww->readlock); // writer가 진입하는 순간 reader를 재운다
    sem_post(&rww->lock);

*/

void rwwlock_release_writelock(rwwlock_t *rww){
    sem_wait(&rww->lock);

    sem_post(&rww->writelock);
    sem_post(&rww->readlock); // writer가 진입하는 순간 reader를 깨운다

    sem_post(&rww->lock);

}
void rwwlock_acquire_readlock(rwwlock_t *rww){
    sem_wait(&rww->readlock);
    rww->readers++;
    sem_post(&rww->readlock);
    
}
void rwwlock_release_readlock(rwwlock_t *rww){
    sem_wait(&rww->readlock);
    rww->readers--;
    sem_post(&rww->readlock);
}
