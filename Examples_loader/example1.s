.global main
.text
main:
    mov r0, #5
loop:
    subs r0, r0, #1
    bne loop
end:
    swi 0x123456
.data
