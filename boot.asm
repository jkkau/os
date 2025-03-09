ORG 0  ; set the offset to 0

jmp 0x7c0:main

main:
    cli  ; disable interupts
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    mov ax, 0x0
    mov ss, ax   ; [ss:sp] is used for stack, increase from up to the down memory address
    mov sp, 0x7c00
                 ; no need to set cs and ip, [cs:ip] is pointing to 0x7c00 to execute the bootloader, set by BIOS.
    sti  ; enable interupts

    mov si, message  ; mov address of 'Hello World!0' to si
    call print

    jmp $

message:
    db 'Hello World!', 0
    
print:
    mov ah, 0eh  ; tell BIOS print one byte once
_loop: 
    lodsb  ; mov first byte(si point to) to al, increase si by one byte
    cmp al, 0
    je .done  ; if al equal to 0, jmp to .done
    int 10h   ; int->interupt, call BIOS to print al
    jmp _loop
.done:
    ret

times 510-($-$$) db 0

dw 0xAA55       ; little endian