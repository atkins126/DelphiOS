 const

  keyboard_map = ('01234567890-=*-QWERTYUIOP[]00ASDFGHJKL;\,00ZXCVBNM00');


 type
   TMouseRectEvent = procedure (X,Y:Integer) of object;
    TKeyEvent = procedure (Key:Byte) of object;

 type
  TMouseRect=Record
    X,Y,W,H:Integer;
    Callback: TMouseRectEvent;
  end;

 TClosePc=object
 public
   procedure DrawCloseBtn();
   procedure CloseBtnDown(X,Y:Integer);
 end;



 TApplications=object
  private
    Activite:Boolean;
    Name:PChar;
    Left:Integer;
    Top :Integer;
    Width:Integer;
    Height:Integer;
    Color:Integer;
    
    KEY_X:Integer;
    KEY_Y:Integer;

  public
    procedure CloseBtnDown(X,Y:Integer);
    procedure IconMouseDown(X,Y:Integer);
    procedure FormKeyDown(Key:Byte);
    procedure Create(X,Y,W,H:Integer;Text:PChar;C:Integer);
    procedure DrawIcon();
    procedure DrawForm();
 End;






  TClick = procedure(X, Y: Integer);
  TMouseMove = procedure(Index:Integer; X, Y: Integer);
  TMouseDown = procedure(X, Y: Integer);
  TKeyDown = procedure(key: integer);



   procedure RegisterKeyEvent(Callback: TKeyEvent);forward;
   procedure UnRegisterKeyEvent(Callback: TKeyEvent);forward;

   procedure RegisterMouseRect(X,Y,W,H:Integer;Callback: TMouseRectEvent); forward;
  procedure UnRegisterMouseRect(Callback: TMouseRectEvent); forward;
  procedure WriteStr(X, Y: integer; text: PChar;Color:Byte); forward;
  function ReadPixel( x,  y:Integer):byte ;forward;
  procedure PutPixel(x,y:Integer;c:byte);forward;
  procedure FillRectangle(x, y, w, h:Integer;Color:Byte); forward;


 procedure PutChar( x,  y:Integer;Index:Integer; c1:byte); forward;
