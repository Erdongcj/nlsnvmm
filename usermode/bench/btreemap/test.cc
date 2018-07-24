#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <pthread.h>
#include <assert.h>
#include <wrap.hh>
#include <tx_api.h>
#include <numals.h>
#include "btree_map.h"

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

int main(int argc, char **argv)
{
	int cpu_status[3]; /* cpu_status = [# physical cpu, # cores per node, hyperthreading] */
//	pthread_t thread[2];
	cpu_status[0] = cpu_status[1] = cpu_status[2] = 0;
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
	threadID_t* threadID = (threadID_t*)malloc(sizeof(threadID_t));
	init_thread(threadID, 0);

	struct btree_map* bmap = btree_map_create();
//	free((void*)threadID);
//	threadID_t* mythreadID = (threadID_t*)malloc(sizeof(threadID_t));
	
//	init_thread(mythreadID, 0);
	btree_map_insert(bmap, 2, 2);
	btree_map_insert(bmap, 3, 3);
	struct timeval begin1, end1;
	float diff1;
	gettimeofday(&begin1, NULL);

	btree_map_insert(bmap, 78, 4);
	
	gettimeofday(&end1, NULL);
	diff1 = (end1.tv_sec - begin1.tv_sec)*1000000 + (end1.tv_usec - begin1.tv_usec);
//	printf(" times diff..........insert..............................................................: %f\n", diff1);
	
	btree_map_insert(bmap, 12, 22);
//
	btree_map_insert(bmap, 160, 66);
	btree_map_insert(bmap, 24, 60);
	btree_map_insert(bmap, 17, 33);
	btree_map_insert(bmap, 15, 55);
	btree_map_insert(bmap, 33, 50);
//	uint64_t v3 = hm_tx_get(hsm, 12);
//	printf("v3 is ................%d\n", v3);
	struct timeval begin2, end2;
	float diff2;
	gettimeofday(&begin2, NULL);
	btree_map_insert(bmap, 11, 50);
	btree_map_insert(bmap, 22, 50);
	btree_map_insert(bmap, 35, 50);
	btree_map_insert(bmap, 16, 50);
	btree_map_insert(bmap, 6, 50);
	btree_map_insert(bmap, 8, 50);
	btree_map_insert(bmap, 90, 50);
	btree_map_insert(bmap, 75, 50);
	btree_map_insert(bmap, 61, 50);
	btree_map_insert(bmap, 52, 50);
	btree_map_insert(bmap, 18, 50);
	btree_map_insert(bmap, 46, 50);
	btree_map_insert(bmap, 40, 50);
	btree_map_insert(bmap, 129, 50);
	btree_map_insert(bmap, 80, 50);
	btree_map_insert(bmap, 500, 50);
	btree_map_insert(bmap, 5, 50);
	btree_map_insert(bmap, 21, 50);
	btree_map_insert(bmap, 1, 50);
	btree_map_insert(bmap, 37, 50);
	btree_map_insert(bmap, 48, 50);
	btree_map_insert(bmap, 25, 50);
	btree_map_insert(bmap, 101, 50);
	btree_map_insert(bmap, 20, 50);
	btree_map_insert(bmap, 100, 50);
	srand((unsigned)time(NULL));
	for(int i=0; i<100; i++){
//		srand((unsigned)time(NULL));
//		int j = rand() % 1000;
		btree_map_insert(bmap, rand()%1000, 50);
	}
//	for(int i=4; i<1001; i++){
//		btree_map_insert(bmap, i/3, 4*i);
//		btree_map_insert(bmap, i, 4*i);
//	}
	gettimeofday(&end2, NULL);
	diff2 = (end2.tv_sec - begin2.tv_sec)*1000000 + (end2.tv_usec - begin2.tv_usec);
	printf(" times diff..................insert 1000 entries......................................................: %f\n", diff2);
//	uint64_t ret = hm_tx_remove(hsm, 160);
//	printf("ret is ................%d\n", ret);
//	for(int i = 4; i< 1001; i++){
//		uint64_t v2 = btree_map_get(bmap, i);
//	}
	uint64_t v2 = btree_map_get(bmap, 61);
	free(threadID);
	return 0;
}

