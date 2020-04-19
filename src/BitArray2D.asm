
        global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here
; find index of bit
        mov rax, rdx        ; get row
        imul rax, rsi       ; multiply by row width
        add rax, rcx        ; add column
        mov rcx, rax        ; move index into rcx

        ; calculate byte offset
        mov rdx, 0          ; clear rdx
        mov r11, 8          ; move the divisor into register 11 (r11)
        div r11             ; divide the index by 8
        mov r9, rax         ; save the byte offset in register 9 (r9)

        ; calculate bit offset
        imul r11            ; multiply the byte offset by 8 (which is stored in r11)
        mov r8, rax         ; move the scaled byte offset into register 8 (r8)
        mov rax, rcx        ; move the index into rax
        sub rax, r8         ; subtract the byte offset from the index
        mov rax, r8         ; save the bit offset in register 8 (r8)

        ; create bit mask
        mov rdx, 7
        sub rdx, r8         ; subtract the bit offset
        mov rax, 1          ; move 1 into rax
        mov cl, r8b         ; move the bit offset into cl
        shl rax, cl         ; shift 1 left by the bit offset
        mov rdx, rax        ; move the bit mask into rdx

        ; access byte
        mov rax, rdi        ; move array pointer into rax
        add rax, r9         ; add byte offset

        ; specific part to this function
        or rax, rdx

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax




get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here - for now returning 0
        ; mov rax, 0

        ; find index of bit
        mov rax, rdx        ; get row
        imul rax, rsi       ; multiply by row width
        add rax, rcx        ; add column
        mov rcx, rax        ; move index into rcx

        ; calculate byte offset
        mov rdx, 0          ; clear rdx
        mov r11, 8          ; move the divisor into register 11 (r11)
        div r11             ; divide the index by 8
        mov r9, rax         ; save the byte offset in register 9 (r9)

        ; calculate bit offset
        imul r11            ; multiply the byte offset by 8 (which is stored in r11)
        mov r8, rax         ; move the scaled byte offset into register 8 (r8)
        mov rax, rcx        ; move the index into rax
        sub rax, r8         ; subtract the byte offset from the index
        mov rax, r8         ; save the bit offset in register 8 (r8)

        ; create bit mask
        mov rdx, 7
        sub rdx, r8         ; subtract the bit offset
        mov rax, 1          ; move 1 into rax
        mov cl, r8b         ; move the bit offset into cl
        shl rax, cl         ; shift 1 left by the bit offset
        mov rdx, rax        ; move the bit mask into rdx

        ; access byte
        mov rax, rdi        ; move array pointer into rax
        add rax, r9         ; add byte offset

        ; specific part to this function

        or rax, rdx
        cmp rax, 0
        setg al
        movsx rax, al

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax
