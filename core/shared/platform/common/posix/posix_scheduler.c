/*
 * Copyright (C) 2023 Midokura Japan KK.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <sched.h>

#include "platform_api_extension.h"

int
os_sched_setscheduler(uint pid, int policy, const void* param)
{
    return sched_setscheduler(pid, policy, param);
}

int
os_sched_getparam(uint pid, void *param)
{
    return sched_getparam(pid, param);
}

int
os_sched_getscheduler(uint pid)
{
    return sched_getscheduler(pid);
}

int os_sched_setaffinity(uint pid,
                         uint cpu_set_size,
                         const void *mask)
{
    return sched_setaffinity(pid, cpu_set_size, mask);
}
