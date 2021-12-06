const
       VGA_SEQUENCER	=	$3C4;  // Sequencer base addr
      VGA_CRTC	=	$3D4;
      VGA_GRAPHICS 	=	$3CE;   // Graphics Controller base addr




    VGA_AC_INDEX	=	$3C0;
  	VGA_AC_WRITE	=	$3C0;
  	VGA_AC_READ	 =	$3C1;
  	VGA_MISC_WRITE	=	$3C2;
    VGA_SEQ_INDEX	=	$3C4;
    VGA_SEQ_DATA	 =	$3C5;
  	VGA_DAC_READ_INDEX =	$3C7;
  	VGA_DAC_WRITE_INDEX	= $3C8;
  	VGA_DAC_DATA	=	$3C9;
  	VGA_MISC_READ	 =	$3CC;
    VGA_GC_INDEX 	=	$3CE;
    VGA_GC_DATA 	 =	$3CF;
   //*			COLOR emulation		MONO emulation */
    VGA_CRTC_INDEX	=	$3D4;
    VGA_CRTC_DATA	=	$3D5;
  	VGA_INSTAT_READ	=	$3DA;

  	VGA_NUM_SEQ_REGS =	5;
  	VGA_NUM_CRTC_REGS	= 25;
  	VGA_NUM_GC_REGS	 =	9;
  	VGA_NUM_AC_REGS	 =	21;
  	VGA_NUM_REGS	 =	(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS);


    VGA_SET_RESET = 0;
    VGA_ENABLE_SET_RESET = 1 ;
    VGA_COLOR_COMPARE = 2 ;
    VGA_DATA_ROTATE = 3 ;
    VGA_READ_MAP_SELECT = 4 ;
    VGA_MODE = 5 ;
    VGA_MISC = 6 ;
    VGA_COLOR_DONT_CARE = 7  ;
    VGA_BIT_MASK = 8 ;
    VGA_MAP_MASK = 2 ;

     MODE320X200X256=(#$63+
                      #$03#$01#$0F#$00#$0E+
                      #$5F#$4F#$50#$82#$54#$80#$BF#$1F+
	                    #$00#$41#$00#$00#$00#$00#$00#$00+
	                    #$9C#$0E#$8F#$28#$40#$96#$B9#$A3#$FF+
                      #$00#$00#$00#$00#$00#$40#$05#$0F#$FF+
                      #$00#$01#$02#$03#$04#$05#$06#$07+
	                    #$08#$09#$0A#$0B#$0C#$0D#$0E#$0F+
	                    #$41#$00#$0F#$00#$00);  

      

     SCREEN_WIDTH =320;
     SCREEN_HEIGHT =200;

     SCREEN= PAnsiChar($A0000);


procedure SetVideoMode(regs:PAnsiChar); stdcall;
var
 I,Index:Integer;
 r3:byte;
begin
  Index:=0;


  //* write MISCELLANEOUS reg */
  WritePortB(VGA_MISC_WRITE, ord(regs[Index]));
  Index:=Index+1;

  //* write SEQUENCER regs */
  for i := 0 to  VGA_NUM_SEQ_REGS-1 do
  begin
    WritePortB(  VGA_SEQUENCER, i);
    WritePortB(VGA_SEQ_DATA,  ord(regs[Index]));
    Index:=Index+1;
  end;

 //* unlock CRTC registers */
   WritePortB(  VGA_CRTC, $03);
   WritePortB(VGA_CRTC_DATA, ReadPortB(VGA_CRTC_DATA) or $80);
   WritePortB(  VGA_CRTC, $11);
   WritePortB(VGA_CRTC_DATA, ReadPortB(VGA_CRTC_DATA) and (not $80));


   //* write CRTC regs */
   for i := 0 to  VGA_NUM_CRTC_REGS-1 do
   begin
    WritePortB(  VGA_CRTC, i);
    WritePortB(VGA_CRTC_DATA,  ord(regs[Index]));
    Index:=Index+1;
   end;

   //* write GRAPHICS CONTROLLER regs */
    for i := 0 to VGA_NUM_GC_REGS-1 do
    begin
      WritePortB( VGA_GRAPHICS, i);
      WritePortB(VGA_GC_DATA,  ord(regs[Index]));
      Index:=Index+1;
   end;

    /// write ATTRIBUTE CONTROLLER regs */
   for i := 0 to VGA_NUM_AC_REGS-1 do
    begin
      ReadPortB(VGA_INSTAT_READ);
      WritePortB(VGA_AC_INDEX, i);
      WritePortB(VGA_AC_WRITE,  ord(regs[Index]));
      Index:=Index+1;
   end;

   /// lock 16-color palette and unblank display */
     ReadPortB(VGA_INSTAT_READ);
     WritePortB(VGA_AC_INDEX, $20);
end;

procedure SetColor(color, red,  green, blue:byte);
begin
  WritePortB($3C8,color);
	WritePortB($3C9, red);
	WritePortB($3C9, green);
	WritePortB($3C9, blue);
end;

procedure SetPalette();
var
  c:Integer;
  Index:Integer;
begin
    Index:=1;
    WritePortB($03C6,$ff);
    WritePortB($3c8,0);
    for c:=0 to 82-1 do
    begin
       SetColor(c,byte(colors[Index]),byte(colors[Index+1]),byte(colors[Index+2]));
       Index:=Index+3;
    end;
end;

function ReadPixel( x,  y:Integer):byte ;
begin
   Result:= byte(Screen[(y * SCREEN_WIDTH) + x]);
end;

procedure PutPixel(x,y:Integer;c:byte);
begin
   Screen[SCREEN_WIDTH * y + x] := char(c);
end;

procedure Cls(c:byte=0);
var
 x,y:Integer;
begin
   for  y := 0 to SCREEN_height-1 do
   begin
     for x := 0 to  SCREEN_width-1 do
     begin
        PutPixel(x,y,c);
     end;
   end;
end;

procedure PutChar( x,  y:Integer;Index:Integer; c1:byte);
var
 i,l,j,h,c:Integer;
 b:byte;
begin

  j := x;
  h := y;

 for l := 0 to  8-1 do
 begin
  for i := 7 downto  0 do
  begin

   b:= ord(font[index+l]) ;

   if ((b and (1 shl i)))<>0 then
    c := c1  // New color
   else
    c := ReadPixel(j, h);  // Don't overwrite background

    PutPixel(j, h, c);
    j:=j+1;
  end;
  h:=h+1;
  j := x;
 end;
end;

procedure WriteStr(X, Y: integer; text: PChar;Color:Byte);
var
  i: integer;
begin
  i:=0;
  repeat
    PutChar(X,Y,ord(text[i]) shl 3,Color);
    inc(x,8);
    i:=i + 1;
  until text[i] = #0
end;


procedure WriteInt(x,y,i: Integer;c:byte);
var
        buffer: array [0..11] of Char;
        str: PChar;
        digit: cardinal;
        minus: Boolean;
begin
        str := @buffer[11];
        str^ := #0;
        if (i < 0) then begin
                digit := -i;
                minus := True;
        end else begin
                digit := i;
                minus := False;
        end;
        repeat
                Dec(str);
                str^ := Char((digit mod 10) + Byte('0'));
                digit := digit div 10;
        until (digit = 0);
        if (minus) then begin
                Dec(str);
                str^ := '-';
        end;

       WriteStr(x,y,str,4);
end;


procedure vesa_init();
begin

  SetVideoMode(MODE320X200X256);
  SetPalette();
 // SetColor(16,$aa,$aa,$aa);
 // SetColor(17,$aa,$00,$00);

 //// SetColor(15,255,255,255);
 // SetColor(4,255,0,0);
  Cls(1);
end;
