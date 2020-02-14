/*
 * *********************************************************************************
 * Processor microcode 
 * *********************************************************************************
 */
 // ROM 1
#define O1  0b00000001000000000000000000000000
#define O2  0b00000010000000000000000000000000
#define O3  0b00000100000000000000000000000000
#define ROE 0b00001000000000000000000000000000
#define RRW 0b00010000000000000000000000000000
#define TI  0b00100000000000000000000000000000
#define TO  0b01000000000000000000000000000000
#define SND 0b10000000000000000000000000000000

// ROM 2
#define HLT 0b00000000000000000000000100000000
#define JBO 0b00000000000000000000001000000000
#define JAO 0b00000000000000000000010000000000
#define SU  0b00000000000000000000100000000000
#define RI  0b00000000000000000001000000000000
#define BO  0b00000000000000000010000000000000
#define AO  0b00000000000000000100000000000000
#define SF  0b00000000000000001000000000000000

// ROM 3
#define II  0b00000000000000010000000000000000
#define MIL 0b00000000000000100000000000000000
#define MIH 0b00000000000001000000000000000000
#define RO  0b00000000000010000000000000000000
#define CE  0b00000000000100000000000000000000
#define COL 0b00000000001000000000000000000000
#define COH 0b00000000010000000000000000000000
#define AI  0b00000000100000000000000000000000

// ROM 4
#define ICR 0b00000000000000000000000000000001
#define BI  0b00000000000000000000000000000010
#define EO  0b00000000000000000000000000000100
#define JML 0b00000000000000000000000000001000
#define JMH 0b00000000000000000000000000010000
#define JMZ 0b00000000000000000000000000100000
#define JMN 0b00000000000000000000000001000000
#define JMC 0b00000000000000000000000010000000

// Fetch OLD
//#define FCH MIL|COL, RO|II|CE

// Fetch
#define FCH MIL|COL, MIH|COH, RO|II|CE

// Null values
#define NLS ICR,0,0

// Out 100
#define D100 O3|O1
#define D10  O1|O2
#define D1   O1
#define CHAR O1|O2|O3
#define INS  O2|O3


// OP CODE 16
const PROGMEM uint32_t data[] = {

//    C4        C5         C6            C7            C8         C9           C10          C11         C12           C13          C14      C15           C16         CMD           HEX   ADDRESSING
//===================================================================================================================================================================================================
FCH,  ICR,      0,         0,            0,            0,         0,           0,           0,          0,            0,           0,       0,            0,       // NOP           0x00  Implied
FCH,  COL|MIL,  COH|MIH,   RO|AI|CE,     ICR,          0,         0,           0,           0,          0,            0,           0,       0,            0,       // LDI           0x01  Immediate
FCH,  COL|MIL,  COH|MIH,   RO|BI|CE,     AI|EO|SF,     ICR,       0,           0,           0,          0,            0,           0,       0,            0,       // ADC           0x02  Immediate
FCH,  HLT,      0,         0,            0,            0,         0,           0,           0,          0,            0,           0,       0,            0,       // HLT           0x03  Implied
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|JMH,      TO|JML,      ICR,        0,            0,           0,       0,            0,       // JMP           0x04  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|BI|CE,     AI|SU|EO|SF,  ICR,       0,           0,           0,          0,            0,           0,       0,            0,       // SBC           0x05  Immediate
FCH,  COL|MIL,  COH|MIH,   RO|BI|CE,     SU|EO|SF,     ICR,       0,           0,           0,          0,            0,           0,       0,            0,       // CMP           0x06  Immediate
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   TO|JML|JMZ,  RO|JMH|JMZ,  CE,         ICR,          0,           0,       0,            0,       // BEQ           0x07  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   TO|JML|JMC,  RO|JMH|JMC,  CE,         ICR,          0,           0,       0,            0,       // BCS           0x08  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   TO|JML|JMN,  RO|JMH|JMN,  CE,         ICR,          0,           0,       0,            0,       // BMI           0x09  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL|CE,   RI|AO,      ICR,          0,           0,       0,            0,       // STA           0x0A  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL|CE,   RO|AI|SF,   ICR,          0,           0,       0,            0,       // LDA           0x0B  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|O2|O3|CE,  ICR,          0,         0,           0,           0,          0,            0,           0,       0,            0,       // OUT I         0x0C  Immediate
FCH,  COL|MIL,  COH|MIH,   RO|CHAR|CE,   ICR,          0,         0,           0,           0,          0,            0,           0,       0,            0,       // OUT C         0x0D  Immediate
FCH,  COL|MIL,  COH|MIH,   RO|D100,      CE,           RO|D10,    0,           RO|D1,       ICR,        0,            0,           0,       0,            0,       // OUT D         0x0E  Immediate
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL|CE,   RO|O2|O3,   ICR,          0,           0,       0,            0,       // OUT I         0x0F  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL|CE,   RO|CHAR,    ICR,          0,           0,       0,            0,       // OUT C         0x10  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL,      RO|D100,    CE,           RO|D10,      0,       RO|D1,        0,       // OUT D         0x11  Absolute
FCH,  AO|O2|O3, ICR,       0,            0,            0,         0,           0,           0,          0,            0,           0,       0,            0,       // OUT I         0x12  Accumulator
FCH,  AO|CHAR,  ICR,       0,            0,            0,         0,           0,           0,          0,            0,           0,       0,            0,       // OUT C         0x13  Accumulator
FCH,  AO|D100,  0,         AO|D10,       0,            AO|D1,     ICR,         0,           0,          0,            0,           0,       0,            0,       // OUT D         0x14  Accumulator
FCH,  AO|BI,    AI|SF|EO,  ICR,          0,            0,         0,           0,           0,          0,            0,           0,       0,            0,       // ASL a         0x15  Accumulator
FCH,  COL|MIL,  COH|MIH,   CE|RO,        RO|SND,       RO|SND,    RO|SND,      RO|SND,      RO,         ICR,          0,           0,       0,            0,       // SND           0x16  Immediate
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL,      RI|JAO,     JAO|CE,       ICR,         0,       0,            0,       // SJA nnnn      0x17  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL,      RI|JBO,     JBO|CE,       ICR,         0,       0,            0,       // SJB nnnn      0x18  Absolute
FCH,  JAO|AI,   JAO|CE,    ICR,          0,            0,         0,           0,           0,          0,            0,           0,       0,            0,       // LJA a         0x19  Accumulator
FCH,  JBO|AI,   JAO|CE,    ICR,          0,            0,         0,           0,           0,          0,            0,           0,       0,            0,       // LJB a         0x1A  Accumulator
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL,      RO|BI|CE,   AI|EO|SF,     ICR,         0,       0,            0,       // ADC nnnn      0x1B  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL,      RO|BI|CE,   AI|SU|EO|SF,  ICR,         0,       0,            0,       // SBC nnnn      0x1C  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL,      RO|BI|CE,   SU|EO|SF,     ICR,         0,       0,            0,       // CMP nnnn      0x1D  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL,      RO|SND|CE,  RO|SND,       ICR,         0,       0,            0,       // SND nnnn      0x1E  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|MIL|CE,    MIH,          RO|BI,     AI|EO|SF,    ICR,         0,          0,            0,           0,       0,            0,       // ADC nn        0x1F  Zeropage
FCH,  AO|MIL,   TO|MIH,    RO|CHAR,      ICR,          0,         0,           0,           0,          0,            0,           0,       0,            0,       // OUT c, a, t   0x20  Accu./Treg
FCH,  AO|TI,    ICR,       0,            0,            0,         0,           0,           0,          0,            0,           0,       0,            0,       // LDT a         0x21  Implied
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     ICR,          0,         0,           0,           0,          0,            0,           0,       0,            0,       // LDT #nn       0x22  Immediate
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL,      RO|TI|CE,   ICR,          0,           0,       0,            0,       // LDT nnnn      0x23  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|MIH|CE,    AO|MIL,       RO|AI,     ICR,         0,           0,          0,            0,           0,       0,            0,       // LDA nn, a     0x24  Abs./Accu.
FCH,  AO,       AO|SND,    AO|SND,       AO|SND,       AO|SND,    AO,          ICR,         0,          0,            0,           0,       0,            0,       // SND a         0x25  Accumulator
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIL,      TO|MIH,      RI|COL,     TO|MIL,       RI|COH,      CE,      ICR,          0,       // JSR nnnn      0x26  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|JMH,      TO|JML,      CE,         CE,           CE,          CE,      ICR,          0,       // RTS           0x27  Implied
FCH,  COL|MIL,  COH|MIH,   RO|MIL|CE,    MIH,          RO|BI,     AI|SU|EO|SF, ICR,         0,          0,            0,           0,       0,            0,       // SBC nn        0x28  Zeropage
FCH,  COL|MIL,  COH|MIH,   RO|JML,       JMH,          ICR,       0,           0,           0,          0,            0,           0,       0,            0,       // JMP nn        0x29  Zeropage
FCH,  COL|MIL,  COH|MIH,   RO|MIL,       MIH|CE,       RO|BI,     SU|EO|SF,    ICR,         0,          0,            0,           0,       0,            0,       // CMP nn        0x2A  Zeropage
FCH,  COL|MIL,  COH|MIH,   RO|JML|JMZ,   JMH|JMZ,      CE,        ICR,         0,           0,          0,            0,           0,       0,            0,       // BEQ nn        0x2B  Zeropage
FCH,  COL|MIL,  COH|MIH,   RO|JML|JMC,   JMH|JMC,      CE,        ICR,         0,           0,          0,            0,           0,       0,            0,       // BCS nn        0x2C  Zeropage
FCH,  COL|MIL,  COH|MIH,   RO|JML|JMN,   JMH|JMN,      CE,        ICR,         0,           0,          0,            0,           0,       0,            0,       // BMI nn        0x2D  Zeropage
FCH,  COL|MIL,  COH|MIH,   RO|MIL,       MIH|CE,       RI|AO,     ICR,         0,           0,          0,            0,           0,       0,            0,       // STA nn        0x2E  Zeropage    
FCH,  COL|MIL,  COH|MIH,   RO|MIL,       MIH|CE,       RO|AI|SF,  ICR,         0,           0,          0,            0,           0,       0,            0,       // LDA nn        0x2F  Zeropage    
FCH,  COL|MIL,  COH|MIH,   RO|BI|CE,     COL|MIL,      COH|MIH,   RO|TI|CE,    COL|MIL,     COH|MIH,    RO|MIH,       CE|TO|MIL,   RO|AI,   EO|RI|SF,     RO|AI,   // INC nnnn      0x30  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|BI|CE,     COL|MIL,      COH|MIH,   RO|MIL,      MIH|CE,      RO|AI,      EO|RI|SF,     RO|AI,       ICR,     0,            0,       // INC nn        0x31  Zeropage
FCH,  COL|MIL,  COH|MIH,   RO|BI|CE,     COL|MIL,      COH|MIH,   RO|TI|CE,    COL|MIL,     COH|MIH,    RO|MIH,       CE|TO|MIL,   RO|AI,   EO|SU|SF|RI,  RO|AI,   // DEC nnnn      0x32  Absolute
FCH,  COL|MIL,  COH|MIH,   RO|BI|CE,     COL|MIL,      COH|MIH,   RO|MIL,      MIH|CE,      RO|AI,      EO|SU|SF|RI,  RO|AI,       ICR,     0,            0,       // DEC nn        0x33  Zeropage
FCH,  COL|MIL,  COH|MIH,   RO|BI|CE,     COL|MIL,      COH|MIH,   RO|TI|CE,    COL|MIL,     COH|MIH,    RO|MIH,       CE|TO|MIL,   BO|RI,   ICR,          0,       // STM #nn, nnnn 0x34  Imp. Abs.
FCH,  COL|MIL,  COH|MIH,   RO|BI|CE,     COL|MIL,      COH|MIH,   MIH|CE,      RO|MIL,      BO|RI,      ICR,          0,           0,       0,            0,       // STM #nn, nn   0x35  Imp. Zero.

// testrelendo parancsok:

FCH,  COL|MIL,  COH|MIH,   RO|INS|CE,    COL|MIL,      COH|MIH,    RO|INS|CE,  ICR,         0,          0,            0,           0,       0,            0,       // OUT i, x, y    0x36  Immediate
FCH,  AO|INS,   COL|MIL,   COH|MIH,      RO|INS|CE,    ICR,        0,          0,           0,          0,            0,           0,       0,            0,       // OUT i, a, y    0x37  Immediate
FCH,  COL|MIL,  COH|MIH,   RO|INS|CE,    AO|INS,       ICR,        0,          0,           0,          0,            0,           0,       0,            0,       // OUT i, x, a    0x38  Immediate

FCH,  COL|MIL,  COH|MIH,   RO|MIH|CE,    AO|MIL,       RO|CHAR,    ICR,        0,           0,          0,            0,           0,       0,            0,       // OUT c, a, unn  0x39  Accu./Upper

FCH,  COL|MIL,  COH|MIH,   RO|CHAR|CE,   0,            RO|CHAR,    ICR,        0,           0,          0,            0,           0,       0,            0,       // OUT c, #nn, x2  0x3A
FCH,  COL|MIL,  COH|MIH,   RO|CHAR|CE,   0,            RO|CHAR,    0,          RO|CHAR,     ICR,        0,            0,           0,       0,            0,       // OUT c, #nn, x3  0x3B
FCH,  COL|MIL,  COH|MIH,   RO|CHAR|CE,   0,            RO|CHAR,    0,          RO|CHAR,     0,          RO|CHAR,      ICR,         0,       0,            0,       // OUT c, #nn, x4  0x3C
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,    RO|MIH,     TO|MIL|CE,   RO|CHAR,    0,            RO|CHAR,     ICR,     0,            0,       // OUT c, nnnn, x2 0x3D
FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,    RO|MIH,     TO|MIL|CE,   RO|CHAR,    0,            RO|CHAR,     0,       RO|CHAR,      ICR,     // OUT c, nnnn, x3 0x3E
FCH,  COL|MIL,  COH|MIH,   RO|MIH|CE,    AO|MIL,       RO|CHAR,    0,          RO|CHAR,     ICR,        0,            0,           0,       0,            0,       // OUT c, a, unn x2  0x3F
FCH,  COL|MIL,  COH|MIH,   RO|MIH|CE,    AO|MIL,       RO|CHAR,    0,          RO|CHAR,     0,          RO|CHAR,      ICR,         0,       0,            0,       // OUT c, a, unn x3  0x40
FCH,  COL|MIL,  COH|MIH,   RO|MIH|CE,    AO|MIL,       RO|CHAR,    0,          RO|CHAR,     0,          RO|CHAR,      0,           RO|CHAR, ICR,          0,       // OUT c, a, unn x4  0x41

FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL|CE,   RO|BI|AI,   AI|EO|RI|SF,  ICR,         0,       0,            0,       // ASL nnnn      0x42  Absolute

FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH|CE,   AO|MIL,      TO|RI,      ICR,          0,           0,       0,            0,       // STM nn, a, #nn   0x43

FCH, COL|MIL,  COH|MIH,    RO|TI|CE,     COL|MIL,     COH|MIH,   RO|MIL,      TO|MIH|CE,     RO|CHAR,    ICR,          0,           0,       0,            0,       // OUT c, nn, nn   0x44


FCH,  COL|MIL,  COH|MIH,   RO|TI|CE,     COL|MIL,      COH|MIH,   RO|MIH,      TO|MIL,      RO|MIH|CE,   AO|MIL,         RO|CHAR,       ICR,       0,            0,       // OUT c, a, HH  0x45


//FCH,  COL|MIL,  COH|MIH,  CE|RO|RRW,    COL|MIL,      COH|MIH,    RO|MIL|CE,   ROE|RI,    COL|MIL,    RO|MIL|CE,  ROE|RI,  NULLS,   // RTS    0x34
FCH,  COL|MIL,  CE|RO|RRW, COL|MIL,      RO|MIL|CE,    ROE|RI,     COL|MIL,    RO|MIL|CE,   ROE|RI,       ICR,           0,           0,       0,            0,        // RTR    0x36   
FCH,  COL|MIL,  COH|MIH,   CE|RO|RRW,    COL|MIL,      COH|MIH,    RO|CE|RRW,  COL|MIL,     COH|MIH,      RO|CE|RRW,     ICR,         0,       0,            0,        // RTW    0x37

};

/*
    BEQ, BCS, BMI:  CE nem biztos hogy kell es a forditot is ellenorizni ehhez
    INC, DEC:  Ezeken lehet hogy lehetne optimalizalni, ha megcsereljuk az A, B reg muveleteket
*/

/*
FETCH,  COL|MIL,      CE|RO|RRW,       COL|MIL,      RO|MIL|CE,   ROE|RI,    COL|MIL,    RO|MIL|CE,  ROE|RI,  NULLS,   // RTR    0x17   
FETCH,  COL|MIL,      CE|RO|RRW,       COL|MIL,      RO|CE|RRW,   COL|MIL,   RO|CE|RRW,  ICR,        0,       NULLS,   // RTW    0x18   
};
*/


/*
 * *********************************************************************************
 * EEPROM WRITER
 * *********************************************************************************
 */
 
/*
 * Variables
 */
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

int command;
int shift;

/*
 * Set EEPROM Address
 */
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(1);
}

/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}

void writeROM(int shift)
{
    int cmds;
    int idx;
    int curr;
    int addr;
    int i = 0;
    
    cmds = (( sizeof(data)/sizeof( pgm_read_dword(data + 0)) / 16 ) - 1 );
    idx = 0;
    
    for (i = 0; i <= cmds; i++) {
      curr = 0;

      //Serial.println( " " );
      Serial.print( "." );
      //Serial.print( i );
      //Serial.print( ": " );

      for (int j = 15; j >= 0; j--) {
        addr = curr + i;
        curr = (curr + 128);

        int dat = pgm_read_dword(data + idx) >> shift;

        //Serial.print( addr );
        //Serial.print( " - " );
        /*Serial.print( ":" );
        Serial.print( dat );
        Serial.print( " " );*/

        writeEEPROM( addr, dat );
        
        idx++;
      } 
    }

    Serial.println( " Done!" );
    Serial.print( i );
    Serial.print( " commands. \n\n" );
}


/*
 * Init the EEPROM Writer
 */
void setup() {
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);

  Serial.println("Write specified EEPROM: 1, 2, 3, 4 (n = EEPROM number)");
  Serial.println("Erase EEPROM: e");
  Serial.println("Read EEPROM: r \n");
}

/*
 * Program loop
 */
void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();

    switch (command) {
      
      //Erase EEPROM
      case 'e':
        Serial.print("Erasing EEPROM .");
        
        for (int address = 0; address <= 2048; address += 1) {
          writeEEPROM(address, 0x00);
          if (address % 64 == 0) Serial.print(".");
        }

        Serial.print(" Done!\n\n");
      break;

      //Read EEPROM
      case 'r':
        Serial.print("Reading EEPROM ... \n\n");
      
        for (int base = 0; base <= 2048; base += 16) {
          byte data[16];
          for (int offset = 0; offset <= 15; offset += 1) {
            data[offset] = readEEPROM(base + offset);
          }
      
          char buf[80];
          sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
                  base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
                  data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
      
          Serial.println(buf);
        }
      break;

      //Write EEPROM's
      case '1':
        Serial.println("Wrinting ROM 1 ");
        writeROM(24);
        break;
      case '2':
        Serial.println("Wrinting ROM 2 ");
        writeROM(8);
        break;
      case '3':
        Serial.println("Wrinting ROM 3 ");
        writeROM(16);
        break;
      case '4':
        Serial.println("Wrinting ROM 4 ");
        writeROM(0);
        break;
    }
  }
}
