#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<math.h>

unsigned char calc_checksum(char cw[], int cwl, int seglen){
  int sum = 0;
  for(int i = 0; i<cwl; i+=seglen){
    unsigned char segment = 0;
    for(int j = 0; j<seglen; j++){
      segment = (segment << 1) | (cw[i+j]-'0');
    }
    sum+=segment;
  }
  int max = pow(2,seglen) -1;
  while(sum>max){
    sum = (sum & max) + (sum >> seglen);
  }
  return sum;
}

int main(void){
  int sd, cd, cadl, seglen;
  struct sockaddr_in sad, cad;
  char cw[50], ucw[50];

  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sad.sin_family = AF_INET;
  sad.sin_port = htons(6789);
  sad.sin_addr.s_addr = inet_addr("127.0.0.1");
  bind(sd, (struct sockaddr*)&sad, sizeof(sad));
  listen(sd, 10);

  printf("Server listening at port 6789 ...\n");
  cadl = sizeof(cad);
  cd = accept(sd, (struct sockaddr*)&cad, &cadl);
  printf("Client Connected!!\n");

  // Clear buffer and receive with proper handling
  memset(cw, 0, sizeof(cw));
  int bytes_received = recv(cd, cw, sizeof(cw)-1, 0);

  if(bytes_received > 0) {
      cw[bytes_received] = '\0';  // Ensure null termination
      printf("Bytes received: %d\n", bytes_received);
      printf("Codeword received = %s\n", cw);
  } else {
      printf("No data received or connection closed\n");
      return -1;
  }
  printf("Enter the seglen: ");
  scanf("%d", &seglen);

  if (seglen==0 || (seglen&(seglen -1))!=0){
    printf("Invalid Segment Length\n");
    return -1;
  }

  if(strlen(cw)%seglen!=0){
    int remainder = strlen(cw)%seglen;
    int padding = seglen - remainder;
    printf("Padding required = %d\n", padding);
    for(int i = 0; i<padding; i++){
      ucw[i] = '0';
    }
    ucw[padding] = '\0';
  }
  strcat(ucw, cw);
  unsigned char checksum = calc_checksum(ucw, strlen(ucw), seglen);
  printf("Checksum = %u\n", checksum);
  unsigned int r = pow(2, seglen) - 1;

  if (checksum == r){
    printf("Correct data recieved..\n");
    ucw[strlen(ucw)-seglen] = '\0';
    printf("Actual data: %s\n", ucw);
  }
  else{
    printf("Incorrect data recieved..\n");
  }
  close(cd);
  close(sd);
  return 0;
}