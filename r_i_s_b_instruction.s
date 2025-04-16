addi x1, x0, 10       ; Load immediate value 10 into x1
addi x2, x0, 20       ; Load immediate value 20 into x2
add x3, x1, x2        ; x3 = x1 + x2 (x3 = 10 + 20 = 30)
sub x4, x2, x1        ; x4 = x2 - x1 (x4 = 20 - 10 = 10)
bne x3, x4, not_equal ; Branch to not_equal if x3 != x4 (30 != 10)
addi x5, x0, 0        ; x5 = 0 (executed if x3 == x4)
sw x5, 0(x6)          ; Store x5 to memory address in x6 (memory store)
addi x7, x0, 100      ; Load immediate value 100 into x7
blt x5, x7, less_than ; Branch to less_than if x5 < x7 (0 < 100)
addi x8, x0, 200      ; Load immediate value 200 into x8
sw x8, 4(x6)          ; Store x8 to memory address in x6 + 4 (memory store)
beq x0, x0, finish                 ; Branch to end (essentially acts like jump)
not_equal: addi x5, x0, 1 ; x5 = 1 (executed if x3 != x4)
sw x5, 8(x6)          ; Store x5 to memory address in x6 + 8 (memory store)
addi x9, x0, 50       ; Load immediate value 50 into x9
blt x5, x9, less_than ; Branch to less_than_final if x5 < x9 (1 < 50)
addi x10, x0, 150     ; Load immediate value 150 into x10
sw x10, 12(x6)        ; Store x10 to memory address in x6 + 12 (memory store)
beq x0, x0, finish              ; Branch to finish
less_than: addi x9, x0, 25 ; x9 = 25 (executed if x5 < x7 or x5 < x9)
sw x9, 16(x6)         ; Store x9 to memory address in x6 + 16 (memory store)
finish: add x11, x5, x9 ; x11 = x5 + x9 (final result)
