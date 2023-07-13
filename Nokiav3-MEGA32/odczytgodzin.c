#include <avr/io.h>
#include "odczytgodzin.h"
#include "LCD/lcd44780.h"
#include "I2C_TWI/i2c_twi.h"
#include "nokia/lcd.h"

void zapisgodziny (void){
	uint8_t bufor[4];		// rezerwacja bufora 4 bajty
	bufor[0] = 0;			// setne czêœci sekundy
	bufor[1] = dec2bcd(25);	// sekundy
	bufor[2] = dec2bcd(33);	// minuty
	bufor[3] = dec2bcd(16);	// godziny
	TWI_write_buf( PCF8583_ADDR, 0x01, 4, bufor );
}

void aktualizacja(void){
	enum {ss=1, mm, hh};
	uint8_t sekundy, minuty, godziny;
	uint8_t bufor[4];
	TWI_read_buf( PCF8583_ADDR, 0x01, 4, bufor );
	sekundy = bcd2dec( bufor[ss] );
	minuty = bcd2dec( bufor[mm] );
	godziny = bcd2dec( bufor[hh] );
// Kopiowanie do tablicy
	bsek[0]=sekundy;
	bmin[0]=minuty;
	bgodz[0]=godziny;
}

void odczytgodzinyLCD (void){

	// wyœwietlenie czasu na LCD
	lcd_locate(1,0);
	if( bgodz[0] < 10 ) lcd_str("0");
	lcd_int(bgodz[0]);
	lcd_str(":");

	if( bmin[0] < 10 ) lcd_str("0");
	lcd_int(bmin[0]);
	lcd_str(":");

	if( bsek[0] < 10 ) lcd_str("0");
	lcd_int(bsek[0]);

}

void odczytgodzinyNokia(void){

// Ustalenie t³a
	box(38,40,1,25,PIXEL_ON); //godz
	box(42,44,1,61,PIXEL_ON); //min
	box(46,48,1,61,PIXEL_ON); //sek

lcd_goto_xy_exact(11,2);
uint8_t jednosci,dziesiatki;

dziesiatki=bgodz[0]/10;
jednosci=bgodz[0]-(dziesiatki*10);
lcd_chrx2(dziesiatki);
lcd_chrx2(jednosci);
dwukropek();

LcdLine(1,bgodz[0]+1,39,39,PIXEL_ON);
if (bgodz[0]==0) LcdLine(2,24,39,39,PIXEL_OFF);

//Minuty
dziesiatki=bmin[0]/10;
jednosci=bmin[0]-(dziesiatki*10);
lcd_chrx2(dziesiatki);
lcd_chrx2(jednosci);
dwukropek();

LcdLine(1,bmin[0]+1,43,43,PIXEL_ON);
if (bmin[0]==0) LcdLine(2,60,43,43,PIXEL_OFF);

// Sekundy
dziesiatki=bsek[0]/10;
jednosci=bsek[0]-(dziesiatki*10);
lcd_chrx2(dziesiatki);
lcd_chrx2(jednosci);

LcdLine(1,bsek[0]+1,47,47,PIXEL_ON);
if (bsek[0]==0) LcdLine(2,60,47,47,PIXEL_OFF);
LcdUpdate();
}

void odczytgodzinyNokiaanalog(void){

	if (bsek[0]==0){tarcza();LcdLine(42,42,2,24,PIXEL_ON);LcdUpdate(); }// 0
	if (bsek[0]==1){tarcza();LcdLine(44,42,2,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==2){tarcza();LcdLine(46,42,3,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==3){tarcza();LcdLine(48,42,3,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==4){tarcza();LcdLine(50,42,4,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==5){tarcza();LcdLine(52,42,4,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==6){tarcza();LcdLine(53,42,5,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==7){tarcza();LcdLine(55,42,7,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==8){tarcza();LcdLine(58,42,9,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==9){tarcza();LcdLine(60,42,11,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==10){tarcza();LcdLine(61,42,13,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==11){tarcza();LcdLine(62,42,15,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==12){tarcza();LcdLine(63,42,17,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==13){tarcza();LcdLine(64,42,19,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==14){tarcza();LcdLine(64,42,21,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==15){tarcza();LcdLine(65,42,24,24,PIXEL_ON);LcdUpdate(); }//15
	if (bsek[0]==16){tarcza();LcdLine(65,42,25,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==17){tarcza();LcdLine(64,42,30,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==18){tarcza();LcdLine(63,42,33,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==19){tarcza();LcdLine(61,42,36,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==20){tarcza();LcdLine(60,42,38,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==21){tarcza();LcdLine(59,42,39,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==22){tarcza();LcdLine(58,42,40,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==23){tarcza();LcdLine(56,42,42,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==24){tarcza();LcdLine(54,42,43,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==25){tarcza();LcdLine(52,42,45,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==26){tarcza();LcdLine(50,42,45,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==27){tarcza();LcdLine(48,42,46,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==28){tarcza();LcdLine(46,42,46,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==29){tarcza();LcdLine(44,42,46,24,PIXEL_ON);LcdUpdate(); }

	if (bsek[0]==30){tarcza();LcdLine(42,42,25,47,PIXEL_ON);LcdUpdate(); }//30

	if (bsek[0]==31){tarcza();LcdLine(40,42,46,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==32){tarcza();LcdLine(38,42,46,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==33){tarcza();LcdLine(36,42,46,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==34){tarcza();LcdLine(34,42,45,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==35){tarcza();LcdLine(32,42,45,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==36){tarcza();LcdLine(30,42,43,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==37){tarcza();LcdLine(28,42,42,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==38){tarcza();LcdLine(26,42,40,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==39){tarcza();LcdLine(25,42,39,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==40){tarcza();LcdLine(24,42,38,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==41){tarcza();LcdLine(23,42,36,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==42){tarcza();LcdLine(21,42,33,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==43){tarcza();LcdLine(20,42,30,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==44){tarcza();LcdLine(19,42,25,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==45){tarcza();LcdLine(19,42,24,24,PIXEL_ON);LcdUpdate(); }//45
	if (bsek[0]==46){tarcza();LcdLine(19,42,22,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==47){tarcza();LcdLine(20,42,19,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==48){tarcza();LcdLine(20,42,17,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==49){tarcza();LcdLine(21,42,15,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==50){tarcza();LcdLine(23,42,12,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==51){tarcza();LcdLine(24,42,11,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==52){tarcza();LcdLine(25,42,9,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==53){tarcza();LcdLine(27,42,7,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==54){tarcza();LcdLine(29,42,6,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==55){tarcza();LcdLine(31,42,5,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==56){tarcza();LcdLine(32,42,3,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==57){tarcza();LcdLine(34,42,3,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==58){tarcza();LcdLine(36,42,2,24,PIXEL_ON);LcdUpdate(); }
	if (bsek[0]==59){tarcza();LcdLine(39,42,2,24,PIXEL_ON);LcdUpdate(); } //59

}

// konwersja liczby dziesiêtnej na BCD
uint8_t dec2bcd(uint8_t dec) {
return ((dec / 10)<<4) | (dec % 10);
}

// konwersja liczby BCD na dziesiêtn¹
uint8_t bcd2dec(uint8_t bcd) {
    return ((((bcd) >> 4) & 0x0F) * 10) + ((bcd) & 0x0F);
}
