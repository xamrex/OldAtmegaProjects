#include <avr/io.h>
#include <util/delay.h>


#define PORT_3510i PORTB
#define SCK_3510i 5
#define SIO_3510i 3
#define CS_3510i  4
#define RES_3510i 0 //mo¿e byæ podpiêty pod 3.3V na sta³e
#define CMD 0
#define DATA 1

void init_3510iport(void)
{
DDRB = (1<<SIO_3510i)|(1<<SCK_3510i)|(1<<CS_3510i);
PORTB=0xff;
}

void send_to_lcd(unsigned char data, unsigned char op_kind)
{

   int i;
   PORT_3510i &= ~(1<<CS_3510i); //CS ustaw na zero
   PORT_3510i &= ~(1<<SCK_3510i);
   if (op_kind==0)
   {                         //komenda - wyslij zero
      PORT_3510i &= ~(1<<SIO_3510i);
   }
   else
   {                        //dane - wyslij jeden
      PORT_3510i |= 1<<SIO_3510i;
   }
   PORT_3510i |= 1<<SCK_3510i;
   //transmisja bitow D7-D0
   for (i=7; i>=0; i--)
   {
      PORT_3510i &= ~(1<<SCK_3510i);
   if ((data & _BV(i))==0)
   {
   PORT_3510i &= ~(1<<SIO_3510i);
   }
   else
   {
   PORT_3510i |= 1<<SIO_3510i;
   }
   PORT_3510i |= 1<<SCK_3510i;
   }
   PORT_3510i |= 1<<CS_3510i;

}


void init_3510i(void)
{
//inicjacja wyswietlacza nokia 3510i
//hardware reset (gdy pin reset podpiêty)
/*
    PORT_3510i &= ~(1<<RES_3510i);
    _delay_ms(5);
    PORT_3510i |= 1<<RES_3510i | 1<<SCK_3510i | 1<<SIO_3510i | 1<<CS_3510i;
    _delay_ms(5);
*/
    send_to_lcd(0x01, CMD);        // LCD Software Reset
    _delay_ms(10);
    send_to_lcd(0xC6, CMD);        // Initial Escape
   _delay_ms(10);
// phase 2: display setup 1
    send_to_lcd(0xB9, CMD);        // Refresh set
    send_to_lcd(0x00, DATA);
    send_to_lcd(0xB6, CMD);        // Display Control
    send_to_lcd(128, DATA);
    send_to_lcd(128, DATA);        //no N line inversion
    send_to_lcd(129, DATA);        //frame freq; bias rate of LCD drive voltage; 98x67; diplay duty=1/67;
    send_to_lcd(84, DATA);
    send_to_lcd(69, DATA);
    send_to_lcd(82, DATA);
    send_to_lcd(67, DATA);
    send_to_lcd(0xB3, CMD);        // Gray Scale Position
    send_to_lcd(1, DATA);
    send_to_lcd(2, DATA);
    send_to_lcd(4, DATA);
    send_to_lcd(8, DATA);
    send_to_lcd(16, DATA);
    send_to_lcd(30, DATA);
    send_to_lcd(40, DATA);
    send_to_lcd(50, DATA);
    send_to_lcd(60, DATA);
    send_to_lcd(70, DATA);
    send_to_lcd(80, DATA);
    send_to_lcd(90, DATA);
    send_to_lcd(100, DATA);
    send_to_lcd(110, DATA);
    send_to_lcd(127, DATA);
    send_to_lcd(0xB5, CMD);        // Gamma Curve Set
    send_to_lcd(1, DATA);
    send_to_lcd(0xBD, CMD);        // Common driver setup
    send_to_lcd(0, DATA);
//phase 3: power supply
    send_to_lcd(0xBE, CMD);        // Power Control
    send_to_lcd(4, DATA);
    send_to_lcd(0x11, CMD);        // Sleep out
    send_to_lcd(0xB7, CMD);        // Temperature gradient set
   for (int i=0; i<=14;i++)
   {
    send_to_lcd(0, DATA);
   }
    send_to_lcd(0x03, CMD);        // Booster Voltage ON
    _delay_ms(40);
    send_to_lcd(0x20, CMD);        // Inversion control (OFF)
    send_to_lcd(0xBA, CMD);        // Voltage control
    send_to_lcd(117, DATA);
    send_to_lcd(3, DATA);
    send_to_lcd(0x25, CMD);        // Write contrast
    send_to_lcd(63, DATA);
    send_to_lcd(0x2D, CMD);        // Colour set (only for 8-bit/pixel)
    send_to_lcd(0x00, DATA);       // RED
    send_to_lcd(0x02, DATA);
    send_to_lcd(0x04, DATA);
    send_to_lcd(0xC6, DATA);
    send_to_lcd(0x09, DATA);
    send_to_lcd(0x0b, DATA);
    send_to_lcd(0x0d, DATA);
    send_to_lcd(0x0f, DATA);
    send_to_lcd(0x00, DATA);       // GREEN
    send_to_lcd(0x02, DATA);
    send_to_lcd(0x04, DATA);
    send_to_lcd(0xC6, DATA);
    send_to_lcd(0x09, DATA);
    send_to_lcd(0x0b, DATA);
    send_to_lcd(0x0d, DATA);
    send_to_lcd(0x0f, DATA);
    send_to_lcd(0x00, DATA);       // BLUE
    send_to_lcd(0x05, DATA);
    send_to_lcd(0x0a, DATA);
    send_to_lcd(0x0f, DATA);
    send_to_lcd(0x3A, CMD);        // 8bpp
    send_to_lcd(0x02, DATA);
    send_to_lcd(0x29, CMD);        // Display On

}

int main(void)
{

DDRD=0b11000000;
init_3510iport();
init_3510i();
PORTD=0b00000000;
unsigned int n;
unsigned char kolor;
while(1)
{
send_to_lcd(0x2C, CMD);

for(n=0;n<6566;n++)
{
if(n==0)kolor=0b11111111;
if(n==2000)kolor=0b00011100;
if(n==4000)kolor=0b00000011;
send_to_lcd(kolor, DATA);
}

}
return 0;
}
