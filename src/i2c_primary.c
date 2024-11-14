#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>

#include "i2c_primary.h"

// USISR mask
#define USISR_CLOCK_8_BITS 0b11110000
#define USISR_CLOCK_1_BIT  0b11111110

#define I2C_CLOCK_STROBE()    { USICR |= 0b00101011; }

#define I2C_SET_SDA_OUTPUT()  { DDRB |=   _BV(I2C_SDA_PIN); }
#define I2C_SET_SDA_INPUT()   { DDRB &=  ~_BV(I2C_SDA_PIN); }

#define I2C_SET_SCL_OUTPUT()  { DDRB |=   _BV(I2C_SCL_PIN); }
#define I2C_SET_SCL_INPUT()   { DDRB &=  ~_BV(I2C_SCL_PIN); }

#define I2C_SET_SDA_HIGH()    { PORTB |=   _BV(I2C_SDA_PIN); }
#define I2C_SET_SDA_LOW()     { PORTB &=  ~_BV(I2C_SDA_PIN); }

#define I2C_SET_SCL_HIGH()    { PORTB |=   _BV(I2C_SCL_PIN); }
#define I2C_SET_SCL_LOW()     { PORTB &=  ~_BV(I2C_SCL_PIN); }

#define I2C_SDA_IS_HIGH()      (PINB & _BV(I2C_SDA_PIN))
#define I2C_SCL_IS_NOT_HIGH() !(PINB & _BV(I2C_SCL_PIN))

#define I2C_WAIT_HIGH()       { _delay_us(WAIT_HIGH); }
#define I2C_WAIT_LOW()        { _delay_us(WAIT_LOW);  }


void i2c_init() {

	I2C_SET_SDA_OUTPUT();
	I2C_SET_SCL_OUTPUT();

	I2C_SET_SDA_HIGH();
	I2C_SET_SCL_HIGH();

	USIDR = 0xFF;

	USICR = (1 << USIWM1) | (1 << USICS1) | (1 << USICLK);
	/*USICR =*/
		/*(0<<USISIE)|(0<<USIOIE)|             // Interrupts disabled*/
		/*(1<<USIWM1)|(0<<USIWM0)|             // Set USI in Two-wire mode.*/
		/*(1<<USICS1)|(0<<USICS0)|(1<<USICLK)| // Software clock strobe as source.*/
		/*(0<<USITC); */

	USISR = _BV(USISIF) |
					_BV(USIOIF) |
					_BV(USIPF) |
					_BV(USIDC) |      // Clear flags,
					(0x0 << USICNT0); // and reset counter.

}

void i2c_start() {
	I2C_SET_SDA_HIGH();
	I2C_SET_SCL_HIGH();
	while (I2C_SCL_IS_NOT_HIGH());
	I2C_SET_SDA_LOW();
	_delay_us(WAIT_LOW);
	I2C_SET_SCL_LOW();
	I2C_SET_SDA_HIGH();
}

void i2c_stop() {
	I2C_SET_SDA_LOW();
	I2C_SET_SCL_HIGH();
	while (I2C_SCL_IS_NOT_HIGH()); // clock stretching
	_delay_us(WAIT_HIGH);
	I2C_SET_SDA_HIGH();
	_delay_us(WAIT_HIGH);
}

unsigned char i2c_transfer(unsigned char usisr_mask) {
	I2C_SET_SCL_LOW();

	USISR = usisr_mask;

	// transfer until counter overflow
	do {
		_delay_us(WAIT_LOW);
		I2C_CLOCK_STROBE();
		while (I2C_SCL_IS_NOT_HIGH()); // clock stretching
		_delay_us(WAIT_HIGH);
		I2C_CLOCK_STROBE();
	} while (!(USISR & _BV(USIOIF)));
	_delay_us(WAIT_LOW);

	unsigned char temp_data = USIDR;
	// release SDA
	// "The output pin (DO or SDA, depending on the wire mode)
	// is connected via the output latch to the most significant
	// bit (bit 7) of the USI Data Register."
	// (so write 1 to set SDA high before the next operation,
	// otherwise it'll be pulled down and read may not work)
	USIDR = 0xFF;

	return temp_data;
}

// controller sends a byte to the bus
// returns false if there's a valid nack, otherwise true
bool i2c_write_byte(unsigned char data) {
	USIDR = data;
	i2c_transfer(USISR_CLOCK_8_BITS);

	// wait for ack
	I2C_SET_SDA_INPUT();
	unsigned char ack = i2c_transfer(USISR_CLOCK_1_BIT);
	I2C_SET_SDA_OUTPUT();

	return ack & 0x01;
}

// controller reads 1 byte from the bus
// and sends a nack if wanna read another one
// (1 = will read another one, 0 = stop sending)
// returns the read byte
unsigned char i2c_read_byte(unsigned char ack) {

	I2C_SET_SDA_INPUT();
	unsigned char data = i2c_transfer(USISR_CLOCK_8_BITS);
	I2C_SET_SDA_OUTPUT();

	USIDR = ack;
	i2c_transfer(USISR_CLOCK_1_BIT);

	return data;
}
