/*
 * m8_spi.h
 *
 * Created: 2014-04-05 22:46:45
 *  Author: Virtmedia
 * Description:
 *  Biblioteka udost�pnia funkcje u�atwiaj�ce korzystanie z interfejsu SPI
 *  dost�pnego w mikrokontrolerach ATMEGA8 / ATMEGA48/88/168 i podobnych.
 *  Biblioteka umo�liwia przesy�anie pojedynczych bajt�w, jak i ca�ych tablic.
 *  Dodano funkcje umo�liwiaj�ce transfer w tle, za pomoc� przerwa�.
 *  Aby rozpocz�� korzystanie z biblioteki, nale�y j� skonfigurowa� w tym pliku
 *  poni�ej; zdefiniowa� prawid�owo warto�� F_CPU i wywo�a� funkcj� spi_init()
 *  na pocz�tku dzia�ania funkcji main() w sekcji inicjalizacyjnej.
 *  Nast�pnie mo�na korzysta� z dost�pnych funkcji, kt�re zostan� opisane poni�ej.
 */ 

/*	NIE GOTOWE!!	*/

#ifndef M8_SPI_H_
#define M8_SPI_H_

#include <avr/io.h>
#include <stdint.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

/*	Sekcja konfiguracyjna	*/
//Pin CS, domy�lnie u�ywany
//W przypadku korzystania z wielu uk�ad�w pod SPI, nale�y zdefiniowa� w�asne
//piny CS dla ka�dego z nich, najlepiej w osobnych bibliotekach.
//Je�eli nie chcesz, aby funkcje z tej biblioteki samodzielnie sterowa�y tym pinem, zakomentuj poni�sz� lini�.
//#define SPI_USE_CS	1

#define SPI_CS_PIN	7
#define SPI_CS_PORT	PORTD
#define SPI_CS_DDR	DDRD

/* Poni�szymi warto�ciami wybieramy tzw. SPI Mode.
   Wi�kszo�� uk�ad�w dzia�a na mode=0, np:
   23LCV1024, MAX7219, 74HC4094, 74HC595, wy�wietlacze Nokii 3310 i MC60
   */
//Mode 0
#define SPI_CLOCK_POLARITY	0
#define SPI_CLOCK_PHASE	0
//Mode 1
//#define SPI_CLOCK_POLARITY	0
//#define SPI_CLOCK_PHASE	1
//Mode 2
//#define SPI_CLOCK_POLARITY	1
//#define SPI_CLOCK_PHASE	0
//Mode 3
//#define SPI_CLOCK_POLARITY	1
//#define SPI_CLOCK_PHASE	1

//Wybieramy pr�dko�� transferu wzgl�dem F_CPU
//#define SPI_SPEED	0x00	//F_CPU/2
#define SPI_SPEED	0x01	//F_CPU/8
//#define SPI_SPEED	0x10	//F_CPU/32
//#define SPI_SPEED	0x11	//F_CPU/64

//Wybieramy kolejno�� przesy�ania bit�w w bajcie. Domy�lnie 0.
// 0 - MSB First
// 1 - LSB First
#define SPI_LSBTOMSB	0

//Funkcja wywo�ywana po zako�czeniu przesy�ania w tle.
#define SPI_BG_END	spi_cs_high
/*	Koniec sekcji konfiguracyjnej. Poni�ej proponuj� nic nie zmienia�	*/


/*	Inicjalizacja SPI	*/
void spi_init(void);
#ifdef SPI_USE_CS
/*	Ustawienie pinu CS w stan niski. Zwykle na pocz�tku nadawania	*/
inline void spi_cs_low(void);
/*	Ustawienie pinu CS w stan wysok. Zwykle po zako�czeniu nadawania	*/
inline void spi_cs_high(void);
#endif
/*	Sprawdzenie, czy SPI zako�czy� wysy�a� dane	0x00-nie sko�czy�, 0x80 - sko�czy�*/
uint8_t spi_is_ready(void);
/*	Poczekaj, a� SPI wy�le poprzednie dane	*/
void spi_wait_for_ready(void);
/*	Wysy�a 1 bajt przez SPI. Zwraca odebrany bajt. Nie ster	*/
uint8_t spi_send_byte(uint8_t data_to_send);
/*	Wysy�a bajty z bufora, zast�puj�c je otrzymanymi danymi	*/
void spi_send_data(uint8_t *buffer, uint8_t buffer_size);
/*	Wysy�a dane z bufora w pami�ci FLASH, nie odczytuje odebranych danych	*/
void spi_send_pgmdata(const uint8_t *buffer, uint8_t buffer_size);
/*	Wysy�a/odbiera dane w tle korzystaj�c z bufora. Po zako�czeniu nadawania zostanie wywo�ana funkcja "end_function".	
	Funkcja ta jest potrzebna chocia�by do zmiany stanu pinu CS. mo�esz u�y� funkcji spi_cs_high()	*/
void spi_send_data_bg(uint8_t *buffer, uint8_t buffer_size, void (*end_function)(void));
/*	Wysy�a dane w tle korzystaj�c z bufora w pami�ci Flash. Po zako�czeniu nadawania zostanie wywo�ana funkcja "end_function".	*/
void spi_send_pgmdata_bg(const uint8_t *buffer, uint8_t buffer_size, void (*end_function)(void));

#endif /* M8_SPI_H_ */