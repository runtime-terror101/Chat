#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

//printf = puts
char msg[1024];

void error(int code){
    if (code == 1){
        puts("\n Error, socket cannot be made");
    }
    else if (code == 2){
        puts("\n Error, can't write message \n");
    }
    else if (code == 3){
        puts("\n Error, connection to socket failed \n");
    }

}


void make_message(char client_name[100], char msg[1024], int sock){
    int l ;
    char message[1024];
    strcpy(message,client_name);
    strcat(message,":");
    strcat(message,msg);
    
    l = write(sock,message,strlen(message));
        if(l < 0){
            error(2);
        }
}


void *receive(void *my_sock)
{
    int length;
    int sock = *((int *)my_sock);
    
    while((length = recv(sock,msg,1024,0)) > 0) {
        msg[length] = '\0';
        printf(msg,stdout);  
    }
}

int main(int argc,char *argv[]){            //main takes client name as argument
    pthread_t thread_new;
    char* message[1024];
    int sock;
    
    
    char client_name[100];
    strcpy(client_name, argv[1]);
    sock = socket( AF_INET, SOCK_STREAM,0);
    if(sock == -1){
        error(1);
    }

    struct sockaddr_in Server;
    Server.sin_port = htons(1234);
    Server.sin_family= AF_INET;
    Server.sin_addr.s_addr = inet_addr("127.0.0.1");
    if( (connect( sock ,(struct sockaddr *)&Server,sizeof(Server))) == -1 )
        error(3);

    else{
        puts("Connection established ............\n");
    }

    //creating a client thread which is always waiting for a message
    pthread_create(&thread_new,NULL, (void *)receive, &sock);

    while(fgets(msg,1024,stdin) > 0) {
        make_message(client_name,msg,sock); 
    }

    
    pthread_join(thread_new,NULL);
    close(sock);
    return 0;
}