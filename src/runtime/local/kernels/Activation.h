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

#pragma once

#include "runtime/local/context/DaphneContext.h"
#include "runtime/local/datastructures/DataObjectFactory.h"
#include "runtime/local/datastructures/DenseMatrix.h"

namespace NN::Activation {
    struct ReLU {
        static inline int getActivationType() { /* ToDo: ReLU activation */ return 0; }
    };

    template<typename OP, typename DTRes, typename DTArg>
    struct Forward {
        static void apply(DTRes *&res, [[maybe_unused]] const DTArg *data, DCTX(dctx)) {
            throw std::runtime_error("C++ ReLU activation not implemented"); }
    };

    template<typename OP, typename DTRes, typename DTArg>
    struct Backward {
        static void apply(DTRes *&res, const DTArg *grad_in, const DTArg *data_in, DCTX(dctx)) {
            throw std::runtime_error("C++ ReLU activation not implemented"); }
    };
}
