/* Exercise 4.10: Give, in the same manner as Figure 4.2, the
 * contents of the stack as the following expression is evaluated
 * by Program 4.2.
 *
 * 5 9 * 8 7 4 6 + * 2 1 3 * + * + *
 */

5|    5
9|    5    9
*|   45
8|   45    8
7|   45    8    7
4|   45    8    7    4
6|   45    8    7    4    6
+|   45    8    7   10
*|   45    8   70
2|   45    8   70    2
1|   45    8   70    2    1
3|   45    8   70    2    1    3
*|   45    8   70    2    3
+|   45    8   70    5
*|   45    8  350
+|   45  358
*|16110
