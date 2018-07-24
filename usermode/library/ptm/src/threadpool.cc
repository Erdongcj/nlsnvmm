#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <pthread.h>  
#include <assert.h>
#include "threadpool.h"

//share resource  
//static CThread_pool *pool = NULL;  
//s_sharedMemory SHARED_MEMORY;
CThread_pool* pool_init (s_nodeMemory* nodeMemory)  
{  
    CThread_pool *pool = NULL;
	cpu_set_t cpus;
    pool = (CThread_pool *) malloc (sizeof (CThread_pool));  
  
    pthread_mutex_init (&(pool->queue_lock), NULL);  
    pthread_cond_init (&(pool->queue_ready), NULL);  
  
    pool->queue_head = NULL;  
  
    pool->max_thread_num = cpu_status[0];  
    pool->cur_queue_size = 0;  
  
    pool->shutdown = 0;  
  
    pool->threadid = (pthread_t *) malloc (cpu_status[0] * sizeof (pthread_t));  
    int i = 0;  
    threadMem threadmem;
//	s_nodeMemory* currMemory;
    for (i = 0; i < cpu_status[0]; i++)  
    {   
        threadmem.nodeMemory = NODE_MEMORY + i;
        if(threadmem.nodeMemory != nodeMemory){
            pthread_create (&(pool->threadid[i]), NULL, thread_routine, (void*)pool);  
            CPU_ZERO(&cpus); /* Clears set, so that it contains no CPUs. */
            CPU_SET(i*(cpu_status[1]/2), &cpus); /* Add CPU cpu to set. */
            pthread_setaffinity_np(pool->threadid[i], sizeof(cpu_set_t), &cpus);
        }
    }  
	return pool;
}  
  
  
  
/*向线程池中加入任务*/  
int  
pool_add_worker (CThread_pool *pool, void *(*process) (void *arg), void *arg)  
{  
    /*构造一个新任务*/  
    CThread_worker *newworker = (CThread_worker *) malloc (sizeof (CThread_worker));  
    newworker->process = process;  
    newworker->arg = arg;  
    newworker->next = NULL;/*别忘置空*/  
//  	printf("(s_threadId*)(newworker->arg)->taiEntry is ........%lx\n", ((s_threadId*)(newworker->arg))->tailEntry);
    pthread_mutex_lock (&(pool->queue_lock));  
    /*将任务加入到等待队列中*/  
    CThread_worker *member = pool->queue_head;  
    if (member != NULL)  
    {  
        while (member->next != NULL)  
            member = member->next;  
        member->next = newworker;  
    }  
    else  
    {  
        pool->queue_head = newworker;  
    }  
  
    assert (pool->queue_head != NULL);  
  
    pool->cur_queue_size++;  
    pthread_mutex_unlock (&(pool->queue_lock));  
    
	/*好了，等待队列中有任务了，唤醒一个等待线程； 
    注意如果所有线程都在忙碌，这句没有任何作用*/  
    pthread_cond_signal (&(pool->queue_ready));  
    return 0;  
}  
  
  
  
/*销毁线程池，等待队列中的任务不会再被执行，但是正在运行的线程会一直 
把任务运行完后再退出*/  
int  
pool_destroy (CThread_pool *pool)  
{  
    if (pool->shutdown)  
        return -1;/*防止两次调用*/  
    pool->shutdown = 1;  
  
    /*唤醒所有等待线程，线程池要销毁了*/  
    pthread_cond_broadcast (&(pool->queue_ready));  
  
    /*阻塞等待线程退出，否则就成僵尸了*/  
    int i;  
    for (i = 0; i < pool->max_thread_num; i++)  
        pthread_join (pool->threadid[i], NULL);  
    free (pool->threadid);  
  
    /*销毁等待队列*/  
    CThread_worker *head = NULL;  
    while (pool->queue_head != NULL)  
    {  
        head = pool->queue_head;  
        pool->queue_head = pool->queue_head->next;  
        free (head);  
    }  
    /*条件变量和互斥量也别忘了销毁*/  
    pthread_mutex_destroy(&(pool->queue_lock));  
    pthread_cond_destroy(&(pool->queue_ready));  
      
    free (pool);  
    /*销毁后指针置空是个好习惯*/  
    pool=NULL;  
    return 0;  
}  
  
  
  
void *  
thread_routine (void *arg)  
{  
//    printf ("starting thread 0x%x\n", pthread_self ());  
    CThread_pool *pool = (CThread_pool*)(arg);
	while (1)  
    {  
        pthread_mutex_lock (&(pool->queue_lock));  
        /*如果等待队列为0并且不销毁线程池，则处于阻塞状态; 注意 
        pthread_cond_wait是一个原子操作，等待前会解锁，唤醒后会加锁*/  
  //      printf("pool->cur_queue_size is !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!%d\n", pool->cur_queue_size);
		while (pool->cur_queue_size == 0 && !pool->shutdown)  
        {  
    //        printf ("thread 0x%x is waiting\n", pthread_self ());  
            pthread_cond_wait (&(pool->queue_ready), &(pool->queue_lock));  
        }  
  
        /*线程池要销毁了*/  
        if (pool->shutdown)  
        {  
            /*遇到break,continue,return等跳转语句，千万不要忘记先解锁*/  
            pthread_mutex_unlock (&(pool->queue_lock));  
    //        printf ("thread 0x%x will exit\n", pthread_self ());  
            pthread_exit (NULL);  
        }  
  
      //  printf ("thread 0x%x is starting to work\n", pthread_self ());  
  	//	printf("pool->cur_queue_size is !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!................................................%d\n", pool->cur_queue_size);
        /*assert是调试的好帮手*/  
        assert (pool->cur_queue_size != 0);  
        assert (pool->queue_head != NULL);  
          
        /*等待队列长度减去1，并取出链表中的头元素*/  
        pool->cur_queue_size--;  
        CThread_worker *worker = pool->queue_head;  
        pool->queue_head = worker->next;  
        pthread_mutex_unlock (&(pool->queue_lock));  
  
        /*调用回调函数，执行任务*/  
//		printf("(s_threadId*)(worker->arg)->tailEntry is ...................%lx\n", ((s_threadId*)(worker->arg))->tailEntry);
 //       exit(1);
		(*(worker->process)) (worker->arg);  
		
        free (worker);  
        worker = NULL;  
    }  
    /*这一句应该是不可达的*/  
    pthread_exit (NULL);  
}  
/* 
void* combine_concurrent(CThread_pool *pool, s_nodeMemory* nodeMemory, s_entries* to){
//	if (pthread_mutex_init(&SHARED_MEMORY.globalMtx, NULL) != 0) {
//  		return (void*)-1;
//	}
//    printf("to is ..................%lx\n", to);
	s_threadId *threadDetails = (s_threadId*)malloc(sizeof(s_threadId));
*/	/*连续向池中投入cpu_status[0]个任务*/
/*	int *workingnum = (int *) malloc (sizeof (int)*cpu_status[0]);
//	printf("nodeMemory is is is is is ...............%lx\n", nodeMemory);
	s_nodeMemory *currMemory;
	for (int i=0; i<cpu_status[0]; i++) { 
		threadDetails->ready = 0;
		currMemory = NODE_MEMORY + i;
//		threadDetails->tailEntry = to;
		
		if(currMemory != nodeMemory){
//			printf("threadDetails->nodeMemory is is is ....................%lx\n", threadDetails->nodeMemory);
			threadDetails->nodeMemory = currMemory;
			threadDetails->tailEntry = to;
//			printf("threadDetails.tailEntry is ....................%lx\n", threadDetails->tailEntry);
			  //          printf("nodeMemory->ht is ...................%lx\n", nodeMemory->ht);
			  //          printf("(threadDetails.nodeMemory->ht) is .......................%lx\n", (threadDetails.nodeMemory)->ht);
			
			pool_add_worker(pool, combine, (void*)threadDetails);
		}
	}
//	if (pthread_mutex_destroy(&SHARED_MEMORY.globalMtx) != 0) {
//		return (void*)-1;
//	}
	return (void*)0;
}



void* combine(void* arg)
{
	printf("combine start\n");
	struct timeval begin1, end1;
	float diff1;
	gettimeofday(&begin1, NULL);
//	int pthreadReturnValue;
	s_nodeMemory* nodeMemory;
	s_threadId* thId = (s_threadId*)arg;
//	printf("&SHARED_MEMORY.globalMtx is !!!!!!!!!!!!!!!!!!!!!!!!!!!!!%lx\n", &SHARED_MEMORY.globalMtx);
//	if (pthread_mutex_lock(&SHARED_MEMORY.globalMtx) != 0) {
//		printf("111111111111111111111\n");
//		return (void*)-1;
//	}
//	printf("222222222222222222222222222222222222222\n");
	nodeMemory = thId->nodeMemory;
//	if ((nodeMemory->myCPUID = sched_getcpu()) == -1) {
//		return (void*)-1;
//	}
	printf("nodeMemory->ht combine is ................%lx\n", nodeMemory->ht);
	updateFromLog(nodeMemory, thId->tailEntry);

	nodeMemory->localTail = thId->tailEntry;
//	printf("nodeMemory ->localTail is ...............%lx\n", nodeMemory->localTail);
	thId->ready = 1;
//	if (pthread_cond_signal(&SHARED_MEMORY.globalCond) != 0) {
//		return (void*)-1;
//	}
//	if (pthread_mutex_unlock(&SHARED_MEMORY.globalMtx) != 0) {
//		return (void*)-1;
//	}
	gettimeofday(&end1, NULL);
	diff1 = (end1.tv_sec - begin1.tv_sec)*1000000 + (end1.tv_usec - begin1.tv_usec);
//  printf(" times diff........................................................................: %f\n", diff1);
	pthread_exit(0);
}
*/
