#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

void code_gen(char* dw, char* cw){
  //for even parity
  int cnt1 = 0,i;
  for(i = 0; i<strlen(dw); i++){
    *(cw+i) = *(dw+i);
    if (*(dw+i)=='1') cnt1++;
  }
  if (cnt1%2==0){
    *(cw+i) = '0';
  }
  else{
    *(cw+i) = '1';
  }
  i++;
  *(cw+i)='\0';
  printf("Codeword = %s", cw);
  
}

int main(void){
  int sd;
  struct sockaddr_in sad;
  char dw[50], cw[51];

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_port = htons(9898);
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");

  connect(sd,(struct sockaddr*)&sad, sizeof(sad));
  printf("Enter the dataword: ");

  fgets(dw,sizeof(dw),stdin);
  dw[strcspn(dw,"\n")] = '\0';
 
  
  
  printf("Recieved dataword is: %s\n", dw);

  code_gen(dw,cw);

  send(sd,cw, sizeof(cw),0);
  
  close(sd);
  return 0;
}