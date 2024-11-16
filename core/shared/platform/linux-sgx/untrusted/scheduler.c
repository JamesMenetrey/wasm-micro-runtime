/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */
#define _GNU_SOURCE

#include <sched.h>

int
ocall_sched_setscheduler(unsigned int pid, int policy, const void* param)
{
    return sched_setscheduler(pid, policy, param);
}

int
ocall_sched_getparam(unsigned int pid, void *param)
{
    return sched_getparam(pid, param);
}

int
ocall_sched_getscheduler(unsigned int pid)
{
    return sched_getscheduler(pid);
}

int
ocall_sched_setaffinity(unsigned int pid,
                         size_t cpu_set_size,
                         const void *mask)
{
    return sched_setaffinity(pid, cpu_set_size, mask);
}
