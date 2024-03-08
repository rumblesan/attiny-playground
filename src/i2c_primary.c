#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#include "i2c_primary.h"


#define I2C_CLOCK_STROBE()    { USICR |= _BV(USITC); }

#define I2C_SET_SDA_OUTPUT()  { I2C_SDA_DDR |=   _BV(I2C_SDA_PIN); }
#define I2C_SET_SDA_INPUT()   { I2C_SDA_DDR &=  ~_BV(I2C_SDA_PIN); }

#define I2C_SET_SCL_OUTPUT()  { I2C_SCL_DDR |=   _BV(I2C_SCL_PIN); }
#define I2C_SET_SCL_INPUT()   { I2C_SCL_DDR &=  ~_BV(I2C_SCL_PIN); }

#define I2C_SET_SDA_HIGH()    { I2C_SDA_PORT |=   _BV(I2C_SDA_PIN); }
#define I2C_SET_SDA_LOW()     { I2C_SDA_PORT &=  ~_BV(I2C_SDA_PIN); }

#define I2C_SET_SCL_HIGH()    { I2C_SCL_PORT |=   _BV(I2C_SCL_PIN); }
#define I2C_SET_SCL_LOW()     { I2C_SCL_PORT &=  ~_BV(I2C_SCL_PIN); }

#define I2C_SDA_IS_HIGH()      (I2C_SDA_PINR & _BV(I2C_SDA_PIN))
#define I2C_SCL_IS_NOT_HIGH() !(I2C_SCL_PINR & _BV(I2C_SCL_PIN))


void i2c_init() {

	I2C_SET_SDA_OUTPUT();
	I2C_SET_SCL_OUTPUT();

	I2C_SET_SDA_HIGH();
	I2C_SET_SCL_HIGH();

	USIDR = 0xFF;

	USICR = _BV(USIWM1) | _BV(USICS1) | _BV(USICLK);
	/*USICR =*/
    /*(0<<USISIE)|(0<<USIOIE)|             // Interrupts disabled*/
    /*(1<<USIWM1)|(0<<USIWM0)|             // Set USI in Two-wire mode.*/
    /*(1<<USICS1)|(0<<USICS0)|(1<<USICLK)| // Software clock strobe as source.*/
    /*(0<<USITC); */

	USISR = _BV(USISIF) |
					_BV(USIOIF) |
					_BV(USIPF) |
					_BV(USIDC) |       // Clear flags,
					(0x0 << USICNT0); // and reset counter.

}

void i2c_start() {
	I2C_SET_SDA_HIGH();
	I2C_SET_SCL_HIGH();
	while (I2C_SCL_IS_NOT_HIGH()); // clock stretching
	I2C_SET_SDA_LOW();
	_delay_us(WAIT_LONG);
	I2C_SET_SCL_LOW();
	I2C_SET_SDA_HIGH();
}

void i2c_stop() {
	I2C_SET_SDA_LOW();
	I2C_SET_SCL_HIGH();
	while (I2C_SCL_IS_NOT_HIGH()); // clock stretching
	_delay_us(WAIT_LONG);
	I2C_SET_SDA_HIGH();
	_delay_us(WAIT_SHORT);
}

uint8_t i2c_transfer(uint8_t usisr_mask) {
	USISR = usisr_mask;

	uint8_t temp_data = 0x00;

	// transfer until counter overflow
	do {
		_delay_us(WAIT_LONG);
		I2C_CLOCK_STROBE();
		while (I2C_SCL_IS_NOT_HIGH()); // clock stretching
		_delay_us(WAIT_SHORT);
		I2C_CLOCK_STROBE();
	} while (!(USISR & _BV(USIOIF)));

	temp_data = USIDR;
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
bool i2c_write_byte(uint8_t data) {
	USIDR = data;
	i2c_transfer(USISR_CLOCK_8_BITS);

	// wait for ack
	I2C_SET_SDA_INPUT();
	uint8_t ack = i2c_transfer(USISR_CLOCK_1_BIT);
	I2C_SET_SDA_OUTPUT();

	return ack & 0x01;
}

// controller reads 1 byte from the bus
// and sends a nack if wanna read another one
// (1 = will read another one, 0 = stop sending)
// returns the read byte
uint8_t i2c_read_byte(uint8_t ack) {

	I2C_SET_SDA_INPUT();
	uint8_t data = i2c_transfer(USISR_CLOCK_8_BITS);
	I2C_SET_SDA_OUTPUT();

	USIDR = ack;
	i2c_transfer(USISR_CLOCK_1_BIT);

	return data;
}
