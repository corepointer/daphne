/*
 * Copyright 2022 The DAPHNE Consortium
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

#pragma once

#include <cstdint>
#include "DataPlacement.h"
#include "runtime/local/context/CUDAContext.h"

class AllocationDescriptorCUDA : public IAllocationDescriptor {
    ALLOCATION_TYPE type = ALLOCATION_TYPE::GPU_CUDA;
    uint32_t device_id{};
    DaphneContext* dctx{};
    std::shared_ptr<std::byte> data{};
    size_t alloc_id{};

public:
    AllocationDescriptorCUDA() = delete;

    AllocationDescriptorCUDA(DaphneContext* ctx, uint32_t device_id) : device_id(device_id), dctx(ctx) { }

    ~AllocationDescriptorCUDA() override {
        // ToDo: for now we free if this is the last context-external ref to the buffer
        if(data.use_count() == 2) {
            CUDAContext::get(dctx, device_id)->free(alloc_id);
        }
    }

    [[nodiscard]] ALLOCATION_TYPE getType() const override { return type; }

    // [[nodiscard]] uint32_t getLocation() const { return device_id; }
    [[nodiscard]] std::string getLocation() const override { return std::to_string(device_id); }

    void createAllocation(size_t size, bool zero) override {
        auto ctx = CUDAContext::get(dctx, device_id);
        data = ctx->malloc(size, zero, alloc_id);
    }

    std::shared_ptr<std::byte> getData() override { return data; }

    [[nodiscard]] std::unique_ptr<IAllocationDescriptor> clone() const override {
        return std::make_unique<AllocationDescriptorCUDA>(*this);
    }

    void transferTo(std::byte* src, size_t size) override {
//        CHECK_CUDART(cudaMemcpy(data.get(), src, size, cudaMemcpyHostToDevice));
        auto dev_ptr = data.get();
        
#ifndef NDEBUG
        std::ios state(nullptr);
        state.copyfmt(std::cout);
        std::cout << "calling cudaMemcpy" << "\n\tdev_ptr: " << dev_ptr << "\n\tsrc_ptr: " << src <<
        "\n\tsize: " << size << std::endl;
        std::cout << "addressof dev_ptr in cudaMemcpy: " << &dev_ptr << std::endl;
#endif
//        if(size == 2912) {
////        std::cout << "freeing" << std::endl;
////        CHECK_CUDART(cudaFree(dev_ptr));
//            auto dsize = size / sizeof(double);
//            auto dptr = reinterpret_cast<double*>(src);
//            auto num_items = std::min(static_cast<size_t>(dsize), 5ul);
//            std::vector<double> tmpvec;
//            for(auto i = 0ul; i < num_items-1; i++) {
//                std::cout << dptr[i] << ", ";
//                tmpvec.push_back(dptr[i]);
//            }
//            std::cout << dptr[num_items] << std::endl;
//            tmpvec.push_back(dptr[num_items]);
//
//            double* myptr;
//            std::cout << "mallocing" << std::endl;
//            CHECK_CUDART(cudaMalloc(reinterpret_cast<void**>(&myptr), size));
//            std::cout << "myptr: " << myptr << std::endl;
//            std::cout << "memsetting first" << std::endl;
//            CHECK_CUDART(cudaMemset(myptr, 0, size));
//            std::cout << "copying..." << std::endl;
//            CHECK_CUDART(cudaMemcpy(myptr, tmpvec.data(), size, cudaMemcpyHostToDevice));
//            std::cout << "resetting" << std::endl;
//            data.reset(reinterpret_cast<std::byte*>(myptr));
//        }
//        else
            CHECK_CUDART(cudaMemcpy(dev_ptr, src, size, cudaMemcpyHostToDevice));
#ifndef NDEBUG
        size_t available; size_t total;
        cudaMemGetInfo(&available, &total);
        std::cout << "Available mem: " << (available / (1048576)) << "Mb" << std::endl;
        std::cout.copyfmt(state);
#endif
    }
    void transferFrom(std::byte* dst, size_t size) override {
        CHECK_CUDART(cudaMemcpy(dst, data.get(), size, cudaMemcpyDeviceToHost));
    };

    bool operator==(const IAllocationDescriptor* other) const override {
        if(getType() == other->getType())
            return(getLocation() == dynamic_cast<const AllocationDescriptorCUDA *>(other)->getLocation());
        return false;
    }
};
