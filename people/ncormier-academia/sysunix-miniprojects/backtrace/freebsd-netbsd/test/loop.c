int tata()
{
	while(1)
	sleep(1);
}

int titi()
{
	tata();
}

int tutu()
{
	titi();
}

int main()
{
	tutu();
}
