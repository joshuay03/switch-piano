#define F_CPU 16000000UL
		 
#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__ 
#include <util/delay.h>

/*
 *  Setting, clearing, and reading bits in registers. reg is the name of a register and pin is the index 
 * (0..7) of the bit to set, clear or read.
 */
#define SET_BIT(reg, pin) (reg) |= (1 << (pin))
#define CLEAR_BIT(reg, pin) (reg) &= ~(1 << (pin))
#define WRITE_BIT(reg, pin, value) (reg) = (((reg) & ~(1 << (pin))) | ((value) << (pin)))
#define BIT_VALUE(reg, pin) (((reg) >> (pin)) & 1)
#define BIT_IS_SET(reg, pin) (BIT_VALUE((reg),(pin))==1)

//UART definitions
//define baud rate 
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

// Data direction register, data register, input pin address and pin number definitions

// Define data direction register, data register, input pin address and pin number of the Speaker
#define SPEAKER_DDR DDRB
#define SPEAKER_PORT PORTB
#define SPEAKER_PINNUMBER 4
// Define the data direction register, data register, input pin address and pin number of the LED
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_PIN PINB
#define LED_PINNUMBER 5
// Define the data direction register, data register, input pin address and pin number of the 'Middle C' 
// switch
#define MIDDLECSWITCH_DDR DDRB
#define MIDDLECSWITCH_PORT PORTB
#define MIDDLECSWITCH_PIN PINB
#define MIDDLECSWITCH_PINNUMBER 1
// Define the data direction register, data register, input pin address and pin number of the 'D' switch
#define DSWITCH_DDR DDRB
#define DSWITCH_PORT PORTB
#define DSWITCH_PIN PINB
#define DSWITCH_PINNUMBER 0
// Define the data direction register, data register, input pin address and pin number of the 'E' switch
#define ESWITCH_DDR DDRD
#define ESWITCH_PORT PORTD
#define ESWITCH_PIN PIND
#define ESWITCH_PINNUMBER 7
// Define the data direction register, data register, input pin address and pin number of the 'F' switch
#define FSWITCH_DDR DDRD
#define FSWITCH_PORT PORTD
#define FSWITCH_PIN PIND
#define FSWITCH_PINNUMBER 6
// Define the data direction register, data register, input pin address and pin number of the 'G' switch
#define GSWITCH_DDR DDRD
#define GSWITCH_PORT PORTD
#define GSWITCH_PIN PIND
#define GSWITCH_PINNUMBER 5
// Define the data direction register, data register, input pin address and pin number of the 'A' switch
#define ASWITCH_DDR DDRD
#define ASWITCH_PORT PORTD
#define ASWITCH_PIN PIND
#define ASWITCH_PINNUMBER 4
// Define the data direction register, data register, input pin address and pin number of the 'B' switch
#define BSWITCH_DDR DDRD
#define BSWITCH_PORT PORTD
#define BSWITCH_PIN PIND
#define BSWITCH_PINNUMBER 3
// Define the data direction register, data register, input pin address and pin number of the 'C' switch
#define CSWITCH_DDR DDRD
#define CSWITCH_PORT PORTD
#define CSWITCH_PIN PIND
#define CSWITCH_PINNUMBER 2

// Musical note frequency definitions

// Define the frequency for the musical note 'Middle C'
#define MIDDLEC_FREQUENCY 261.63
// Define the frequency for the musical note 'D'
#define D_FREQUENCY 293.66
// Define the frequency for the musical note 'E'
#define E_FREQUENCY 329.63
// Define the frequency for the musical note 'F'
#define F_FREQUENCY 349.23
// Define the frequency for the musical note 'G'
#define G_FREQUENCY 392
// Define the frequency for the musical note 'A'
#define A_FREQUENCY 440
// Define the frequency for the musical note 'B'
#define B_FREQUENCY 493.88
// Define the frequency for the musical note 'C'
#define C_FREQUENCY 523.25

// Functions declaration
void setup(void);
void process(void);
void uart_init(unsigned int ubrr);
void uart_putchar(unsigned char data);
void switchpiano(float duration, float frequency, unsigned char note);

void setup(void) {
  // initialise uart 
  uart_init(MYUBRR);

  // Set up data direction register Pin 7 for output
  SET_BIT(SPEAKER_DDR, SPEAKER_PINNUMBER);

	// Enable B5 as output, led on B5
	SET_BIT(LED_DDR, LED_PINNUMBER);

  // Enable D6 and D7 as inputs
  CLEAR_BIT(MIDDLECSWITCH_DDR, MIDDLECSWITCH_PINNUMBER);
  CLEAR_BIT(DSWITCH_DDR, DSWITCH_PINNUMBER);
  CLEAR_BIT(ESWITCH_DDR, ESWITCH_PINNUMBER);
  CLEAR_BIT(FSWITCH_DDR, FSWITCH_PINNUMBER);
  CLEAR_BIT(GSWITCH_DDR, GSWITCH_PINNUMBER);
  CLEAR_BIT(ASWITCH_DDR, ASWITCH_PINNUMBER);
  CLEAR_BIT(BSWITCH_DDR, BSWITCH_PINNUMBER);
  CLEAR_BIT(CSWITCH_DDR, CSWITCH_PINNUMBER);

  OCR0A = 128;
  // set PWM for 50% duty cycle

  TCCR0A |= (1 << COM0A1);
  // set none-inverting mode
 
  // TinkerCAD Errata: timer clocking must be enabled before WGM
  // set prescaler to 8 and starts PWM  
  TCCR0B = (1 << CS01);
  
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  // set fast PWM Mode
}

void process(void) {
  unsigned char sent_char_middle_c = 'C';
  unsigned char sent_char_d = 'd';
  unsigned char sent_char_e = 'e';
  unsigned char sent_char_f = 'f';
  unsigned char sent_char_g = 'g';
  unsigned char sent_char_a = 'a';
  unsigned char sent_char_b = 'b';
  unsigned char sent_char_c = 'c';

  //send character for heartbeat, for non blocking read/write
  uart_putchar('\n');

  // Detect pressed 'Middle C' switch
  if (BIT_IS_SET(MIDDLECSWITCH_PIN, MIDDLECSWITCH_PINNUMBER)) {
    switchpiano(15, MIDDLEC_FREQUENCY, sent_char_middle_c);
    while ( BIT_IS_SET(MIDDLECSWITCH_PIN, MIDDLECSWITCH_PINNUMBER) ) {
      // Block until switch released.
    }
  }
  
  // Detect pressed 'D' switch
  if (BIT_IS_SET(DSWITCH_PIN, DSWITCH_PINNUMBER)) {
    switchpiano(15, D_FREQUENCY, sent_char_d);
    while ( BIT_IS_SET(DSWITCH_PIN, DSWITCH_PINNUMBER) ) {
      // Block until switch released.
    }
  }

  // Detect pressed 'E' switch
  if (BIT_IS_SET(ESWITCH_PIN, ESWITCH_PINNUMBER)) {
    switchpiano(15, E_FREQUENCY, sent_char_e);
    while ( BIT_IS_SET(ESWITCH_PIN, ESWITCH_PINNUMBER) ) {
      // Block until switch released.
    }
  }

  // Detect pressed 'F' switch
  if (BIT_IS_SET(FSWITCH_PIN, FSWITCH_PINNUMBER)) {
    switchpiano(15, F_FREQUENCY, sent_char_f);
    while ( BIT_IS_SET(FSWITCH_PIN, FSWITCH_PINNUMBER) ) {
      // Block until switch released.
    }
  }

  // Detect pressed 'G' switch
  if (BIT_IS_SET(GSWITCH_PIN, GSWITCH_PINNUMBER)) {
    switchpiano(15, G_FREQUENCY, sent_char_g);
    while ( BIT_IS_SET(GSWITCH_PIN, GSWITCH_PINNUMBER) ) {
      // Block until switch released.
    }
  }

  // Detect pressed 'A' switch
  if (BIT_IS_SET(ASWITCH_PIN, ASWITCH_PINNUMBER)) {
    switchpiano(15, A_FREQUENCY, sent_char_a);
    while ( BIT_IS_SET(ASWITCH_PIN, ASWITCH_PINNUMBER) ) {
      // Block until switch released.
    }
  }

  // Detect pressed 'B' switch
  if (BIT_IS_SET(BSWITCH_PIN, BSWITCH_PINNUMBER)) {
    switchpiano(15, B_FREQUENCY, sent_char_b);
    while ( BIT_IS_SET(BSWITCH_PIN, BSWITCH_PINNUMBER) ) {
      // Block until switch released.
    }
  }

  // Detect pressed 'C' switch
  if (BIT_IS_SET(CSWITCH_PIN, CSWITCH_PINNUMBER)) {
    switchpiano(15, C_FREQUENCY, sent_char_c);
    while ( BIT_IS_SET(CSWITCH_PIN, CSWITCH_PINNUMBER) ) {
      // Block until switch released.
    } 
  }
}

// Initialize the UART
void uart_init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)(ubrr);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3<<UCSZ00);
}

//transmit data
void uart_putchar(unsigned char data) {
  while (!( UCSR0A & (1<<UDRE0))); /* Wait for empty transmit buffer*/
  
  UDR0 = data;            /* Put data into buffer, sends the data */    	
}

void switchpiano(float duration, float frequency, unsigned char note) {
  // Physics variables
  float wavelength;
  long int i, cycles;

  wavelength=(1.0/frequency)*1000;
  // Standard physics formula.
  cycles=duration/wavelength;
  // The number of cycles.
  #define half_period wavelength/2

  uart_putchar(note);
  
  // The output pin 7 is toggled
  // for
  for (i=0;i<cycles;i++) {
    _delay_ms(half_period);
    // Wait 1 half wavelength.
    SET_BIT(SPEAKER_PORT, SPEAKER_PINNUMBER);
    // Output 5 V to port Pin 7.
    LED_PORT^=(1<<PINB5);
    _delay_ms(half_period);
    // Wait 1 half wavelength.
    CLEAR_BIT(SPEAKER_PORT, SPEAKER_PINNUMBER);
    // 0 V at port pin 7.
    LED_PORT^=(1<<PINB5);
  }
}

int main(void) {
  setup();
  
  for (;;) {
    	process();
  }
}