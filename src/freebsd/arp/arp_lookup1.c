/* FreeBSD lookup arp cache */
#include <sys/param.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/sockio.h>
#include <sys/sysctl.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <net/route.h>
#include <net/iso88025.h>

#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <arpa/inet.h>

#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <netdb.h>
#include <nlist.h>
#include <paths.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

/* struct sockaddr_dl: Structure of a Link-Level sockaddr
 * struct rt_msghdr: Structures for routing messages.
struct sockaddr_inarp {
	u_char	sin_len;
	u_char	sin_family;
	u_short sin_port;
	struct	in_addr sin_addr;
	struct	in_addr sin_srcaddr;
	u_short	sin_tos;
	u_short	sin_other;
#define SIN_PROXY 1
};
 */
typedef void (action_fn)(struct sockaddr_dl *sdl,
	struct sockaddr_inarp *s_in, struct rt_msghdr *rtm);

static int search(u_long addr, action_fn *action);
static action_fn print_entry;

static int nflag;	/* no reverse dns lookups */
static char *rifname;

static time_t	expire_time;

int
main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr; //server address(destination)

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(50001);
	servaddr.sin_addr.s_addr = inet_addr("192.168.1.11");
	
	search(inet_addr("192.168.1.12"), print_entry); //lookup arp cache
	
	char sendline[100];
	sprintf(sendline, "Hello, world. You are beautiful. I love you!"); //45bytes
	sendto(sockfd, sendline, strlen(sendline)+1, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	close(sockfd);
}

/*
 * Search the arp table and do some action on matching entries
 */
static int
search(u_long addr, action_fn *action)
{
	int mib[6]; //Management Information Base
	size_t needed;
	char *lim, *buf, *next;
	struct rt_msghdr *rtm;
	struct sockaddr_inarp *sin2;
	struct sockaddr_dl *sdl;
	char ifname[IF_NAMESIZE];
	int st, found_entry = 0;

	mib[0] = CTL_NET;
	mib[1] = PF_ROUTE;
	mib[2] = 0;
	mib[3] = AF_INET;
	mib[4] = NET_RT_FLAGS;
#ifdef RTF_LLINFO
	mib[5] = RTF_LLINFO;
#else
	mib[5] = 0;
#endif	
	if (sysctl(mib, 6, NULL, &needed, NULL, 0) < 0)
		err(1, "route-sysctl-estimate");
	if (needed == 0)	/* empty table */
		return 0;
	buf = NULL;
	for (;;) {
		buf = reallocf(buf, needed);
		if (buf == NULL)
			errx(1, "could not reallocate memory");
		st = sysctl(mib, 6, buf, &needed, NULL, 0);
		if (st == 0 || errno != ENOMEM)
			break;
		needed += needed / 8;
	}
	if (st == -1)
		err(1, "actual retrieval of routing table");
	lim = buf + needed;
	for (next = buf; next < lim; next += rtm->rtm_msglen) {
		rtm = (struct rt_msghdr *)next;
		sin2 = (struct sockaddr_inarp *)(rtm + 1);
		sdl = (struct sockaddr_dl *)((char *)sin2 + SA_SIZE(sin2));
		if (rifname && if_indextoname(sdl->sdl_index, ifname) &&
		    strcmp(ifname, rifname))
			continue;
		if (addr) {
			if (addr != sin2->sin_addr.s_addr)
				continue;
			found_entry = 1;
		}
		(*action)(sdl, sin2, rtm);
	}
	free(buf);
	return (found_entry);
}

/*
 * Display an arp entry
 */
static char lifname[IF_NAMESIZE];
static int64_t lifindex = -1;

static void
print_entry(struct sockaddr_dl *sdl,
	struct sockaddr_inarp *addr, struct rt_msghdr *rtm)
{
	const char *host;
	struct hostent *hp;
	struct iso88025_sockaddr_dl_data *trld;
	int seg;

	if (nflag == 0)
		hp = gethostbyaddr((caddr_t)&(addr->sin_addr),
		    sizeof addr->sin_addr, AF_INET);
	else
		hp = 0;
	if (hp)
		host = hp->h_name;
	else {
		host = "?"; //unkown hostname
		if (h_errno == TRY_AGAIN)
			nflag = 1;
	}
	printf("%s (%s) at ", host, inet_ntoa(addr->sin_addr));
	if (sdl->sdl_alen) {
		if ((sdl->sdl_type == IFT_ETHER ||
		    sdl->sdl_type == IFT_L2VLAN ||
		    sdl->sdl_type == IFT_BRIDGE) &&
		    sdl->sdl_alen == ETHER_ADDR_LEN)
			printf("%s", ether_ntoa((struct ether_addr *)LLADDR(sdl)));
		else {
			int n = sdl->sdl_nlen > 0 ? sdl->sdl_nlen + 1 : 0;

			printf("%s", link_ntoa(sdl) + n);
		}
	} else
		printf("(incomplete)");
	if (sdl->sdl_index != lifindex &&
	    if_indextoname(sdl->sdl_index, lifname) != NULL) {
        	lifindex = sdl->sdl_index;
		printf(" on %s", lifname);
        } else if (sdl->sdl_index == lifindex)
		printf(" on %s", lifname);
	if (rtm->rtm_rmx.rmx_expire == 0)
		printf(" permanent");
	else {
		static struct timeval tv;
		if (tv.tv_sec == 0)
			gettimeofday(&tv, 0);
		if ((expire_time = rtm->rtm_rmx.rmx_expire - tv.tv_sec) > 0)
			printf(" expires in %d seconds", (int)expire_time);
		else
			printf(" expired");
	}
	if (addr->sin_other & SIN_PROXY)
		printf(" published (proxy only)");
	if (rtm->rtm_flags & RTF_ANNOUNCE)
		printf(" published");
	switch(sdl->sdl_type) {
	case IFT_ETHER:
                printf(" [ethernet]");
                break;
	case IFT_ISO88025:
                printf(" [token-ring]");
		trld = SDL_ISO88025(sdl);
		if (trld->trld_rcf != 0) {
			printf(" rt=%x", ntohs(trld->trld_rcf));
			for (seg = 0;
			     seg < ((TR_RCF_RIFLEN(trld->trld_rcf) - 2 ) / 2);
			     seg++) 
				printf(":%x", ntohs(*(trld->trld_route[seg])));
		}
                break;
	case IFT_FDDI:
                printf(" [fddi]");
                break;
	case IFT_ATM:
                printf(" [atm]");
                break;
	case IFT_L2VLAN:
		printf(" [vlan]");
		break;
	case IFT_IEEE1394:
                printf(" [firewire]");
                break;
	case IFT_BRIDGE:
		printf(" [bridge]");
		break;
	default:
		break;
        }
		
	printf("\n");
}