#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(){
    int sd;
    struct sockaddr_in sad;
    char msg[50];

    // Create and connect socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    sad.sin_family = AF_INET;
    sad.sin_port = htons(9095);
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sd, (struct sockaddr*)&sad, sizeof(sad));

    printf("Connected! Type 'bye' to exit.\n");

    while(1) {
        // Send message
        memset(msg, 0, sizeof(msg));
        printf("You: ");
        scanf("%[^\n]s", msg);
        send(sd, msg, strlen(msg), 0);

        if(strcmp(msg, "bye") == 0) break;
        
        // Receive reply
        memset(msg, 0, sizeof(msg));
        recv(sd, msg, sizeof(msg)-1, 0);
        printf("Server: %s\n", msg);

        if(strcmp(msg, "bye") == 0) break;
    }

    close(sd);
    return 0;
}