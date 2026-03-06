
struct file_chunk {
    char data[1024];
    int bytes_read;
    int error_code; /* 0 for success, -1 for EOF, >0 for errors */
};

program FILE_PROG {
    version FILE_VERS {
        file_chunk READ_FILE(string) = 1;
    } = 1;
} = 0x20000001;