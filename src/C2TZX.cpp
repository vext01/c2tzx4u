// C2TZX.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdio.h>
unsigned int initCodeHeader(unsigned char* header, unsigned int codeSize, unsigned int codeLocation)
{
   unsigned char checksum = 0;

   header[0] = 16;  // 0x10 ID
   header[1] = 232; // 1 second pause
   header[2] = 3;   // 1 second pause
   header[3] = 19;  // ??
   header[4] = 0;   // ??
   header[5] = 0;   // ??
   header[6] = 3;   // ??
   header[7] = 's';
   header[8] = 'd';
   header[9] = 'c';
   header[10] = 'c';
   header[11] = ' ';
   header[12] = 'g';
   header[13] = 'a';
   header[14] = 'y';
   header[15] = ' ';
   header[16] = ' ';
   header[17] = codeSize & 255; // Code size
   header[18] = codeSize >> 8;  // Code size
   header[19] = codeLocation & 255; // Data location
   header[20] = codeLocation >> 8;  // Data location
   header[21] = 0;
   header[22] = 128;

   for(int i = 6; i<=22; i++)
   {
      checksum ^= header[i];
   }

   header[23] = checksum; // Checksum

   return 24;
}

unsigned int initCodeDataSection(unsigned char* codeDataHeader, unsigned char* codeSection, unsigned int codeSize)
{
   unsigned char checksum = 0;

   codeDataHeader[0] = 16;  // 0x10 ID
   codeDataHeader[1] = 232; // 1 second pause
   codeDataHeader[2] = 3;   // 1 second pause
   unsigned int codeSizePlus2 = codeSize + 2; // +2 for 0xFF and checksum
   codeDataHeader[3] = codeSizePlus2 & 255; // Code size
   codeDataHeader[4] = codeSizePlus2 >> 8;  // Code size
   codeDataHeader[5] = 255; // ??

   unsigned int i;
   for(i=0; i<codeSize; i++)
   {
      codeDataHeader[6+i] = codeSection[i]; // Code
   }

   for(i=5; i<6+codeSize; i++)
   {
      checksum ^= codeDataHeader[i];
   }

   codeDataHeader[6+codeSize] = checksum; // Checksum

   return 7 + codeSize;
}

int main(int argc, char* argv[])
{
   if(argc < 4)
   {
      printf("********************************************\r\n");
      printf("*                                          *\r\n");
      printf("* Usage:                                   *\r\n");
      printf("* c2tzx header bin output                  *\r\n");
      printf("*                                          *\r\n");
      printf("* Example:                                 *\r\n");
      printf("* c2tzx header.bin my_code.bin my_prog.tzx *\r\n");
      printf("*                                          *\r\n");
      printf("********************************************\r\n");
      return 0;
   }
   FILE* header = fopen (argv[1], "rb" );
   FILE* code = fopen (argv[2], "rb" );
   FILE* tzx = fopen (argv[3], "w+b" );

   unsigned int codeLocation = 32768;
   unsigned int i;

   if(!header || !code || !tzx)
      return 0;

   unsigned char headerData[4000];
   unsigned int headerSize = (unsigned int)fread(headerData, 1, 4000, header);

   unsigned char codeData[10000];
   unsigned int codeSize = (unsigned int)fread(codeData, 1, 4000, code);

   unsigned char codeHeader[24];
   unsigned char codeDataHeader[10000];

   unsigned int codeHeaderLength = initCodeHeader(codeHeader, codeSize, codeLocation);
   unsigned int codeDataHeaderLength = initCodeDataSection(codeDataHeader, codeData, codeSize);

   unsigned char tzxData[20000];

   unsigned int off = 0;
   for(i=0; i<headerSize; i++)
   {
      tzxData[i] = headerData[i];
   }
   off += headerSize;
   for(i=0; i < codeHeaderLength; i++)
   {
      tzxData[off+i] = codeHeader[i];
   }
   off += codeHeaderLength;
   for(i=0; i < codeDataHeaderLength; i++)
   {
      tzxData[off+i] = codeDataHeader[i];
   }
   off += codeDataHeaderLength;

   fwrite(tzxData, 1, off, tzx);


   fclose(header);
   fclose(code);
   fclose(tzx);

   //delete codeDataHeader;

	return 0;
}
