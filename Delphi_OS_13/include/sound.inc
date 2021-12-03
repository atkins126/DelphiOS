procedure Sound(Hz: LongWord);
var
  Divisor: LongWord;
  Temp: Byte;
begin
  Divisor:=1193180 div Hz;
  WritePortB($43,$B6);
  WritePortB($42,Divisor);
  WritePortB($42,Divisor shr 8);
  Temp:=ReadPortB($61);
  if Temp<>(Temp or 3) then WritePortB($61,Temp or 3);
end;

procedure NoSound;
var
  Temp: Byte;
begin
  Temp:=ReadPortB($61) and $FC;
  WritePortB($61,Temp);
end;
 
// Make a beep
procedure beep();
var
 I:Integer;
begin
	for i := 0 to 10000-1 do
  begin
		Sound(10000);
	// timer_wait(100);
 	nosound();
	// set_PIT_2(old_frequency);
 end;
end;
