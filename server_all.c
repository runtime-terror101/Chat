#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

int clients[10];
int n=0;
int all = 0;   // true if user wants to send message to all

void error(int code){
    if(code == 1){
        printf("Error: can't send \n");
    }
    else if(code == 2){
        printf("Error: can't bind \n");
    }
    else if(code == 3){
        printf("Error: couldn't listen \n");
    }
    else if(code == 4){
        printf("Error, couldn't accept  \n");
    }
}

void sendtoone(char *msg,int this, int port){
    send(clients[port-4],msg,strlen(msg),0) ;
}

void sendall(char *msg,int this){
    int i;
    
    for(i = 0; i < n; i++) {
        if(clients[i] != this) {
            if(send(clients[i],msg,strlen(msg),0) < 0) {
                error(1);
                continue;
            }
        }
    }
    
}

void *getmsg(void *client_sock){
    int sock = *((int *)client_sock);
    char msg[1024];
    int len;
    while((len = recv(sock,msg,1024,0)) > 0) {
        msg[len] = '\0';
        

        if(strstr(msg, "ALL") != NULL) {
            sendall(msg, sock);
        }
       
        else{
            //printf("Msg = %s \n", msg);
            len=strlen(msg);
            char c = ':';
            int pos = 0;
            for(int i=0;i<len;i++){  
                if(msg[i]==c){
                    pos = i+1;
                    //printf("character position:%d \n",pos);
                    }
                }

            // char sub;
            // sub = msg[pos];
            char baklol[2];
            baklol[0] = msg[pos];
            // printf("character position:%s \n",msg);
            //printf("LMAO sub %c \n", msg[pos]);
            int m = atoi(baklol);
            //printf("M: %d\n", m);
            // printf("LMAO sub %d \n", m);
            sendtoone(msg,sock,m);
        }   
            
        } 
        
    }



int main(){
    struct sockaddr_in this_server;
    pthread_t recvt;
    int sock=0 , Client_sock=0;

    this_server.sin_family = PF_INET;
    this_server.sin_port = htons(1234);
    this_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    this_server.sin_zero[10];
    
    sock = socket( PF_INET , SOCK_STREAM, 0 );
    if( bind( sock, (struct sockaddr *)&this_server, sizeof(this_server)) == -1 )
        error(2);
    else
        printf("Server Started\n");

    if( listen( sock ,10 ) == -1 )
        error(3);

    while(1){
        if( (Client_sock = accept(sock, (struct sockaddr *)NULL,NULL)) < 0 )
            error(4);
        
        clients[n]= Client_sock;
        n++;
        printf("Client joined port %d\n", Client_sock);

        
        // creating a thread for each client 
        pthread_create(&recvt,NULL,(void *)getmsg,&Client_sock);
        
    }
    return 0; 

}