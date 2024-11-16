/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#define _GNU_SOURCE

#include <stdlib.h>
#include <pthread.h>

int
ocall_pthread_rwlock_init(void **rwlock, void *attr)
{
    int ret = 0;

    *rwlock = malloc(sizeof(pthread_rwlock_t));
    if (*rwlock == NULL)
        return -1;

    ret = pthread_rwlock_init((pthread_rwlock_t *)*rwlock, NULL);
    if (ret != 0) {
        free(*rwlock);
        *rwlock = NULL;
    }
    (void)attr;
    return ret;
}

int
ocall_pthread_rwlock_destroy(void *rwlock)
{
    pthread_rwlock_t *lock = (pthread_rwlock_t *)rwlock;
    int ret;

    ret = pthread_rwlock_destroy(lock);
    free(lock);
    return ret;
}

int
ocall_pthread_rwlock_rdlock(void *rwlock)
{
    return pthread_rwlock_rdlock((pthread_rwlock_t *)rwlock);
}

int
ocall_pthread_rwlock_wrlock(void *rwlock)
{
    return pthread_rwlock_wrlock((pthread_rwlock_t *)rwlock);
}

int
ocall_pthread_rwlock_unlock(void *rwlock)
{
    return pthread_rwlock_unlock((pthread_rwlock_t *)rwlock);
}

#include <stdio.h>

int
ocall_pthread_setaffinity_np(pthread_t thread, unsigned int cpu_set_size, const void *cpu_set)
{
    printf(">>> pthread_self: %ld\n", pthread_self());
    printf(">>> ocall_pthread_setaffinity_np(%ld)\n", thread);
    printf(">>> Trying to read cpu_set of size %d:", cpu_set_size);
    for (int i = 0; i < 128; i++) printf("%d,", ((char*)cpu_set)[i]);
    printf("\n");
    int ret = pthread_setaffinity_np(thread, cpu_set_size, cpu_set);
    printf(">>> AFTER ocall_pthread_setaffinity_np(%ld)\n", thread);
    return ret;
}