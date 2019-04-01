/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Author: pepe
 *
 * Created on 2019. március 3., 20:38
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h>
//#include <c++/7/bits/std_function.h> /* POSIX terminal control definitions */

/*
 * Constans
 */

#define START ':'
#define CR '\r'
#define LF '\n'
#define MAXPACKETSIZE 256


// Function to convert hexadecimal to decimal 0..0xFFFF
int hexaToDec(char* hexVal) 
{ 
	int len = strlen(hexVal); 
	
	// Initializing base value to 1, i.e 16^0 
	int base = 1; 
	
	int dec_val = 0; 
	
	// Extracting characters as digits from last character 
	for (int i=len-1; i>=0; i--) 
	{ 
		// if character lies in '0'-'9', converting 
		// it to integral 0-9 by subtracting 48 from 
		// ASCII value. 
		if (hexVal[i]>='0' && hexVal[i]<='9') 
		{ 
			dec_val += (hexVal[i] - 48)*base; 
				
			// incrementing base by power 
			base = base * 16; 
		} 

		// if character lies in 'A'-'F' , converting 
		// it to integral 10 - 15 by subtracting 55 
		// from ASCII value 
		else if (hexVal[i]>='A' && hexVal[i]<='F') 
		{ 
			dec_val += (hexVal[i] - 55)*base; 
		
			// incrementing base by power 
			base = base*16; 
		} 
	} 
	
	return dec_val; 
} 

// function to convert decimal to hexadecimal 0..65535
 void decToHexa(unsigned int n, unsigned char * hexa) 
{    
    // char array to store hexadecimal number 
    char hexaDeciNum[4]; 
      
    // counter for hexadecimal number array 
    int i = 0; 
    while(n!=0) 
    {    
        // temporary variable to store remainder 
        int temp  = 0; 
          
        // storing remainder in temp variable. 
        temp = n % 16; 
          
        // check if temp < 10 
        if(temp < 10) 
        { 
            hexaDeciNum[i] = temp + 48; 
            i++; 
        } 
        else
        { 
            hexaDeciNum[i] = temp + 55; 
            i++; 
        } 
          
        n = n/16; 
    } 
      
    // printing hexadecimal number array in reverse order 
    for(int j=i-1; j>=0; j--) 
        *hexa++ = hexaDeciNum[j]; 
} 
 
unsigned char LRC(unsigned char* msg, unsigned int len){
    unsigned char lrc = 0;
    unsigned char dec = 0;
    unsigned char hex[2];
    while (len--) {
        hex[0] = *msg++;
        hex[1] = *msg++;
        dec = hexaToDec(hex);
        lrc += dec; 
    }
    lrc = (unsigned char)(-((char)lrc));
    return lrc;
}



/*
 * ASCII Protokol
 ****************
 * Start    ':'
 * Addres   Hi
 * Addres   Lo
 * Function Hi
 * Function Lo
 * Data(N -1)
 * Data(0)
 * LRC Hi
 * LRC Lo
 * End Hi 'Cr'
 * End Lo 'Lf'
 * **************
 */

unsigned char* create_packet(unsigned char* addres , unsigned char* funct , unsigned char* data){
    unsigned char packet[MAXPACKETSIZE];
    unsigned char lrc[3];
    unsigned char hosz, _lrc;
    size_t j = 0;
    memset(packet,'\0',0xFF * sizeof(char));
    packet[j++] = START;            // ':'
    //Add Addres
    packet[j++] = addres[0];
    packet[j++] = addres[1];
    //Add Function Code
    packet[j++] = funct[0];
    packet[j++] = funct[1];
    //Add Data
    strcat(packet, data);
    j = strlen(packet);
    //Add LRC
    _lrc = LRC(packet + 1, j - 1);
    decToHexa(_lrc, lrc);
    packet[j++] = lrc[0];
    packet[j++] = lrc[1];
    //Add end of packet
    packet[j++] = CR;
    packet[j++] = LF;
    puts(packet);
    return packet;
}

char packetFinder(unsigned char* packet){
    
    static unsigned char packetBuff[MAXPACKETSIZE];
    static unsigned char packetBuffpointer;
    
    strncat(packetBuff, packet, MAXPACKETSIZE - sizeof(packetBuff));
    
   
}

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */

int open_port(void)
{
  int fd; /* File descriptor for the port */
  struct termios options;
  fd = open("/dev/tnt0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
   /*
    * Could not open the port.
    */

    perror("open_port: Unable to open /dev/tty... - ");
  }
  else
    tcgetattr(fd, &options);
    cfsetispeed(&options, B38400);
    cfsetospeed(&options, B38400);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag |= PARENB;
    options.c_cflag |= PARODD;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_iflag |= (INPCK | ISTRIP);
    tcsetattr(fd, TCSANOW, &options);

    fcntl(fd, F_SETFL, FNDELAY);
    //fcntl(fd, F_SETFL, 0);
  return (fd);
}




int main(int argc, char** argv) {
//    int fd = open_port();
//
//    //n = write(fd, "ATZ\r", 4);
//    //fcntl(fd, F_SETFL, 0);
//    //close(fd);
//    write(fd, "ATZ\r", 4);
//    char data_in[255];
//    while(read(fd, &data_in[0], sizeof(char))){
//       write(fd, "ATZ\r", 4);
//       if(data_in[0] != '\0'){
//            printf("%02x\n",data_in[0]);
//        }
//        data_in[0] = '\0'; //reset and wait for next char
//    }
    
 //   puts(hexaToDec("10"));
//    unsigned char hex[4];
//    int dec;
//    decToHexa(0xEFEA,hex);
//    dec = hexaToDec(hex);
//    printf("%d", dec);
//    puts(hex);
    create_packet("01", "03" , "00040002");
    return (EXIT_SUCCESS);
}

