int main(void)
{
    int type;
    double op2;
    char s[MAXOP];
    int flag = TRUE;

    while((type = Getop(s)) != EOF)
    {
        switch(type)
        {
           /* other cases snipped for brevity */
 
            case '%':
                op2 = pop();
                if(op2)
                    push(fmod(pop(), op2));
                else
                    printf("\nError: Division by zero!");
                break;
 
        }
    }
    return EXIT_SUCCESS;
}
