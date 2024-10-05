#include "LcdDriver.h"

#ifdef ASIM
	uint16_t asim_buffer[LcdDriver::WIDTH*LcdDriver::HEIGHT];
	static uint16_t asim_x = 0;
	static uint16_t asim_y = 0;
	static uint16_t asim_xs = 0;
	static uint16_t asim_xe = 0;
	static uint16_t asim_ys = 0;
	static uint16_t asim_ye = 0;

#endif

LcdDriver::LcdDriver()
{
}

void LcdDriver::writeByte(uint8_t da) 
{ 
  DEV_Digital_Write(DEV_CS_PIN, 0);
  DEV_Digital_Write(DEV_DC_PIN, 1);
  DEV_SPI_WRITE(da);
  DEV_Digital_Write(DEV_CS_PIN, 1);
}  

void LcdDriver::writeWord(uint16_t da)
{
#ifdef ASIM 
		int idx = asim_x + asim_y * LcdDriver::WIDTH;
		asim_buffer[idx] = da;

		asim_x++;
		if (asim_x >= asim_xe) {
			asim_x = asim_xs;
			asim_y++;
		}
		// Check if we are done
		if (asim_y >= asim_ye) {
			asim_y = asim_ys;
		}
#else
 // UBYTE i = (da >> 8) & 0xff;
  DEV_Digital_Write(DEV_CS_PIN, 0);
  DEV_Digital_Write(DEV_DC_PIN, 1);
  DEV_SPI_WRITE(da>>8);
  DEV_SPI_WRITE(da);
  DEV_Digital_Write(DEV_CS_PIN, 1);
#endif

}   

void LcdDriver::writeRegister(uint8_t da)  
{ 
  
  DEV_Digital_Write(DEV_CS_PIN, 0);
  DEV_Digital_Write(DEV_DC_PIN, 0);
  DEV_SPI_WRITE(da);
  //DEV_Digital_Write(DEV_CS_PIN,1);
}

/******************************************************************************
function: 
    Common register initialization
******************************************************************************/
void LcdDriver::init()
{
	// reset
	DEV_Digital_Write(DEV_CS_PIN,0);
	DEV_Delay_ms(100);
	DEV_Digital_Write(DEV_RST_PIN,0);
	DEV_Delay_ms(100);
	DEV_Digital_Write(DEV_RST_PIN,1);
	DEV_Delay_ms(100);

	// start initialization
 	writeRegister(0xEF);
	writeRegister(0xEB);
	writeByte(0x14); 
	
  	writeRegister(0xFE);			 
	writeRegister(0xEF); 

	writeRegister(0xEB);	
	writeByte(0x14); 

	writeRegister(0x84);			
	writeByte(0x40); 

	writeRegister(0x85);			
	writeByte(0xFF); 

	writeRegister(0x86);			
	writeByte(0xFF); 

	writeRegister(0x87);			
	writeByte(0xFF);

	writeRegister(0x88);			
	writeByte(0x0A);

	writeRegister(0x89);			
	writeByte(0x21); 

	writeRegister(0x8A);			
	writeByte(0x00); 

	writeRegister(0x8B);			
	writeByte(0x80); 

	writeRegister(0x8C);			
	writeByte(0x01); 

	writeRegister(0x8D);			
	writeByte(0x01); 

	writeRegister(0x8E);			
	writeByte(0xFF); 

	writeRegister(0x8F);			
	writeByte(0xFF); 


	writeRegister(0xB6);
	writeByte(0x00);
	writeByte(0x20);

	writeRegister(0x36);
	writeByte(0x08);

	writeRegister(0x3A);			
	writeByte(0x05); 


	writeRegister(0x90);			
	writeByte(0x08);
	writeByte(0x08);
	writeByte(0x08);
	writeByte(0x08); 

	writeRegister(0xBD);			
	writeByte(0x06);
	
	writeRegister(0xBC);			
	writeByte(0x00);	

	writeRegister(0xFF);			
	writeByte(0x60);
	writeByte(0x01);
	writeByte(0x04);

	writeRegister(0xC3);			
	writeByte(0x13);
	writeRegister(0xC4);			
	writeByte(0x13);

	writeRegister(0xC9);			
	writeByte(0x22);

	writeRegister(0xBE);			
	writeByte(0x11); 

	writeRegister(0xE1);			
	writeByte(0x10);
	writeByte(0x0E);

	writeRegister(0xDF);			
	writeByte(0x21);
	writeByte(0x0c);
	writeByte(0x02);

	writeRegister(0xF0);   
	writeByte(0x45);
	writeByte(0x09);
	writeByte(0x08);
	writeByte(0x08);
	writeByte(0x26);
 	writeByte(0x2A);

 	writeRegister(0xF1);    
 	writeByte(0x43);
 	writeByte(0x70);
 	writeByte(0x72);
 	writeByte(0x36);
 	writeByte(0x37);  
 	writeByte(0x6F);


 	writeRegister(0xF2);   
 	writeByte(0x45);
 	writeByte(0x09);
 	writeByte(0x08);
 	writeByte(0x08);
 	writeByte(0x26);
 	writeByte(0x2A);

 	writeRegister(0xF3);   
 	writeByte(0x43);
 	writeByte(0x70);
 	writeByte(0x72);
 	writeByte(0x36);
 	writeByte(0x37); 
 	writeByte(0x6F);

	writeRegister(0xED);	
	writeByte(0x1B); 
	writeByte(0x0B); 

	writeRegister(0xAE);			
	writeByte(0x77);
	
	writeRegister(0xCD);			
	writeByte(0x63);		


	writeRegister(0x70);			
	writeByte(0x07);
	writeByte(0x07);
	writeByte(0x04);
	writeByte(0x0E); 
	writeByte(0x0F); 
	writeByte(0x09);
	writeByte(0x07);
	writeByte(0x08);
	writeByte(0x03);

	writeRegister(0xE8);			
	writeByte(0x34);

	writeRegister(0x62);			
	writeByte(0x18);
	writeByte(0x0D);
	writeByte(0x71);
	writeByte(0xED);
	writeByte(0x70); 
	writeByte(0x70);
	writeByte(0x18);
	writeByte(0x0F);
	writeByte(0x71);
	writeByte(0xEF);
	writeByte(0x70); 
	writeByte(0x70);

	writeRegister(0x63);			
	writeByte(0x18);
	writeByte(0x11);
	writeByte(0x71);
	writeByte(0xF1);
	writeByte(0x70); 
	writeByte(0x70);
	writeByte(0x18);
	writeByte(0x13);
	writeByte(0x71);
	writeByte(0xF3);
	writeByte(0x70); 
	writeByte(0x70);

	writeRegister(0x64);			
	writeByte(0x28);
	writeByte(0x29);
	writeByte(0xF1);
	writeByte(0x01);
	writeByte(0xF1);
	writeByte(0x00);
	writeByte(0x07);

	writeRegister(0x66);			
	writeByte(0x3C);
	writeByte(0x00);
	writeByte(0xCD);
	writeByte(0x67);
	writeByte(0x45);
	writeByte(0x45);
	writeByte(0x10);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0x00);

	writeRegister(0x67);			
	writeByte(0x00);
	writeByte(0x3C);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0x00);
	writeByte(0x01);
	writeByte(0x54);
	writeByte(0x10);
	writeByte(0x32);
	writeByte(0x98);

	writeRegister(0x74);			
	writeByte(0x10);	
	writeByte(0x85);	
	writeByte(0x80);
	writeByte(0x00); 
	writeByte(0x00); 
	writeByte(0x4E);
	writeByte(0x00);					
	
    writeRegister(0x98);			
	writeByte(0x3e);
	writeByte(0x07);

	writeRegister(0x35);	
	writeRegister(0x21);

	writeRegister(0x11);
	DEV_Delay_ms(120);
	writeRegister(0x29);
	DEV_Delay_ms(20);
  /*************************/
} 

void LcdDriver::drawArea(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t  ye)
{ 
#ifdef ASIM
	asim_xs = xs;
	asim_xe = xe;
	asim_ys = ys;
	asim_ye = ye;

	asim_x = asim_xs;
	asim_y = asim_ys;
#else 
  writeRegister(0x2a);
  writeByte(0x00);
  writeByte(xs);
  writeByte(0x00);  
  writeByte(xe);
  
  writeRegister(0x2b);
  writeByte(0x00);
  writeByte(ys);
  writeByte(0x00);
  writeByte(ye);
  
  writeRegister(0x2c);
#endif
};


