/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "platform_api_vmcore.h"

#define TRACE_OCALL_FAIL() os_printf("ocall %s failed!\n", __FUNCTION__)

struct sched_param {
	int sched_priority;
	int __reserved1;
#if _REDIR_TIME64
	long __reserved2[4];
#else
	struct {
		time_t __reserved1;
		long __reserved2;
	} __reserved2[2];
#endif
	int __reserved3;
};

int
ocall_sched_setscheduler(int *p_ret, uint pid, int policy, const struct sched_param *param, unsigned int param_size);
int
ocall_sched_getparam(int *p_ret, uint pid, const struct sched_param *param, unsigned int param_size);
int
ocall_sched_getscheduler(int *p_ret, uint pid);

int
os_sched_setscheduler(uint pid, int policy, const void *param)
{
    int ret;

    if (ocall_sched_setscheduler(&ret, pid, policy, param, sizeof(struct sched_param)) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }
    
    if (ret == -1)
        errno = get_errno();

    return ret;
}

int
os_sched_getparam(uint pid, void *param)
{
    int ret;

    if (ocall_sched_getparam(&ret, pid, param, sizeof(struct sched_param)) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }
    
    if (ret == -1)
        errno = get_errno();

    return ret;
}

int
os_sched_getscheduler(uint pid)
{
    int ret;

    if (ocall_sched_getscheduler(&ret, pid) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }
    
    if (ret == -1)
        errno = get_errno();

    return ret;
}
