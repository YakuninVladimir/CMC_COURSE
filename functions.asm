extern pow
section .data
    f_4 dq 4.0
    f_2 dq 2.0
    f_8 dq -8.0
    f_3 dq 3.0
    f_1 dq -1.0
section .text

global f1
global f2 
global f3
global f1_der
global f2_der
global f3_der

f1: ;1 + 4/(x^2 + 1)
    push ebp
    mov ebp, esp
    finit
    fld qword[f_4]
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fld1
    faddp
    fdivp
    fld1
    faddp
    leave 
    ret
    
f2: ;x^3
    push ebp
    mov ebp, esp
    finit 
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fld qword[ebp + 8]
    fmulp
    leave
    ret
    
f3: ;2^(-x)
    push ebp
    mov ebp, esp
    sub esp, 16
    and esp, 0xfffffff0
    finit 
    fldz
    fld qword[ebp + 8]
    fsubp
    fstp qword[esp + 8]
    mov eax, dword[f_2 ]
    mov edx, dword[f_2 + 4]
    mov dword[esp], eax
    mov dword[esp + 4], edx
    call pow
    leave
    ret

f1_der: ;-8.0 * x / (pow(x, 4) + 2 * x * x + 1.0);
    push ebp
    mov ebp, esp
    sub esp, 16
    and esp, 0xfffffff0
    finit
    fld qword[f_8]
    fld qword[ebp + 8]
    fmulp
    fld1
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fld qword[f_2]
    fmulp
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fmulp
    fmulp
    faddp
    faddp
    fdivp
    leave 
    ret

f2_der: ; 3 * x * x
    push ebp
    mov ebp, esp
    finit
    fld qword[f_3]
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fmulp
    leave 
    ret


f3_der: ;-1 * pow(2, -x) * log(2);
    push ebp
    mov ebp, esp
    sub esp, 16
    and esp, 0xfffffff0
    finit
    fld qword[ebp + 8]
    fstp qword[esp]
    call f3
    fld qword[f_1]
    fldln2
    fmulp
    fmulp
    leave 
    ret