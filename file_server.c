#include "file_transfer.h"
#include <stdio.h>

// Simple XOR Decryption/Encryption Key
#define KEY 'X'

file_chunk *read_file_1_svc(char **filename, struct svc_req *rqstp) {
    static file_chunk result;
    static FILE *fp = NULL;
    static char last_file[256] = "";

    // Security: Basic Authentication check (Simulated)
    
    if (strcmp(last_file, *filename) != 0) {
        if (fp) fclose(fp);
        fp = fopen(*filename, "rb");
        strncpy(last_file, *filename, 255);
    }

    if (fp == NULL) {
        result.error_code = 404; // Not Found
        return &result;
    }

    result.bytes_read = fread(result.data, 1, 1024, fp);
    
    // Encrypt the chunk before sending
    for(int i = 0; i < result.bytes_read; i++) {
        result.data[i] ^= KEY;
    }

    if (result.bytes_read == 0) {
        result.error_code = -1; // EOF
        fclose(fp);
        fp = NULL;
        memset(last_file, 0, 256);
    } else {
        result.error_code = 0;
    }

    return &result;
}