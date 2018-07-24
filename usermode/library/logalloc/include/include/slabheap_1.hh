/* 
 * (c) Copyright 2016 Hewlett Packard Enterprise Development LP
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _LOGMANAGEMNET_SLABHEAP_HH_
#define _LOGMANAGEMENT_SLABHEAP_HH_
#include <sys/time.h>
#include <list>
#include <math.h>
#include "assert_nd.hh"

#include "extentheap.hh"

#include "slab.hh"

static uint64_t max_sb_num = 2*1024*1024;
/**
 * @brief Slab heap organizes slabs in per-sizeclass free lists 
 *
 * @details 
 * This class methods are not-thread safe. User is responsible for proper
 * serialization via lock/unlock.
 * 
 */


template<typename Context, template<typename> class TPtr, template<typename> class PPtr>
class SlabHeap
{
public:
 //   Slab *slab = NULL;
    typedef ExtentHeap<Context, TPtr, PPtr> ExtentHeapT;

public:
    SlabHeap(size_t slabsize)
        : slabsize_(slabsize)
	{ 
        int err = pthread_mutex_init(&mutex_, NULL);
        ASSERT_ND(err == 0);
    }

    SlabHeap(size_t slabsize, SlabHeap* parentslabheap, ExtentHeapT* extentheap)
        : slabsize_(slabsize),
          parentslabheap_(parentslabheap),
          extentheap_(extentheap)
    {
        int err = pthread_mutex_init(&mutex_, NULL);
        ASSERT_ND(err == 0);
		alloc_size = 0;
    }

    ErrorCode init(Context &ctx)
    {
        if (extentheap_) {
            for (typename ExtentHeapT::Iterator it = extentheap_->begin(); 
                 it != extentheap_->end(); ++it) 
            {
                if ((*it).nvheader()->is_free() && (*it).nvheader()->size() == slabsize_) 
                {
                    Slab* slab = insert_slab((*it).nvextent());
                    if (!slab) {
                        return kErrorCodeOutofmemory;    
                    }
                }
            }
        }
        return kErrorCodeOk;
    }


    ErrorCode malloc(Context& ctx, size_t size_bytes, TPtr<void>* ptr)
	{
		pthread_mutex_lock(&mutex_);
		Slab* slab = find_slab(size_bytes);
		if (slab == NULL) {
            slab = reuse_empty_slab();
        }
		if (slab == NULL && extentheap_) {
			for(int i = 1; i <= max_sb_num; i++){
				TPtr<void> region;
				uint64_t ret = extentheap_->malloc(ctx, slabsize_, &region);
				if (ret == kErrorCodeOk) {
					slab = Slab::init(region, slabsize_);
					if(slab != NULL)
						insert_slab(slab);
            	}
			}
			std::list<Slab>::iterator iter = current_slabs_.begin();
			last_base = iter->head;
		}
		if(slab != NULL){
		   *ptr = alloc(size_bytes);
        }
		pthread_mutex_unlock(&mutex_);
		return kErrorCodeOk;
    }
/*
	Slab* find_slab(size_t size_bytes)
	{
		if (current_slabs_.size()){
			for(std::list<Slab>::iterator iter = current_slabs_.begin(); iter != current_slabs_.end(); iter++){
				if(iter->offset == 0){
						return &(*iter);
				}
			}
		}
		return NULL;
	}
	
	Slab* find_slab(size_t size_bytes, typename std::list<Slab>& sl)
	{
		Slab &slab = sl.back();
		if(size_bytes <= slab.unusedSlabsize())
			return &slab;

	}


	TPtr<void> alloc(size_t sizebytes)
	{
		size_t left_size = sizebytes;
		TPtr<char> first_base;
		std::list<Slab>::iterator it;
		int i = 0;
		for(std::list<Slab>::iterator iter = current_slabs_.begin(); iter != current_slabs_.end(); iter++){
			if(iter->offset == 0){
				it = iter;
				std::advance(it, -i);
				first_base = static_cast<TPtr<char>>(it->head);
				TPtr<char> base = static_cast<TPtr<char>>((iter->head));
				printf("iter->head is ........................%d\n", (iter->head).get());
				if(left_size > slabsize_){
					iter->offset = iter->offset + slabsize_;
					left_size = left_size - slabsize_;
				}else{
					iter->offset = iter->offset + left_size;
					return TPtr<void>(first_base);
				}
				i++;
			}
		}
	}
*/
	Slab* find_slab(size_t size_bytes)
	{
		if(current_slabs_.size()){
			size_t left_size = max_sb_num * slabsize_ - alloc_size;
			if(left_size < size_bytes)
				return NULL;
			return &(current_slabs_.front());
		}
		return NULL;
	}

	TPtr<void> alloc(size_t sizebytes)
	{
		TPtr<char> base = static_cast<TPtr<char>>(last_base);
//		printf("last_base is ..................%d\n", last_base.get());
		last_base = last_base + sizebytes;
		alloc_size = alloc_size + sizebytes;
		return TPtr<void>(base);
	}

/*
	Slab* findslab(size_t size_bytes, typename std::list<SlabT>& sl)
	{
		if (sl.size()){
			SlabT &slab = sl.back();
			if(size_bytes <= (&slab)->unusedSlabsize())
				return &slab;
		}
		return NULL;
	}
*/    
	void free_slab(void* ptr)
    {
//		printf("ptr is ..............%d\n", ptr);
//		Slab* slab = Slab::init(ptr, slabsize_);
		auto it = current_slabs_.begin();
		while(it != current_slabs_.end()){
			if((it->head).get() == ptr){
				it = current_slabs_.erase(it);
//				printf("free success\n");
				break;
			}else
				++it;
		}
		Slab* slab = Slab::init(it->head, slabsize_);
//        slab->remove();
        insert_slab_to_empty(slab);

    }
	
	void* get_first_ptr()
	{
		auto it = current_slabs_.begin();
		return (it->head).get();
	}

    Slab* insert_slab(TPtr<void> region)
    {
		Slab* slab = Slab::init(region, slabsize_);
		if(slab != NULL){
			slab_list.push_back(*slab);
        	return slab;
		}
		return NULL;
    }
    void insert_slab(Slab* slab)
    {
		current_slabs_.push_back(*slab);
    }


    void move_slab(Slab* slab)
    {
//        slab->remove();
		empty_slabs_.pop_front();
		current_slabs_.push_back(*slab);
//        slab->insert(&current_slabs_);

    }


    friend std::ostream& operator<<(std::ostream& os, const SlabHeap& slabheap)
    {
 //       slabheap.stream_to(os);
        return os;
    }
/*
    void lock()
    {
        pthread_mutex_lock(&mutex_);
    }

    void unlock()
    {
        pthread_mutex_unlock(&mutex_);
    }
*/
private:
    void insert_slab_to_empty(Slab* slab)
    {
//        slab->insert(&empty_slabs_);
    	empty_slabs_.push_front(*slab);
	}

    Slab* reuse_empty_slab()
    {
//        SlabT* slab = NULL;
        if (empty_slabs_.size()) {
        	Slab &slab = empty_slabs_.front();
            move_slab(&slab);
        	return &slab;
		}
		return NULL;
    }
//protected:
public:
    size_t            slabsize_;
	size_t 		      off_;
	SlabHeap*         parentslabheap_;
    ExtentHeapT*      extentheap_;
    pthread_mutex_t   mutex_;
    size_t alloc_size;
	typename std::list<Slab> slab_list;
	typename std::list<Slab> current_slabs_;
	typename std::list<Slab> empty_slabs_;
	TPtr<void> last_base;
	//typename SlabT::SlabList slablist;
//    typename SlabT::SlabList empty_slabs_; 
//    typename SlabT::SlabList current_slabs_; 
};


#endif // _ALPS_LAYER_SLABHEAP_HH_

