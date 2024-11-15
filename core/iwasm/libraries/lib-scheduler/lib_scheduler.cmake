# Copyright (C) 2019 Intel Corporation.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set (LIB_SCHEDULER_DIR ${CMAKE_CURRENT_LIST_DIR})

add_definitions (-DWASM_ENABLE_LIB_SCHEDULER=1)

include_directories(${LIB_SCHEDULER_DIR})

file (GLOB source_all ${LIB_SCHEDULER_DIR}/*.c)

set (LIB_SCHEDULER_SOURCE ${source_all})

