#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(void){
  int sd, cd, cadl;
  struct sockaddr_in sad,cad;
  char cw[50], gen[50], temp[50], dw[50];

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_port = htons(9115);
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(sd, (struct sockaddr*)&sad, sizeof(sad));
  listen(sd, 10);
  printf("Server listening to port 9115 ...\n");

  cadl = sizeof(cad);
  cd = accept(sd, (struct sockaddr*)&cad, &cadl);
  printf("Client Connected!!\n");

  recv(cd,cw, sizeof(cw),0);
  recv(cd, gen, sizeof(gen), 0);

  printf("Recieved Codeword = %s\n",cw);
  printf("Recieved Generator = %s\n", gen);

  strcpy(temp, cw);

  while(strlen(cw)>=strlen(gen)){
    if(cw[0]=='0'){
      for(int i = 0; i<strlen(cw)-1;i++){
        cw[i] = cw[i+1];
      }
      cw[strlen(cw)-1] = '\0';
      continue;
    }
    for(int i = 0; i<strlen(gen); i++){
      cw[i] = (cw[i]^gen[i])+'0';
    }
  }
  int flag = 0;
  for(int i = 0; i<strlen(cw); i++){
    if (cw[i]!='0'){
      flag= 1;
      break;
    }
  }

  if (flag==0){
    printf("Accepted\n");
  }
  else{
    printf("Rejected\n");
  }

  close(cd);
  close(sd);
  return 0;
}