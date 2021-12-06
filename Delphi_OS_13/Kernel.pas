unit Kernel;

interface

CONST
     SEQU_ADDR = $3c4;
     CRTC_ADDR = $3d4;
     GRAC_ADDR = $3ce;





//------------- Kernel Code  Begin --------------------------

procedure Main(); stdcall; forward;
procedure  loader; stdcall; forward;
procedure loader_end();  stdcall; forward;


implementation

procedure  loader; stdcall;
asm
   cli
   call   main
   hlt
end;





procedure WritePortB(port:word; val:byte);
begin
  asm
    PUSH EAX
    PUSH EDX
    MOV DX, port
    MOV AL, val
    OUT DX, AL
    POP EDX
    POP EAX
  end;
end;


function ReadPortB(port:word):byte;
var
  inb:byte;
begin
  asm
    PUSH EAX
    PUSH EDX
    MOV DX, port
    IN AL, DX
    MOV inb, AL
    POP EDX
    POP EAX
  end;
  Result:=inb;
end;

{

procedure WritePortW(port:word; val:Word);
begin
  asm
    PUSH EAX
    PUSH EDX
    MOV DX, port
    MOV AX, val
    OUT DX, AX
    POP EDX
    POP EAX
  end;
end;


 }

 {$include include/functions.inc}

 {$I Include/Systems.inc}
 {$include include/Applications.inc}
{$include include/Crt.inc}
{$include include/Fonts.inc}
{$include include/Vesa.inc}

{$include include/sound.inc}


procedure FillRectangle(x, y, w, h:Integer;Color:Byte);
var
 a,b:Integer;
begin
    a := y ;
    while a < (y + h)-1do
    begin
      b := x;
      while b < (x + w)-1do
      begin
        PutPixel(b, a, Color);
        inc(b);
      end;
    inc(a);
   end;

  
end;



{$I include/idt.inc}

{$I include/mouse.inc}
{$I include/keyboard.inc}










procedure Progress;
var
  j:Integer;
  msg:PChar;
  len:Integer;
begin

   FillRectangle((SCREEN_WIDTH div 2)-50, SCREEN_HEIGHT - 50, 100, 6, 15); // Green
   msg:='System Install...';
   len:=StrLen(msg);
    For j:=0 to 100 do  begin
       if j mod 30=0 then
          WriteStr((SCREEN_WIDTH div 2) - ((len*8) div 2) ,SCREEN_HEIGHT div 2,msg,4)
       else
          WriteStr((SCREEN_WIDTH div 2) - ((len*8) div 2) ,SCREEN_HEIGHT div 2,msg,15);


      FillRectangle((SCREEN_WIDTH div 2)-50, SCREEN_HEIGHT - 50, j, 6, 4); // Green
      Sleep(2);
    end;
   Cls(0);
   msg:='HOSGELDINIZ';
   len:=StrLen(msg);
   WriteStr((SCREEN_WIDTH div 2) - ((len*8) div 2) ,SCREEN_HEIGHT div 2,msg,15) ;
   Sleep(200);
   Cls(0);
end;



procedure ToolBar();
begin
   FillRectangle(0, 0, 25, SCREEN_HEIGHT, 7); // Green
end;


procedure TClosePc.CloseBtnDown(X,Y:Integer);
begin
   asm
    mov al,0FEh
    out 64h,al
   end;
end;

procedure TClosePc.DrawCloseBtn();
begin
  FillRectangle(0, SCREEN_HEIGHT-16, 25, 25,4);
  WriteStr(4+2, SCREEN_HEIGHT-13,'X',15);
  RegisterMouseRect(0, SCREEN_HEIGHT-16, 25, 25,CloseBtnDown);
end;



procedure Main(); stdcall;
var
  Applications:TApplications;

  Editors:TApplications;
  Graphic:TApplications;
  ClosePc:TClosePc;
begin
  Vesa_init();
  idt_init();
  init_keyboard();
  init_mouse();


  beep();

  Progress;
  
  ToolBar();
  ClosePc.DrawCloseBtn();


   Editors.Create(50,50,200,100,'Editors',15);
   Graphic.Create(50,50,200,100,'Graphic',15);


  asm
   sti
  end;

 while true do
 begin
 end;

end;

procedure loader_end(); stdcall;
begin

end;


end.
 