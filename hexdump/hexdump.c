#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PROGRAM_NAME "hexdump"
#define BUFFER_SIZE 16

void print_usage();
void search_options(int argc, char *argv[], int *i);
void process_file(const char *filename);

int canonical_option = -1;
uint32_t length_option = 0;
char *file_to_read = NULL;

int main(int argc, char *argv[])
{
    printf("argc = %d\n", argc);
    if (argc < 2)
    {
        print_usage();
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            if (argv[i][0] == '-')
            {
                search_options(argc, argv, &i);
            }
            else
            {
                file_to_read = argv[i];
            }
        }
    }

    if (file_to_read)
    {
        process_file(file_to_read);
    }

    return 0;
}

void remove_non_printable_ascii_char(unsigned char *buffer)
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (buffer[i] < '!' || buffer[i] > '~')
        {
            // replace non printable char with '.'
            buffer[i] = '.';
        }
    }
}
void process_input_data(FILE *input)
{
    unsigned char buffer[BUFFER_SIZE + 1] = {0};
    size_t bytes_read;
    size_t total_bytes_read = 0x00;
    size_t buffer_read_size = BUFFER_SIZE;
    // Read data in chunks
    //[TODO] able to change the BUFFER_SIZE accoring to the argument value
    // if default read bytes size is greater than the arg size ( -n )
    if ((length_option != 0) && (length_option < buffer_read_size))
    {
        buffer_read_size = length_option;
    }
    while ((bytes_read = fread(buffer, sizeof(char), buffer_read_size, input)) > 0)
    {
        printf("%08lx ", total_bytes_read);
        total_bytes_read += bytes_read;
        for (uint32_t i = 0; i < bytes_read; i++)
        {
            unsigned char buf = buffer[i];
            printf("%02x ", buf);
        }

        // print ASCII characters option
        if (canonical_option != -1)
        {
            remove_non_printable_ascii_char(buffer);
            // if the hex data is not algign to the BUFFER_SIZE
            // we need to fill up the reamining with spaces
            if (bytes_read < buffer_read_size + 1)
            {
                for (uint32_t i = bytes_read; i < BUFFER_SIZE; i++)
                    printf("   ");
            }

            // there is an extra space before ascii start
            printf(" |");
            for (uint32_t i = 0; i < bytes_read; i++)
            {
                char buf = buffer[i];
                printf("%c", buf);
            }
            printf("|");
        }
        memset(buffer, 0, ((buffer_read_size + 1) * sizeof(unsigned char)));
        printf("\n");

        if (length_option != 0)
        {
            if (total_bytes_read >= length_option)
            {
                break;
            }
            else if ((length_option - total_bytes_read) < buffer_read_size)
            {
                buffer_read_size = (length_option - total_bytes_read);
            }
        }
    }
    // after printing all, let's print the totoal size
    printf("%08lx\n", total_bytes_read);
}

void process_file(const char *filename)
{
    FILE *input_f = fopen(filename, "rb");
    if (input_f == NULL)
    {
        fprintf(stderr, "%s: cannot open %s\n", PROGRAM_NAME, filename);
        return;
    }
    process_input_data(input_f);
    fclose(input_f);
}

void search_options(int argc, char *argv[], int *i)
{
    static const char HELP_SHORT[] = "-h";
    static const char HELP_LONG[] = "--help";
    static const char CANONICAL_SHORT[] = "-C";
    static const char CANONICAL_LONG[] = "--canonical";
    static const char LENGTH_SHORT[] = "-n";
    static const char LENGTH_LONG[] = "--length";

#define STRLEN(s) (sizeof(s) - 1)

    enum
    {
        HELP_SHORT_SIZE = STRLEN(HELP_SHORT),
        HELP_LONG_SIZE = STRLEN(HELP_LONG),
        CANONICAL_SHORT_SIZE = STRLEN(CANONICAL_SHORT),
        CANONICAL_LONG_SIZE = STRLEN(CANONICAL_LONG),
        LENGTH_SHORT_SIZE = STRLEN(LENGTH_SHORT),
        LENGTH_LONG_SIZE = STRLEN(LENGTH_LONG)
    };

    if ((memcmp(argv[*i], HELP_SHORT, HELP_SHORT_SIZE) == 0) || (memcmp(argv[*i], HELP_LONG, HELP_LONG_SIZE) == 0))
    {
        print_usage();
    }
    else if ((memcmp(argv[*i], CANONICAL_SHORT, CANONICAL_SHORT_SIZE) == 0) || (memcmp(argv[*i], CANONICAL_LONG, CANONICAL_LONG_SIZE) == 0))
    {
        canonical_option = 1;
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
}
