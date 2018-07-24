#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <inttypes.h>
#include <sys/time.h>
#include <tx_api.h>
#include <wrap.hh>
#include "hashmap_tx.h"

#define INIT_BUCKETS_NUM 1024001
struct entry{
	uint64_t key;
	uint64_t value;
	struct entry* next;
};

struct buckets{
	struct entry* bucket[];
};

struct hashmap_tx{
	struct buckets* buckets;
};

static void create_hashmap(struct hashmap_tx* hashmap)
{
	size_t len = INIT_BUCKETS_NUM;
	size_t sz = sizeof(struct buckets) + len * sizeof(struct entry);
	TX_BEGIN{
	   	TX_ADD(hashmap, sizeof(struct hashmap_tx));
		((struct hashmap_tx*)TX_RW(hashmap))->buckets = (struct buckets*)malloc(sz);
		TX_ADD(((struct hashmap_tx*)TX_RW(hashmap))->buckets, sz);
		for(int i = 0; i < len; i++){
			struct entry* e = (struct entry*)malloc(sizeof(struct entry));
	   		TX_ADD(e, sizeof(struct entry));
	   		struct entry* en = (struct entry*)TX_RW(e);
	   		en->key = 0;
	   		en->value = 0;
	   		en->next = NULL;
	   		((struct buckets*)TX_RW((((struct hashmap_tx*)TX_RW(hashmap))->buckets)))->bucket[i] = e;
	   	}
	}TX_END
}


static uint64_t hash(uint64_t value)
{
	size_t len = INIT_BUCKETS_NUM; 
	return value % len;

}
int hm_tx_insert(struct hashmap_tx* hashmap, uint64_t key, uint64_t value)
{
	size_t len = INIT_BUCKETS_NUM;
	size_t sz = sizeof(struct buckets) + len * sizeof(struct entry);
	uint64_t h = hash(key);
	struct entry* var;
	struct entry* prev = NULL;
	struct entry* e;
	TX_BEGIN{
		struct hashmap_tx* ht = (struct hashmap_tx*)TX_RO(hashmap);
		if(ht != NULL){			
			struct buckets* buckets = ht->buckets;
			struct buckets* bucks = (struct buckets*)TX_RO(buckets);
			if(bucks != NULL){
				var = bucks->bucket[h];
				while(var != NULL){
					struct entry* v = (struct entry*)TX_RO(var);
					if(v != NULL){
						prev = var;
						if(v->key == key){
							break;
						}
						var = v->next;
					}else{
						return 0;
					}

				}
				if(var != NULL){
					
					e = var;
					if(TX_ADD(e, sizeof(struct entry)) == false){
                        return 0;
                    }

					struct entry* cur = (struct entry*)TX_READ(e);
					if(cur == NULL){
						return 0;
					}
//					printf("cur is ............%d, key is .........%d\n", cur->key, key);
					((struct entry*)TX_RW(e))->key = key;
					((struct entry*)TX_RW(e))->value = value;
					((struct entry*)TX_RW(e))->next = cur->next;
				}else{
					
					e = (struct entry*)malloc(sizeof(struct entry));

					if(e == NULL){
						printf("malloc failed\n");
						exit(1);
					}
					if(TX_ADD(e, sizeof(struct entry)) == false){
						return 0;
                    }
					struct entry* en = ((struct entry*)TX_RW(e));
					if(en != NULL){
						en->key = key;
						en->value = value;
						en->next = NULL;
					}else{
						return 0;
					}
					struct entry* pe = (struct entry*)TX_READ(prev);
lock:
					if(TX_ADD(prev, sizeof(struct entry)) == false){
                        return 0;
                    }

					if(pe == NULL){
						return 0;
					}
					if(pe->next != NULL)
					{
						while(pe->next != NULL)
						{
							printf("retry retry\n");
							prev = pe->next;
							pe = (struct entry*)TX_READ(prev);
						}
						goto lock;
					}
					struct entry* p = (struct entry*)TX_RW(prev);
					if(p == NULL){
						return 0;
					}
					p->key = pe->key;
					p->value = pe->value;
					p->next = e;
				}
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}TX_END
	return 1;
}

int hm_tx_remove(struct hashmap_tx* hashmap, uint64_t key)
{
	size_t len = INIT_BUCKETS_NUM;
	size_t sz = sizeof(struct buckets) + len * sizeof(struct entry);

	struct entry* var;
	struct entry* prev = NULL;
	uint64_t h = hash(key);
	int ret;
	TX_BEGIN{
		struct hashmap_tx* ht = (struct hashmap_tx*)TX_RO(hashmap);
		if(ht != NULL){
			struct buckets* buckets = ht->buckets;
			struct buckets* bucks = (struct buckets*)TX_RO(buckets);
			if(bucks != NULL){
				while(var != NULL){
				    struct entry* v = (struct entry*)TX_RO(var);
				    if(v != NULL){
				        prev = var;
				        if(v->key == key){
				            break;
				        }
				        var = v->next;
				    }else{
				        return 0;
				    }
				}

				if(var != NULL){
					struct entry* cur = (struct entry*)TX_READ(var);
					if(cur == NULL){
						return 0;
					}
					struct entry* pe = (struct entry*)TX_READ(prev);
					if(pe == NULL){
						return 0;
					}
					prev->key = pe->key;
					prev->value = pe->value;
					prev->next = cur->next;
					if(TX_ADD(prev, sizeof(struct entry)) == false){
						return 0;
					}
					free(var);
					ret = 1;
				}else{
					ret = 0;
				}
			}else{
				return 0;
			}
		}else{
			return 0;
		}
	}TX_END
	return ret;
}

int hm_tx_get(struct hashmap_tx* hashmap, uint64_t key)
{  
	struct entry* var;
	uint64_t h = hash(key);
	int ret;
	TX_BEGIN{
		struct hashmap_tx* ht = (struct hashmap_tx*)TX_RO(hashmap);
		if(ht != NULL){
			struct buckets* buckets = ht->buckets;
			struct buckets* bucks = (struct buckets*)TX_RO(buckets);
			if(bucks != NULL){
				var = bucks->bucket[h];
				while(var != NULL){
					struct entry* v = (struct entry*)TX_RO(var);
					if(v != NULL){
						if(v->key == key){
							break;
						}
						var = v->next;
					}else{
						return 0;
					}
				}
				if(var == NULL){
					ret = 0;
				}else{
					ret = 1;
				}
			}else{
				return 0;
			}   
		}else{
			return 0;
		}
	}TX_END
	return ret;
}   

struct hashmap_tx* hm_tx_create()
{
	struct hashmap_tx* hashmap = (struct hashmap_tx*)malloc(sizeof(struct hashmap_tx));
	create_hashmap(hashmap);
	return hashmap;
}
