// fptree开头的所有代码，都是我用这个多线程测试程序测试的数据结构对象，你可以换成自己的测试对象
// 编译代码：g++ -O3 -DNDEBUG -DMUTEX -m64 -D_REENTRANT -fno-strict-aliasing -DINTEL -Wno-unused-value -Wno-format  -o ./main main.c -m64 -lpthread


/*****************************************************************************************************************************************/
/*下面开始是测试程序*/
/*****************************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <signal.h>
#include <sys/time.h>
#include <assert.h>
#include <tx_api.h>
#include <numals.h>
#include <numa.h>
#include <ssalloc.h>
//#define  __USE_GNU
#include <sched.h>
#include <pthread.h>
#include "btree.h"


//持续的时间范围
#define DEFAULT_DURATION                1
//初始的树规模
#define DEFAULT_INITIAL                 256
//线程个数
#define DEFAULT_NB_THREADS              4
//范围
#define DEFAULT_RANGE                   0x7FFFFFFF
//随机种子
#define DEFAULT_SEED                    0
//更新频率
#define DEFAULT_UPDATE                  50
// 连续的insert和delete是否是同一个key
#define DEFAULT_ALTERNATE               0
// 是否保证每次update都插入
#define DEFAULT_EFFECTIVE               1
// 值的分布
#define DEFAULT_UNBALANCED              0

#define XSTR(s)                         STR(s)
#define STR(s)                          #s

#define VAL_MIN                         INT_MIN
#define VAL_MAX                         INT_MAX


inline long rand_range(long r); /* declared in test.c */
static int count;
volatile size_t stop;
unsigned int global_seed;
#ifdef TLS
    __thread unsigned int *rng_seed;
#else /* ! TLS */
    pthread_key_t rng_seed_key;
#endif /* ! TLS */
unsigned int levelmax;

// 内存屏障，还是多线程的屏障
typedef struct barrier {
	pthread_cond_t complete;
	pthread_mutex_t mutex;
	int count;
	int crossing;
} barrier_t;

void barrier_init(barrier_t *b, int n)
{
	pthread_cond_init(&b->complete, NULL);//初始化条件变量
	pthread_mutex_init(&b->mutex, NULL);//初始化互斥锁
	b->count = n;
	b->crossing = 0;
}

void barrier_cross(barrier_t *b)
{
	pthread_mutex_lock(&b->mutex);
	/* One more thread through */
	b->crossing++;
	/* If not all here, wait */
	if (b->crossing < b->count) {
		pthread_cond_wait(&b->complete, &b->mutex);
	} else {
		pthread_cond_broadcast(&b->complete);
		/* Reset for next time */
		b->crossing = 0;
	}
	pthread_mutex_unlock(&b->mutex);
}

int floor_log_2(unsigned int n) {
  int pos = 0;
  if (n >= 1<<16) { n >>= 16; pos += 16; }
  if (n >= 1<< 8) { n >>=  8; pos +=  8; }
  if (n >= 1<< 4) { n >>=  4; pos +=  4; }
  if (n >= 1<< 2) { n >>=  2; pos +=  2; }
  if (n >= 1<< 1) {           pos +=  1; }
  return ((n == 0) ? (-1) : pos);
}


inline long rand_range(long r) {
	int m = RAND_MAX;
	int d, v = 0;

	do {
		d = (m > r ? r : m);
		v += 1 + (int)(d * ((double)rand()/((double)(m)+1.0)));
		r -= m;
	} while (r > 0);
	return v;
}
long rand_range(long r);

/* Thread-safe, re-entrant version of rand_range(r) */
inline long rand_range_re(unsigned int *seed, long r) {
	int m = RAND_MAX;
	int d, v = 0;

	do {
		d = (m > r ? r : m);
		v += 1 + (int)(d * ((double)rand_r(seed)/((double)(m)+1.0)));
		r -= m;
	} while (r > 0);
	return v;
}
long rand_range_re(unsigned int *seed, long r);

typedef struct thread_data {
    int           id;
	unsigned int  first;
	long          range;
	int           update;
	int           alternate;
	int           effective;
	unsigned long nb_add;
	unsigned long nb_added;
	unsigned long nb_remove;
	unsigned long nb_removed;
	unsigned long nb_contains;
	unsigned long nb_found;
	unsigned long nb_aborts;
	unsigned long nb_aborts_locked_read;
	unsigned long nb_aborts_locked_write;
	unsigned long nb_aborts_validate_read;
	unsigned long nb_aborts_validate_write;
	unsigned long nb_aborts_validate_commit;
	unsigned long nb_aborts_invalid_memory;
	unsigned long nb_aborts_double_write;
	unsigned long max_retries;
	unsigned int  seed;
	unsigned long insert_time;
	unsigned long get_time;
	BTree      *bmap;
	barrier_t     *barrier;
	unsigned long failures_because_contention;
	threadID_t* threadID;
} thread_data_t;

char* cleanString(char* oldstr) { /* Return string without spaces and new line chars (\n) */
	char* newstr = (char*) malloc(strlen(oldstr)+1); /* The memory will be free at str2int() */
	char* op = oldstr;
	char* np = newstr;
	do {
		if ((*op != ' ')&&(*op != '\n')) {
			*np++ = *op;
		}
	} while (*op++);
	return newstr;
}
int str2int(char* str) { /* Convert string to integer */
	int dec = 0;
	int i = 0;
	int len = 0;
	len = strlen(str);
	for (i=0;i<len;i++) {
		if ((48 <= str[i])&&(str[i] <= 57)) { /* If it's an integer, 48<=char<=57 */
			dec = dec*10 + (str[i]-'0');
		}
	}
	free(str); /* Free the memory allocated at cleanString() */
	return dec;
}
int unique_elements(int* arr, int len) { /* Count unique elements in an array of integers */
	int count = 0;
	int i, j;
	for (i=0;i<len;i++) {
		for (j=0; j<i; j++) {
			if (arr[i] == arr[j]) {
				break;
			}
		}
		if (i == j) { /* No duplicate element found between index 0 to i */
			count++;
		}
	}
	return count;
}
void analyze_pc(int* results) { /* results = [# physical cpu, # cores per node, hyperthreading] */
	char* ptr;
	FILE* fp;
	char* line = NULL;
	size_t len = 0;
	int cpu0cores = 0;
	int coresIDArray[MAX_CORES];
	int coresIDCount = 0;
	results[0] = results[1] = results[2] = 0; /* Double check */
	if ((fp = fopen("/proc/cpuinfo", "rb")) == NULL) {
		return;
	}
	while (getline(&line, &len, fp) != -1) {
		strtok_r(line, ":", &ptr); /* Split line by ':' */
		if ((strncmp(line,"physical id",11) == 0)&&(coresIDCount < 1024)) {
			coresIDArray[coresIDCount] = str2int(cleanString(ptr));
			coresIDCount++;
		} else if ((results[1] == 0)&&(strncmp(line,"siblings",8) == 0)) {
			results[1] = str2int(cleanString(ptr));
		} else if ((cpu0cores == 0)&&(strncmp(line,"cpu cores",9) == 0)) {
			cpu0cores = str2int(cleanString(ptr));
		}
	}
	fclose(fp);
	if (line) {
		free(line);
	}
	results[0] = unique_elements(coresIDArray,coresIDCount);
	results[2] = (cpu0cores != results[1]); /* if '!=' then hyperthreading is ON */
}

void *test(void *data)
{
	int unext, last = -1;                                              // last似乎是保存上一次写入的值， unext来随机选择下一个操作是否是更新
	uint64_t val = 0;                                                  // val就是key-val对的value
//	ssalloc_init();
	thread_data_t *d = (thread_data_t *)data;                          // 线程状态
	init_thread(d->threadID, 0);
	printf("thread is working\n");
	barrier_cross(d->barrier);                                         /* Wait on barrier */
	unext = (rand_range_re(&d->seed, 100) - 1 < d->update);            /* Is the first op an update? */
	
	while (stop == 0) {

		if (unext) {                                                   // update
				val = rand_range_re(&d->seed, d->range);
				if (btree_map_insert(d->bmap, val, val)) {
//					printf("count is ...........%d\n", count++);
					d->nb_added++;
					last = val;
				}
				d->nb_add++;
		}else {                                                         // read
			if (d->alternate) {
				if (d->update == 0) {
					if (last < 0) {
						val = d->first;
						last = val;
					} else {                                           // last >= 0
						val = rand_range_re(&d->seed, d->range);
						last = -1;
					}
				} else {                                               // update != 0
					if (last < 0) {
						val = rand_range_re(&d->seed, d->range);
						//last = val;
					} else {
						val = last;
					}
				}
			}else
				val = rand_range_re(&d->seed, d->range);
				uint64_t v = btree_map_get(d->bmap, val);
				if (v){ 
					d->nb_found++;
				}
				d->nb_contains++;
		}

		// Is the next op an update? 
		if (d->effective)                                              // a failed remove/add is a read-only tx
			unext = ((100 * (d->nb_added + d->nb_removed)) < (d->update * (d->nb_add + d->nb_remove + d->nb_contains)));
		else                                                           // remove/add (even failed) is considered as an update
			unext = (rand_range_re(&d->seed, 100) - 1 < d->update);

	}

	return NULL;
}

void catcher(int sig)
{
	printf("CAUGHT SIGNAL %d\n", sig);
}

int main(int argc, char **argv)
{
	int cpu_status[3]; /* cpu_status = [# physical cpu, # cores per node, hyperthreading] */
    cpu_status[0] = cpu_status[1] = cpu_status[2] = 0;
	cpu_set_t mask;
    printf("\e[1;1H\e[2J"); /* Clear screen */
    printf("Analyzing your machine...\n");
    analyze_pc(cpu_status);
    printf("\n");
    printf("Nodes (physical cpu's):        %d\n", cpu_status[0]);
    printf("Cores per node:                %d\n", cpu_status[1]);
    if (cpu_status[2]) {
    	printf("Hyperthreading:                ON\n");
    } else {
        printf("Hyperthreading:                OFF\n");
    }
    init_numals((void*)&cpu_status);
	struct option long_options[] = {
		// These options don't set a flag
		{"help",                      no_argument,       NULL, 'h'},
		{"duration",                  required_argument, NULL, 'd'},
		{"initial-size",              required_argument, NULL, 'i'},
		{"thread-num",                required_argument, NULL, 't'},
		{"range",                     required_argument, NULL, 'r'},
		{"seed",                      required_argument, NULL, 'S'},
		{"update-rate",               required_argument, NULL, 'u'},
		{"unbalance",                 required_argument, NULL, 'U'},
		{"elasticity",                required_argument, NULL, 'x'},
		{NULL,                        0,                 NULL, 0  }
	};
	int               i, c;
    unsigned long     size;
	uint64_t          last = 0;
	uint64_t          val = 0;
	unsigned long     reads, effreads, updates, effupds, aborts, aborts_locked_read, aborts_locked_write,
	                  aborts_validate_read, aborts_validate_write, aborts_validate_commit,
	                  aborts_invalid_memory, aborts_double_write, insert_time, get_time, max_retries, failures_because_contention;
	thread_data_t     *data;
	pthread_t         *threads;
	pthread_attr_t    attr;
	barrier_t         barrier;
	struct timeval    start, end;
	struct timespec   timeout;
	int duration =    DEFAULT_DURATION;   //1000;
	int initial =     DEFAULT_INITIAL;    //1000000;
	int nb_threads =  DEFAULT_NB_THREADS; //2;
	long range =      DEFAULT_RANGE;
	int seed =        DEFAULT_SEED;
	int update =      DEFAULT_UPDATE;     //100;
	int alternate =   DEFAULT_ALTERNATE;
	int effective =   DEFAULT_EFFECTIVE;  //0;
    int unbalanced =  DEFAULT_UNBALANCED;
	sigset_t          block_set;

	while(1) {
		i = 0;
		c = getopt_long(argc, argv, "hAf:d:i:t:r:S:u:U:", long_options, &i);
		if(c == -1) break;
		if(c == 0 && long_options[i].flag == 0) c = long_options[i].val;

		switch(c) {
				case 0:
					break;
				case 'h':
					printf("intset -- STM stress test "
								 "(skip list)\n\n"
								 "Usage:\n"
								 "  intset [options...]\n\n"
								 "Options:\n"
								 "  -h, --help\n"
								 "        Print this message\n"
								 "  -A, --Alternate\n"
								 "        Consecutive insert/remove target the same value\n"
								 "  -f, --effective <int>\n"
								 "        update txs must effectively write (0=trial, 1=effective, default=" XSTR(DEFAULT_EFFECTIVE) ")\n"
								 "  -d, --duration <int>\n"
								 "        Test duration in milliseconds (0=infinite, default=" XSTR(DEFAULT_DURATION) ")\n"
								 "  -i, --initial-size <int>\n"
								 "        Number of elements to insert before test (default=" XSTR(DEFAULT_INITIAL) ")\n"
								 "  -t, --thread-num <int>\n"
								 "        Number of threads (default=" XSTR(DEFAULT_NB_THREADS) ")\n"
								 "  -r, --range <int>\n"
								 "        Range of integer values inserted in set (default=" XSTR(DEFAULT_RANGE) ")\n"
								 "  -s, --seed <int>\n"
								 "        RNG seed (0=time-based, default=" XSTR(DEFAULT_SEED) ")\n"
								 "  -u, --update-rate <int>\n"
								 "        Percentage of update transactions (default=" XSTR(DEFAULT_UPDATE) ")\n"
					             "  -U, --unbalance <int>\n"
								 "        Percentage of skewness of the distribution of values (default=" XSTR(DEFAULT_UNBALANCED) ")\n"

								 );
					exit(0);
				case 'A':
					alternate = 1;
					break;
				case 'f':
					effective =  atoi(optarg);
					break;
				case 'd':
					duration =   atoi(optarg);
					break;
				case 'i':
					initial =    atoi(optarg);
					break;
				case 't':
					nb_threads = atoi(optarg);
					break;
				case 'r':
					range =      atol(optarg);
					break;
				case 'S':
					seed =       atoi(optarg);
					break;
				case 'u':
					update =     atoi(optarg);
					break;
                case 'U':
                    unbalanced = atoi(optarg);
                    break;
				case '?':
					printf("Use -h or --help for help\n");
					exit(0);
				default:
					exit(1);
		}
	}

	assert(duration >= 0);
	assert(initial >= 0);
	assert(nb_threads > 0);
	assert(range > 0 && range >= initial);
	assert(update >= 0 && update <= 100);

	printf("Set type     : hashmap\n");
	printf("Duration     : %d\n",  duration);
	printf("Initial size : %u\n",  initial);
	printf("Nb threads   : %d\n",  nb_threads);
	printf("Value range  : %ld\n", range);
	printf("Seed         : %d\n",  seed);
	printf("Update rate  : %d\n",  update);
	printf("Alternate    : %d\n",  alternate);
	printf("Efffective   : %d\n",  effective);
	printf("Type sizes   : int=%d/long=%d/ptr=%d/word=%d\n",
				                   (int)sizeof(int), (int)sizeof(long), (int)sizeof(void *), (int)sizeof(uintptr_t));
	timeout.tv_sec =               duration / 1000;
	timeout.tv_nsec =              (duration % 1000) * 1000000;

	if ((data = (thread_data_t *)malloc(nb_threads * sizeof(thread_data_t))) == NULL) {
		perror("malloc");
		exit(1);
	}
	if ((threads = (pthread_t *)malloc(nb_threads * sizeof(pthread_t))) == NULL) {
		perror("malloc");
		exit(1);
	}

	if (seed == 0) srand((int)time(0));
	else srand(seed);

	levelmax = floor_log_2((unsigned int) initial);

	threadID_t* threadID = (threadID_t*)malloc(sizeof(threadID_t));
	init_thread(threadID, 0);
	BTree* bmap = btree_map_create();
	stop = 0;

	global_seed = rand();
#ifdef TLS
	rng_seed = &global_seed;
#else /*! TLS */
	if (pthread_key_create(&rng_seed_key, NULL) != 0) {
		fprintf(stderr, "Error creating thread local\n");
		exit(1);
	}
	pthread_setspecific(rng_seed_key, &global_seed);
#endif /* ! TLS */

	// Populate set
	printf("Adding %d entries to hashmap\n", initial);
	i = 0;
//	ssalloc_init();
//    init_thread(threadID, 0);
	struct timeval start_time, end_time;
	uint64_t       time_interval;
	gettimeofday(&start_time, NULL);
    	while (i < initial) {
		if (unbalanced) val = rand_range_re(&global_seed, initial);
		else val = rand_range_re(&global_seed, range);
		if (btree_map_insert(bmap, val, (uint64_t)(&val))) {
			last = val;
			i++;
//			btree_map_get(bmap, val);
		}
	}

    gettimeofday(&end_time, NULL);
	time_interval = 1000000 * (end_time.tv_sec - start_time.tv_sec) + end_time.tv_usec - start_time.tv_usec;
	printf("Insert time_interval = %lu us\n", time_interval );
	printf("average insert op = %lu us\n",    time_interval  / initial);
	printf("Level max    : %d\n",             levelmax);



	//return 0;

	// Access set from all threads
	barrier_init(&barrier, nb_threads + 1);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
//	int numcpus=numa_num_task_cpus();
//	numa_set_localalloc();
//	bitmask* bm = numa_bitmask_alloc(numcpus);
//	int NodeNumber=numa_max_node();
//	for(int numnuma = 0; numnuma <= NodeNumber; numnuma++){
//		numa_node_to_cpus(numnuma, bm);
//	}
	
	for (i = 0; i < nb_threads; i++) {
		printf("Creating thread %d\n", i);
        data[i].id =                          i + 1;
		data[i].first =                       last;
		data[i].range =                       range;
		data[i].update =                      update;
		data[i].alternate =                   alternate;
		data[i].effective =                   effective;
		data[i].nb_add =                      0;
		data[i].nb_added =                    0;
		data[i].nb_remove =                   0;
		data[i].nb_removed =                  0;
		data[i].nb_contains =                 0;
		data[i].nb_found =                    0;
		data[i].nb_aborts =                   0;
		data[i].nb_aborts_locked_read =       0;
		data[i].nb_aborts_locked_write =      0;
		data[i].nb_aborts_validate_read =     0;
		data[i].nb_aborts_validate_write =    0;
		data[i].nb_aborts_validate_commit =   0;
		data[i].nb_aborts_invalid_memory =    0;
		data[i].nb_aborts_double_write =      0;
		data[i].max_retries =                 0;
		data[i].seed =                        rand();
		data[i].bmap =                          bmap;
		data[i].barrier =                     &barrier;
		data[i].failures_because_contention = 0;
		data[i].threadID = (threadID_t*)malloc(sizeof(threadID_t));
		if (pthread_create(&threads[i], &attr, test, (void *)(&data[i])) == 0) {
			CPU_ZERO(&mask);
        	CPU_SET(i, &mask);
			if(pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &mask)<0)
	        {
        	    printf("pthread_setaffinity_np failed\n");
				exit(1);
            }
		}else {
			fprintf(stderr, "Error creating thread\n");
			exit(1);
		}

	}
	pthread_attr_destroy(&attr);

	// Catch some signals
	if (signal(SIGHUP, catcher) == SIG_ERR || //signal(SIGINT, catcher) == SIG_ERR ||
	    signal(SIGTERM, catcher) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

    // Start threads
	barrier_cross(&barrier);

	printf("STARTING...\n");
	gettimeofday(&start, NULL);
	if (duration > 0) {
		nanosleep(&timeout, NULL);
	} else {
		sigemptyset(&block_set);
		sigsuspend(&block_set);
	}

        stop = 1;
	gettimeofday(&end, NULL);
	printf("STOPPING...\n");

	// Wait for thread completion
	for (i = 0; i < nb_threads; i++){
//			printf("begin begin !!!!!!\n");
    		if (pthread_join(threads[i], NULL) != 0) {
			fprintf(stderr, "Error waiting for thread completion\n");
			exit(1);
    		}
//			printf("end endend end!!!!!!!!!!!\n");
	}
	duration =                    (end.tv_sec * 1000 + end.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000);
	aborts =                      0;
	aborts_locked_read =          0;
	aborts_locked_write =         0;
	aborts_validate_read =        0;
	aborts_validate_write =       0;
	aborts_validate_commit =      0;
	aborts_invalid_memory =       0;
	aborts_double_write =         0;
	failures_because_contention = 0;
	reads =                       0;
	effreads =                    0;
	updates =                     0;
	effupds =                     0;
	insert_time =                 0;
	get_time =                    0; 
	max_retries =                 0;
	for (i = 0; i < nb_threads; i++) {
        	aborts +=                       data[i].nb_aborts;
		aborts_locked_read +=           data[i].nb_aborts_locked_read;
		aborts_locked_write +=          data[i].nb_aborts_locked_write;
		aborts_validate_read +=         data[i].nb_aborts_validate_read;
		aborts_validate_write +=        data[i].nb_aborts_validate_write;
		aborts_validate_commit +=       data[i].nb_aborts_validate_commit;
		aborts_invalid_memory +=        data[i].nb_aborts_invalid_memory;
		aborts_double_write +=          data[i].nb_aborts_double_write;
		failures_because_contention +=  data[i].failures_because_contention;
		reads +=                        data[i].nb_contains;
		effreads +=                     data[i].nb_contains + (data[i].nb_add - data[i].nb_added) + (data[i].nb_remove - data[i].nb_removed);
		updates +=                      (data[i].nb_add + data[i].nb_remove);
		effupds +=                      data[i].nb_removed + data[i].nb_added;
		insert_time +=                  data[i].insert_time;
		get_time +=                     data[i].get_time;
		if (max_retries < data[i].max_retries)
			max_retries = data[i].max_retries;
	}
	printf("Duration      : %d (ms)\n",          duration);
	printf("#txs          : %lu (%f / s)\n",     reads + updates, (reads + updates) * 1000.0 / duration);

	printf("#read txs     : ");
	if (effective) {
		printf("%lu (%f / s)\n",                 effreads,        effreads * 1000.0 / duration);
		printf("  #contains   : %lu (%f / s)\n", reads,           reads * 1000.0 / duration);
	} else printf("%lu (%f / s)\n",              reads,           reads * 1000.0 / duration);

	printf("#eff. upd rate: %f \n",              100.0 * effupds / (effupds + effreads));

	printf("#update txs   : ");
	if (effective) {
		printf("%lu (%f / s)\n",                 effupds, effupds * 1000.0 / duration);
		printf("  #upd trials : %lu (%f / s)\n", updates, updates * 1000.0 / duration);
	} else printf("%lu (%f / s)\n",              updates, updates * 1000.0 / duration);

	printf("#aborts       : %lu (%f / s)\n",     aborts, aborts * 1000.0 / duration);
	printf("  #lock-r     : %lu (%f / s)\n",     aborts_locked_read, aborts_locked_read * 1000.0 / duration);
	printf("  #lock-w     : %lu (%f / s)\n",     aborts_locked_write, aborts_locked_write * 1000.0 / duration);
	printf("  #val-r      : %lu (%f / s)\n",     aborts_validate_read, aborts_validate_read * 1000.0 / duration);
	printf("  #val-w      : %lu (%f / s)\n",     aborts_validate_write, aborts_validate_write * 1000.0 / duration);
	printf("  #val-c      : %lu (%f / s)\n",     aborts_validate_commit, aborts_validate_commit * 1000.0 / duration);
	printf("  #inv-mem    : %lu (%f / s)\n",     aborts_invalid_memory, aborts_invalid_memory * 1000.0 / duration);
	printf("  #dup-w      : %lu (%f / s)\n",     aborts_double_write, aborts_double_write * 1000.0 / duration);
	printf("  #failures   : %lu\n",              failures_because_contention);
	printf("Max retries   : %lu\n",              max_retries);

#ifndef TLS
	pthread_key_delete(rng_seed_key);
#endif /* ! TLS */

	free(threads);
	free(data);
	free(threadID);
	for(int i = 0; i<nb_threads; i++){
		free(data[i].threadID);
	}
	return 0;
}

