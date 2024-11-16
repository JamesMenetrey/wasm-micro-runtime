/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "bh_common.h"
#include "bh_log.h"
#include "wasm_export.h"
#include "../interpreter/wasm.h"
#include "../common/wasm_runtime_common.h"

static int
sched_setscheduler_wrapper(wasm_exec_env_t exec_env,
                uint pid, 
                int policy,
                const void *param)
{
    return os_sched_setscheduler(pid, policy, param);
}

static int
sched_getscheduler_wrapper(wasm_exec_env_t exec_env,
                    uint pid)
{
    return os_sched_getscheduler(pid);
}

static int
sched_getparam_wrapper(wasm_exec_env_t exec_env,
                uint pid,
                void *param)
{
    return os_sched_getparam(pid, param);
}

static int sched_setaffinity_wrapper(uint pid,
                             size_t cpu_set_size,
                             const void *mask)
{
    return os_sched_setaffinity(pid, cpu_set_size, mask);
}

/* clang-format off */
#define REG_NATIVE_FUNC(func_name, signature) \
    { #func_name, func_name##_wrapper, signature, NULL }
/* clang-format on */

static NativeSymbol native_symbols_lib_scheduler[] = {
    REG_NATIVE_FUNC(sched_setscheduler, "(ii*)i"),
    REG_NATIVE_FUNC(sched_getscheduler, "(i)i"),
    REG_NATIVE_FUNC(sched_getparam, "(i*)i"),
    REG_NATIVE_FUNC(sched_setaffinity, "(ii*)i"),
};

uint32
get_lib_scheduler_export_apis(NativeSymbol **p_lib_scheduler_apis)
{
    *p_lib_scheduler_apis = native_symbols_lib_scheduler;
    return sizeof(native_symbols_lib_scheduler) / sizeof(NativeSymbol);
}
