#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<math.h>

unsigned char calculate_checksum(char dw[], int dwl, int seglen){
  int sum = 0;
  for(int i = 0; i<dwl; i+=seglen){
    unsigned char segment = 0;
    for(int j = 0; j<seglen; j++){
      segment = (segment << 1) | (dw[i+j]-'0');
    }
    sum+=segment;
  }
  int max = pow(2,seglen)-1;
  while(sum>max){
    sum = (sum & max) + (sum >> seglen);
  }
  return sum;
}

int main(void){
  int sd, seglen;
  struct sockaddr_in sad;
  char dw[50], cw[50], udw[50];

  for (int i = 0; i<50; i++){
    udw[i] = 0;
  }
  // printf("udw %s\n", udw);

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_port = htons(6789);
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");

  connect(sd,(struct sockaddr*)&sad, sizeof(sad));

  printf("Enter the dataword: ");
  fgets(dw, sizeof(dw), stdin);
  dw[strcspn(dw, "\n")] = '\0';

  printf("Enter the segment length: ");
  scanf("%d", &seglen);

  //check if the length is of power of two
  if(seglen==0 || (seglen & (seglen-1)) !=0) {
    printf("Invalid Segment Length\n");
    return -1;
  }

  if(strlen(dw)%seglen!=0){
    int remainder = strlen(dw)%seglen;
    int padding = seglen - remainder;
    printf("padding required = %d\n", padding);
    for(int i = 0; i<padding; i++){
      udw[i] = '0';
    }
  }
  strcat(udw, dw);
  strcpy(dw, udw);
  strcpy(cw, dw);
  unsigned char checksum = calculate_checksum(dw, strlen(dw), seglen);
  printf("CS = %u\n", checksum);
  printf("checksum :\n");

  for (int i = seglen-1; i>=0; i--){
    int res = ((checksum >> i) & 1);
    printf("%d",res);
    char c = ((res + '0') == '0')? '1' : '0';
    udw[seglen-i-1] = c;
  }
  printf("\n");
  udw[seglen]='\0';
  strcat(cw,udw);
  printf("Codeword : %s\n", cw);
  send(sd, cw, strlen(cw), 0);
  close(sd);
  return 0;
}