const
 
  keyboard_map = ('01234567890-=*-QWERTYUIOP[]00ASDFGHJKL;\,00ZXCVBNM00');


type

 

  TClick = procedure();
  TMouseMove = procedure(Index:Integer; X, Y: Integer);
  TMouseDown1 = procedure(Index:Integer; X, Y: Integer);
  TMouseDown = procedure(X, Y: Integer);
  TKeyDown = procedure(key: integer);

  TButton=Record
    Left:Integer;
    Top :Integer;
    Width:Integer;
    Height:Integer;
    Color:Integer;
    Text:PChar;
    OnClick:TClick;
    OnMouseMove :TMouseMove;
    OnMouseDown :TMouseDown;
  end;




  TForm= packed Record
   Active:Boolean;
   ActiveControl:Boolean;
   Left:Integer;
   Top :Integer;
   Width:Integer;
   Height:Integer;
   Color:Integer;
   Text:PChar;
   KEY_X:Integer;
   KEY_Y:Integer;
   HeaderColor:Integer;
   OnClick:TClick;
   CloseBtn:TButton;
   OnKeyDown:TKeyDown;
 end;


  TIcon=Record
   Active:Boolean;
   Left:Integer;
   Top :Integer;
   Width:Integer;
   Height:Integer;
   Color:Integer;
   Text:PChar;
   OnClick:TClick;
   OnMouseMove :TMouseMove;
   OnMouseDown :TMouseDown;
  end;


  TApplications=Record
    Name:PChar;
    Icon:TIcon;
    Form:TForm;
  end;

  const
  Editor_Icon = (
    #0#0#0#1#1#1#1#1#1#1#1#1#1#0#0#0+
    #0#0#1#1#1#1#1#1#1#1#1#1#1#1#0#0+
    #0#1#1#2#2#2#2#2#2#2#2#2#2#1#1#0+
    #1#1#2#2#2#1#1#1#1#1#1#2#2#2#1#1+
    #1#1#2#2#2#1#1#1#1#1#1#2#2#2#1#1+
    #1#1#2#2#2#1#2#2#2#2#2#2#2#2#1#1+
    #1#1#2#2#2#1#2#2#2#2#2#2#2#2#1#1+
    #1#1#2#2#2#1#1#1#1#1#2#2#2#2#1#1+
    #1#1#2#2#2#1#1#1#1#1#2#2#2#2#1#1+
    #1#1#2#2#2#1#2#2#2#2#2#2#2#2#1#1+
    #1#1#2#2#2#1#2#2#2#2#2#2#2#2#1#1+
    #1#1#2#2#2#1#1#1#1#1#1#2#2#2#1#1+
    #1#1#2#2#2#1#1#1#1#1#1#2#2#2#1#1+
    #0#1#1#2#2#2#2#2#2#2#2#2#2#1#1#0+
    #0#0#1#1#1#1#1#1#1#1#1#1#1#1#0#0+
    #0#0#0#1#1#1#1#1#1#1#1#1#1#0#0#0);


var


  Application:TApplications;
  IconIndex :Integer=20;
  ActiveForm:Integer=0;


procedure Icon_Draw(x,y:integer);
var
 w,h,addr:integer;
 c:byte;
begin

  for addr := 0 to  256-1 do
  begin
    w:=addr mod 16;
    h:=addr div 16 mod 16;

   if ord(Editor_Icon[addr+1]) =1 then
   begin
      c := 15  // New color
   end
   else
   if ord(Editor_Icon[addr+1]) =2 then
   begin
      c := 4  // New color
   end
   else
    c := ReadPixel(x+w,y+h);

    PutPixel(x+w,y+h, c);
  end;
end;








procedure BtnCloseMouseDown( X, Y: Integer);
begin
   FillRectangle(Application.Form.Left,Application.Form.Top, Application.Form.Width, Application.Form.Height,0);
   Application.Form.Active:=False;
end;




procedure CreateButton(var Result :TButton;Left:Integer;Top :Integer;Width:Integer;Height:Integer;Color:Byte;Text:PChar);
begin
   Result.Left:=Left;
   Result.Top:=Top;
   Result.Width:= Width;
   Result.Height:= Height;
   Result.Color:=Color;
   Result.Text:=Text;
   FillRectangle(Left,Top, Width, Height,Color); // CloseBtn
   WriteStr((Left+Width)-18, Top+2,Text,15);
End;

Procedure DrawForm(var Form:TForm);
var
 X,Y:Integer;
off:Integer;
 PIXBYTES    :integer;
begin


   PIXBYTES    := SCREEN_WIDTH SHR 3;
  // FillChar(Form.BackPixels,Sizeof(Form.BackPixels),#0);

   for X:=Form.Left TO (Form.Left+Form.Width-1) DO
   Begin
     for Y:=Form.Top TO (Form.Top+Form.Height-1) DO
     Begin

        off := y*PIXBYTES + (x shr 3) ;

           // if ReadPixel(x,y)=0 then continue;
         //  Form.BackPixels[off]:=ReadPixel(x,y);

     end;
   end;



   FillRectangle(Form.Left,Form.Top,Form.Width,Form.Height,Form.Color); // Form
   FillRectangle(Form.Left,Form.Top, Form.Width, 20,Form.HeaderColor); // Header
   WriteStr(Form.Left+2, Form.Top+2,Form.Text,15);
   CreateButton(Form.CloseBtn,(Form.Left+Form.Width)-20, Form.Top, 20, 20,4,'X');
   Form.CloseBtn.OnMouseDown:=BtnCloseMouseDown;
end;

Procedure DrawIcon(var Icon:TIcon);
begin
   FillRectangle(Icon.Left,Icon.Top,Icon.Width,Icon.Height,Icon.Color); // Form
   WriteStr(Icon.Left+2, Icon.Top+2,Icon.Text,4);

end;


procedure KeyDown(key: integer);
begin
  case key of
  28:begin
    Application.Form.KEY_X:=Application.Form.Left+10;
    Inc(Application.Form.KEY_Y,8);
    end;
  57:begin
     Inc(Application.Form.KEY_X,8);
    end;
  14:begin
     Dec(Application.Form.KEY_X,8);
     FillRectangle(Application.Form.KEY_X,Application.Form.KEY_Y,9,9,Application.Form.Color);
    end;
  else
  begin
   if Application.Form.KEY_X >(Application.Form.Left+Application.Form.Width)-10 then
   begin
     Application.Form.KEY_X:=Application.Form.Left+10;
     Inc(Application.Form.KEY_Y,8);
   end;

   if Application.Form.KEY_Y >(Application.Form.Top+Application.Form.Height)-10 then
   begin
     Application.Form.KEY_X:=Application.Form.Left+10;
     Application.Form.KEY_Y:= Application.Form.Top+20;
     FillRectangle(Application.Form.Left,Application.Form.Top+20,Application.Form.Width,Application.Form.Height-20,Application.Form.Color);
   end;

   PutChar(Application.Form.KEY_X,Application.Form.KEY_Y,ord(keyboard_map[key]) shl 3,4);
   Inc(Application.Form.KEY_X,8);
   end;
  end;
end;



procedure CreateForm(var Result:TForm;Left:Integer;Top :Integer;Width:Integer;Height:Integer;Text:PChar);
begin
  if not Result.Active then
   begin
      Result.Text:=Text;
      Result.Left:=Left;
      Result.Top:=Top;
      Result.KEY_X:=Left+10;
      Result.KEY_Y:=Top+20;
      Result.Width:=Width;
      Result.Height:=Height;
      Result.HeaderColor:=8;
      Result.OnKeyDown:=KeyDown;
      Result.Active:=true;
      DrawForm(Result);

  end;
end;

procedure CreateIcon(var Result:TIcon; Left:Integer;Top :Integer;Width:Integer;Height:Integer;Color:Byte;Text:PChar;MouseDown:TMouseDown);
begin
   if  Not Result.Active then
   begin
      Result.Text:=Text;
      Result.Left:=IconIndex+Left;
      Result.Top:=Top;
      Result.Width:=Width;
      Result.Height:=Height;
      Result.Color:=Color;
      Icon_Draw(Result.Left,Result.Top);
      //DrawIcon(Result);
      Result.OnMouseDown:=MouseDown;
      Result.Active:=true;
      Inc(IconIndex,75);
    end;
end;


procedure ApplicationIconOnClick( X, Y: Integer);
var
 dx,dy:Integer;
begin
   if not Application.Form.Active then
   begin
     dx:=(SCREEN_WIDTH div 2) -(100);
     dy:=(SCREEN_HEIGHT div 2)-(50) ;
     CreateForm(Application.Form,dx,dy,200,100,Application.Name);
   end;
end;


Function CreateApplication(Text:PChar;Color:Byte):TApplications;
begin
       Application.Name:=Text;
       Application.Form.Color:=Color;
       CreateIcon(Application.Icon,20, 20, 50, 30,15,Text,ApplicationIconOnClick);
       Result:=Application;

  
end;




