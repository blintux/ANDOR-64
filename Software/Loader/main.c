// Includes
#include <stdio.h>
#include <stdlib.h>
#include <ftdi.h>
#include <string.h>
#include <assert.h>

// FTDI settings
#define VENDOR     (0x0403)  // Vendor
#define PRODUCT    (0x6001)  // Product
#define BIT_DATA   (0x01)    // Data - TXD
#define BIT_CLOCK  (0x04)    // Clock - RTS
#define BIT_LATCH  (0x08)    // Latch - CTS
#define BIT_ENABLE (0x10)    // Output - DTR

/* FDI PINS:
TX  0x01
RX  0x02
RTS 0x04
CTS 0x08
DTR 0x10
DSR 0x20
DCD 0x40
RI  0x80
*/


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
    ftdi_set_bitmode(&ftdic, BIT_DATA | BIT_CLOCK | BIT_LATCH | BIT_ENABLE, BITMODE_BITBANG);

    ftdi_set_baudrate(&ftdic, 4500);

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



int send_byte(int data, int ld, int clk, int mil, int mih, int ri, int rst )
{
    unsigned char buf[2*8*2+1];  // Latch pulse, HC595_CT clock pulses
    unsigned char *b=buf;
    unsigned char state, state_ld, state_clk, state_mil, state_mih, state_ri, state_rst;
    int f, i;

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
        state_clk = ( i == 6 && clk == 1 ) ? 1 : 0;
        state_ld  = ( i == 5 && ld  == 1 ) ? 1 : 0;
        state_mil = ( i == 4 && mil == 1 ) ? 1 : 0;
        state_mih = ( i == 3 && mih == 1 ) ? 1 : 0;
        state_ri  = ( i == 2 && ri  == 1 ) ? 1 : 0;
        state_rst = ( i == 1 && rst == 1 ) ? 1 : 0;

        state = 0 || state_ld || state_clk || state_mil || state_mih || state_ri || state_rst || 0 ;

        *b++=state;
        state |= BIT_CLOCK;
        *b++=state;
    }

    *b++=BIT_LATCH;

    f = ftdi_write_data(&ftdic, buf, (b-buf));

    //load_delay(0.1);

    return f;
}

const char *FormatBytes(long long bytes, char *str)
{
    const char *sizes[5] = { "B", "KB", "MB", "GB", "TB" };

    int i;
    double dblByte = bytes;
    for (i = 0; i < 5 && bytes >= 1024; i++, bytes /= 1024)
        dblByte = bytes / 1024.0;

    sprintf(str, "%.2f", dblByte);

    return strcat(strcat(str,  " "), sizes[i]);
}


int main(int argc, char **argv)
{
    FILE *fp;
    char *filename;
    char byte_data[65536];
    char byte_split[65536];
    char s[32] = "";

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

    int j = 1;

    byte_split[0]=byte_data[0];

    for ( int i=1; i<strlen(byte_data); i++ ) {

        div_t ize = div(i,2);

        if( ize.rem == 0 ){
           byte_split[j]=' ';
           j++;
        }

        byte_split[j]=byte_data[i];

        j++;

    }

    //printf("KI:%s\n", byte_split);


// byte_split

    bytes = str_split(byte_split, ' ');

    if (bytes)
    {
        // Send reset signal
        send_byte(0, 1, 0, 0, 0, 0, 1 );
        load_delay(5);

        int addr;

        int uppers = 0;
        int lowers = 0;

        int total_bytes = 0;
        int uppers_last = 0;

        for (addr = 0; *(bytes + addr); addr++)
        {
            data = hex2int( *(bytes + addr ) );

            //printf (" Byte: %d \t Address: %#04x \t Data: %#04x\n", addr, addr, data);

            // Set address

            if ( uppers != uppers_last ) {
                send_byte(uppers, 1, 0, 0, 1, 0, 0 );
                send_byte(uppers, 1, 1, 0, 1, 0, 0 );
               // send_byte(uppers, 1, 0, 0, 1, 0, 0 );
            }

            uppers_last = uppers;

            //        ADDR  LD  CLK MIL MIH RI  RST
            send_byte(addr, 1,  0,  1,  0,  0,  0 );
            send_byte(addr, 1,  1,  1,  0,  0,  0 );
           // send_byte(addr, 1,  0,  1,  0,  0,  0 );

            // Write data
            send_byte(data, 1, 0, 0, 0, 1, 0 );
            send_byte(data, 1, 1, 0, 0, 1, 0 );
            //send_byte(data, 1, 0, 0, 0, 1, 0 );


            free(*(bytes + addr));

            lowers++;

            if ( lowers >= 256 ) {
                uppers++;
                lowers = 0;
            }

            total_bytes++;
        }

        free(bytes);

        load_delay(5);

        // Write RTS command
        // Uppers
        send_byte(255, 1, 0, 0, 1, 0, 0 );
        send_byte(255, 1, 1, 0, 1, 0, 0 );
        // Lowers
        send_byte(253, 1,  0,  1,  0,  0,  0 );
        send_byte(253, 1,  1,  1,  0,  0,  0 );
        // Data
        send_byte(39, 1, 0, 0, 0, 1, 0 );
        send_byte(39, 1, 1, 0, 0, 1, 0 );
        send_byte(39, 1, 0, 0, 0, 1, 0 );

        //        ADDR  LD  CLK MIL MIH RI  RST


        // Send reset signal
        send_byte(0, 1, 0, 0, 0, 0, 1 );
        load_delay(5);


        printf("Data sent: %d byte (%s) \n", total_bytes, FormatBytes(total_bytes, s) );
    }

    // Close FTDI device
    close_ftdi();

    return 0;
}
