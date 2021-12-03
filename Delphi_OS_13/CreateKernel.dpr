program CreateKernel;

{$APPTYPE CONSOLE}
//------------------------------------------------------------------------------------------------------------//
//               Code By Ekrem KOCAK                                                                          //
//             ekremkocak@hotmail.com                                                                         //
//                  Kýrþehir  2006                                                                           //                                                                                                                                                                                                                   //
//                                                                                                            //
//----------------------------------------------------------------------------------------------------------- //

uses
  Windows,
  SysUtils,
  Classes,
  Dialogs,
  ShellApi,
  Kernel in 'Kernel.pas';

Type

  Tmultiboot_hdr=packed record
       magic       :cardinal;
       flags       :cardinal;
       checksum    :cardinal;
       header_addr :cardinal;
       load_addr   :cardinal;
       load_end_addr:cardinal;
       bss_end_addr :cardinal;
       entry_addr   :cardinal;
       mode_type    :cardinal;
       width        :cardinal;
       height       :cardinal;
       depth        :cardinal;
end;










//------------- Kernel Code  End --------------------------


var
 multiboot_hdr:Tmultiboot_hdr;

  fsize:cardinal;
  fnc:pointer;
  image_base:integer;
 
  entry_addr:integer;
   test1:array of byte;

  f:file;
begin
  assignfile(f,'kernel.bin');
  rewrite(f,1);


  image_base:=cardinal(@loader)-sizeof(multiboot_hdr);
  entry_addr:=cardinal(@loader);
 fsize:=cardinal(@loader_end)-cardinal(@loader);
  // fsize:=cardinal(@test1)-cardinal(@loader);

   fillchar(multiboot_hdr,sizeof(multiboot_hdr),0);
   multiboot_hdr.magic:=($1BADB002);
   multiboot_hdr.flags:=(1 shl 2) or (1 shl 16) ;
   multiboot_hdr.checksum:=cardinal(-multiboot_hdr.magic-multiboot_hdr.flags) ;
   multiboot_hdr.header_addr:=image_base;
   multiboot_hdr.load_addr:=image_base;
   multiboot_hdr.load_end_addr:=$00000000;
   multiboot_hdr.bss_end_addr:=$00000000;
   multiboot_hdr.entry_addr:=entry_addr;
   multiboot_hdr.mode_type:=0;
   multiboot_hdr.width:=0;
   multiboot_hdr.height:=0;
   multiboot_hdr.depth:=0;

   blockwrite(f,multiboot_hdr,sizeof(multiboot_hdr));
   fnc:=@loader;
   blockwrite(f,fnc^,fsize);

 
   closefile(f);

    ShellExecute(0, nil, 'cmd.exe', '/C qemu.exe -kernel kernel.bin', nil, SW_HIDE);


end.
