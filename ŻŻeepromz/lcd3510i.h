////////////////////////////////////////////////////////////////////////////////////////////
// lcd3510i.h - Biblioteka obs�ugi wy�wietlacza Nokia3510i
//
// Autor: Rados�aw Koppel          Kompilator: WinAVR 20050214
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LCD3510I_H_INCLUDED
#define LCD3510I_H_INCLUDED

// Potrzebne typy zmiennych
typedef uint8_t lcd_pixel;

// Makra pomocnicze

// Tworzenie koloru palety
#define LCD_palRGB(r, g, b) ((uint16_t)((r&0x0F)<<8) | ((g&0x0F)<<4) | ((b&0x0F)<<0))

// Rozmiary pola wy�wietlacza
#define LCD_SX 98
#define LCD_SY 67

// Funkcje obs�ugi LCD
void lcd_Init(void);
void lcd_On(void);
void lcd_Off(void);

// Funkcje obs�ugi palety
void lcd_loadRGB_P(const prog_uint16_t* pRGB, uint16_t size);

// Funkcje obs�ugi bufora
void lcd_Cls(lcd_pixel pix);
void lcd_Update(void);

// Funkcje rysuj�ce
void lcd_Pixel(uint8_t x, uint8_t y, lcd_pixel color);

//**********************************************************************************************
// Definicje komend

/*
NOP, NOP2 - brak operacji
	Komenda powoduje wyj�cie z trybu zapisu / odczytu pami�ci. Umo�liwia wyj�cie z trybu testowego (patrz komenda TSTMOD).
	Dla zapewnienia stabilno�ci dzia�ania zalecane jest jej periodyczne wywo�ywanie.
*/
#define LCD_NOP 0x00
#define LCD_NOP2 0xAA

/*
SWRESET - reset programowy
	Jej dzia�anie jest identyczne jak podanie stanu niskiego na wej�cie /RST
*/
#define LCD_SWRESET 0x01

/*
BSTOFF / BSTON - wy��cza / w��cza wbudowan� przetwornice napi�cia.
	Komendy SLPIN/SLPOUT tak�e wy��czaj� i w��czaj� przetwornic�,
	jednak ta komenda zmienia jej stan niezale�nie.

	* Po zerowaniu przetwornica jest wy��czona
*/
#define LCD_BSTOFF 0x02
#define LCD_BSTON 0x03

/*
RDDST - umo�liwia odczyt statusu wy�wietlacza. Brak zastosowania w projekcie.
	!!! Aby odczyt by� mo�liwy, linia LCD_SIO musi mie� mo�liwo�� stania si� lini� wej�ciow�!!!
	W odpowiedzi przesy�ane jest 5 bajt�w
	0: b��dne dane - wynika z dzia�ania interfejsu
	1: D31 - D24
	2: ...
	3: ...
	4: D7 - D0

	Szczeg�y w dokumentacji
*/
#define LCD_RDDST 0x09 // ODCZYT !!!

/*
SLPIN / SLPOUT - wej�cie w tryb u�pienia
	W trybie u�pienia przetwornica napi�cia jest wy��czona, oscylator zatrzymany. Wszystkie piksele nieaktywne.

	* Po zerowaniu wy�wietlacz jest w trybie u�pienia
*/
#define LCD_SLPIN 0x10
#define LCD_SLPOUT 0x11

/*
PTLON - wej�cie w tryb niepe�nego wy�wietlania
	W trybie tym istnieje mo�liwo�� wybrania zakresu obs�ugiwanej powierzchni wy�wietlacza
	Pozosta�e piksele s� wygaszone.
	Ustawienie zakresu wy�wietlanych linii jest mo�liwe przy pomocy komendy PLTAR
	Aby wyj�� z tego trybu nale�y poda� komend� NORON.

	* Po zerowaniu wy�wietlacz jest w trybie normalnym
*/
#define LCD_PTLON 0x12

/*
NORON - wej�cie w tryb normalny
	Wprowadza wy�wietlacz w zwyk�y tryb wy�wietlania.

	* Jest to domy�lny stan po zerowaniu uk�adu
*/
#define LCD_NORON 0x13

/*
INVOFF / INVON - wy��czenie / w��czenie inwersji pikseli
	Dzia�anie komendy odnosi si� do sposobu przetwarzania danych z pami�ci na obraz.
	Nie zmienia zawarto�ci pami�ci obrazu.

	* Po zerowaniu inwersja jest wy��czona
*/
#define LCD_INVOFF 0x20
#define LCD_INVON 0x21

/*
FIXOFF / FIXON - wygaszenie / zapalenie wszystkich pikseli
	Sterownik zaprzestaje dost�pu do pami�ci obrazu i wysterowuje wszystkie piksele
	jako wy��czone / w��czone.
	Wyj�cie z tego trybu jest mo�liwe przez wydanie komendy NORON albo PLTON

	* Po zerowaniu wszystkie piksele s� wy��czone
*/
#define LCD_FIXOFF 0x22
#define LCD_FIXON 0x23

/*
WRTCNTR - ustawienie kontrastu
	Po wys�aniu komendy nale�y wys�a� dan� o warto�ci 0-127.
	�redni kontrast ma warto�� 63. Napi�cie wy�wietlacza ro�nie wraz ze wzrostem warto�ci kontrastu.

	* Po zerowaniu domy�lna warto�� kontrastu wynosi 63
*/
#define LCD_WRCNTR 0x25

/*
DISOFF / DISON - wy��czenie / w��czenie wy�wietlacza
*/
#define LCD_DISOFF 0x28
#define LCD_DISON 0x29

/*
CASET - ustawienie adres�w kolumn
PASET - ustawienie adres�w stron

		+--------------------+
		| +-------------+    | < SP - strona (linia) pocz�tkowa
		| | Okno zapisu |    |
		| +-------------+    | < EP - strona (linia) ko�cowa
		|                    |
		+--------------------+
		  ^         ^
		  SC        EC - kolumna pocz�tkowa i ko�cowa

	Po komendach nast�puje przes�anie 8 bitowych liczb oznaczaj�cych pocz�tek i koniec okna zapisu.
	Pocz�tek musi by� liczb� mniejsz� ni� koniec. Je�li podano warto�ci kt�re nie mieszcz� si� w pami�ci
	obrazu, przesy�ane w to miejsce dane b�d� ignorowane.

	Funkcje umo�liwiaj� zdefiniowanie sposobu "okna" w kt�rym b�d� umieszczane zapisywane dane.
	Jest to o tyle wygodne, �e je�li chcemy od�wie�y� tylko cz�� obrazu, nie musimy zmienia� ustawie�.
	Wysy�amy blok danych a sterownik umieszcza je w odpowiednich miejscach pami�ci, tak aby pasowa�y
	do zdefiniowanego okna.

	* Po zerowaniu, zale�nie od wybranego typu wy�wietlacza przyj�te s� nast�puj�ce warto�ci warto�ci obejmuj�ce ca�y obszar wy�wietlacza
	dla naszego 98x67:
	SC=0; EC=97; SP=0; EP=66;
*/
#define LCD_CASET 0x2A
#define LCD_PASET 0x2B

/*
RAMWR - rozpocz�cie zapisu do pami�ci wy�wietlacza
	Po wys�aniu tej komendy adres zapisu jest ustawiany na pocz�tek zdefiniowanego okna.
	Ka�da wys�ana dana powoduje przesuni�cie wska�nika o jedn� pozycj�.
	Zapis przerywa wys�anie dowolnej innej komendy.
*/
#define LCD_RAMWR 0x2C

/*
RGBSET - ustawienie palety kolor�w dla interfejsu 8bpp
	Umo�liwia ustawienie palety kolor�w s�u��cej do przej�cia mi�dzy trybem 256 kolor�w (8bpp) na tryb 4096 kolor�w (12bpp).
	Oddzielnie ustawiane s� sk�adowe wszystkich kolor�w.
	W trybie 8bpp kolory czerwony oraz zielony maj� 3 bity a niebieski 2.
	Dane palety maj� format jak ni�ej:

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

	Uwaga: Dane w pami�ci przechowywane s� w postaci 12bpp. Zmiana odbywa si� jeszcze przed zapisem warto�ci do pami�ci obrazu.
	Zatem sama zmiana palety nie daje od razu widocznych zmian na wy�wietlaczu.

	* Po zerowaniu zawarto�� palety jest nieokre�lona. Je�li zamierzamy korzysta� z trynu 8bpp konieczna jest inicjacja.
*/
#define LCD_RGBSET 0x2D

/*
RAMRD - odczyt danych z pami�ci obrazu. Brak zastosowania w projekcie.
	!!! Aby odczyt by� mo�liwy, linia LCD_SIO musi mie� mo�liwo�� stania si� lini� wej�ciow�!!!
*/
#define LCD_RAMRD 0x2E // ODCZYT !!!

/*
PLTAR - obszar cz�ciowy
	Funkcja umo�liwia ustawienie pocz�tkowej i ko�cowej wy�wietlanej linii.
	Aby jej dzia�anie by�o widoczne konieczne jest wej�cie w specjalny tryb komend� PLTON.
*/
#define LCD_PTLAR 0x30

/*
VSCRDEF - ustawienie obszar�w przewijania
*/
#define LCD_VSCRDEF 0x33

/*
MADCTL - kontrola dost�pu do pami�ci
	Ustawia spos�b dost�pu do pami�ci zar�wno od strony mikrokontrolera jak i od strony kontrolera LCD
	Za komend� nale�y wys�a� jeden bajt zawieraj�cy parametry:
	B7 B6 B5 B4 B3 0 0 B0

	B7: Ustawia pozycj� zerowej strony (lini)
		0 - od g�ry do do�u
		1 - od do�u do g�ry
	B6: Ustawia pozycj� zerowej kolumny
		0 - od lewej do prawej
		1 - od prawej do lewej
	B5: Ustawienie kolejno�ci zmian lini / kolumny
		0 - tryb normalny: zmieniany jest adres kolumny a� przeskanowana zostanie ca�a linia w oknie,
			wtedy zmienia si� adres linii a kolumna zaczyna si� od pocz�tku.
			+------------+
			|123456789ABC|
			|DEFGHIJKLMNO|
			|PRSTUWXYZ   |
			+------------+
		1 - tryb odwr�cony: zmieniany jest adres linii a� przeskanowana zostanie ca�a kolumna,
			wtedy zmienia si� adres kolumny a linia zaczyna si� od pocz�tku.
			+------------+
			|147ADGJMPTX |
			|258BEHKNRUY |
			|369CFILOSWZ |
			+------------+
	B4: Wybranie kierunku skanowania linii wy�wietlacza
		0 - skanowanie od g�ry do do�u
		1 - skanowanie od do�u do g�ry
	B3: Kolejno�� sk�adowych koloru
		0 - RGB
		1 - BGR
	B0: Odwr�cenie wy�wietlacza w pionie
		0 - tryb normalny
		1 - wy�wietlacz odwr�cony

	* Po zerowaniu, warto�� domy�lna to same 0
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
VSCRSADD - Adres pocz�tku obszaru przewijanego
*/
#define LCD_VSCRSADD 0x37

/*
IDMOFF / IDMON - wyj�cie / wej�cie w tryb IDLE
	W trybie IDLE, pob�r mocy jest zmniejszony, wy�wietlacz wy�wietla tylko 8 kolor�w
	(brane pod uwag� s� tylko najstarsze bity z ka�dej sk�adowej koloru).

	* Po zerowaniu tryb IDLE jest wy��czony
*/
#define LCD_IDMMOFF 0x38
#define LCD_IDMON 0x39

/*
COLMOD - tryb koloru
	Umo�liwia wybranie trybu koloru z punktu widzenia interfejsu z mikrokontrolerem.
	Za komend� nale�y przes�a� dan� ustawiaj�c� wybrany tryb:
	2 - 256 kolr�w
	3 - 4096 kolor�w

	W pami�ci kolory przechowywane s� w formacie 12 bitowym. Zmiana odbywa si� w chwili wpisywania danych.

	* Domy�lnym trybem po zerowaniu jest tryb 12bpp
*/
#define LCD_COLMOD 0x3A
	#define LCD_COL8bpp 2
	#define LCD_COL12bpp 3

/*
TSTMOD - Wej�cie w tryb testowy
	Funkcja umo�liwia testowanie uk�adu. U�yteczna praktycznie tylko z punktu widzenia producenta.
	W trybie testowym wy�wietlacz nie jest sterowany prawid�owo.
	Istnieje mo�liwo��, �e wy�wietlacz odbierze t� w�a�nie komend� z powodu zak��ce�.
	Wyj�cie z trybu testowego odbywa umo�liwia komenda NOP lub NOP2.
*/
#define LCD_TSTMOD 0xDE

/*
INIESC - inicjacja ustawie� wewn�trznych sterownika
	Konieczne wywo�anie w odpowiednim miejscu sekwencji inicjuj�cej.
	Powinna by� wywo�ana przed rozpocz�ciem konfiguracji.
*/
#define LCD_INIESC 0xC6

/*
GCPSET0, GCPSET1 - ustawienie skali szaro�ci (krzywa gamma)
	Ustawiane mo�na ustawi� dwie krzywe wybierane nast�pnie instrukcj� GAMSET.
	Po komendzie nale�y przes�a� 15 warto�ci, kolejno poziom szaro�ci (nat�enie koloru)
	dla danej 0001, 0010, ..., 1111.
	Warto�� dla danej koloru 0000 jest ustawiona na sta�e.
	Nale�y zadba� aby wpisywane warto�ci by�y rosn�ce.

	Uwaga: Nie zmienia� w czasie wy�wietlania obrazu (aktywny wy�wietlacz)
	* Po zerowaniu zawarto�� obu krzywych jest nieokre�lona
*/
#define LCD_GCPSET0 0xB3
#define LCD_GCPSET1 0xB4

/*
GAMSET - umo�liwia wybranie jednej z definiowanych krzywych gamma
	Wys�ana za instrukcj� dana umo�liwia wybranie krzywej:
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
DISCTL - konfiguracja wy�wietlacza
	Za komend� nale�y przes�a� 7 bajt�w danych konfigurujacych wy�wietlacz.
	W to dane standardowe zale�ne od konkretnego modu�u wy�wietlacza.
	Szczeg�y w dokumentacji

	Uwaga 1: Funkcja ustawaia tak�e parametry kt�rych nie nale�y zmienia� w czasie pracy wy�wietlacza.
	Uwaga 2: W starszej wersji sterownika (zwykle nie mamy mo�liwo�ci sprawdzenia z jak� mamy do czynienia)
		ustawienia powinny by� zmieniane tylko raz po zerowaniu.

	* Po zerowaniu ustawienia s� nieokre�lone. Nale�y przeprowadzi� inicjacj�.
*/
#define LCD_DISCTL 0xB6

/*
TEMPGRD - ustawienie kompensacji temperaturowej
	Po komendzie nale�y przes�a� 14 bajt�w. Tylko dwa najm�odsze bity z pierwszego bajtu maj� znaczenie.
	Reszta s�u�y do testowania uk�adu (przydatne dla producenta). Bezpiecznie mo�na wys�a� same 0 (w��cznie z pierwszym bajtem)
	B1 B0    %/K
	 0  0   -0.05
	 0  1   -0.10
	 1  0   -0.15
	 1  1   -0.20

	* Po zerowaniu ustawienia s� nieokre�lone. Nale�y przeprowadzi� inicjacj�.
*/
#define LCD_TEMPGRD 0xB7

/*
REFSET - ustawienie statusu sterownika
	Powinna by� wywo�ana przed konfiguracj� sterownika (? - na pewno przy inicjacj. Nie jestem pewien czy zawsze)
	Za komend� trzeba wys�a� bajt danych z 0 na 3 najm�odszych pozycjach. Reszta bit�w nie ma znaczenia.
*/
#define LCD_REFSET 0xB9

/*
VOLCTL - kontrola napi�cia przetwornicy (steruj�cego wy�wietlaczem)
		Po komendzie nale�y przes�a� dwa bajty
		1: 0-127 - warto�� napi�cia
		2: dwa najm�odsze bity = 1, reszta bez znaczenia

	* Po zerowaniu ustawienia s� nieokre�lone. Nale�y przeprowadzi� inicjacj�.
*/
#define LCD_VOLCTL 0xBA

/*
COMOUT - spos�b sterowania wyprowadze� wsp�lnych (linie)
	Po instrukcji nale�y przes�a� jeden bajt. Znacenie maj� 4 najm�odsze bity:
	* * * * B3 B2 B1 B0
	B3: kolejno�� wy�wietlania po��wek wy�wietlacza. Dla wy�wietlacza o 67 liniach musi by� = 0
	B2: wyb�r sposobu sterowania
		0 - normalnie
		1 - z przeplotem
	B1, B0 - kierunek przemiatania wyprowadze�. Dla nas ma ma�e znaczenie, wpisujemy 00

*/
#define LCD_COMOUT 0xBD

/*
PWRCTL - sterowanie zasilaniem
	Po komendzie nale�y przes�a� jeden bajt danych:
	* B6 B5 B4 B3 B2 B1 B0
	B6 - wyb�r �r�d�a zegara dla przetwornicy
		0 - ustawienie przy pomocy B2, B1, B0
		1 - po�owa cz�stotliwo�ci taktowania wy�wietlacza
	B5 - musi by� r�wny 0. Wa�ny dla ustawie� wewn�trznych
	B4 - sterowanie sposobem wyr�wnania napi�cia zasilania matrycy
		0 - przy pomocy wewn�trznej rezystancji (dla wy�wietlacza 3510i)
		1 - przy pomocy zewn�trznej rezystancji
	B3 - zmiana mocy wyj�cia generuj�cego napi�cie matrycy
		0 - du�a moc
		1 - ma�a moc
		Ma�a moc umo�liwia zaoszcz�dzenie oko�o 100uA, jednak obraz mo�e sta� si� niestabilny.
	B2, B1, B0 - warto�� od 0 do 4. Ustawienie cz�stotliwo�ci pracy przetwornicy.
		Wi�ksza warto�� - wi�sza cz�stotliwo��. Powoduje to wi�kszy pob�r pr�du ale mniejsze zniekszta�cenia obrazu.

	Szczeg�y w dokumentacji
	* Po zerowaniu ustawienia s� nieokre�lone. Nale�y przeprowadzi� inicjacj�.
*/
#define LCD_PWRCTL 0xBE

// Koniec definicji komend
//**********************************************************************************************

#endif //LCD3510I_H_INCLUDED
