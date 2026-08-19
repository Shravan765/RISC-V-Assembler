go: srli x1, x2, 10
bgeu x1, zero, try
beq x0, x0, go
try: jal x0, go