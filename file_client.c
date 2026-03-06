#include "file_transfer.h"
#include <stdio.h>

#define KEY 'X'

void download_file(char *host, char *remote_file) {
    CLIENT *clnt;
    file_chunk *result;
    FILE *fp;

    clnt = clnt_create(host, FILE_PROG, FILE_VERS, "tcp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }

    fp = fopen("downloaded_output.txt", "wb");
    
    do {
        result = read_file_1(&remote_file, clnt);
        if (result == NULL) {
            clnt_perror(clnt, "Call failed");
            break;
        }

        if (result->error_code == 404) {
            printf("Error: File not found on server.\n");
            break;
        }

        // Decrypt the received chunk
        for(int i = 0; i < result->bytes_read; i++) {
            result->data[i] ^= KEY;
        }

        fwrite(result->data, 1, result->bytes_read, fp);
        
    } while (result->error_code == 0);

    fclose(fp);
    clnt_destroy(clnt);
    printf("Transfer complete.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <server_host> <filename>\n", argv[0]);
        exit(1);
    }
    download_file(argv[1], argv[2]);
    return 0;
}