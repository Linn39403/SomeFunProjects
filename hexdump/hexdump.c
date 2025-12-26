#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PROGRAM_NAME "hexdump"
#define BUFFER_SIZE 16

// Function Prototypes
void print_usage();
void search_options(int argc, char *argv[], int *i);
void process_file(const char *filename);
void remove_non_printable_ascii_char(unsigned char *buffer, size_t size);
void format_line(char *output_payload, unsigned char *buffer, size_t bytes_read);

// Global Options
int canonical_option = -1;
int decimal_option = -1;
uint32_t length_option = 0;
char *file_to_read = NULL;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            search_options(argc, argv, &i);
        } else {
            file_to_read = argv[i];
        }
    }

    if (file_to_read) {
        process_file(file_to_read);
    }

    return 0;
}

void remove_non_printable_ascii_char(unsigned char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (buffer[i] < ' ' || buffer[i] > '~') { // Improved range check
            buffer[i] = '.';
        }
    }
}

/**
 * Refactored: Logic only. 
 * Formats a single line of data into the provided char buffer.
 */
void format_line(char *out, unsigned char *buffer, size_t bytes_read) {
    char temp[64] = {0};
    out[0] = '\0';

    if (decimal_option == 1) {
        uint32_t i;
        for (i = 0; i < bytes_read - 1; i += 2) {
            uint16_t val = (buffer[i + 1] << 8) | buffer[i];
            sprintf(temp, "  %05u ", val);
            strcat(out, temp);
        }
        if (i < bytes_read) {
            sprintf(temp, "  %05u ", buffer[i]);
            strcat(out, temp);
        }
    } else {
        for (uint32_t i = 0; i < bytes_read; i++) {
            sprintf(temp, "%02x ", buffer[i]);
            strcat(out, temp);
        }
    }

    if (canonical_option == 1) {
        // Alignment padding
        if (bytes_read < BUFFER_SIZE) {
            int pad = (BUFFER_SIZE - bytes_read) * (decimal_option == 1 ? 4 : 3);
            for (int i = 0; i < pad; i++) strcat(out, " ");
        }

        strcat(out, " |");
        unsigned char ascii_buf[BUFFER_SIZE + 1];
        memcpy(ascii_buf, buffer, bytes_read);
        remove_non_printable_ascii_char(ascii_buf, bytes_read);
        ascii_buf[bytes_read] = '\0';
        strcat(out, (char *)ascii_buf);
        strcat(out, "|");
    }
}

void process_input_data(FILE *input) {
    unsigned char buffer[BUFFER_SIZE] = {0};
    char output_line[256];
    size_t bytes_read;
    size_t total_bytes_read = 0;
    size_t to_read = (length_option > 0 && length_option < BUFFER_SIZE) ? length_option : BUFFER_SIZE;

    while ((bytes_read = fread(buffer, 1, to_read, input)) > 0) {
        printf("%08lx ", (unsigned long)total_bytes_read);
        
        format_line(output_line, buffer, bytes_read);
        printf("%s\n", output_line);

        total_bytes_read += bytes_read;
        if (length_option > 0) {
            if (total_bytes_read >= length_option) break;
            if (length_option - total_bytes_read < BUFFER_SIZE)
                to_read = length_option - total_bytes_read;
        }
        memset(buffer, 0, BUFFER_SIZE);
    }
    printf("%08lx\n", (unsigned long)total_bytes_read);
}

void process_file(const char *filename) {
    FILE *input_f = fopen(filename, "rb");
    if (!input_f) {
        fprintf(stderr, "%s: cannot open %s\n", PROGRAM_NAME, filename);
        return;
    }
    process_input_data(input_f);
    fclose(input_f);
}

// ... search_options and print_usage remain largely the same ...
//
void search_options(int argc, char *argv[], int *i)
{
    static const char HELP_SHORT[] = "-h";
    static const char HELP_LONG[] = "--help";
    static const char CANONICAL_SHORT[] = "-C";
    static const char CANONICAL_LONG[] = "--canonical";
    static const char LENGTH_SHORT[] = "-n";
    static const char LENGTH_LONG[] = "--length";
    static const char DECIMAL_SHORT[] = "-d";
    static const char DECIMAL_LONG[] = "--two-bytes-decimal";

#define STRLEN(s) (sizeof(s) - 1)

    enum
    {
        HELP_SHORT_SIZE = STRLEN(HELP_SHORT),
        HELP_LONG_SIZE = STRLEN(HELP_LONG),
        CANONICAL_SHORT_SIZE = STRLEN(CANONICAL_SHORT),
        CANONICAL_LONG_SIZE = STRLEN(CANONICAL_LONG),
        LENGTH_SHORT_SIZE = STRLEN(LENGTH_SHORT),
        LENGTH_LONG_SIZE = STRLEN(LENGTH_LONG),
        DECIMAL_SHORT_SIZE = STRLEN(DECIMAL_SHORT),
        DECIMAL_LONG_SIZE = STRLEN(DECIMAL_LONG)
    };

    if ((memcmp(argv[*i], HELP_SHORT, HELP_SHORT_SIZE) == 0) || (memcmp(argv[*i], HELP_LONG, HELP_LONG_SIZE) == 0))
    {
        print_usage();
    }
    else if ((memcmp(argv[*i], CANONICAL_SHORT, CANONICAL_SHORT_SIZE) == 0) || (memcmp(argv[*i], CANONICAL_LONG, CANONICAL_LONG_SIZE) == 0))
    {
        canonical_option = 1;
    }
    else if ((memcmp(argv[*i], DECIMAL_SHORT, DECIMAL_SHORT_SIZE) == 0) || (memcmp(argv[*i], DECIMAL_LONG, DECIMAL_LONG_SIZE) == 0))
    {
        decimal_option = 1;
    }
    else if ((memcmp(argv[*i], LENGTH_SHORT, LENGTH_SHORT_SIZE) == 0) || (memcmp(argv[*i], LENGTH_LONG, LENGTH_LONG_SIZE) == 0))
    {
        if (*i + 1 >= argc)
        {
            fprintf(stderr, "%s: -n/--length needs a number\n", PROGRAM_NAME);
            exit(1);
        }
        if (argv[*i + 1][0] == '0' && argv[*i + 1][1] == 'x')
        {
            // this is hex input
            length_option = (uint32_t)strtoul(argv[*i + 1], NULL, 16);
        }
        else
        {
            length_option = (uint32_t)strtoul(argv[*i + 1], NULL, 10);
        }
        (*i)++; // consume the number
    }
}

void print_usage(void)
{
    printf("Usage:\n %s [options] <file>...\n\n", PROGRAM_NAME);
    printf("Options:\n");
    printf("-C, --canonical           canonical hex+ASCII display\n");
    printf("-n, --length <length>     interpret only length bytes of input\n");
    printf("-d, --two-bytes-decimal   Display the input offset in hexadecimal, followed by 8 space-separaated, five column, zero-filled, two-byte units of input data\n");
    printf("                          in unsigned decimal, per line.\n");
}
