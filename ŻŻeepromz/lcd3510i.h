////////////////////////////////////////////////////////////////////////////////////////////
// lcd3510i.h - Biblioteka obs³ugi wyœwietlacza Nokia3510i
//
// Autor: Rados³aw Koppel          Kompilator: WinAVR 20050214
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LCD3510I_H_INCLUDED
#define LCD3510I_H_INCLUDED

// Potrzebne typy zmiennych
typedef uint8_t lcd_pixel;

// Makra pomocnicze

// Tworzenie koloru palety
#define LCD_palRGB(r, g, b) ((uint16_t)((r&0x0F)<<8) | ((g&0x0F)<<4) | ((b&0x0F)<<0))

// Rozmiary pola wyœwietlacza
#define LCD_SX 98
#define LCD_SY 67

// Funkcje obs³ugi LCD
void lcd_Init(void);
void lcd_On(void);
void lcd_Off(void);

// Funkcje obs³ugi palety
void lcd_loadRGB_P(const prog_uint16_t* pRGB, uint16_t size);

// Funkcje obs³ugi bufora
void lcd_Cls(lcd_pixel pix);
void lcd_Update(void);

// Funkcje rysuj¹ce
void lcd_Pixel(uint8_t x, uint8_t y, lcd_pixel color);

//**********************************************************************************************
// Definicje komend

/*
NOP, NOP2 - brak operacji
	Komenda powoduje wyjœcie z trybu zapisu / odczytu pamiêci. Umo¿liwia wyjœcie z trybu testowego (patrz komenda TSTMOD).
	Dla zapewnienia stabilnoœci dzia³ania zalecane jest jej periodyczne wywo³ywanie.
*/
#define LCD_NOP 0x00
#define LCD_NOP2 0xAA

/*
SWRESET - reset programowy
	Jej dzia³anie jest identyczne jak podanie stanu niskiego na wejœcie /RST
*/
#define LCD_SWRESET 0x01

/*
BSTOFF / BSTON - wy³¹cza / w³¹cza wbudowan¹ przetwornice napiêcia.
	Komendy SLPIN/SLPOUT tak¿e wy³¹czaj¹ i w³¹czaj¹ przetwornicê,
	jednak ta komenda zmienia jej stan niezale¿nie.

	* Po zerowaniu przetwornica jest wy³¹czona
*/
#define LCD_BSTOFF 0x02
#define LCD_BSTON 0x03

/*
RDDST - umo¿liwia odczyt statusu wyœwietlacza. Brak zastosowania w projekcie.
	!!! Aby odczyt by³ mo¿liwy, linia LCD_SIO musi mieæ mo¿liwoœæ stania siê lini¹ wejœciow¹!!!
	W odpowiedzi przesy³ane jest 5 bajtów
	0: b³êdne dane - wynika z dzia³ania interfejsu
	1: D31 - D24
	2: ...
	3: ...
	4: D7 - D0

	Szczegó³y w dokumentacji
*/
#define LCD_RDDST 0x09 // ODCZYT !!!

/*
SLPIN / SLPOUT - wejœcie w tryb uœpienia
	W trybie uœpienia przetwornica napiêcia jest wy³¹czona, oscylator zatrzymany. Wszystkie piksele nieaktywne.

	* Po zerowaniu wyœwietlacz jest w trybie uœpienia
*/
#define LCD_SLPIN 0x10
#define LCD_SLPOUT 0x11

/*
PTLON - wejœcie w tryb niepe³nego wyœwietlania
	W trybie tym istnieje mo¿liwoœæ wybrania zakresu obs³ugiwanej powierzchni wyœwietlacza
	Pozosta³e piksele s¹ wygaszone.
	Ustawienie zakresu wyœwietlanych linii jest mo¿liwe przy pomocy komendy PLTAR
	Aby wyjœæ z tego trybu nale¿y podaæ komendê NORON.

	* Po zerowaniu wyœwietlacz jest w trybie normalnym
*/
#define LCD_PTLON 0x12

/*
NORON - wejœcie w tryb normalny
	Wprowadza wyœwietlacz w zwyk³y tryb wyœwietlania.

	* Jest to domyœlny stan po zerowaniu uk³adu
*/
#define LCD_NORON 0x13

/*
INVOFF / INVON - wy³¹czenie / w³¹czenie inwersji pikseli
	Dzia³anie komendy odnosi siê do sposobu przetwarzania danych z pamiêci na obraz.
	Nie zmienia zawartoœci pamiêci obrazu.

	* Po zerowaniu inwersja jest wy³¹czona
*/
#define LCD_INVOFF 0x20
#define LCD_INVON 0x21

/*
FIXOFF / FIXON - wygaszenie / zapalenie wszystkich pikseli
	Sterownik zaprzestaje dostêpu do pamiêci obrazu i wysterowuje wszystkie piksele
	jako wy³¹czone / w³¹czone.
	Wyjœcie z tego trybu jest mo¿liwe przez wydanie komendy NORON albo PLTON

	* Po zerowaniu wszystkie piksele s¹ wy³¹czone
*/
#define LCD_FIXOFF 0x22
#define LCD_FIXON 0x23

/*
WRTCNTR - ustawienie kontrastu
	Po wys³aniu komendy nale¿y wys³aæ dan¹ o wartoœci 0-127.
	Œredni kontrast ma wartoœæ 63. Napiêcie wyœwietlacza roœnie wraz ze wzrostem wartoœci kontrastu.

	* Po zerowaniu domyœlna wartoœæ kontrastu wynosi 63
*/
#define LCD_WRCNTR 0x25

/*
DISOFF / DISON - wy³¹czenie / w³¹czenie wyœwietlacza
*/
#define LCD_DISOFF 0x28
#define LCD_DISON 0x29

/*
CASET - ustawienie adresów kolumn
PASET - ustawienie adresów stron

		+--------------------+
		| +-------------+    | < SP - strona (linia) pocz¹tkowa
		| | Okno zapisu |    |
		| +-------------+    | < EP - strona (linia) koñcowa
		|                    |
		+--------------------+
		  ^         ^
		  SC        EC - kolumna pocz¹tkowa i koñcowa

	Po komendach nastêpuje przes³anie 8 bitowych liczb oznaczaj¹cych pocz¹tek i koniec okna zapisu.
	Pocz¹tek musi byæ liczb¹ mniejsz¹ ni¿ koniec. Jeœli podano wartoœci które nie mieszcz¹ siê w pamiêci
	obrazu, przesy³ane w to miejsce dane bêd¹ ignorowane.

	Funkcje umo¿liwiaj¹ zdefiniowanie sposobu "okna" w którym bêd¹ umieszczane zapisywane dane.
	Jest to o tyle wygodne, ¿e jeœli chcemy odœwie¿yæ tylko czêœæ obrazu, nie musimy zmieniaæ ustawieñ.
	Wysy³amy blok danych a sterownik umieszcza je w odpowiednich miejscach pamiêci, tak aby pasowa³y
	do zdefiniowanego okna.

	* Po zerowaniu, zale¿nie od wybranego typu wyœwietlacza przyjête s¹ nastêpuj¹ce wartoœci wartoœci obejmuj¹ce ca³y obszar wyœwietlacza
	dla naszego 98x67:
	SC=0; EC=97; SP=0; EP=66;
*/
#define LCD_CASET 0x2A
#define LCD_PASET 0x2B

/*
RAMWR - rozpoczêcie zapisu do pamiêci wyœwietlacza
	Po wys³aniu tej komendy adres zapisu jest ustawiany na pocz¹tek zdefiniowanego okna.
	Ka¿da wys³ana dana powoduje przesuniêcie wskaŸnika o jedn¹ pozycjê.
	Zapis przerywa wys³anie dowolnej innej komendy.
*/
#define LCD_RAMWR 0x2C

/*
RGBSET - ustawienie palety kolorów dla interfejsu 8bpp
	Umo¿liwia ustawienie palety kolorów s³u¿¹cej do przejœcia miêdzy trybem 256 kolorów (8bpp) na tryb 4096 kolorów (12bpp).
	Oddzielnie ustawiane s¹ sk³adowe wszystkich kolorów.
	W trybie 8bpp kolory czerwony oraz zielony maj¹ 3 bity a niebieski 2.
	Dane palety maj¹ format jak ni¿ej:

	Dane                          Index
	 X   X   X   X  R3 R2 R1 R0     0
	 X   X   X   X  R3 R2 R1 R0     1
	 X   X   X   X  R3 R2 R1 R0     2
	 X   X   X   X  R3 R2 R1 R0     3
	 X   X   X   X  R3 R2 R1 R0     4
	 X   X   X   X  R3 R2 R1 R0     5
	 X   X   X   X  R3 R2 R1 R0     6
	 X   X   X   X  R3 R2 R1 R0     7

	 X   X   X   X  G3 G2 G1 G0     0
	 X   X   X   X  G3 G2 G1 G0     1
	 X   X   X   X  G3 G2 G1 G0     2
	 X   X   X   X  G3 G2 G1 G0     3
	 X   X   X   X  G3 G2 G1 G0     4
	 X   X   X   X  G3 G2 G1 G0     5
	 X   X   X   X  G3 G2 G1 G0     6
	 X   X   X   X  G3 G2 G1 G0     7

	 X   X   X   X  B3 B2 B1 B0     0
	 X   X   X   X  B3 B2 B1 B0     1
	 X   X   X   X  B3 B2 B1 B0     2
	 X   X   X   X  B3 B2 B1 B0     3

	Uwaga: Dane w pamiêci przechowywane s¹ w postaci 12bpp. Zmiana odbywa siê jeszcze przed zapisem wartoœci do pamiêci obrazu.
	Zatem sama zmiana palety nie daje od razu widocznych zmian na wyœwietlaczu.

	* Po zerowaniu zawartoœæ palety jest nieokreœlona. Jeœli zamierzamy korzystaæ z trynu 8bpp konieczna jest inicjacja.
*/
#define LCD_RGBSET 0x2D

/*
RAMRD - odczyt danych z pamiêci obrazu. Brak zastosowania w projekcie.
	!!! Aby odczyt by³ mo¿liwy, linia LCD_SIO musi mieæ mo¿liwoœæ stania siê lini¹ wejœciow¹!!!
*/
#define LCD_RAMRD 0x2E // ODCZYT !!!

/*
PLTAR - obszar czêœciowy
	Funkcja umo¿liwia ustawienie pocz¹tkowej i koñcowej wyœwietlanej linii.
	Aby jej dzia³anie by³o widoczne konieczne jest wejœcie w specjalny tryb komend¹ PLTON.
*/
#define LCD_PTLAR 0x30

/*
VSCRDEF - ustawienie obszarów przewijania
*/
#define LCD_VSCRDEF 0x33

/*
MADCTL - kontrola dostêpu do pamiêci
	Ustawia sposób dostêpu do pamiêci zarówno od strony mikrokontrolera jak i od strony kontrolera LCD
	Za komend¹ nale¿y wys³aæ jeden bajt zawieraj¹cy parametry:
	B7 B6 B5 B4 B3 0 0 B0

	B7: Ustawia pozycjê zerowej strony (lini)
		0 - od góry do do³u
		1 - od do³u do góry
	B6: Ustawia pozycjê zerowej kolumny
		0 - od lewej do prawej
		1 - od prawej do lewej
	B5: Ustawienie kolejnoœci zmian lini / kolumny
		0 - tryb normalny: zmieniany jest adres kolumny a¿ przeskanowana zostanie ca³a linia w oknie,
			wtedy zmienia siê adres linii a kolumna zaczyna siê od pocz¹tku.
			+------------+
			|123456789ABC|
			|DEFGHIJKLMNO|
			|PRSTUWXYZ   |
			+------------+
		1 - tryb odwrócony: zmieniany jest adres linii a¿ przeskanowana zostanie ca³a kolumna,
			wtedy zmienia siê adres kolumny a linia zaczyna siê od pocz¹tku.
			+------------+
			|147ADGJMPTX |
			|258BEHKNRUY |
			|369CFILOSWZ |
			+------------+
	B4: Wybranie kierunku skanowania linii wyœwietlacza
		0 - skanowanie od góry do do³u
		1 - skanowanie od do³u do góry
	B3: Kolejnoœæ sk³adowych koloru
		0 - RGB
		1 - BGR
	B0: Odwrócenie wyœwietlacza w pionie
		0 - tryb normalny
		1 - wyœwietlacz odwrócony

	* Po zerowaniu, wartoœæ domyœlna to same 0
*/
#define LCD_MADCTL 0x36
	#define LCD_P0TOP 0
	#define LCD_P0BOTTOM 0x80
	#define LCD_C0LEFT 0
	#define LCD_C0RIGHT 0x40
	#define LCD_MODENORM 0
	#define LCD_MODEINV 0x20
	#define LCD_SCANTOPBOTTOM 0
	#define LCD_SCANBOTTOMTOP 0x10
	#define LCD_RGB 0
	#define LCD_BGR 0x08
	#define LCD_VERTNORM 0
	#define LCD_VERTINV 0x01

/*
VSCRSADD - Adres pocz¹tku obszaru przewijanego
*/
#define LCD_VSCRSADD 0x37

/*
IDMOFF / IDMON - wyjœcie / wejœcie w tryb IDLE
	W trybie IDLE, pobór mocy jest zmniejszony, wyœwietlacz wyœwietla tylko 8 kolorów
	(brane pod uwagê s¹ tylko najstarsze bity z ka¿dej sk³adowej koloru).

	* Po zerowaniu tryb IDLE jest wy³¹czony
*/
#define LCD_IDMMOFF 0x38
#define LCD_IDMON 0x39

/*
COLMOD - tryb koloru
	Umo¿liwia wybranie trybu koloru z punktu widzenia interfejsu z mikrokontrolerem.
	Za komend¹ nale¿y przes³aæ dan¹ ustawiaj¹c¹ wybrany tryb:
	2 - 256 kolrów
	3 - 4096 kolorów

	W pamiêci kolory przechowywane s¹ w formacie 12 bitowym. Zmiana odbywa siê w chwili wpisywania danych.

	* Domyœlnym trybem po zerowaniu jest tryb 12bpp
*/
#define LCD_COLMOD 0x3A
	#define LCD_COL8bpp 2
	#define LCD_COL12bpp 3

/*
TSTMOD - Wejœcie w tryb testowy
	Funkcja umo¿liwia testowanie uk³adu. U¿yteczna praktycznie tylko z punktu widzenia producenta.
	W trybie testowym wyœwietlacz nie jest sterowany prawid³owo.
	Istnieje mo¿liwoœæ, ¿e wyœwietlacz odbierze t¹ w³aœnie komendê z powodu zak³óceñ.
	Wyjœcie z trybu testowego odbywa umo¿liwia komenda NOP lub NOP2.
*/
#define LCD_TSTMOD 0xDE

/*
INIESC - inicjacja ustawieñ wewnêtrznych sterownika
	Konieczne wywo³anie w odpowiednim miejscu sekwencji inicjuj¹cej.
	Powinna byæ wywo³ana przed rozpoczêciem konfiguracji.
*/
#define LCD_INIESC 0xC6

/*
GCPSET0, GCPSET1 - ustawienie skali szaroœci (krzywa gamma)
	Ustawiane mo¿na ustawiæ dwie krzywe wybierane nastêpnie instrukcj¹ GAMSET.
	Po komendzie nale¿y przes³aæ 15 wartoœci, kolejno poziom szaroœci (natê¿enie koloru)
	dla danej 0001, 0010, ..., 1111.
	Wartoœæ dla danej koloru 0000 jest ustawiona na sta³e.
	Nale¿y zadbaæ aby wpisywane wartoœci by³y rosn¹ce.

	Uwaga: Nie zmieniaæ w czasie wyœwietlania obrazu (aktywny wyœwietlacz)
	* Po zerowaniu zawartoœæ obu krzywych jest nieokreœlona
*/
#define LCD_GCPSET0 0xB3
#define LCD_GCPSET1 0xB4

/*
GAMSET - umo¿liwia wybranie jednej z definiowanych krzywych gamma
	Wys³ana za instrukcj¹ dana umo¿liwia wybranie krzywej:
	* * * * * * P11 P10
	P11 P10
	 0   0  - bez zmian
	 0   1  - krzywa 0
	 1   0  - krzywa 1
	 1   1  - bez zmian

	* Po zerowaniu wybrana jest krzywa 0
*/
#define LCD_GAMSET 0xB5
	#define LCD_GAM0 1
	#define LCD_GAM1 2

/*
DISCTL - konfiguracja wyœwietlacza
	Za komend¹ nale¿y przes³aæ 7 bajtów danych konfigurujacych wyœwietlacz.
	W to dane standardowe zale¿ne od konkretnego modu³u wyœwietlacza.
	Szczegó³y w dokumentacji

	Uwaga 1: Funkcja ustawaia tak¿e parametry których nie nale¿y zmieniaæ w czasie pracy wyœwietlacza.
	Uwaga 2: W starszej wersji sterownika (zwykle nie mamy mo¿liwoœci sprawdzenia z jak¹ mamy do czynienia)
		ustawienia powinny byæ zmieniane tylko raz po zerowaniu.

	* Po zerowaniu ustawienia s¹ nieokreœlone. Nale¿y przeprowadziæ inicjacjê.
*/
#define LCD_DISCTL 0xB6

/*
TEMPGRD - ustawienie kompensacji temperaturowej
	Po komendzie nale¿y przes³aæ 14 bajtów. Tylko dwa najm³odsze bity z pierwszego bajtu maj¹ znaczenie.
	Reszta s³u¿y do testowania uk³adu (przydatne dla producenta). Bezpiecznie mo¿na wys³aæ same 0 (w³¹cznie z pierwszym bajtem)
	B1 B0    %/K
	 0  0   -0.05
	 0  1   -0.10
	 1  0   -0.15
	 1  1   -0.20

	* Po zerowaniu ustawienia s¹ nieokreœlone. Nale¿y przeprowadziæ inicjacjê.
*/
#define LCD_TEMPGRD 0xB7

/*
REFSET - ustawienie statusu sterownika
	Powinna byæ wywo³ana przed konfiguracj¹ sterownika (? - na pewno przy inicjacj. Nie jestem pewien czy zawsze)
	Za komend¹ trzeba wys³aæ bajt danych z 0 na 3 najm³odszych pozycjach. Reszta bitów nie ma znaczenia.
*/
#define LCD_REFSET 0xB9

/*
VOLCTL - kontrola napiêcia przetwornicy (steruj¹cego wyœwietlaczem)
		Po komendzie nale¿y przes³aæ dwa bajty
		1: 0-127 - wartoœæ napiêcia
		2: dwa najm³odsze bity = 1, reszta bez znaczenia

	* Po zerowaniu ustawienia s¹ nieokreœlone. Nale¿y przeprowadziæ inicjacjê.
*/
#define LCD_VOLCTL 0xBA

/*
COMOUT - sposób sterowania wyprowadzeñ wspólnych (linie)
	Po instrukcji nale¿y przes³aæ jeden bajt. Znacenie maj¹ 4 najm³odsze bity:
	* * * * B3 B2 B1 B0
	B3: kolejnoœæ wyœwietlania po³ówek wyœwietlacza. Dla wyœwietlacza o 67 liniach musi byæ = 0
	B2: wybór sposobu sterowania
		0 - normalnie
		1 - z przeplotem
	B1, B0 - kierunek przemiatania wyprowadzeñ. Dla nas ma ma³e znaczenie, wpisujemy 00

*/
#define LCD_COMOUT 0xBD

/*
PWRCTL - sterowanie zasilaniem
	Po komendzie nale¿y przes³aæ jeden bajt danych:
	* B6 B5 B4 B3 B2 B1 B0
	B6 - wybór Ÿród³a zegara dla przetwornicy
		0 - ustawienie przy pomocy B2, B1, B0
		1 - po³owa czêstotliwoœci taktowania wyœwietlacza
	B5 - musi byæ równy 0. Wa¿ny dla ustawieñ wewnêtrznych
	B4 - sterowanie sposobem wyrównania napiêcia zasilania matrycy
		0 - przy pomocy wewnêtrznej rezystancji (dla wyœwietlacza 3510i)
		1 - przy pomocy zewnêtrznej rezystancji
	B3 - zmiana mocy wyjœcia generuj¹cego napiêcie matrycy
		0 - du¿a moc
		1 - ma³a moc
		Ma³a moc umo¿liwia zaoszczêdzenie oko³o 100uA, jednak obraz mo¿e staæ siê niestabilny.
	B2, B1, B0 - wartoœæ od 0 do 4. Ustawienie czêstotliwoœci pracy przetwornicy.
		Wiêksza wartoœæ - wiêsza czêstotliwoœæ. Powoduje to wiêkszy pobór pr¹du ale mniejsze zniekszta³cenia obrazu.

	Szczegó³y w dokumentacji
	* Po zerowaniu ustawienia s¹ nieokreœlone. Nale¿y przeprowadziæ inicjacjê.
*/
#define LCD_PWRCTL 0xBE

// Koniec definicji komend
//**********************************************************************************************

#endif //LCD3510I_H_INCLUDED
