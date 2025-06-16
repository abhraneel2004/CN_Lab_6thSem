#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(void){
  int sd, cd, cadl;
  struct sockaddr_in sad, cad;
  char str[50];

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_port = htons(6968);
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(sd,(struct sockaddr*)&sad, sizeof(sad));
  listen(sd, 10);
  printf("Server listening on 6968\n");
  cadl = sizeof(cad);
  
  cd = accept(sd, (struct sockaddr*)&cad, &cadl);
  printf("client connected!\n");

  while(1){
    memset(str, 0, sizeof(str));
    recv(cd, str, sizeof(str),0);
    printf("Client Message: %s\n", str);
    // printf("%d\n", strcmp(str,"bye"));
    if(strcmp(str,"bye")==0) break;
    memset(str, 0, sizeof(str));
    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str,"\n")]='\0';
    send(cd, str, sizeof(str),0);

    // printf("%d\n", strcmp(str,"bye"));
    if(strcmp(str,"bye")==0) break;
  }
  
  close(sd);
  close(cd);
  return 0;
}