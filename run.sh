#!/usr/bin/env bash 

script=$(pwd)/scripts/deliverables/del_7_2_example.daphne

customer=inCustomer=\"$(pwd)/benchmarks/tpc-h/data/customer.csv\"
orders=inOrders=\"$(pwd)/benchmarks/tpc-h/data/orders.csv\"

bin/daphne --api MorphStore --libdir lib $script $customer $orders
#bin/daphne --libdir lib $script $customer $orders

bin/daphne --api MorphStore $script $customer $orders

