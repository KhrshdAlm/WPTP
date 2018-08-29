
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <linux/pkt_sched.h>
#include <linux/param.h>
#include <linux/if_arp.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#include "utilities.h"






int get_u8(__u8 *val, const char *arg, int base)
{
	unsigned long res;
	char *ptr;

	if (!arg || !*arg)
		return -1;

	res = strtoul(arg, &ptr, base);
	/* empty string or trailing non-digits */
	if (!ptr || ptr == arg || *ptr)
		return -1;

	/* overflow */
	if (res == ULONG_MAX && errno == ERANGE)
		return -1;

	if (res > 0xFFUL)
		return -1;

	*val = res;
	return 0;
}






const char *format_host(int af, int len, const void *addr,
			char *buf, int buflen)
{
#ifdef RESOLVE_HOSTNAMES
	if (resolve_hosts) {
		const char *n;

		len = len <= 0 ? af_byte_len(af) : len;

		if (len > 0 &&
		    (n = resolve_address(addr, len, af)) != NULL)
			return n;
	}
#endif

	switch (af) {
	case AF_INET:
	case AF_INET6:
		return inet_ntop(af, addr, buf, buflen);
	case AF_MPLS:
		return mpls_ntop(af, addr, buf, buflen);
	case AF_IPX:
		return ipx_ntop(af, addr, buf, buflen);
	case AF_DECnet:
	{
		struct dn_naddr dna = { 2, { 0, 0, }};
		memcpy(dna.a_addr, addr, 2);
		return dnet_ntop(af, &dna, buf, buflen);
	}
//	case AF_PACKET:
//		return ll_addr_n2a(addr, len, ARPHRD_VOID, buf, buflen);
	default:
		return "???";
	}
}









/* From IPRoute2 (09.09.2015): */
const char *ll_addr_n2a(const unsigned char *addr, int alen, int type, char *buf, int blen)
{
	int i;
	int l;

	if (alen == 4 &&
	    (type == ARPHRD_TUNNEL || type == ARPHRD_SIT || type == ARPHRD_IPGRE)) {
		return inet_ntop(AF_INET, addr, buf, blen);
	}
	if (alen == 16 && type == ARPHRD_TUNNEL6) {
		return inet_ntop(AF_INET6, addr, buf, blen);
	}
	l = 0;
	for (i=0; i<alen; i++) {
		if (i==0) {
			snprintf(buf+l, blen, "%02x", addr[i]);
			blen -= 2;
			l += 2;
		} else {
			snprintf(buf+l, blen, ":%02x", addr[i]);
			blen -= 3;
			l += 3;
		}
	}
	return buf;
}


#ifndef offsetof
# define offsetof(type, member) ((size_t) &((type *)0)->member)
#endif

struct hlist_node {
	struct hlist_node *next, **pprev;
};

struct hlist_head {
	struct hlist_node *first;
};

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

#define hlist_for_each(pos, head) \
	for (pos = (head)->first; pos ; pos = pos->next)

struct ll_cache {
	struct hlist_node idx_hash;
	struct hlist_node name_hash;
	unsigned	flags;
	unsigned 	index;
	unsigned short	type;
	char		name[IFNAMSIZ];
};

#define IDXMAP_SIZE	1024
static struct hlist_head idx_head[IDXMAP_SIZE];

static struct ll_cache *ll_get_by_index(unsigned index)
{
	struct hlist_node *n;
	unsigned h = index & (IDXMAP_SIZE - 1);

	hlist_for_each(n, &idx_head[h]) {
		struct ll_cache *im
			= container_of(n, struct ll_cache, idx_hash);
		if (im->index == index)
			return im;
	}

	return NULL;
}

int ll_index_to_type(unsigned idx)
{
	const struct ll_cache *im;

	if (idx == 0)
		return -1;

	im = ll_get_by_index(idx);
	return im ? im->type : -1;
}
/* End IPRoute2 (09.09.2015): */
