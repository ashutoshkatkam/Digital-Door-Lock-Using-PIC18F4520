#include "config.h"    // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <pic18f4520.h>

#define RS LATD0                   /* PORTD 0 pin is used for Register Select */
#define EN LATD1                   /* PORTD 1 pin is used for Enable */
#define ldata LATC                 /* PORTB is used for transmitting data to LCD */

unsigned char keyfind();  /* function to find pressed key */

#define write_port LATB  /* latch register to write data on port */
#define read_port PORTB  /* PORT register to read data of port */
#define Direction_Port TRISB

unsigned char col_loc,rowloc,temp_col;

#define LCD_Port TRISC              
#define LCD_Control TRISD

unsigned char keypad[4][4] =	{{'7','8','9','/'},
				{'4','5','6','x'},
				{'1','2','3','-'},
				{' ','0','=','+'} };
void LCD_Init();
void LCD_Command(char );
void LCD_Char(char x);
void LCD_String(const char *);
void LCD_String_xy(char ,char ,const char*);
void MSdelay(unsigned int );

int password[4] = {'1','2','3','4'};
int array[4];

/*****************************Main Program*******************************/

void main(void)
{    
    int i;
    char key;                  /* Use Internal Oscillator with Frequency 8MHZ */
    RBPU=0;
    OSCCON=0x72;                   /* Use Internal Oscillator with Frequency 8MHZ */ 
    LCD_Init();                    /* Initialize 16x2 LCD */
    LCD_String_xy(1,0,"Hello Welcome to");    /* Display string at location(row,location). */
    LCD_String_xy(2,0,"Our Project");  /* This function passes string to display */
    MSdelay(3000); /*Display string at location(row,location). */
    LCD_Command(0x01);
    /* This function passes string to display */    
    LCD_String_xy(1,0,"Enter password");
    LCD_Command(0xc0); //Moves cursor to second line forcefully
    /*while(1)
    {
    //    key = keyfind();  /* find a pressed key */
    //    LCD_Char(key);  /* display pressed key on LCD16x2 */
        
    //}
    //for(i=0;i<4;i++)
    //{
    // array[i] = keyfind();
    // LCD_Char(array[i]); 
    //}
    int a1='1';
    int b1='2';
    int c1= '3';
    int d1= '4';
    int e1 = '+';
    int a2,b2,c2,d2,e2;
    TRISA = 0x00;
    PORTA = 0x00;
    a2 = keyfind();
    LCD_Char(a2);
    b2 = keyfind();
    LCD_Char(b2);
    c2 = keyfind();
    LCD_Char(c2);
    d2 = keyfind();
    LCD_Char(d2);
    LCD_Command(0x01);
    LCD_String_xy(1,0,"Password entered");
    if((a1 == a2) && (b1 == b2) && (c1 == c2) && (d1 == d2) )
    {
        LCD_String_xy(2,0,"Correct password");
        MSdelay(3000);  
        //RA1 = 0;
        //RA2 = 1;
        LATA = 0x02;
        MSdelay(3000);
        //RA1 = 0;
        //RA2 = 0;
        LATA = 0x06;
        LCD_Command(0x01);
        LCD_String_xy(1,0,"Door Opened");
        LCD_String_xy(2,0,"Enter + to close");
        LCD_Command(0xc0);
        e2 = keyfind();
        LCD_Char(e2);
        if(e1 == e2)
        {
            LCD_Command(0x01);
            LCD_String_xy(1,0,"Door closed");
            LATA = 0x04;
            //RA1 = 1;
            //RA2 = 0;
            MSdelay(3000);
            LATA = 0x06;
            //RA1 = 0;
            //RA2 = 0;
            LCD_Command(0x01);
        }
        
        
    }
    /*if(a1 == a2)
    {
        if(b1 == b2)
        {
            if(c1 == c2)
            {
                if(d1 == d2)
                {
                   LCD_String_xy(2,0,"Correct password");
                   MSdelay(3000); 
                }
                }
            }
        }*/
    if((a1 != a2) || (b1 != b2) || (c1 != c2) || (d1 != d2)) 
    {
        LCD_Command(0x01);
        LCD_String_xy(1,0,"Wrong password");
        RA0 = 1;
        MSdelay(3000);
        RA0 = 0;
    }
    
}

/****************************Functions********************************/
void LCD_Init()
{
    MSdelay(15);           /* 15ms,16x2 LCD Power on delay */
    LCD_Port = 0x00;       /* Set PORTB as output PORT for LCD data(D0-D7) pins */
    LCD_Control = 0x00;    /* Set PORTD as output PORT LCD Control(RS,EN) Pins */
    LCD_Command(0x38);     /* uses 2 line and initialize 5*7 matrix of LCD */
    LCD_Command(0x01);     /* clear display screen */
    LCD_Command(0x0c);     /* display on cursor off */
    LCD_Command(0x06);     /* increment cursor (shift cursor to right) */
}

void LCD_Clear()
{
    	LCD_Command(0x01); /* clear display screen */
}

void LCD_Command(char cmd )
{
	ldata= cmd;            /* Send data to PORT as a command for LCD */   
	RS = 0;                /* Command Register is selected */
	EN = 1;                /* High-to-Low pulse on Enable pin to latch data */ 
	EN = 0;
	MSdelay(3);	
}

void LCD_Char(char dat)
{
	ldata= dat;            /* Send data to LCD */  
	RS = 1;                /* Data Register is selected */
	EN=1;                  /* High-to-Low pulse on Enable pin to latch data */   
	EN=0;
	MSdelay(1);
}


void LCD_String(const char *msg)
{
	while((*msg)!=0)
	{		
	  LCD_Char(*msg);
	  msg++;	
    	}
		
}

void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    LCD_String(msg);

}
/*********************************Delay Function********************************/
void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);      /*This count Provide delay of 1 ms for 8MHz Frequency */
 }

unsigned char keyfind()
{      
    
    Direction_Port = 0xf0;  /* PORTD.0-PORTD.3 as a Output Port and PORTD.4-PORTD.7 as a Input Port*/
    unsigned char temp1;
   
        write_port = 0xf0;  /* Make lower nibble as low(Gnd) and Higher nibble as High(Vcc) */
      do
      {
        do
        {
          
            col_loc = read_port & 0xf0;  /* mask port with f0 and copy it to col_loc variable */   
        
        }while(col_loc!=0xf0);  /* Check initially at the start there is any key pressed*/ 
         col_loc = read_port & 0xf0;  /* mask port with f0 and copy it to col_loc variable */  
      }while(col_loc!=0xf0);
      
      MSdelay(50);
        write_port = 0xf0;  /* Make lower nibble as low(Gnd) and Higher nibble as High(Vcc) */
        do
        { do
            
            {
               
                col_loc = read_port & 0xf0;
            }while(col_loc==0xf0);  /* Wait for key press */
         col_loc = read_port & 0xf0;
        }while(col_loc==0xf0);  /* Wait for key press */
               
        MSdelay(20);
         
        col_loc = read_port & 0xf0;
               
   
    
    while(1)
    {
        write_port  = 0xfe;  /* make Row0(D0) Gnd and keep other row(D1-D3) high */
        col_loc = read_port & 0xf0;         /* Read Status of PORT for finding Row */
        temp_col=col_loc;
        if(col_loc!=0xf0)
        {
            rowloc=0;  /* If condition satisfied get Row no. of key pressed */
            while(temp_col!=0xf0)  /* Monitor the status of Port and Wait for key to release */
            {
                temp_col = read_port & 0xf0;  /* Read Status of PORT for checking key release or not */    
            }
            break;
        }
        
        write_port = 0xfd;  /* make Row1(D1) Gnd and keep other row(D0-D2-D3) high */
        col_loc = read_port & 0xf0;  /* Read Status of PORT for finding Row */
        temp_col=col_loc;
        if(col_loc!=0xf0)
        {
            rowloc=1;  /* If condition satisfied get Row no. of key pressed*/
            while(temp_col!=0xf0)  /* Monitor the status of Port and Wait for key to release */
            {
                temp_col = read_port & 0xf0;  /* Read Status of PORT for checking key release or not */
            }
            break;
        }
        
        write_port = 0xfb;  /* make Row0(D2) Gnd and keep other row(D0-D1-D3) high */
        col_loc = read_port & 0xf0;  /* Read Status of PORT for finding Row*/
        temp_col=col_loc;
        if(col_loc!=0xf0)
        {
            rowloc=2;  /* If condition satisfied get Row no. of key pressed */
            while(temp_col!=0xf0)  /* Wait for key to release */
            {
                temp_col = read_port & 0xf0;  /* Read Status of PORT for checking key release or not */
            }
            break;
        }
        
        write_port = 0xf7;  /* make Row0(D3) Gnd and keep other row(D0-D2) high */
        col_loc = read_port & 0xf0;  /* Read Status of PORT for finding Row */
        temp_col=col_loc;
        if(col_loc!=0xf0)
        {
            rowloc=3;  /* If condition satisfied get Row no. of key pressed */
            while(temp_col!=0xf0)  /* Wait for key to release */
            {
                temp_col = read_port & 0xf0;  /* Read Status of PORT for checking key release or not */
            }
            break;
        }
        
    }

    
    while(1)
    {
        
        if(col_loc==0xe0)
        {
             return keypad[rowloc][0];  /* Return key pressed value to calling function */            
        }
        else 
            if(col_loc==0xd0)
        {
            return keypad[rowloc][1];  /* Return key pressed value to calling function */   
        }
        else
            if(col_loc==0xb0)
        {
            return keypad[rowloc][2];  /* Return key pressed value to calling function */
        }
        else
            
        {
            return keypad[rowloc][3];  /* Return key pressed value to calling function */           
        }    
    }
    
   MSdelay(300);     
 
} 
