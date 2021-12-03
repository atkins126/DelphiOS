var
  IconIndex:Integer=0;



procedure TApplications.IconMouseDown(X,Y:Integer);
begin
 DrawForm();
 
end;




procedure TApplications.Create(X,Y,W,H:Integer;Text:PChar;C:Integer);
begin
  Name:= Text;
  Left:=X;
  Top:=Y;
  Width:=W;
  Height:=H;
  Color:=C;
  DrawIcon();
  
  Activite:=False;
end;

procedure TApplications.CloseBtnDown(X,Y:Integer);
begin
   FillRectangle(Left,Top,Width,Height,0);
   Activite:=False;
   UnRegisterKeyEvent(FormKeyDown);
   UnRegisterMouseRect(CloseBtnDown);
   KEY_X:=Left+8;
   KEY_Y:=Top+20;
end;


procedure TApplications.DrawForm();
begin
  if Activite then Exit;
  
   FillRectangle(Left,Top,Width,Height,Color); // Form
   FillRectangle(Left,Top, Width, 20,7); // Header
   FillRectangle(Left,Top+Height-8, Width, 8,7); // FOOTER
   WriteStr(Left+2, Top+2,Name,15);

   FillRectangle(Left+Width-16,Top,16,20,4); // CloseBtn
   WriteStr(Left+Width-16+2, Top+2,'X',0);
   RegisterMouseRect(Left+Width-16,Top,16,16,CloseBtnDown);
   RegisterKeyEvent( FormKeyDown);
   KEY_X:=Left+8;
   KEY_Y:=Top+20;
   Activite:=True;
end;


procedure TApplications.FormKeyDown(Key:Byte);
begin

  if not Activite then Exit;

 case key of
  28:begin
    KEY_X:=Left+10;
    Inc(KEY_Y,8);
    end;
  57:begin
     Inc(KEY_X,8);
    end;
  14:begin
     if KEY_X <(Left+10) then
     begin
       KEY_X:=(Left+(Width-10));
       Dec(KEY_Y,8);
     end;
     Dec(KEY_X,8);
     FillRectangle(KEY_X,KEY_Y,9,9,Color);
    end;
  else
  begin
   if KEY_X >(Left+Width)-10 then
   begin
     KEY_X:=Left+10;
     Inc(KEY_Y,8);
   end;

   if KEY_Y >(Top+Height)-10 then
   begin
     KEY_X:=Left+10;
     KEY_Y:= Top+20;
     FillRectangle(Left,Top+20,Width,Height-20,Color);
     FillRectangle(Left,Top+Height-8, Width, 8,7); // FOOTER
   end;

   PutChar(KEY_X,KEY_Y,ord(keyboard_map[key]) shl 3,0);
   Inc(KEY_X,8);
   end;
  end;
end;




procedure TApplications.DrawIcon();
var
 Buff:array[0..256-1] of byte;
 I:Integer;
 X,Y,W,H,addr:integer;
 C:byte;
begin
   X:=5;
   Y:= IconIndex;

   FillChar(Buff,256,#2);
   Buff[0]:=0;
   Buff[1]:=0;
   Buff[16]:=0;


   Buff[14]:=0;
   Buff[15]:=0;
   Buff[31]:=0;


   Buff[224]:=0;
   Buff[240]:=0;
   Buff[241]:=0;


   Buff[254]:=0;
   Buff[255]:=0;
   Buff[239]:=0;


    for addr := 0 to  256-1 do
  begin
    w:=addr mod 16;
    h:=addr div 16 mod 16;

   if ord(Buff[addr]) =1 then
   begin
      c := 15  // New color
   end
   else
   if ord(Buff[addr]) =2 then
   begin
      c := 4  // New color
   end
   else
    c := ReadPixel(x+w,y+h);

    PutPixel(X+w,Y+h, c);
  end;

  
  RegisterMouseRect(X,Y,16,16,IconMouseDown);

  PutChar(X+4,Y+4,ORD(Name[0]) shl 3,1);
  Inc(IconIndex,20);

end;



