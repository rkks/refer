package Tuesday;
  
sub new
{
    bless {  }, shift;
}
  
# wear a coat only on Tuesday
sub coat
{
    return [ localtime ]->[6] == 2;
}
  
1;
