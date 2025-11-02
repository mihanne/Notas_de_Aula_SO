#include <stdio.h>

int main()
{
  int i, soma=0;
  
  for (i=0; i<5; i++)
  {
    soma +=i;
    printf("i vale %d e soma vale %d\n", i, soma);
  }
}
