/* 1-24.c */

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK 1024

enum
{
    CODE,       /* nothing of the following */
    COMMENT,    /* inside a comment */
    QUOTE1,     /* inside '' */
    QUOTE2      /* inside "" */
};

int main(void)
{
    int ch;
    int state = CODE;
    char stack[MAX_STACK];
    size_t top = 0; /* points to the top of the stack :-) */
    size_t line = 1;
    int error = 0;  /* for ok-message */

    while ((ch = getchar()) != EOF)
    {
        if (ch == '\n')
        {
            line++;
        }

        switch (state)
        {
        case CODE:
            if (ch == '\'')
            {
                state = QUOTE1;
            }
            else if (ch == '"')
            {
                state = QUOTE2;
            }
            else if (ch == '/')
            {
                int second = getchar();

                if (second == '*')
                {
                    state = COMMENT;
                }
                else
                {
                    ungetc(second, stdin);
                }
            }
            else if (ch == '(' || ch == '[' || ch == '{')
            {
                if (top < MAX_STACK)
                {
                    stack[top++] = ch;
                }
                else
                {
                    printf("Stack too small!\n");
                    return EXIT_FAILURE; /* exit gracefully :-) */
                }
            }
            else if (ch == ')' || ch == ']' || ch == '}')
            {
                if (top == 0) /* found closing brace but stack is empty */
                {
                    printf("Line %lu: Closing '%c' found without "
                           "counterpart.\n", (unsigned long)line, ch);
                    error = 1;
                }
                else
                {
                    char open = stack[--top];

                    if ((ch == ')' && open != '(') ||
                        (ch == ']' && open != '[') ||
                        (ch == '}' && open != '{'))
                    {
                        printf("Line %lu: Closing '%c' does not match "
                             "opening '%c'.\n", (unsigned long)line, ch, open);
                        error = 1;
                    }
                }
            }
            break;
        case COMMENT:
            if (ch == '*')
            {
                int second = getchar();

                if (second == '/')
                {
                    state = CODE;
                }
                else
                {
                    ungetc(second, stdin);
                }
            }
            break;
        case QUOTE1:
            if (ch == '\\')
            {
                (void)getchar(); /* an escaped char inside '' throw it away */
            }
            else if (ch == '\'')
            {
                state = CODE;
            }
            break;
        case QUOTE2:
            if (ch == '\\')
            {
                (void)getchar(); /* an escaped char inside "" throw it away */
            }
            else if (ch == '"')
            {
                state = CODE;
            }
            break;
        }
    }

    if (state == COMMENT)
    {
        printf("Code ends inside comment!\n");
    }
    else if (state == QUOTE1)
    {
        printf("Code ends inside single quotes!\n");
    }
    else if (state == QUOTE2)
    {
        printf("Code ends inside double quotes!\n");
    }
    else if (top == 0 && error == 0)
    {
        printf("Code seems to be ok.\n");
    }
    if (top > 0) /* still something in the stack */
    {
        size_t i;
        for (i = 0; i < top; i++)
        {
            printf("Opening '%c' found without counterpart.\n", stack[i]);
        }
    }

    return 0;
}

