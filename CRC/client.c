#include<stdio.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>

int main(void){
  int sd;
  struct sockaddr_in sad;
  char dw[50], cw[50], gen[50];

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_port = htons(9115);
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");
  connect(sd, (struct sockaddr*)&sad, sizeof(sad));

  printf("Enter the Dataword: ");
  fgets(dw, sizeof(dw), stdin);
  dw[strcspn(dw,"\n")] = '\0';

  printf("Enter the Generator: ");
  fgets(gen, sizeof(gen), stdin);
  gen[strcspn(gen,"\n")] = '\0';

  int genl = strlen(gen), dwl = strlen(dw);
  if (genl&(genl-1)!=0 || genl>dwl) return -1;

  int i;
  for(i = dwl; i<(dwl+genl)-1; i++){
    *(dw+i) = '0';
  }
  *(dw+i) = '\0';
  printf("Padded dw: %s\n", dw);

  

  send(sd, cw, sizeof(cw), 0);
  send(sd, gen, sizeof(gen), 0);
  close(sd);
  return 0;
}