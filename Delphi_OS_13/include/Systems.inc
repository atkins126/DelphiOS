 {$ifndef Systems}
{$define Systems}
//------------------------------------------------------------------------------------------------------------//
//               Code By Ekrem KOCAK                                                                          //
//             ekremkocak@hotmail.com                                                                         //
//                  Kýrþehir  2006                                                                           //                                                                                                                                                                                                                   //
//                                                                                                            //
//----------------------------------------------------------------------------------------------------------- //

 const
  
  BUFFER_SIZE   = 200;
  

 


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

procedure Move( const Source; var Dest; count : Integer );
var
  S, D: PChar;
  I: Integer;
begin
  S := PChar(@Source);
  D := PChar(@Dest);
  if S = D then Exit;
  if Cardinal(D) > Cardinal(S) then
    for I := count-1 downto 0 do
      D[I] := S[I]
  else
    for I := 0 to count-1 do
      D[I] := S[I];
end;

function CompareText(const S1, S2: string): Integer; assembler;
asm
        PUSH    ESI
        PUSH    EDI
        PUSH    EBX
        MOV     ESI,EAX
        MOV     EDI,EDX
        OR      EAX,EAX
        JE      @@0
        MOV     EAX,[EAX-4]
@@0:    OR      EDX,EDX
        JE      @@1
        MOV     EDX,[EDX-4]
@@1:    MOV     ECX,EAX
        CMP     ECX,EDX
        JBE     @@2
        MOV     ECX,EDX
@@2:    CMP     ECX,ECX
@@3:    REPE    CMPSB
        JE      @@6
        MOV     BL,BYTE PTR [ESI-1]
        CMP     BL,'a'
        JB      @@4
        CMP     BL,'z'
        JA      @@4
        SUB     BL,20H
@@4:    MOV     BH,BYTE PTR [EDI-1]
        CMP     BH,'a'
        JB      @@5
        CMP     BH,'z'
        JA      @@5
        SUB     BH,20H
@@5:    CMP     BL,BH
        JE      @@3
        MOVZX   EAX,BL
        MOVZX   EDX,BH
@@6:    SUB     EAX,EDX
        POP     EBX
        POP     EDI
        POP     ESI
end;


function StrEnd(const Str: PChar): PChar; assembler;
asm
        MOV     EDX,EDI
        MOV     EDI,EAX
        MOV     ECX,0FFFFFFFFH
        XOR     AL,AL
        REPNE   SCASB
        LEA     EAX,[EDI-1]
        MOV     EDI,EDX
end;



function StrCopy(Dest: PChar; const Source: PChar): PChar;
asm
        PUSH    EDI
        PUSH    ESI
        MOV     ESI,EAX
        MOV     EDI,EDX
        MOV     ECX,0FFFFFFFFH
        XOR     AL,AL
        REPNE   SCASB
        NOT     ECX
        MOV     EDI,ESI
        MOV     ESI,EDX
        MOV     EDX,ECX
        MOV     EAX,EDI
        SHR     ECX,2
        REP     MOVSD
        MOV     ECX,EDX
        AND     ECX,3
        REP     MOVSB
        POP     ESI
        POP     EDI
end;



function StrECopy(Dest: PChar; const Source: PChar): PChar; assembler;
asm
        PUSH    EDI
        PUSH    ESI
        MOV     ESI,EAX
        MOV     EDI,EDX
        MOV     ECX,0FFFFFFFFH
        XOR     AL,AL
        REPNE   SCASB
        NOT     ECX
        MOV     EDI,ESI
        MOV     ESI,EDX
        MOV     EDX,ECX
        SHR     ECX,2
        REP     MOVSD
        MOV     ECX,EDX
        AND     ECX,3
        REP     MOVSB
        LEA     EAX,[EDI-1]
        POP     ESI
        POP     EDI
end;

function StrComp(const Str1, Str2: PChar): Integer; assembler;
asm
        PUSH    EDI
        PUSH    ESI
        MOV     EDI,EDX
        MOV     ESI,EAX
        MOV     ECX,0FFFFFFFFH
        XOR     EAX,EAX
        REPNE   SCASB
        NOT     ECX
        MOV     EDI,EDX
        XOR     EDX,EDX
        REPE    CMPSB
        MOV     AL,[ESI-1]
        MOV     DL,[EDI-1]
        SUB     EAX,EDX
        POP     ESI
        POP     EDI
end;

function CompareStr(const S1, S2: string): Integer; assembler;
asm
        PUSH    ESI
        PUSH    EDI
        MOV     ESI,EAX
        MOV     EDI,EDX
        OR      EAX,EAX
        JE      @@1
        MOV     EAX,[EAX-4]
@@1:    OR      EDX,EDX
        JE      @@2
        MOV     EDX,[EDX-4]
@@2:    MOV     ECX,EAX
        CMP     ECX,EDX
        JBE     @@3
        MOV     ECX,EDX
@@3:    CMP     ECX,ECX
        REPE    CMPSB
        JE      @@4
        MOVZX   EAX,BYTE PTR [ESI-1]
        MOVZX   EDX,BYTE PTR [EDI-1]
@@4:    SUB     EAX,EDX
        POP     EDI
        POP     ESI
end;

function StrCat(Dest: PChar; const Source: PChar): PChar;
begin
  StrCopy(StrEnd(Dest), Source);
  Result := Dest;
end;


procedure StrCpy(Dest, Src: PChar);
asm
 test edx, edx
 jz @Exit
 push edi
 push esi
 push ecx
 push eax
 mov edi, edx
 xor eax, eax
 xor ecx, ecx
 not ecx
 repne scasb
 sub edi, edx
 mov ecx, edi
 pop edi
 mov esi, edx
 rep movsb
 pop ecx
 pop esi
 pop edi
@Exit:
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


procedure ToHex(Value, Digits: cardinal; OutBuff: PChar);
asm  
 push esi
 push edi
 push ecx
 mov esi, esp
 sub esp, 32
 push ecx
@CvtLoop:
 push edx
 push esi
@D1:
 xor edx, edx
 mov ecx, 16
 div ecx
 dec esi
 add dl, '0'
 cmp dl, '0' + 10
 jb @D2
 add dl, ('A' - '0') - 10
@D2:
 mov [esi], dl
 or eax, eax
 jne @D1
 pop ecx
 pop edx
 sub ecx, esi
 sub edx, ecx
 jbe @D5
 add ecx, edx
 mov al, '0'
 sub esi, edx
 jmp @z
@zloop:
 mov [esi + edx], al
@z:
 dec edx
 jnz @zloop
 mov [esi], al
@D5:
 pop edi
 rep movsb
 xor eax, eax
 mov [edi], al
 add esp, 32
 pop ecx
 pop edi
 pop esi
end;
           

function UpCase(Str: PChar):PChar; stdcall;
var
 r: cardinal;
begin
  result:=Str;
  for r := 0 to StrLen(result) do
   if result[r] in ['a'..'z'] then Dec(result[r], Ord('a') - Ord('A'));


end;

procedure Copy(Src, Dst: PChar; Index, Count: cardinal);
asm
 push esi
 push edi
 push ecx
 mov esi, eax
 add esi, ecx
 dec esi
 mov edi, edx
 mov ecx, [ebp+8]
 rep movsb
 xor eax, eax
 stosb
 pop ecx
 pop edi
 pop esi
end;

procedure ZeroMemory(Addr: pointer; Size: cardinal);
asm
 push edi
 push ecx
 mov edi, eax
 mov ecx, edx
 xor eax, eax
 rep stosb
 pop ecx
 pop edi
end;
{
Procedure AlignText(Buff: PChar; Aligment: byte);
var
 Len, r: dword;
begin
 Len := StrLen(Buff);
 if Len < Aligment then
  begin
   for r := 0 to Aligment - Len do Buff[Len + r] := ' ';
   Buff[Aligment] := #0;
  end;
end;
   }

procedure FillByte(Dest: Pointer; const B: Byte; Count: Word);
var
  dp: PByte;
begin
  dp:=PByte(Dest);
  while Count>0 do begin
    dp^:=B;
    Inc(dp);
    Dec(Count);
  end;
end;

procedure FillWord(Dest: Pointer; const W: Word; Count: Word);
var
  dp: PWord;
begin
  dp:=PWord(Dest);
  while Count>0 do begin
    dp^:=W;
    Inc(dp);
    Dec(Count);
  end;
end;

procedure FillChar(var Dest; count: Integer; Value: Char);
var
  I: Integer;
  P: PChar;
begin
  P := PChar(@Dest);
  for I := count-1 downto 0 do
    P[I] := Value;
end;


procedure Restart;
begin
  asm
    { PMODE ONLY!!! }
      mov al,0FEh
      out 64h,al // Causes a Soft Reset
    { END }
    { USER MODE
      DB 0EAh  // Jump to reboot address (FFFF:0000)
      DW 0000h
      DW 0FFFFh
    }
  end;
end;

procedure ShutDown; stdcall;
asm
    cli
    hlt
  
end;

function StrAdd(Str1, Str2:PChar):PChar;
var
Ret:array[0..BUFFER_SIZE]of Char;
I,Len:Integer;
Ret1:PChar;
begin
   FillChar(Ret,Sizeof(Ret),#0);
   I := 0;
   while(Str1[I] <> #0) do 
   begin
	Ret[I] := str1[I];
	Inc(I);
   end;
   Len := I;
   I := 0;
   while(Str2[I] <> #0) do 
   begin
	Ret[Len+I] := Str2[I];
	Inc(I);
   end;
	
   Ret1 := Ret;
   Result:= Ret1;
end;

{$endif}
