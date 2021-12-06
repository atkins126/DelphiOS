
type
  TMouseRectArray = Array[0..255] of TMouseRect;

const
   mouse_pointer = (
    #1#0#0#0#0#0#0#0+
    #1#1#0#0#0#0#0#0+
    #1#0#1#0#0#0#0#0+
    #1#0#0#1#0#0#0#0+
    #1#0#0#0#1#0#0#0+
    #1#0#1#1#1#1#0#0+
    #1#1#0#0#0#0#0#0+
    #1#0#0#0#0#0#0#0);

var
   Mouse_Cycle : Integer;
   Mouse_Byte : Array[0..2] of byte;
   Mouse_X: Integer;
   Mouse_Y: Integer;
   Mouse_x_movement : integer;
   Mouse_y_movement : integer;
   Mouse_y_overflow : boolean;
   Mouse_x_overflow : boolean;
   Mouse_y_sign     : boolean;
   Mouse_x_sign     : boolean;
   Mouse_MMB_Down   : Boolean;
   Mouse_RMB_Down   : Boolean;
   Mouse_LMB_Down   : Boolean;

   BackPixels:array[0..64-1] of byte;


   MouseRectArray : TMouseRectArray;

procedure RegisterMouseRect(X,Y,W,H:Integer;Callback: TMouseRectEvent);
var
 I:Integer;
begin
  for I:=0 to 255-1 do
  begin
    if not (Assigned(MouseRectArray[I].Callback)) then
    begin

         MouseRectArray[I].X:=X;
         MouseRectArray[I].Y:=Y;
         MouseRectArray[I].W:=W;
         MouseRectArray[I].H:=H;
         MouseRectArray[I].Callback:=Callback;
         break;
    end;
  end;

 {
  Inc(MouseArea_N);
  MouseArea[MouseArea_N].X:=X;
  MouseArea[MouseArea_N].Y:=Y;
  MouseArea[MouseArea_N].W:=W;
  MouseArea[MouseArea_N].H:=H;
  MouseArea[MouseArea_N].OnMouseDown:= callback;
 }
end;


procedure UnRegisterMouseRect(Callback: TMouseRectEvent);
var
 I:Integer;
begin
  for I:=0 to 255-1 do
  begin
    if @MouseRectArray[I].Callback = @Callback then
    begin
       MouseRectArray[I].Callback:=nil;
       break;
    end;
  end;
end;
  
  
procedure SetMouseClick(x,y:Integer);
var
 I:Integer;
begin

 {
 For I:=1 to   255-1 do
   begin
     if not (Assigned(MouseArea[I].OnMouseDown)and
        (x>MouseArea[I].x) and
        (y>MouseArea[I].y) and
        (x<MouseArea[I].x+MouseArea[I].w) and
        (y<MouseArea[I].y+MouseArea[I].h))
      then
      begin
        FillRectangle(MouseArea[I].x, MouseArea[I].y,MouseArea[I].w,MouseArea[I].h, 0);
        MouseArea[I].OnMouseDown:=nil;
        
     end;
   end;
 }

 for I:=0 to 255-1 do
 begin
    if (Assigned(MouseRectArray[I].Callback)) and
        (x>MouseRectArray[I].X) and
        (y>MouseRectArray[I].Y) and
        (x<MouseRectArray[I].X+MouseRectArray[I].W) and
        (y<MouseRectArray[I].Y+MouseRectArray[I].H)
      then
      begin
        MouseRectArray[I].Callback(x,y);
      end;
 end;
end;


function mouse_wait(w_type : byte) : boolean;
var
  timeout : LongWord;
begin
    timeout:= 100000;
    if (w_type = 0) then begin
        while (timeout > 0) do begin
            if ((ReadPortB($64) AND $01) = $01) then break;
            timeout:= timeout-1;
        end;
    end else begin
        while (timeout > 0) do begin
            if ((ReadPortB($64) AND 2) = 0) then break;
            timeout := timeout - 1;
        end;
    end;
    mouse_wait:= timeout > 0;
end;

procedure mouse_write(value : byte);
begin
    mouse_wait(1);
    WritePortB($64, $D4);
    mouse_wait(1);
    WritePortB($60, value);
end;

function mouse_read : byte;
begin
    mouse_wait(0);
    mouse_read:= ReadPortB($60);
end;

procedure Mouse_Draw(x,y:integer);
var
 w,h,addr:integer;
 c:byte;
begin

  for addr := 0 to  64-1 do
  begin
    w:=addr mod 8;
    h:=addr div 8 mod 8;

   if ord(mouse_pointer[addr+1]) =1 then
   begin
    BackPixels[addr]:= ReadPixel(x+w,y+h);
    c :=15 xor ReadPixel(x+w,y+h);//15  // New color
   end
   else
   if ord(mouse_pointer[addr+1]) =2 then
   begin
    BackPixels[addr]:= ReadPixel(x+w,y+h) ;
    c := 0 xor ReadPixel(x+w,y+h) // New color
   end
   else
    c :=  ReadPixel(x+w,y+h);

    PutPixel(x+w,y+h, c);
  end;
end;

procedure Mouse_Clear(x,y:integer);
var
 w,h,addr:integer;
 c:byte;
begin

  for addr := 0 to  64-1 do
  begin
    w:=addr mod 8;
    h:=addr div 8 mod 8;

   if ord(mouse_pointer[addr+1]) =1 then
   begin
    c :=BackPixels[addr];
    PutPixel(x+w,y+h, c);
   end
   else
   if ord(mouse_pointer[addr+1]) =2 then
   begin
    c :=BackPixels[addr];
    PutPixel(x+w,y+h, c);
   end

  end;
end;


procedure mouse_callback();
var
  b:byte;
  f : byte;
begin
   Mouse_Clear(Mouse_X, Mouse_Y);
        b:= mouse_read;
        if Mouse_Cycle = 0 then begin
            if (b AND $08) = $08 then begin
                Mouse_Byte[Mouse_Cycle]:= b;
                Inc(Mouse_Cycle);
            end;
        end else begin
            Mouse_Byte[Mouse_Cycle]:= b;
            Inc(Mouse_Cycle);
        end;
        if Mouse_Cycle = 3 then begin
            //Process
            f:= Mouse_Byte[0];
            Mouse_x_sign:= (f AND $10 = $10);
            Mouse_y_sign:= (f AND $20 = $20);
            Mouse_MMB_Down:= (f AND $4) = $4;
            Mouse_RMB_Down:= (f AND $2) = $4;
            Mouse_LMB_Down:= (f AND $1) = $1;

            Mouse_x_overflow:= (f AND $40) = $40;
            Mouse_y_overflow:= (f AND $80) = $80;
            Mouse_x_movement:= Mouse_Byte[1];
            Mouse_y_movement:= Mouse_Byte[2];
            If Mouse_x_sign then Mouse_x_movement:= smallint(Mouse_x_movement OR $FF00);
            If Mouse_y_sign then Mouse_y_movement:= smallint(Mouse_y_movement OR $FF00);
            if not(Mouse_x_overflow) and not(Mouse_y_overflow) then begin
                Mouse_X:= Mouse_X + Mouse_x_movement;
                Mouse_Y:= Mouse_Y - Mouse_y_movement;
                if Mouse_X < 0 then Mouse_X:= 0;
                if Mouse_Y < 0 then Mouse_Y:= 0;
                if Mouse_X > (SCREEN_WIDTH-8) then Mouse_X:= (SCREEN_WIDTH-8);
                if Mouse_Y > (SCREEN_HEIGHT-8) then Mouse_Y:= (SCREEN_HEIGHT-8);
            end;
            Mouse_Cycle:= 0;
            if Mouse_LMB_Down then begin

              SetMouseClick(Mouse_X,Mouse_Y);
            end;

            if Mouse_RMB_Down then begin

            end;

            if not Mouse_RMB_Down then begin

                end;

           {  if ((Mouse_X>5) and  (Mouse_X<50)) and
                  ((Mouse_Y>105) and (Mouse_Y<113))  then
               begin
                 PutString(5,105,'Kapat',7);

               end;
               }

           // PutString(Mouse_X, Mouse_Y,'+',4);
            Mouse_Draw(Mouse_X, Mouse_Y);

          {  FillRectangle(20,20,50,10,0);
            FillRectangle(20,30,50,10,0);

            WriteInt(20,20,Mouse_X,4);

           WriteInt(20,30,Mouse_Y,4); }

          end;
    
end;

procedure init_mouse();
var
 status:Byte;
begin
     mouse_x:=0;
     mouse_y:=0;
     mouse_cycle:=0;
     FillChar(BackPixels,64,#7);

    mouse_wait(1);
    WritePortB($64, $A8);
    mouse_wait(1);
    WritePortB($64, $20);
    mouse_wait(0);
    status:= ReadPortB($60) OR $02;
    mouse_wait(1);
    WritePortB($64, $60);
    mouse_wait(1);
    WritePortB($60, status);
    mouse_write($F6);
    mouse_read();
    mouse_write($F4);
    mouse_read();

    RegisterISR(44, mouse_callback);
    
end;





