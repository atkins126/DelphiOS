mkisofs -R -b boot/grub/stage2_eltorito -no-emul-boot  -boot-load-size 32  -boot-info-table -o kernel.iso iso

qemu -cdrom kernel.iso 
