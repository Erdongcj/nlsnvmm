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

#ifndef _LOGMANAGEMENT_HYBRIDHEAP_HH_
#define _LOGMANAGEMENT_HYBRIDHEAP_HH_

#include "assert_nd.hh"
#include "error_code.hh"
#include "slabheap.hh"

/**
 * @brief Hybrid heap 
 *
 * @details 
 * 
 */
template<typename Context, template<typename> class TPtr, template<typename> class PPtr, typename SmallHeap, typename BigHeap>
class HybridHeap
{
public:

    HybridHeap(size_t bigsize, SmallHeap* sh, BigHeap* bh)
        : bigsize_(bigsize),
          sh_(sh),
          bh_(bh)
    { }

    ErrorCode malloc(Context& ctx, size_t size, void* ptr)
    //ErrorCode malloc(Context& ctx, size_t size, TPtr<void>* ptr)
    {
        ErrorCode rc;
        rc = sh_->malloc(ctx, size, ptr);
	printf("hybridheap ptr is................................%d\n", ptr);
        return rc;
    }

    void free(Context& ctx, TPtr<void> ptr) 
    {
        return sh_->free(ctx, ptr);
    }

    size_t getsize(TPtr<void> ptr) 
    {
        size_t size = sh_->getsize(ptr);
        return size;
    }
/*	bool append(Context& ctx, const void* buffer, uint32_t entryLength, TPtr<void> ptr)
	{
		return sh_->append(ctx, buffer, entryLength, &ptr);
	}
*/
protected:
    size_t bigsize_;
    SmallHeap* sh_;
    BigHeap* bh_;
};



#endif // LOGMANAGEMENT_HYBRIDHEAP_HH_
