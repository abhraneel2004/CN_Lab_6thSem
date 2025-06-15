#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

void check_parity(char* cw){
  int cnt1 = 0;
  for(int i =0;i<strlen(cw)-1; i++){
    if (*(cw+i)=='1') cnt1++;
  }
  printf("No. of ones in dw = %d\n",cnt1);
  if(cnt1%2==(*(cw+strlen(cw)-1))-48) printf("Accepted\n");
  else printf("Rejected\n");
}

int main(void){
  int sd, cd, cadl;
  struct sockaddr_in sad, cad;
  char cw[51];

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_port = htons(9898);
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(sd, (struct sockaddr*)&sad, sizeof(sad));
  listen(sd, 10);
  printf("Server listening on port 9898\n");

  cadl = sizeof(cad);
  cd = accept(sd, (struct sockaddr*)&cad, &cadl);
  printf("Client Connected!!\n");

  recv(cd,cw,sizeof(cw),0);
  printf("Recieved Codeword is : %s\n", cw);

  check_parity(cw);

  close(sd);
  close(cd);
  return 0;
}