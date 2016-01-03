/*
 * m8_spi.c
 *
 * Created: 2014-04-05 22:26:37
 *  Author: Virtmedia
 */ 

#include "m8_spi.h"

void spi_init(void)
{
	
	#ifdef SPI_USE_CS
		SPI_CS_DDR |= (1<<SPI_CS_PIN);
	#endif
	SPCR = (0<<SPIE)|(1<<SPE)|(SPI_LSBTOMSB<<DORD)|(1<<MSTR)|(SPI_CLOCK_POLARITY<<CPOL)|(SPI_CLOCK_PHASE<<CPHA)|SPI_SPEED;
	SPSR = (1<<SPI2X);
}
#ifdef SPI_USE_CS
inline void spi_cs_low(void)
{
	SPI_CS_PORT &= ~(1<<SPI_CS_PIN);
}

inline void spi_cs_high(void)
{
	SPI_CS_PORT |= (1<<SPI_CS_PIN);
}
#endif
uint8_t spi_is_ready(void)
{
	return (SPSR & 0x80);
}

void spi_wait_for_ready(void)
{
	//while(!spi_is_ready()){}
	while( !(SPSR & (1<<SPIF)) );
}

uint8_t spi_send_byte(uint8_t data_to_send)
{
	//spi_wait_for_ready();
	//PORTB |= _BV(PORTB0);
	SPDR = data_to_send;
	spi_wait_for_ready();
	return SPDR;
}

void spi_send_data(uint8_t *buffer, uint8_t buffer_size)
{
	while(buffer_size)
	{
		*buffer = spi_send_byte(*buffer);
		buffer_size--;
		buffer++;
	}
}

void spi_send_pgmdata(const uint8_t *buffer, uint8_t buffer_size)
{
	while(buffer_size)
	{
		spi_send_byte(pgm_read_byte(buffer));
		buffer_size--;
		buffer++;
	}
}

void spi_send_data_bg(uint8_t *buffer, uint8_t buffer_size, void (*end_function)(void))
{
	
}

void spi_send_pgmdata_bg(const uint8_t *buffer, uint8_t buffer_size, void (*end_function)(void))
{
	
}

