

#include <stdio.h>
#include <stdlib.h>
#include <ftdi.h>
#include <string.h>
#include <assert.h>

// HC595 Chip settings
#define HC595_CT   (2)     // Number of chips

// FTDI settings
#define BIT_DATA   (1<<0)  // 1: TXD, "data"
#define BIT_CLOCK  (1<<2)  // 4: RTS, "clock"
#define BIT_LATCH  (1<<3)  // 8: CTS, "latch"
#define VENDOR     0x0403  // Vendor
#define PRODUCT    0x6001  // Product



struct ftdi_context ftdic; // FTDI varibles


char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


void load_delay(int ms)
{
    clock_t start_time = clock();

    while (clock() < start_time + ms);
}

void open_ftdi()
{
    int f;

    // Init FTSI
    if ((f=ftdi_init(&ftdic)) < 0)
    {
        fprintf(stderr, "ftdi_init failed!\n");
        exit(-1);
    }

    // Open FTDI device
    f = ftdi_usb_open(&ftdic, VENDOR, PRODUCT);

    if (f < 0 && f != -5)
    {
        fprintf(stderr, "Unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(&ftdic));
        exit(-1);
    }

    // Set bitmode
    ftdi_set_bitmode(&ftdic, BIT_DATA | BIT_CLOCK | BIT_LATCH, BITMODE_BITBANG);
}

void close_ftdi()
{
    ftdi_disable_bitbang(&ftdic);
    ftdi_usb_close(&ftdic);
    ftdi_deinit(&ftdic);
}

uint8_t hex2int(char *hex) {
    uint32_t val = 0;
    while (*hex) {
        // get current character then increment
        uint8_t byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}


int send_byte____(int data, int ld, int clk, int mi, int ri )
{
    unsigned char buf[2*8*HC595_CT+1];  // Latch pulse, HC595_CT clock pulses
    unsigned char *b=buf;
    unsigned char state, state_ld, state_clk, state_mi, state_ri;
    int f, i;

    printf ("D: %d \n", data);

    *buf = 0;

    // Data
    for (i=0; i<8; i++)
    {
        state = (data & (128L>>i))?BIT_DATA:0;
        *b++=state;
        state |= BIT_CLOCK;
        *b++=state;
    }


    // Address control signals
    for (i=0; i<8; i++) {
        state_ld  = ( i == 7 && ld  == 1 ) ? 1 : 0;
        state_clk = ( i == 6 && clk == 1 ) ? 1 : 0;
        state_mi  = ( i == 5 && mi  == 1 ) ? 1 : 0;
        state_ri  = ( i == 4 && ri  == 1 ) ? 1 : 0;

        state = state_ld || state_clk || state_mi || state_ri || 0 || 0 || 0 || 0;

        *b++=state;
        state |= BIT_CLOCK;
        *b++=state;

        //printf ("S: %d \n", state);
    }

    //printf ("S: %d \n", *b);

    *b++=BIT_LATCH;

    f = ftdi_write_data(&ftdic, buf, (b-buf));

    return f;
}



int send_byte(int addr )
{
    unsigned char buf[2*8*HC595_CT+1];  // Latch pulse, HC595_CT clock pulses
    unsigned char *b=buf;
    unsigned char state, state_ld, state_clk, state_mi, state_ri, state_2;
    int f, i;

    printf ("A: %d \n", addr);

    *buf = 0;

    // Data
    for (i=0; i<8; i++)
    {
        state = (addr & (128L>>i))?BIT_DATA:0;
        *b++=state;
        state |= BIT_CLOCK;
        *b++=state;
    }


    // Address control signals
    for (i=0; i<8; i++) {
        state_ld  = ( i == 7 ) ? 1 : 0;
        state_clk = ( i == 6 ) ? 1 : 0;
        state_mi  = ( i == 5 ) ? 1 : 0;

        state = state_ld || state_clk || state_mi || 0 || 0 || 0 || 0 || 0;

        *b++=state;
        state |= BIT_CLOCK;
        *b++=state;

    }

    *b++=BIT_LATCH;

    f = ftdi_write_data(&ftdic, buf, (b-buf));
    load_delay(50);
    *buf = 0;
    *b=buf;
    f = ftdi_write_data(&ftdic, 0, 15 );



    return f;
}


int send_byte_d(int data )
{
    unsigned char buf[2*8*HC595_CT+1];  // Latch pulse, HC595_CT clock pulses
    unsigned char *b=buf;
    unsigned char state, state_ld, state_clk, state_mi, state_ri;
    int f, i;

    printf ("D: %d \n", data);

    *buf = 0;

    // Data
    for (i=0; i<8; i++)
    {
        state = (data & (128L>>i))?BIT_DATA:0;
        *b++=state;
        state |= BIT_CLOCK;
        *b++=state;
    }


    // Address control signals
    for (i=0; i<8; i++) {
        state_ld  = ( i == 7 ) ? 1 : 0;
        state_clk = ( i == 6 ) ? 1 : 0;
        state_ri  = ( i == 4 ) ? 1 : 0;

        state = state_ld || state_clk || 0 || state_ri || 0 || 0 || 0 || 0;

        *b++=state;
        state |= BIT_CLOCK;
        *b++=state;

    }


    *b++=BIT_LATCH;

    f = ftdi_write_data(&ftdic, buf, (b-buf));

    load_delay(50);



    f = ftdi_write_data(&ftdic, 0, 16);


    printf ( "B: %d \n", buf );

    return f;
}



int main(int argc, char **argv)
{
    FILE *fp;
    char *filename;
    char byte_data[65536];

    // Check datafile arugment exist
    if (argc < 2)
    {
        fprintf(stderr, "Missing Filename\n");
        exit(-1);
    }


    // Open data file
    filename = argv[1];
    printf("Data file: %s\n", filename);

    fp = fopen(filename,"r");

    // If file opened successfully, then get the contents
    if ( fp )
    {
        fgets(byte_data, sizeof byte_data, fp);
        fclose(fp);
    }
    else
    {
        fprintf(stderr, "Failed to open the file\n");
        exit(-1);
    }


    // Open FTDI device
    open_ftdi();

    // Set all bits to zero
    //send_byte( 0, 0);

    char** bytes;
    unsigned char data;

    bytes = str_split(byte_data, ' ');

    if (bytes)
    {
        int addr;
        for (addr = 0; *(bytes + addr); addr++)
        {
            data = hex2int( *(bytes + addr ) );

            // DATA - LD - CLK - MI - RI

            //printf ( "S D: %d \n ", data );

            send_byte( addr );   // Set data
            load_delay(100);
            send_byte_d( data );   // Set data


            //send_byte( addr, 1, 1, 1, 0);   // Set data
            //load_delay(1);
           // send_byte( addr, 1, 0, 0, 0);   // Set address
            //load_delay(1);
           // send_byte( 0, 1, 0, 0, 0);   // Set address

        //    load_delay(300);

        //    send_byte( data, 1, 1, 0, 1);   // Set data
          //  load_delay(10);
        //    send_byte( data, 1, 0, 0, 1);   // Set data
          //  load_delay(10);
//            send_byte( 0, 1, 0, 0, 0);   // Set address


          //  load_delay(500);


            printf ("\n");


            free(*(bytes + addr));
        }

        free(bytes);


        //send_byte( 0 );   // Set data

        // Set all bits to zero
        //send_byte(0, 0, 0);
    }

    // Close FTDI device
    close_ftdi();

    return 0;
}