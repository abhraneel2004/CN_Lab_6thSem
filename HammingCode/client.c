#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<math.h>

int main(void) {
    int sd;
    struct sockaddr_in sad;
    char dw[50], cw[50];
    int data1[100], data2[100];
    int dwl, r, i = 0, j = 0, k = 0, z, c;

    // Socket creation
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sad.sin_family = AF_INET;
    sad.sin_port = htons(8566);
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sd, (struct sockaddr*)&sad, sizeof(sad));

    // Input dataword
    printf("Enter the dataword: ");
    fgets(dw, sizeof(dw), stdin);
    dw[strcspn(dw, "\n")] = '\0';
    dwl = strlen(dw);

    // Find number of parity bits required
    while (pow(2, i) < (dwl + i + 1)) {
        i++;
    }
    r = i;
    printf("No. of redundant bits is: %d\n", r);

    // Convert dataword characters to int
    for (i = 0; i < dwl; i++) {
        data1[i] = dw[i] - '0';
    }

    int total = dwl + r;
    // Initialize all positions to -1 (unset)
    for (i = 1; i <= total; i++) {
        data2[i] = -1;
    }

    // Place 999 in parity bit positions (powers of 2)
    for (i = 0; i < r; i++) {
        data2[(int)pow(2, i)] = 999;
    }

    // Fill in data bits
    j = 0;
    for (i = 1; i <= total; i++) {
        if (data2[i] != 999) {
            data2[i] = data1[j++];
        }
    }

    // Calculate parity bits
    for (i = 0; i < r; i++) {
        int parity_pos = pow(2, i);
        c = 0;
        for (j = parity_pos; j <= total; j += 2 * parity_pos) {
            for (k = j; k < j + parity_pos && k <= total; k++) {
                if (k != parity_pos && data2[k] != -1) {
                    c += data2[k];
                }
            }
        }
        data2[parity_pos] = c % 2;
    }

    // Construct codeword (MSB to LSB)
    j = 0;
    for (i = total; i >= 1; i--) {
        cw[j++] = data2[i] + '0';
    }
    cw[j] = '\0';

    printf("Codeword is: %s\n", cw);
    send(sd, cw, strlen(cw), 0);

    close(sd);
    return 0;
}
