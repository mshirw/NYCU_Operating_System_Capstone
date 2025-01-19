#include "cpio_parser.h"
#include "mini_uart.h"
#include "memalloc.h"

cpio_files *cpio_file[FILES_MAX_SIZE];

unsigned int from_hex(char *str, int len) {
    unsigned int result = 0;
    for (int i = 0; i < len; i++) {
        char c = str[i];
        result <<= 4;
        if (c >= '0' && c <= '9') {
            result += c - '0';
        } else if (c >= 'a' && c <= 'f') {
            result += c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            result += c - 'A' + 10;
        }
    }
    return result;
}

void copy_string(char *src, char *dest, int len) {
    for (int i = 0; i < len; i++) {
        dest[i] = src[i];
    }
}

void init_file_struct()
{
    for(int i = 0; i < FILES_MAX_SIZE; i++)
    {
        cpio_file[i] = 0;
    }
}

void parse_cpio(const char *cpio_base_addr) {
    unsigned long int offset = 0;
    unsigned char fileIndex = 0;
    char *filename, *filedata;

    init_file_struct();
    
    while (1)
    {
        cpio_header *header = (cpio_header *)(cpio_base_addr + offset);

        if(uart_cmp_string_length(header->c_magic, "070701", 6) == 0)
        {
            uart_send_string("Invalid CPIO magic number\r\n");
            return;
        }

        unsigned int namesize = from_hex(header->c_namesize, 8);
        unsigned int filesize = from_hex(header->c_filesize, 8);

        filename = (char *)(cpio_base_addr + offset + CPIO_HEADER_SIZE);

        if(filesize != 0)
        {
            cpio_file[fileIndex] = (cpio_files *)malloc(sizeof(cpio_files));
            cpio_file[fileIndex]->file_size = filesize;
            cpio_file[fileIndex]->file_name_size = namesize;
            
            copy_string(filename, cpio_file[fileIndex]->file_name, namesize);
        }

        /*if(filesize != 0 && cmdNum == 5)
        {
            uart_send_string("File: ");
            uart_send_string_length(cpio_file[fileIndex]->file_name, cpio_file[fileIndex]->file_name_size);
            uart_send_string("\r\n");
        }*/
        
        offset += CPIO_HEADER_SIZE + namesize;

        //align to 4 bytes
        if (offset % 4 != 0) {
            offset += 4 - (offset % 4);
        }

        filedata = (char *)(cpio_base_addr + offset);

        if(filesize != 0)
        {
            copy_string(filedata, cpio_file[fileIndex]->file_data, filesize);
            fileIndex++;
        }

        /*if(filesize != 0 && cmdNum == 6)
        {
            uart_send_string("File data: ");
            char *filedata = (char *)(cpio_base_addr + offset);
            copy_string(filedata, cpio_file[fileIndex]->file_data, filesize);
            uart_send_string_length(cpio_file[fileIndex]->file_data, filesize);
            uart_send_string("\r\n");
        }*/

        if (uart_cmp_string_length(filename, "TRAILER!!!", 11) == 1) {
            break;
        }
        
        offset += filesize;

        //align to 4 bytes
        if (offset % 4 != 0) {
            offset += 4 - (offset % 4);
        }
    }
}

void list_file_names()
{
    for(int i = 0; i < 10; i++)
    {
        if(cpio_file[i] != 0)
        {
            uart_send_string("File: ");
            uart_send_string_length(cpio_file[i]->file_name, cpio_file[i]->file_name_size);
            uart_send_string("\r\n");
        }
    }
}

void print_file_data(char *filename)
{
    for(int i = 0; i < 10; i++)
    {
        if(cpio_file[i] != 0)
        {
            if(uart_cmp_string_length(cpio_file[i]->file_name, filename, cpio_file[i]->file_name_size-1) == 1)
            {
                uart_send_string("File data: ");
                uart_send_string_length(cpio_file[i]->file_data, cpio_file[i]->file_size);
                uart_send_string("\r\n");
                return;
            }
        }
    }
    uart_send_string("File not found\r\n");
}