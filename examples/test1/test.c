unsigned char at (0x4000) g_video;
unsigned char at (0xF03C) g_plot;

void ezRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void ezPrintChar(unsigned char off, unsigned char x, unsigned char y);
void ezPlot(unsigned char x, unsigned char y);
void ezPrint(unsigned char x, unsigned char y, const char* text);
void ezDialogRect(unsigned char pat, unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void ezPrintPattern(unsigned char pat, unsigned char x, unsigned char y);


void main(void)
{
   unsigned char i;
   unsigned char y;

   g_video = 42;

   ezRect(50,50,10,50);

   g_video = 42;
   *((&g_video)+1) = 43;



   for(i=0; i<176; i++)
      ezPlot(i,i);

   for(y=0;y<24;y++)
   {
      ezPrint(5,y,"abcdefghijklmn");
   }


   ezDialogRect(255, 0, 0, 32, 24);
   ezDialogRect(0, 10, 10, 10, 10);
   ezDialogRect(0, 2, 2, 24, 2);
   ezDialogRect(0, 1, 1, 4, 4);
   ezDialogRect(0, 21, 17, 8, 4);
   ezDialogRect(255, 12, 12, 4, 4);
   ezPrint(12, 16, "thaan");

   ezPrint(3,2,"flappa clappa");

   ezPrint(21,17,"game");
   ezPrint(21,18,"over");

}

void ezPrint(unsigned char x, unsigned char y, const char* text)
{
   while(*text)
   {
      ezPrintChar(*(text++), x++, y);
   }
}

void ezDialogRect(unsigned char pat, unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
   unsigned char i, j;

   for(j=x; j<x+w; j++)
   {
      for(i=y; i<y+h; i++)
      {
         ezPrintPattern(pat, j,i);
      }
   }
}


void ezPrintChar(unsigned char ch, unsigned char x, unsigned char y)
{
   ch;
   x;
   y;

   _asm

   jp vlaa

mult:
   ld hl,#0
   ld a,b
   or a
   ret z
   ld d,#0
   ld e,c
clappa:
   add hl,de
   djnz clappa
   ret

vlaa:
   ld b, #8
   ld c, 4(ix)//char
   call mult
   ld b, h
   ld c, l

   ld hl, #0x3c00
   add hl,bc


   ld d,h
   ld e,l

// Work out first byte 40, 48, 50
   ld a, 6(ix)
   and #24
   ld b, #64 //Never changes form #64
   add a,b
   ld h,a
   // **** first byte in h ***

// Third nybble
   ld a, 6(ix)
   and #7
   add a,a
   ld b,a
   ld a, 5(ix)
   //and #16
   rrc a
   rrc a
   rrc a
   rrc a
   add a,b
   rlc a
   rlc a
   rlc a
   rlc a
   ld b,a
   // answer in b

// Fourth nybble
   ld a,5(ix)
   and #15
   // answer in a
   or b
   // second byte in a
   ld l,a
   // *** second byte in l ***

   ld b, #8
loop:
   ld a, (de)
   ld (hl),a
   inc h

   inc de
   djnz loop

   _endasm;

}


void ezPrintPattern(unsigned char pat, unsigned char x, unsigned char y)
{
   pat;
   x;
   y;

   _asm

// Work out first byte 40, 48, 50
   ld a, 6(ix)
   and #24
   ld b, #64 //Never changes form #64
   add a,b
   ld h,a
   // **** first byte in h ***

// Third nybble
   ld a, 6(ix)
   and #7
   add a,a
   ld b,a
   ld a, 5(ix)
   //and #16
   rrc a
   rrc a
   rrc a
   rrc a
   add a,b
   rlc a
   rlc a
   rlc a
   rlc a
   ld b,a
   // answer in b

// Fourth nybble
   ld a,5(ix)
   and #15
   // answer in a
   or b
   // second byte in a
   ld l,a
   // *** second byte in l ***

   ld b, #8
   ld a, 4(ix) // Print pattern
p_loop:
   ld (hl),a
   inc h

   djnz p_loop

   _endasm;

}

// ************************************************
// * Draw filled black rectangle                  *
// *                                              *
// * x: top-left  pixel coord - bounds 0 to 255   *
// * y: top-right pixel coord - bounds 0 tp 191   *
// * w: width in pixels       - bounds 255 - x    *
// * h: height in pixels      - bounds 191 - y    *
// *                                              *
// ************************************************
void ezRect(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
   unsigned char i, j;

   for(j=x; j<x+w; j++)
   {
      for(i=y; i<y+h; i++)
      {
         ezPlot(j,i);
      }
   }
}

// ************************************************
// * Plot black pixel                             *
// *                                              *
// * x: pixel coord - bounds 0 to 255             *
// * y: pixel coord - bounds 0 tp 191             *
// *                                              *
// ************************************************
void ezPlot(unsigned char x, unsigned char y)
{
   x;
   y;
   _asm
      ld hl,(#23563)
      ld bc, #4
      add   hl,bc
      ld d,4(ix)
      ld c, #8
      add   hl,bc
      ld e,5(ix)
      ld a,#0xaf
      sub   e
      ret   c
      ld e,a
      and a
      rra
      scf
      rra
      and a
      rra
      xor   e
      and #0xf8
      xor e
      ld h,a
      ld a,d
      rlca
      rlca
      rlca
      xor e
      and #0xc7
      xor e
      rlca
      rlca
      ld l,a
      ld a,d
      and #7
      ld b,a
      inc   b
      ld a,#0xfe
loopidge:   rrca
      djnz  loopidge
      ld b,#0xff
      xor b
      ld b,a
      ld a,(hl)
      or b
      ld (hl),a
      //ret
   _endasm;
}
