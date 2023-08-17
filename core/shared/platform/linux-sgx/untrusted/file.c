/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sched.h>
#include <poll.h>
#include <errno.h>

int
ocall_open(const char *pathname, int flags, bool has_mode, unsigned mode)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    if (has_mode) {
        return open(pathname, flags, (mode_t)mode);
    }
    else {
        return open(pathname, flags);
    }
}

int
ocall_openat(int dirfd, const char *pathname, int flags, bool has_mode,
             unsigned mode)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    if (has_mode) {
        return openat(dirfd, pathname, flags, (mode_t)mode);
    }
    else {
        return openat(dirfd, pathname, flags);
    }
}

int
ocall_close(int fd)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return close(fd);
}

ssize_t
ocall_read(int fd, void *buf, size_t read_size)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    if (buf != NULL) {
        return read(fd, buf, read_size);
    }
    else {
        return -1;
    }
}

off_t
ocall_lseek(int fd, off_t offset, int whence)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return lseek(fd, offset, whence);
}

int
ocall_ftruncate(int fd, off_t length)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return ftruncate(fd, length);
}

int
ocall_fsync(int fd)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return fsync(fd);
}

int
ocall_fdatasync(int fd)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return fdatasync(fd);
}

int
ocall_isatty(int fd)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return isatty(fd);
}

void
ocall_fdopendir(int fd, void **dirp)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    if (dirp) {
        *(DIR **)dirp = fdopendir(fd);
    }
}

void *
ocall_readdir(void *dirp)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    DIR *p_dirp = (DIR *)dirp;
    return readdir(p_dirp);
}

void
ocall_rewinddir(void *dirp)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    DIR *p_dirp = (DIR *)dirp;
    if (p_dirp) {
        rewinddir(p_dirp);
    }
}

void
ocall_seekdir(void *dirp, long loc)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    DIR *p_dirp = (DIR *)dirp;

    if (p_dirp) {
        seekdir(p_dirp, loc);
    }
}

long
ocall_telldir(void *dirp)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    DIR *p_dirp = (DIR *)dirp;
    if (p_dirp) {
        return telldir(p_dirp);
    }
    return -1;
}

int
ocall_closedir(void *dirp)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    DIR *p_dirp = (DIR *)dirp;
    if (p_dirp) {
        return closedir(p_dirp);
    }
    return -1;
}

int
ocall_stat(const char *pathname, void *buf, unsigned int buf_len)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return stat(pathname, (struct stat *)buf);
}

int
ocall_fstat(int fd, void *buf, unsigned int buf_len)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return fstat(fd, (struct stat *)buf);
}

int
ocall_fstatat(int dirfd, const char *pathname, void *buf, unsigned int buf_len,
              int flags)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return fstatat(dirfd, pathname, (struct stat *)buf, flags);
}

int
ocall_mkdirat(int dirfd, const char *pathname, unsigned mode)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return mkdirat(dirfd, pathname, (mode_t)mode);
}

int
ocall_link(const char *oldpath, const char *newpath)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return link(oldpath, newpath);
}

int
ocall_linkat(int olddirfd, const char *oldpath, int newdirfd,
             const char *newpath, int flags)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return linkat(olddirfd, oldpath, newdirfd, newpath, flags);
}

int
ocall_unlinkat(int dirfd, const char *pathname, int flags)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return unlinkat(dirfd, pathname, flags);
}

ssize_t
ocall_readlink(const char *pathname, char *buf, size_t bufsiz)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return readlink(pathname, buf, bufsiz);
}

ssize_t
ocall_readlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return readlinkat(dirfd, pathname, buf, bufsiz);
}

int
ocall_renameat(int olddirfd, const char *oldpath, int newdirfd,
               const char *newpath)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return renameat(olddirfd, oldpath, newdirfd, newpath);
}

int
ocall_symlinkat(const char *target, int newdirfd, const char *linkpath)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return symlinkat(target, newdirfd, linkpath);
}

int
ocall_ioctl(int fd, unsigned long request, void *arg, unsigned int arg_len)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    /* support just int *arg temporally */
    return ioctl(fd, request, (int *)arg);
}

int
ocall_fcntl(int fd, int cmd)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return fcntl(fd, cmd);
}

int
ocall_fcntl_long(int fd, int cmd, long arg)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return fcntl(fd, cmd, arg);
}

ssize_t
ocall_readv(int fd, char *iov_buf, unsigned int buf_size, int iovcnt,
            bool has_offset, off_t offset)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    struct iovec *iov = (struct iovec *)iov_buf;
    ssize_t ret;
    int i;

    for (i = 0; i < iovcnt; i++) {
        iov[i].iov_base = iov_buf + (unsigned)(uintptr_t)iov[i].iov_base;
    }

    if (has_offset)
        ret = preadv(fd, iov, iovcnt, offset);
    else
        ret = readv(fd, iov, iovcnt);

    return ret;
}

ssize_t
ocall_writev(int fd, char *iov_buf, unsigned int buf_size, int iovcnt,
             bool has_offset, off_t offset)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    struct iovec *iov = (struct iovec *)iov_buf;
    int i;
    ssize_t ret;

    for (i = 0; i < iovcnt; i++) {
        iov[i].iov_base = iov_buf + (unsigned)(uintptr_t)iov[i].iov_base;
    }

    if (has_offset)
        ret = pwritev(fd, iov, iovcnt, offset);
    else
        ret = writev(fd, iov, iovcnt);

    return ret;
}

int
ocall_realpath(const char *path, char *buf, unsigned int buf_len)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    char *val = NULL;
    val = realpath(path, buf);
    if (val != NULL) {
        return 0;
    }
    return -1;
}

int
ocall_posix_fallocate(int fd, off_t offset, off_t len)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return posix_fallocate(fd, offset, len);
}

int
ocall_poll(void *fds, unsigned nfds, int timeout, unsigned int fds_len)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return poll((struct pollfd *)fds, (nfds_t)nfds, timeout);
}

int
ocall_getopt(int argc, char *argv_buf, unsigned int argv_buf_len,
             const char *optstring)
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    int ret;
    int i;
    char **argv = (char **)argv_buf;

    for (i = 0; i < argc; i++) {
        argv[i] = argv_buf + (uintptr_t)argv[i];
    }

    return getopt(argc, argv, optstring);
}

int
ocall_sched_yield()
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return sched_yield();
}

int
ocall_get_errno()
{
    printf("\n >>> SGX OCALL DETECTED: %s.\n", __func__);
    return errno;
}
