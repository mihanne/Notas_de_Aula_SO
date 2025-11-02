//gcc -Wall -c -O0 depositar.c
//objdump --no-show-raw-insn -d depositar.o

void depositar (long * saldo, long valor)
{
(*saldo) += valor ;
}
