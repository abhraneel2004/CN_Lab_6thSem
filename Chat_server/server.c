#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(){
    int sd, cd;
    struct sockaddr_in sad, cad;
    socklen_t cadl = sizeof(cad);
    char msg[50];

    // Create, bind, listen
    sd = socket(AF_INET, SOCK_STREAM, 0);
    sad.sin_family = AF_INET;
    sad.sin_port = htons(8195);
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(sd, (struct sockaddr*)&sad, sizeof(sad));
    listen(sd, 1);

    printf("Server waiting...\n");
    cd = accept(sd, (struct sockaddr*)&cad, &cadl);
    printf("Client connected!\n");

    while(1) {
        // Receive message
        memset(msg, 0, sizeof(msg));
        recv(cd, msg, sizeof(msg)-1, 0);
        printf("Client: %s\n", msg);

        if(strcmp(msg, "bye") == 0) break;

        // Send reply
        memset(msg, 0, sizeof(msg));
        printf("You: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = '\0';  // Remove newline
        send(cd, msg, strlen(msg), 0);

        if(strcmp(msg, "bye") == 0) break;
    }

    close(cd);
    close(sd);
    return 0;
}