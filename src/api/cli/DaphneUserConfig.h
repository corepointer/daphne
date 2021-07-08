/*
 * Copyright 2021 The DAPHNE Consortium
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


#ifndef DAPHNE_PROTOTYPE_DAPHNEUSERCONFIG_H
#define DAPHNE_PROTOTYPE_DAPHNEUSERCONFIG_H

#pragma once

#ifdef USE_CUDA
	#include <runtime/local/kernels/CUDA_Context.h>
	class CUDAContext;
#endif

#include <vector>
#include <string>
#include <memory>

/*
 * Container to pass around user configuration
 */
struct DaphneUserConfig {

	bool use_cuda = false;
#ifdef USE_CUDA
	// User config holds once context atm for convenience until we have proper system infrastructure
	std::unique_ptr<CUDAContext> context = nullptr;

	// CUDA device IDs (future work, as we create only one context atm)
	std::vector<int> cuda_devices;
#endif
	std::string build_output_dir;
};
#endif //DAPHNE_PROTOTYPE_DAPHNEUSERCONFIG_H
