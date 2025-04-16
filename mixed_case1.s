srli x1, x2, 10
add x5, x1, x2
beq x5, x3, equal ;;;this line goes to 5th line only when x5 and x3 are equal
;123456789;123456789;123456789;123456789;1234567
addi x4, x4, 1
equal: addi x4, x4, 2
lui x6, 16
sw x4, 0(x6)
sub x8, x7, x1
bne x8, x5, noteq
addi x4, x4, 1
noteq: slli x9, x4, 2 ;this is a comment
addi x10, x9, 5
and x11, x9, x3
or x12, x5, x11
lui x14, 32
addi x14, x14, 1024
sw x13, 0(x14)
end: beq x0, x0, end ;this line is important as it ensures in hardware that the text memory is not accessed below this
