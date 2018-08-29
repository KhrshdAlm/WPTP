#ifndef __UTILITIES_H__
#define __UTILITIES_H__ 1

#include <sys/types.h>
#include <asm/types.h>
#include <resolv.h>
#include <stdlib.h>
#include <stdbool.h>



#ifndef NETLINK_LISTEN_ALL_NSID
#define NETLINK_LISTEN_ALL_NSID		8
#endif
#ifndef NETLINK_LIST_MEMBERSHIPS
#define NETLINK_LIST_MEMBERSHIPS	9
#endif


typedef struct
{
	__u16 flags;
	__u16 bytelen;
	__s16 bitlen;
	/* These next two fields match rtvia */
	__u16 family;
	__u32 data[8];
} inet_prefix;

#define DN_MAXADDL 20
#ifndef AF_DECnet
#define AF_DECnet 12
#endif

struct dn_naddr
{
        unsigned short          a_len;
        unsigned char a_addr[DN_MAXADDL];
};

#define IPX_NODE_LEN 6

struct ipx_addr {
	u_int32_t ipx_net;
	u_int8_t  ipx_node[IPX_NODE_LEN];
};

#ifndef AF_MPLS
# define AF_MPLS 28
#endif

/* Maximum number of labels the mpls helpers support */
#define MPLS_MAX_LABELS 8

#endif /* __UTILITIES_H__ */
