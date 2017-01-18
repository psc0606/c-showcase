/* BPF receiver
 * 
 * Date: 2014-11-20 by psc, freebsd8.3
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <net/bpf.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#define BPFDEV "/dev/bpf"

struct bpf_insn insns[] = {
	/*This filter accepts only IP packets between host 128.3.112.15 and 128.3.112.35 */
	BPF_STMT(BPF_LD+BPF_H+BPF_ABS, 12),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, ETHERTYPE_IP, 0, 8),
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, 26),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, 0xc0a8010b, 0, 2), //192.168.1.11
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, 30),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, 0xc0a8010c, 3, 4), //192.168.1.12
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, 0xc0a8010c, 0, 3),
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, 30),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, 0xc0a8010b, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, (u_int)-1),
	BPF_STMT(BPF_RET+BPF_K, 0),

/* This filter returns only TCP finger packets. 
 * We must parse the IP header to reach the TCP header. 
 * The BPF_JSET instruction checks that the IP fragment offset 
 * is 0 so we are sure that we have a TCP header.
 */
/*	BPF_STMT(BPF_LD+BPF_H+BPF_ABS, 12),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, ETHERTYPE_IP, 0, 10),
	BPF_STMT(BPF_LD+BPF_B+BPF_ABS, 23),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, IPPROTO_TCP, 0, 8),
	BPF_STMT(BPF_LD+BPF_H+BPF_ABS, 20),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_K, 0x1fff, 6, 0),
	BPF_STMT(BPF_LDX+BPF_B+BPF_MSH, 14),
	BPF_STMT(BPF_LD+BPF_H+BPF_IND, 14),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, 79, 2, 0),
	BPF_STMT(BPF_LD+BPF_H+BPF_IND, 16),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, 79, 0, 1),
	BPF_STMT(BPF_RET+BPF_K, (u_int)-1),
	BPF_STMT(BPF_RET+BPF_K, 0), */
};

struct bpf_program filter = 
{
	sizeof(insns) / sizeof(struct bpf_insn),
	insns
};

int main()
{
	int bpfd;
	int pktlen;
	int set = 1;
	int ret = 0;
	char ifrname[] = "em0";
	struct ifreq ifr;

	strcpy(ifr.ifr_name, ifrname);

	bpfd = open(BPFDEV, O_RDONLY);
	if(bpfd < 0)
	{
		perror("Open bpf device");
		exit(EXIT_FAILURE);
	}

	/* BPF read buf, must before BIOCSETIF */
	ret = ioctl(bpfd, BIOCSBLEN, &pktlen);
	if(ret < 0)
	{
		perror("Ioctl BIOCSBLEN");
		goto close_fd;
	}

	/* BPF interface */
	ret = ioctl(bpfd, BIOCSETIF, &ifr);
	if(ret < 0)
	{
		perror("Ioctl BIOCSETIF");
		goto close_fd;
	}

	/* Forces the interface into promiscuous mode */
	ret = ioctl(bpfd, BIOCPROMISC);
	if(ret < 0)
	{
		perror("Ioctl BIOCPROMISC");
		goto close_fd;
	}

	/* Read immediately when set is ture */
	ret = ioctl(bpfd, BIOCIMMEDIATE, &set);
	if(ret < 0)
	{
		perror("Ioctl BIOCIMMEDIATE");
		goto close_fd;
	}

	/* Set the filter program */
	ret = ioctl(bpfd, BIOCSETF, &filter);
	if(ret < 0)
	{
		perror("Ioctl BIOCSETF");
		goto close_fd;
	} 

	/* Set up the select */
	char *buf;
	char *p;
	fd_set fdset;
	struct timeval timeout = {10, 0};

	buf = (char *)malloc(pktlen);
	memset(buf, 0, sizeof(buf));
	FD_ZERO(&fdset); //initializes a descriptor set fdset to the null set
	FD_SET(bpfd, &fdset); //include bpfd in fdset

	ret = select(bpfd+1, &fdset, NULL, NULL, &timeout);
	switch(ret)
	{
		case 0:
			printf("Select timeout.\n");
			goto close_fd;
		case -1:
			perror("Select");
			goto close_fd;
	}

	printf("Starting read data from bpf device...\n");
	if(FD_ISSET(bpfd, &fdset))
	{
		ret = read(bpfd, buf, pktlen);
	}
//	p = (struct bpf_hdr *)buf;
//	p = (char *)p + BPF_WORDALIGN(p->bh_hdrlen + p->bh_caplen)
	printf("pktlen:%d, readnumbytes:%d\n", pktlen, ret);

	close(bpfd);
	return ret;

close_fd:
	close(bpfd);
	exit(EXIT_FAILURE);
}
