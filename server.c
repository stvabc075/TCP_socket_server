// Server that handles connections, sending questions, and managing client responses.
#include <stdio.h>
#include <time.h>
#include <strings.h>
#include "header.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

#define BACKLOG 10
#define BUFSIZE 4000
#define DELAY 1
#define NUMQ 5
#define UPPER 42
#define LOWER 0
#define NI_MAXHOST 1025
#define NI_MAXSERV 32


// Function prototypes
void red(void);
void blue(void);
void reset(void);
void green(void);
int randomno(void);



int main(int argc, char *argv[]){
   srand(time(NULL)); // for random function
    green(); // green text  
    if (argc != 3){  // error message
       fprintf(stderr, "Expected Input: %s <IP address of server> <port number>\nExample Usage: %s 127.0.0.1 5555\n",argv[0],argv[0]);
       exit(-1);
    }

   // server initalisation
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(struct sockaddr_in));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(atoi(argv[2]));

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
       fprintf(stderr, "socket() error.\n");
       exit(-1);
    }

    /*
     * This socket option allows you to reuse the server endpoint
     * immediately after you have terminated it.
     */
    int optval = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR,
                   &optval, sizeof(optval)) == -1)
       exit(-1);

    int rc = bind(lfd, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr));
    if (rc == -1) {
       fprintf(stderr, "bind() error.\n");
       exit(-1);
    }

    if (listen(lfd, BACKLOG) == -1)
       exit(-1);

    fprintf(stdout, "<Listening on %s:%s>\n", argv[1], argv[2]);


    for (;;){ /* Handle clients iteratively */
        char buf[BUFSIZE];
        size_t totRead;
        char* bufr = buf;
        char reply[20];
        char message[] =  "Welcome to Unix Programming Quiz!\nThe quiz comprises five questions posed to you one after the other.\nYou have only one attempt to answer a question.\nYour final score will be sent to you after conclusion of the quiz.\nTo start the quiz, press Y and <enter>.\nTo quit the quiz, press q and <enter>.";
        fprintf(stdout, "\n\n\n<waiting for clients to connect>\n");
        fprintf(stdout, "<ctrl-C to terminate>\n");

        struct sockaddr_storage claddr;
        socklen_t addrlen = sizeof(struct sockaddr_storage);
        int cfd = accept(lfd, (struct sockaddr *)&claddr, &addrlen);
        if (cfd == -1) {
           continue;   /* Print an error message */
        }

        {
           char host[NI_MAXHOST];
           char service[NI_MAXSERV];
           if (getnameinfo((struct sockaddr *) &claddr, addrlen,
                     host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
              fprintf(stdout, "Connection from (%s, %s)\n", host, service);
           else
              fprintf(stderr, "Connection from (?UNKNOWN?)");
        }


        // Write inital message to client
        write(cfd, message, strlen(message));
        // Read reply from client
        read(cfd, reply, sizeof(reply));
        int rightans = 0;
        if(reply[0] == 'Y' || reply[0] == 'y'){ // game initlisation
            int numq = 0;
            while(numq<NUMQ){ 
               char ans[10];
               int rq = randomno();
               size_t totWritten = 0;
               const char* bufw = QuizQ[rq];
               sleep(DELAY);

               while(totWritten < strlen(QuizQ[rq])) { //WRITES QUESTION TO CLIENT
                  ssize_t numWritten = write(cfd, bufw, strlen(QuizQ[rq]) - totWritten);
                  if (numWritten <= 0) {
                     if (numWritten == -1 && errno == EINTR)
                        continue;
                     else {
                        fprintf(stderr, "Write error. Errno %d.\n", errno);
                     }
                  }

                  totWritten += numWritten;
                  bufw += numWritten;
               }
               
               //TAKES ANSWER FROM CLIENT
               read(cfd,ans,sizeof(ans));
               if(strcasecmp(ans,QuizA[rq])==0){ // if right ans
                  char right[18] = "Right Answer. +1\n";
                  sleep(DELAY);
                  write(cfd,right,strlen(right));
                  rightans++;
                  memset(right,  0, sizeof(right));
               }
             else{ // if wrong ans
                  char wrong[] = "Wrong Answer. Right answer is ";
                  strcat(wrong,QuizA[rq]);
                  strcat(wrong,"\n");
                  sleep(DELAY);
                  write(cfd,wrong,strlen(wrong));
                  rightans +=0;
                  memset(wrong,  0, sizeof(wrong));
               }

               numq++;
               memset(ans, 0, sizeof(ans));// resets ans
            }

            char buf2[79];
            sprintf(buf2, "Your quiz score is %i/5. Goodbye!\n\n", rightans );
            write(cfd,buf2,sizeof(buf2)); // sends closing message
        }
        else{ // if anything else other than y is entred
            if (close(cfd) == -1) /* Close connection */
            {
               fprintf(stderr, "close error.\n");
               exit(EXIT_FAILURE);
            }
        }


        if (close(cfd) == -1) /* Close connection */
        {
           fprintf(stderr, "close error.\n");
           exit(EXIT_FAILURE);
        }

    } 


    if (close(lfd) == -1){ /* Close listening socket */
       fprintf(stderr, "close error.\n");
       exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);


}


// color functions, changes the color of text
void red (void){
  printf("\033[1;31m");
}
void blue(void){
  printf("\033[0;34m");
}
void reset (void) {
  printf("\033[0m");
}
void green (void){
   printf("\033[0;32m");
}
int randomno(void){
    return (rand() % (UPPER - LOWER + 1)) + LOWER;
}



