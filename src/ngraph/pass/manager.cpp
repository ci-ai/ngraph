// ----------------------------------------------------------------------------
// Copyright 2017 Nervana Systems Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// ----------------------------------------------------------------------------

#include <iostream>
#include <memory>

#include "ngraph/log.hpp"
#include "ngraph/pass/manager.hpp"
#include "ngraph/node.hpp"

using namespace std;

ngraph::pass::Manager::Manager()
{
}

ngraph::pass::Manager::~Manager()
{
}

void ngraph::pass::Manager::initialize_default_passes()
{
}

void ngraph::pass::Manager::register_pass(std::shared_ptr<TreeBase> p)
{
    if (p == nullptr)
    {
        throw invalid_argument("null pass registered");
    }
    p->check_dependencies(m_tree_passes);
    m_tree_passes.push_back(p);
}

void ngraph::pass::Manager::register_pass(std::shared_ptr<CallBase> p)
{
    if (p == nullptr)
    {
        throw invalid_argument("null pass registered");
    }
    p->check_dependencies(m_call_passes);
    m_call_passes.push_back(p);
}

void ngraph::pass::Manager::run_passes(std::shared_ptr<Node> nodes)
{
    for (shared_ptr<TreeBase> p : m_tree_passes)
    {
        p->run_on_tree(nodes);
        if (p->call_graph_produced())
        {
            m_sorted_list = p->get_call_graph();
        }
    }

    for (shared_ptr<CallBase>& p : m_call_passes)
    {
        p->run_on_call_list(m_sorted_list);
    }
}

const std::list<ngraph::Node*>& ngraph::pass::Manager::get_sorted_list() const
{
    return m_sorted_list;
}