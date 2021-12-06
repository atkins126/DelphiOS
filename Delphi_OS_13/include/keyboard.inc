type
  TKeyEventArray = Array[0..255] of TKeyEvent;

var
  KeyEventArray:TKeyEventArray;

procedure RegisterKeyEvent(Callback: TKeyEvent);
var
 I:Integer;
begin
  for I:=0 to 255-1 do
  begin
    if not (Assigned(KeyEventArray[I])) then
    begin
       KeyEventArray[I]:=Callback;
       break;
    end;
  end;

 // FillRectangle(49,9,10,10,0);
 // WriteInt(50,10,I,4);
end;

procedure UnRegisterKeyEvent(Callback: TKeyEvent);
var
  I:Integer;
begin

  for I:=0 to 255-1 do
  begin
    if (@KeyEventArray[I]=@Callback) then
    begin
       KeyEventArray[I]:=nil;
       break;
    end;
  end;
 // FillRectangle(49,19,10,10,0);
 // WriteInt(50,20,I,4);
end;



procedure keyboard_callback();
var
	b: pchar;
  key:byte;

 I:Integer;
begin

   if (ReadPortB($64) and $1)<>0 then
   begin

   key:= ReadPortB($60);
    if key in [0..86] then
    begin

     for I:=0 to 255-1 do
     begin
      if (Assigned(KeyEventArray[I])) then
       begin
       KeyEventArray[I](key);
      end;
    end;
       
  end;
 end;

end;



procedure init_keyboard();
begin
   RegisterISR(33, keyboard_callback);
end;