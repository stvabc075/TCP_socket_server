// Client responsible for connecting to a server, handling quiz interaction, and displaying results.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define BUFSIZE 4000
#define DELAY .5
#define RUNS 5


// color functions, changes the color of text
void red () {
  printf("\033[1;31m");
}

void blue(){
  printf("\033[0;34m");
}

void reset () {
  printf("\033[0m");
}
void green (){
   printf("\033[0;32m");
}




int main(int argc, char *argv[]) 
{
   red();
    if (argc != 3){
       fprintf(stderr, "Expected Input: %s <IP address of server> <port number>\nExample Usage: %s 127.0.0.1 5555\n",argv[0],argv[0]);
       exit(-1);
    }

    struct sockaddr_in serverAddress;
   
    memset(&serverAddress, 0, sizeof(struct sockaddr_in));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(atoi(argv[2]));

    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1) {
       fprintf(stderr, "socket() error.\n");
       exit(-1);
    }

    int rc = connect(cfd, (struct sockaddr *)&serverAddress, 
                     sizeof(struct sockaddr));
    if (rc == -1) {
       fprintf(stderr, "connect() error, errno %d.\n", errno);
       exit(-1);
    }

    {

        char buf[BUFSIZE];
        size_t totRead;
        char* bufr = buf;

        read(cfd,buf,sizeof(buf));
        printf("%s\n", buf);
        // reads opening message
        

      size_t getline_mem = BUFSIZE;
      char *reply = NULL;
      reset();
      getline(&reply,&getline_mem,stdin);
      write(cfd,reply,sizeof(reply));

      //For initial y or n statement takes user input then sends to server

      if(reply[0] == 'y'|| reply[0] == 'Y' ){ // ENTERS QUIZ LOOP

         int count = 0;
         while(count<RUNS){
            char ans[10];
            char q[BUFSIZE];
            memset(q, 0, BUFSIZE);
            char r[400];
            blue();
            read(cfd,q,BUFSIZE);
            printf("%s\n",q );
            // read and prints question
            reset();
            scanf("%s", ans);
            sleep(DELAY);
            write(cfd,ans,sizeof(ans));
            //takews answer and senmds to server
            blue();
            read(cfd,r,sizeof(r));
            printf("%s\n",r );
            green();
            //reads and prints reply

            memset(ans, 0, sizeof(ans));
            memset(r,  0, sizeof(r));
            memset(q, 0, sizeof(q));
            count++;
            // clears buffer and loops again


         }
            char q[BUFSIZE];
            memset(q, 0, BUFSIZE);
            green();
            read(cfd,q,BUFSIZE);
            printf("%s\n",q ); // reads and prints final answer

      }
      else{ // if anything else other than y is entred
            if (close(cfd) == -1) /* Close connection */
            {
               fprintf(stderr, "close error.\n");
               exit(EXIT_FAILURE);
            }
      }

    }


    if (close(cfd) == -1) /* Close connection */
    {
        fprintf(stderr, "close error.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

