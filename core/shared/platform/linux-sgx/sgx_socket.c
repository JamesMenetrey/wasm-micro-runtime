/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "platform_api_vmcore.h"

#ifndef SGX_DISABLE_WASI

#define TRACE_OCALL_FAIL() os_printf("ocall %s failed!\n", __FUNCTION__)

int
ocall_socket(int *p_ret, int domain, int type, int protocol);
int
ocall_getsockopt(int *p_ret, int sockfd, int level, int optname, void *val_buf,
                 unsigned int val_buf_size, void *len_buf);

int
ocall_sendmsg(ssize_t *p_ret, int sockfd, void *msg_buf,
              unsigned int msg_buf_size, int flags);
int
ocall_recvmsg(ssize_t *p_ret, int sockfd, void *msg_buf,
              unsigned int msg_buf_size, int flags);
int
ocall_shutdown(int *p_ret, int sockfd, int how);

int
ocall_inet_network(uint32_t* retval, const void* cp, int cp_size);

int
ocall_inet_addr(uint32_t* retval, const void* cp, int cp_size);

int
ocall_fcntl_long(int *p_ret, int fd, int cmd, long arg);

int
ocall_setsockopt(int *p_ret, int sockfd, int level, int optname,
                 void *optval, unsigned int optlen);

int
ocall_accept(int *p_ret, int sockfd, void *addr, uint32_t *addrlen,
                 uint32_t addr_size);

uint16_t
ocall_htons(uint16_t *p_ret, uint16_t hostshort);

uint16_t
ocall_ntohs(uint16_t *p_ret, uint16_t netshort);

int
ocall_bind(int *p_ret, int sockfd, const void *addr, uint32_t addrlen);

int
ocall_getsockname(int *p_ret, int sockfd, void *addr, uint32_t *addrlen, uint32_t addr_size);

int
ocall_listen(int *p_ret, int sockfd, int backlog);

int
socket(int domain, int type, int protocol)
{
    int ret;

    if (ocall_socket(&ret, domain, type, protocol) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    if (ret == -1)
        errno = get_errno();

    return ret;
}

int
getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen)
{
    int ret;
    unsigned int val_buf_size = *optlen;

    if (ocall_getsockopt(&ret, sockfd, level, optname, optval, val_buf_size,
                         (void *)optlen)
        != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    if (ret == -1)
        errno = get_errno();

    return ret;
}

ssize_t
sendmsg(int sockfd, const struct msghdr *msg, int flags)
{
    ssize_t ret;
    int i;
    char *p;
    struct msghdr *msg1;

    uint64 total_size = sizeof(struct msghdr) + (uint64)msg->msg_namelen
                        + (uint64)msg->msg_controllen;

    total_size += sizeof(struct iovec) * (msg->msg_iovlen);

    for (i = 0; i < msg->msg_iovlen; i++) {
        total_size += msg->msg_iov[i].iov_len;
    }

    if (total_size >= UINT32_MAX)
        return -1;

    msg1 = BH_MALLOC((uint32)total_size);

    if (msg1 == NULL)
        return -1;

    p = (char *)(uintptr_t)sizeof(struct msghdr);

    if (msg->msg_name != NULL) {
        msg1->msg_name = p;
        memcpy((uintptr_t)p + (char *)msg1, msg->msg_name,
               (size_t)msg->msg_namelen);
        p += msg->msg_namelen;
    }

    if (msg->msg_control != NULL) {
        msg1->msg_control = p;
        memcpy((uintptr_t)p + (char *)msg1, msg->msg_control,
               (size_t)msg->msg_control);
        p += msg->msg_controllen;
    }

    if (msg->msg_iov != NULL) {
        msg1->msg_iov = (struct iovec *)p;
        p += (uintptr_t)(sizeof(struct iovec) * (msg->msg_iovlen));

        for (i = 0; i < msg->msg_iovlen; i++) {
            msg1->msg_iov[i].iov_base = p;
            msg1->msg_iov[i].iov_len = msg->msg_iov[i].iov_len;
            memcpy((uintptr_t)p + (char *)msg1, msg->msg_iov[i].iov_base,
                   (size_t)(msg->msg_iov[i].iov_len));
            p += msg->msg_iov[i].iov_len;
        }
    }

    if (ocall_sendmsg(&ret, sockfd, (void *)msg1, (uint32)total_size, flags)
        != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    if (ret == -1)
        errno = get_errno();

    return ret;
}

ssize_t
recvmsg(int sockfd, struct msghdr *msg, int flags)
{
    ssize_t ret;
    int i;
    char *p;
    struct msghdr *msg1;

    uint64 total_size = sizeof(struct msghdr) + (uint64)msg->msg_namelen
                        + (uint64)msg->msg_controllen;

    total_size += sizeof(struct iovec) * (msg->msg_iovlen);

    for (i = 0; i < msg->msg_iovlen; i++) {
        total_size += msg->msg_iov[i].iov_len;
    }

    if (total_size >= UINT32_MAX)
        return -1;

    msg1 = BH_MALLOC((uint32)total_size);

    if (msg1 == NULL)
        return -1;

    memset(msg1, 0, total_size);

    p = (char *)(uintptr_t)sizeof(struct msghdr);

    if (msg->msg_name != NULL) {
        msg1->msg_name = p;
        p += msg->msg_namelen;
    }

    if (msg->msg_control != NULL) {
        msg1->msg_control = p;
        p += msg->msg_controllen;
    }

    if (msg->msg_iov != NULL) {
        msg1->msg_iov = (struct iovec *)p;
        p += (uintptr_t)(sizeof(struct iovec) * (msg->msg_iovlen));

        for (i = 0; i < msg->msg_iovlen; i++) {
            msg1->msg_iov[i].iov_base = p;
            msg1->msg_iov[i].iov_len = msg->msg_iov[i].iov_len;
            p += msg->msg_iov[i].iov_len;
        }
    }

    if (ocall_recvmsg(&ret, sockfd, (void *)msg1, (uint32)total_size, flags)
        != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    p = (char *)(uintptr_t)(sizeof(struct msghdr));

    if (msg1->msg_name != NULL) {
        memcpy(msg->msg_name, (uintptr_t)p + (char *)msg1,
               (size_t)msg1->msg_namelen);
        p += msg1->msg_namelen;
    }

    if (msg1->msg_control != NULL) {
        memcpy(msg->msg_control, (uintptr_t)p + (char *)msg1,
               (size_t)msg1->msg_control);
        p += msg->msg_controllen;
    }

    if (msg1->msg_iov != NULL) {
        p += (uintptr_t)(sizeof(struct iovec) * (msg1->msg_iovlen));

        for (i = 0; i < msg1->msg_iovlen; i++) {
            memcpy(msg->msg_iov[i].iov_base, (uintptr_t)p + (char *)msg1,
                   (size_t)(msg1->msg_iov[i].iov_len));
            p += msg1->msg_iov[i].iov_len;
        }
    }

    if (ret == -1)
        errno = get_errno();

    return ret;
}

int
shutdown(int sockfd, int how)
{
    int ret;

    if (ocall_shutdown(&ret, sockfd, how) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    if (ret == -1)
        errno = get_errno();

    return ret;
}

int
os_socket_accept(bh_socket_t server_sock, bh_socket_t *sock, void *addr,
                 unsigned int *addrlen)

{
    struct sockaddr addr_tmp;
    unsigned int len = sizeof(struct sockaddr);

    if (ocall_accept(sock, server_sock, &addr_tmp, &len, len) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    if (*sock < 0) {
        return BHT_ERROR;
    }

    return BHT_OK;
}
int
os_socket_bind(bh_socket_t socket, const char *host, int *port)
{
    struct sockaddr_in addr;
    struct linger ling;
    unsigned int socklen;
    int ret;

    assert(host);
    assert(port);

    ling.l_onoff = 1;
    ling.l_linger = 0;

    if (ocall_fcntl_long(&ret, socket, F_SETFD, FD_CLOEXEC) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }
    
    if (ret < 0) {
        goto fail;
    }
    
    if (ocall_setsockopt(&ret, socket, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    if (ret < 0) {
        goto fail;
    }

    if (ocall_inet_addr(&addr.sin_addr.s_addr, host, strlen(host)) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    if (ocall_htons(&addr.sin_port, *port) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    addr.sin_family = AF_INET;

    if (ocall_bind(&ret, socket, &addr, sizeof(addr)) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    if (ret < 0) {
        goto fail;
    }

    socklen = sizeof(addr);

    if (ocall_getsockname(&ret, socket, (void *)&addr, &socklen, socklen) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    if (ret == -1) {
        goto fail;
    }

    if (ocall_ntohs((uint16_t*)port, addr.sin_port) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    return BHT_OK;

fail:
    return BHT_ERROR;
}

int
os_socket_close(bh_socket_t socket)
{
    errno = ENOSYS;
    return -1;
}

int
os_socket_connect(bh_socket_t socket, const char *addr, int port)
{}

int
os_socket_create(bh_socket_t *sock, int tcp_or_udp)
{
    if (!sock) {
        return BHT_ERROR;
    }

    if (1 == tcp_or_udp) {
        if (ocall_socket(sock, AF_INET, SOCK_STREAM, IPPROTO_TCP) != SGX_SUCCESS) {
            TRACE_OCALL_FAIL();
            return -1;
        }
    }
    else if (0 == tcp_or_udp) {
        if (ocall_socket(sock, AF_INET, SOCK_DGRAM, 0) != SGX_SUCCESS) {
            TRACE_OCALL_FAIL();
            return -1;
        }
    }

    os_printf("Allocated the socket: %d\n", *sock);

    return (*sock == -1) ? BHT_ERROR : BHT_OK;
}

int
os_socket_inet_network(const char *cp, uint32 *out)
{
    if (!cp)
        return BHT_ERROR;

    if (ocall_inet_network(out, cp, strlen(cp)) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    return BHT_OK;
}

int
os_socket_listen(bh_socket_t socket, int max_client)
{
    int ret;

    if (ocall_listen(&ret, socket, max_client) != SGX_SUCCESS) {
        TRACE_OCALL_FAIL();
        return -1;
    }

    return ret == 0 ? BHT_OK : BHT_ERROR;
}

int
os_socket_recv(bh_socket_t socket, void *buf, unsigned int len)
{
    errno = ENOSYS;
    return -1;
}

int
os_socket_send(bh_socket_t socket, const void *buf, unsigned int len)
{
    errno = ENOSYS;
    return -1;
}

int
os_socket_shutdown(bh_socket_t socket)
{
    errno = ENOSYS;
    return -1;
}

#endif
