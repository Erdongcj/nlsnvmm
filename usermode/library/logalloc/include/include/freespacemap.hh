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

#ifndef LOGMANAGEMENT_FREESPACEMAP_HH_
#define LOGMANAGEMENT_FREESPACEMAP_HH_

#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "error_code.hh"
#include "error_stack.hh"

//#include "debug.hh"


#include "extentinterval.hh"
#include "extentmap.hh"


template<template<typename> class TPtr>
class FreeSpaceMap: public ExtentMap {
public:
    bool exists_extent(size_t size_nblocks)
    {
        ExtentInterval ex;
        return find_ge(size_nblocks, &ex) == 0 ? true : false;
    }

    int alloc_extent(size_t size_nblocks, ExtentInterval* ex)
    {
        if (remove_ge(size_nblocks, ex) == 0) {
            // if returned size is larger than requested size then return the
            // rest of the space back to the extendmap
            if (ex->len() > size_nblocks) {
                insert(ExtentInterval(ex->start() + size_nblocks, ex->len() - size_nblocks));
            }
            *ex = ExtentInterval(ex->start(), size_nblocks);
            return 0;   
        }
        return -1;
    }

    void free_extent(const ExtentInterval& ex)
    {
        insert(ex);
    }
};



#endif // LOGMANAGEMENT_FREESPACEMAP_HH_
