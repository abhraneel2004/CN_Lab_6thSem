#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>

int isPowerOfTwo(int x) {
    return x && (!(x & (x - 1)));
}

int main() {
    int sd, nsd;
    struct sockaddr_in sad, cad;
    socklen_t cad_len = sizeof(cad);
    char cw[50];
    int data[50];
    int r = 0, total, i, j, k, c, error_pos = 0;

    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sad.sin_family = AF_INET;
    sad.sin_port = htons(8566);
    sad.sin_addr.s_addr = INADDR_ANY;

    bind(sd, (struct sockaddr *)&sad, sizeof(sad));
    listen(sd, 5);
    printf("Waiting for connection...\n");
    nsd = accept(sd, (struct sockaddr *)&cad, &cad_len);
    printf("Client connected.\n");

    recv(nsd, cw, sizeof(cw), 0);
    cw[strcspn(cw, "\n")] = '\0';
    total = strlen(cw);
    printf("Received codeword: %s\n", cw);

    // Convert to integer array (1-based indexing)
    for (i = 0; i < total; i++) {
        data[total - i] = cw[i] - '0'; // Reverse to match position 1..n
    }

    // Calculate r (number of parity bits)
    i = 0;
    while (pow(2, i) <= total)
        i++;
    r = i;

    // Recalculate parity and determine error position
    for (i = 0; i < r; i++) {
        int parity_pos = pow(2, i);
        c = 0;

        for (j = parity_pos; j <= total; j += 2 * parity_pos) {
            for (k = j; k < j + parity_pos && k <= total; k++) {
                if (k != parity_pos)
                    c += data[k];
            }
        }

        if ((c + data[parity_pos]) % 2 != 0) {
            error_pos += parity_pos;
        }
    }

    if (error_pos == 0) {
        printf("No error detected in the codeword.\n");
    } else {
        printf("Error detected at position: %d\n", error_pos);
        data[error_pos] ^= 1;
        printf("Corrected codeword: ");
        for (i = total; i >= 1; i--) {
            printf("%d", data[i]);
        }
        printf("\n");
    }

    // Extract data bits
    printf("Original dataword: ");
    for (i = total; i >= 1; i--) {
        if (!isPowerOfTwo(i)) {
            printf("%d", data[i]);
        }
    }
    printf("\n");

    close(nsd);
    close(sd);
    return 0;
}
