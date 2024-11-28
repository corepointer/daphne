
#include "CreateCUDAContext.h"

namespace CUDA {
void createCUDAContext(DCTX(ctx)) {
    // ToDo: one context per device
    if (ctx->getUserConfig().log_ptr)
        ctx->getUserConfig().log_ptr->registerLoggers();
    for (uint32_t i = 0; i < ctx->getUserConfig().available_cuda_devices; ++i) {
        ctx->cuda_contexts.emplace_back(CUDAContext::createCudaContext(i));
    }
}
} // namespace CUDA