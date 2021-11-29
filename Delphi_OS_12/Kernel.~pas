unit Kernel;

interface


//------------- Kernel Code  Begin --------------------------

procedure Main(); stdcall; forward;
procedure  loader; stdcall; forward;
procedure loader_end();  stdcall; forward;
procedure FillRectangle(x, y, w, h:Integer;Color:Byte);

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




{$include Crt.inc}
{$include Fonts.inc}
{$include Vesa.inc}
{$include Applications.inc}



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



{$I idt.inc}

{$I mouse.inc}
{$I keyboard.inc}





procedure Sleep(milliseconds: Cardinal);
var
i:Integer;
begin
   i := 0;
   while (milliseconds*100000 > i)do
   begin
	i:=i+1;
   end;
end;



function StrLen(Str: PChar): cardinal;
asm
 push esi
 push edi
 push ecx
 push eax
 mov edi, eax
 xor eax, eax
 xor ecx, ecx
 not ecx
 repne scasb
 pop eax
 sub edi, eax
 mov eax, edi
 dec eax
 pop ecx
 pop edi
 pop esi
end;



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



procedure Main(); stdcall;
var
 Application:TApplications;
begin
  vesa_init();
  idt_init();
  init_keyboard();
  init_mouse();

  Progress;
  Cls(0);

  Application:=CreateApplication('Editor',15);

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
 