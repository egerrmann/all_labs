.686
.model flat

.data
angle dd 180
x dd 0
epsilon dd ?
iterator dd 1
factorial dd 1.0
term dd 0
result dd 0

.code
_Arcsin proc
finit
fld dword ptr [esp + 4]
fld dword ptr [esp + 4]
fmul 
fld1
fsubr
fsqrt
fld dword ptr [esp + 4]
fdivr
fld1
fpatan
fild angle
fmul
fldpi
fdiv
frndint
fist angle
mov eax, angle
ret
_Arcsin endp


_CalculateSh proc
finit
fld dword ptr[esp + 8]
fstp epsilon
fld dword ptr[esp + 4]
fst x
fst term
fstp result
finit
forbegin:
fld factorial
fld x
fld term
fld epsilon
fcom 
fstsw AX
sahf
ja forend
fstp epsilon
fstp term
fld dword ptr[esp + 4]
fld dword ptr[esp + 4]
fmulp
fmulp
fst x
mov eax, dword ptr [x]
fxch
inc iterator
fild iterator
inc iterator
fild iterator
fmul
fmul
fst factorial
fdiv
fst term
fld result
fadd
fst result
finit
jmp forbegin
forend:
fld result
ret
_CalculateSh endp

end