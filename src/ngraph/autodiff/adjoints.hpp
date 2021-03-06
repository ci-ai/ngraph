/*******************************************************************************
* Copyright 2017-2018 Intel Corporation
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
*******************************************************************************/

#pragma once

#include <memory>
#include <unordered_map>

#include "ngraph/coordinate.hpp"
#include "ngraph/strides.hpp"

namespace ngraph
{
    class Node;
    class Function;

    namespace runtime
    {
        class Backend;
        class Manager;
    }

    namespace autodiff
    {
        class Adjoints
        {
        public:
            /// @brief (dy/dx)(c) for all x used to compute y
            ///
            /// @param y The dependent value
            /// @param c An expression for where to evaluate the derivatives
            Adjoints(const std::shared_ptr<Node>& y, const std::shared_ptr<Node>& c);

            Adjoints(const Adjoints& adjoints) = default;
            Adjoints& operator=(const Adjoints& adjoints) = default;
            Adjoints() = default;

            /// @brief (dy/dx)(c)
            ///
            /// @param x The node whose adjoint is desired.
            std::shared_ptr<Node> get(const std::shared_ptr<Node>& x);

            /// @brief Add a backprop contribution to x's adjoint
            ///
            /// @param x The adjoint node
            /// @param delta A backprop contribution
            void add_delta(const std::shared_ptr<Node>& x, const std::shared_ptr<Node>& delta);

            /// @brief Add a backprop contribution to a slice of x's adjoint
            ///
            /// @param x The adjoint node
            /// @param delta A backprop contribution
            /// @param lower_bounds Lower bounds of slice to add to
            /// @param upper_bounds Upper bounds of slice to add to
            /// @param strides Strides of slice to add to
            void add_delta_to_slice(const std::shared_ptr<Node>& x,
                                    const std::shared_ptr<Node>& delta,
                                    const Coordinate& lower_bounds,
                                    const Coordinate& upper_bounds,
                                    const Strides& strides);

        protected:
            std::unordered_map<Node*, std::shared_ptr<Node>> m_adjoint_map;
        };
    }
}
