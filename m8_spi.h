/*
 * m8_spi.h
 *
 * Created: 2014-04-05 22:46:45
 *  Author: Virtmedia
 * Description:
 *  Biblioteka udostêpnia funkcje u³atwiaj¹ce korzystanie z interfejsu SPI
 *  dostêpnego w mikrokontrolerach ATMEGA8 / ATMEGA48/88/168 i podobnych.
 *  Biblioteka umo¿liwia przesy³anie pojedynczych bajtów, jak i ca³ych tablic.
 *  Dodano funkcje umo¿liwiaj¹ce transfer w tle, za pomoc¹ przerwañ.
 *  Aby rozpocz¹æ korzystanie z biblioteki, nale¿y j¹ skonfigurowaæ w tym pliku
 *  poni¿ej; zdefiniowaæ prawid³owo wartoœæ F_CPU i wywo³aæ funkcjê spi_init()
 *  na pocz¹tku dzia³ania funkcji main() w sekcji inicjalizacyjnej.
 *  Nastêpnie mo¿na korzystaæ z dostêpnych funkcji, które zostan¹ opisane poni¿ej.
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
//Pin CS, domyœlnie u¿ywany
//W przypadku korzystania z wielu uk³adów pod SPI, nale¿y zdefiniowaæ w³asne
//piny CS dla ka¿dego z nich, najlepiej w osobnych bibliotekach.
//Je¿eli nie chcesz, aby funkcje z tej biblioteki samodzielnie sterowa³y tym pinem, zakomentuj poni¿sz¹ liniê.
//#define SPI_USE_CS	1

#define SPI_CS_PIN	7
#define SPI_CS_PORT	PORTD
#define SPI_CS_DDR	DDRD

/* Poni¿szymi wartoœciami wybieramy tzw. SPI Mode.
   Wiêkszoœæ uk³adów dzia³a na mode=0, np:
   23LCV1024, MAX7219, 74HC4094, 74HC595, wyœwietlacze Nokii 3310 i MC60
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

//Wybieramy prêdkoœæ transferu wzglêdem F_CPU
//#define SPI_SPEED	0x00	//F_CPU/2
#define SPI_SPEED	0x01	//F_CPU/8
//#define SPI_SPEED	0x10	//F_CPU/32
//#define SPI_SPEED	0x11	//F_CPU/64

//Wybieramy kolejnoœæ przesy³ania bitów w bajcie. Domyœlnie 0.
// 0 - MSB First
// 1 - LSB First
#define SPI_LSBTOMSB	0

//Funkcja wywo³ywana po zakoñczeniu przesy³ania w tle.
#define SPI_BG_END	spi_cs_high
/*	Koniec sekcji konfiguracyjnej. Poni¿ej proponujê nic nie zmieniaæ	*/


/*	Inicjalizacja SPI	*/
void spi_init(void);
#ifdef SPI_USE_CS
/*	Ustawienie pinu CS w stan niski. Zwykle na pocz¹tku nadawania	*/
inline void spi_cs_low(void);
/*	Ustawienie pinu CS w stan wysok. Zwykle po zakoñczeniu nadawania	*/
inline void spi_cs_high(void);
#endif
/*	Sprawdzenie, czy SPI zakoñczy³ wysy³aæ dane	0x00-nie skoñczy³, 0x80 - skoñczy³*/
uint8_t spi_is_ready(void);
/*	Poczekaj, a¿ SPI wyœle poprzednie dane	*/
void spi_wait_for_ready(void);
/*	Wysy³a 1 bajt przez SPI. Zwraca odebrany bajt. Nie ster	*/
uint8_t spi_send_byte(uint8_t data_to_send);
/*	Wysy³a bajty z bufora, zastêpuj¹c je otrzymanymi danymi	*/
void spi_send_data(uint8_t *buffer, uint8_t buffer_size);
/*	Wysy³a dane z bufora w pamiêci FLASH, nie odczytuje odebranych danych	*/
void spi_send_pgmdata(const uint8_t *buffer, uint8_t buffer_size);
/*	Wysy³a/odbiera dane w tle korzystaj¹c z bufora. Po zakoñczeniu nadawania zostanie wywo³ana funkcja "end_function".	
	Funkcja ta jest potrzebna chocia¿by do zmiany stanu pinu CS. mo¿esz u¿yæ funkcji spi_cs_high()	*/
void spi_send_data_bg(uint8_t *buffer, uint8_t buffer_size, void (*end_function)(void));
/*	Wysy³a dane w tle korzystaj¹c z bufora w pamiêci Flash. Po zakoñczeniu nadawania zostanie wywo³ana funkcja "end_function".	*/
void spi_send_pgmdata_bg(const uint8_t *buffer, uint8_t buffer_size, void (*end_function)(void));

#endif /* M8_SPI_H_ */