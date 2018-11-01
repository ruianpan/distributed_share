#ifndef DGREP_H
#define DGREP_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>

#define PORT "3490"
#define BACKLOG 10     // how many pending connections queue will hold

/* Array of all the addresses */
char addrs[10][35] = {"fa18-cs425-g23-01.cs.illinois.edu",
                         "fa18-cs425-g23-02.cs.illinois.edu",
                         "fa18-cs425-g23-03.cs.illinois.edu",
                         "fa18-cs425-g23-04.cs.illinois.edu",
                         "fa18-cs425-g23-05.cs.illinois.edu",
                         "fa18-cs425-g23-06.cs.illinois.edu",
                         "fa18-cs425-g23-07.cs.illinois.edu",
                         "fa18-cs425-g23-08.cs.illinois.edu",
                         "fa18-cs425-g23-09.cs.illinois.edu",
                         "fa18-cs425-g23-10.cs.illinois.edu"};

/* Array of log files */
 char logs[10][10] = {" vm1.log",
                          " vm2.log",
                          " vm3.log",
                          " vm4.log",
                          " vm5.log",
                          " vm6.log",
                          " vm7.log",
                          " vm8.log",
                          " vm9.log",
                          " vm10.log"};

/* Array of log files for testing */
 char testlogs[10][20] = {" testvm1.log",
                       " testvm2.log",
                       " testvm3.log",
                       " testvm4.log",
                       " testvm5.log",
                       " testvm6.log",
                       " testvm7.log",
                       " testvm8.log",
                       " testvm9.log",
                       " testvm10.log"};

int recv_all(int sockfd, char *buf, int length);
int send_all(int sockfd, char *buf, int length);
int client(int target, char *kbInput);
void *client_thread(void *arg);
void *server_thread(void *arg);


#endif
