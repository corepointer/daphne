/*
 * Copyright 2024 The DAPHNE Consortium
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

#include <runtime/local/datastructures/DataPlacement.h>
#include <runtime/local/datastructures/Structure.h>

Structure::Structure(size_t numRows, size_t numCols) : refCounter(1), numRows(numRows), numCols(numCols) {
    mdo = std::make_shared<MetaDataObject>();
}

bool Structure::isPinned(const IAllocationDescriptor *alloc_desc) const {
    return alloc_desc ? pinned_allocation == alloc_desc->getType() : pinned_allocation == ALLOCATION_TYPE::HOST;
}

void Structure::pin(const IAllocationDescriptor *alloc_desc) {
    alloc_desc ? pinned_allocation = alloc_desc->getType() : pinned_allocation = ALLOCATION_TYPE::HOST;
}