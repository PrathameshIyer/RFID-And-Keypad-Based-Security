#include<reg51.h>
#include<string.h>
#include<stdio.h>
#define lcdport P2
sbit col1=P0^0;
sbit col2=P0^1;
sbit col3=P0^2;
sbit row1=P0^4;
sbit row2=P0^5;
sbit row3=P0^6;
sbit row4=P0^7;
sbit rs=P1^0;
sbit rw=P1^1;
sbit e=P1^2;
sbit pir=P1^3;
sbit sw=P3^3;
sbit m1=P3^4;
sbit m2=P3^5;
sbit buzzer=P3^7;
char i;
char rfid[12];
char ch;
char pass[4];

void delay(unsigned int itime)
{
unsigned int k,j;
for(k=0;k<itime;k++)
for(j=0;j<1275;j++);
}
void cmd(unsigned char command) //Function to send command instruction to LCD
{
lcdport=command;
rs= 0;
rw=0;	
e=1;
delay(1);
e=0;
}
void lcd_data(unsigned char disp_data) //Function to send display data to LCD
{
lcdport=disp_data;
rs=1;
rw=0;
e=1;
delay(1);
e=0;
}
void lcd_init() //Function to prepare the LCD  and get it ready
{
	 cmd(0x38);
	delay(10);
    cmd(0x0e);
	delay(10);
    cmd(0x01);
	delay(10);
    cmd(0x06);
	delay(10);
    cmd(0x0c);
	delay(10);
    cmd(0x80);
  }

void lcdstring(char *str) //function to display on lcd
{
    while(*str)
    {
			 lcd_data(*str);
        str++;
    }
}
void keypad()
{
cmd(1);
lcdstring("Enter your passkey");
cmd(0xc0);
i=0;
while(i<4)
{
col1=0;
     col2=col3=1;
     if(!row1)
     {
        lcd_data('1');
        pass[i++]='1';
        while(!row1);
   }
     
      else if(!row2)
     {
        lcd_data('4');
        pass[i++]='4';
        while(!row2);
   }
     
      else if(!row3)
     {
        lcd_data('7');
        pass[i++]='7';
        while(!row3);
   }
     
      else if(!row4)
     {
        lcd_data('*');
        pass[i++]='*';
        while(!row4);
   }
     
     col2=0;
     col1=col3=1;
     if(!row1)
     {
        lcd_data('2');
        pass[i++]='2';
        while(!row1);
   }
     
      else if(!row2)
     {
        lcd_data('5');
        pass[i++]='5';
        while(!row2);
   }
     
      else if(!row3)
     {
        lcd_data('8');
        pass[i++]='8';
        while(!row3);
   }
     
      else if(!row4)
     {
        lcd_data('0');
        pass[i++]='0';
        while(!row4);
   }
     
     col3=0;
     col1=col2=1;
     if(!row1)
     {
        lcd_data('3');
        pass[i++]='3';
        while(!row1);
   }
     
      else if(!row2)
     {
        lcd_data('6');
        pass[i++]='6';
        while(!row2);
   }
     
      else if(!row3)
     {
        lcd_data('9');
        pass[i++]='9';
        while(!row3);
   }
     
      else if(!row4)
     {
        lcd_data('#');
        pass[i++]='#';
        while(!row4);
   }
 }
}
void uart_init()
{
TMOD=0x20;
SCON=0x50;
TH1=0xfd;
TR1=1;
}
char rxdata()
{
while(RI==0)
ch=SBUF;
RI=0;
return ch;
}
void accept()
{
cmd(1);
lcdstring("Welcome");
cmd(192);
lcdstring("Password Accept");
delay(200);
}
void wrong()
{
buzzer=0;
cmd(1);
lcdstring("Wrong Passkey");
cmd(0xc0);
lcdstring("PLZ Try Again");
delay(200);
buzzer=1;
}
void main()
{
lcd_init();
	uart_init();
while(1)
{
	if(pir==1)
{
//delay(30);
//buzzer=1;
//lcd_init();
lcdstring("RFID Based");
cmd(0xc0);
lcdstring("Security system");
delay(400);
cmd(0x01);
	cmd(0x80);
lcdstring("Place Your Card:");
	cmd(0xc0);
	delay(100);	
i=0;
        for(i=0;i<12;i++)
        rfid[i]=rxdata(); // to receive serial data and store it
	         rfid[i]='\0';
        cmd(1);
        lcdstring("Your ID No. is:");
	      cmd(0xc0);
	     for(i=0;i<12;i++)
        lcd_data(rfid[i]); //function call to display card no. on lcd
        delay(100);
	if(strncmp(rfid,"160066A5EC39",12)==0)
	{
	keypad();
if(strncmp(pass,"4201",4)==0)
{
accept();
cmd(1);
lcdstring("Access Granted ");
cmd(0xc0);
lcdstring("Person1");
m1=1;
m2=0;
delay(300);
m1=0;
m2=0;
delay(200);
m1=0;
m2=1;
delay(300);
m1=0;
m2=0;
}
else
wrong();
}
else if(strncmp(rfid,"160066BD7AB7",12)==0)
{
keypad();
if(strncmp(pass,"4202",4)==0)
{
accept();
cmd(1);
lcdstring("Access Granted ");
cmd(192);
lcdstring("Person2");
m1=1;
m2=0;
delay(300);
m1=0;
m2=0;
delay(200);
m1=0;
m2=1;
delay(300);
m1=0;
m2=0;
}
else
wrong();
}
else if(strncmp(rfid,"160066203060",12)==0)
{
keypad();
if(strncmp(pass,"4203",4)==0)
{
accept();
cmd(1);
lcdstring("Access Granted ");
cmd(192);
lcdstring("Person3");
m1=1;
m2=0;
delay(300);
m1=0;
m2=0;
delay(200);
m1=0;
m2=1;
delay(300);
m1=0;
m2=0;
}
}
else
wrong();
}
else
	cmd(0x01);
}
}
