/*
** debug.c for  in /u/ept3/cormie_n/lang/c/backtrace/test
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Jan 23 14:45:11 2006 nicolas cormier
** Last update Tue Jan 24 18:52:37 2006 nicolas cormier
*/

int titi()
{
  puts("hello");
  while (1)
    sleep(1);
}

int toto()
{
  titi();
}

int main()
{
  toto();
}

