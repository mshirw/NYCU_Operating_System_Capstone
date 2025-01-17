#define CPIO_HEADER_SIZE 110
#define CPIO_ADDRESS 0x2000000

typedef struct {
    char c_magic[6];
    char c_ino[8];
    char c_mode[8];
    char c_uid[8];
    char c_gid[8];
    char c_nlink[8];
    char c_mtime[8];
    char c_filesize[8];
    char c_devmajor[8];
    char c_devminor[8];
    char c_rdevmajor[8];
    char c_rdevminor[8];
    char c_namesize[8];
    char c_check[8];
} cpio_header;

typedef struct
{
    int file_size;
    int file_name_size;
    char file_name[64];
    char file_data[1024];
}cpio_files;

extern cpio_files *cpio_file[10];
unsigned int from_hex(char *str, int len);
void parse_cpio(const char *cpio_base_addr);
void list_file_names();
void init_file_struct();
void print_file_data(char *filename);