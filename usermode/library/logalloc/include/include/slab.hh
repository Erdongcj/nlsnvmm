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

#ifndef _SLAB_H_
#define _SLAB_H_
#include <stdio.h>
//#include <math.h>
//#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <algorithm>
#include <atomic>
#include <list>
#include <iostream>
#include <signal.h>
#include <sys/time.h>
//#include "debug.hh"


class Slab{
public:
	TPtr<void> head;
	size_t offset;
	size_t slabsize;

public:
	Slab(TPtr<void> region, size_t slab_size)
		: head(region),
		  slabsize(slab_size),
		  offset(0)
	{ }
	static Slab* init(TPtr<void> region, size_t slab_size)
	{
		 Slab *slab = new Slab(region, slab_size);
		 assert(slab != NULL);
//		 slab->head = region;
//		 slab->slabsize = slab_size;
//		 slab->offset = 0;
		 return slab;

	}

	size_t unusedSlabsize()
	{   
		return slabsize - offset;
	}
/*
	void* alloc(size_t sizebytes)
	{
		Slab *slab = &(slab_list_->back());
		uint64_t ret = reinterpret_cast<uint64_t>((char*)(slab.head)) + offset;
		offset += sizebytes;
		void *ptr = reinterpret_cast<void*>(ret);
		return ptr;
	}
*/

	TPtr<void> alloc(size_t sizebytes, std::list<Slab>& slab_list)
	{
		printf("offset is .........%d\n", offset);
		Slab &slab = slab_list.back();
		TPtr<char> base = static_cast<TPtr<char>>((slab.head));
//		TPtr<char> base = static_cast<TPtr<char>>((char*)(&slab));
		TPtr<char> ptr = base + offset;
		offset = offset + sizebytes;

		return TPtr<void>(ptr);
	}


};



#endif // _ALPS_LAYERS_BITS_SLAB_H_

