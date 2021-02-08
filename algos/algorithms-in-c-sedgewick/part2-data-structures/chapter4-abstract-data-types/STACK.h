/* Program 4.1: Pushdown-stack ADT interface
 * This interface defines the basic operations that define a pushdown stack. We
 * assume that the four declarations here are in a file STACK. h, which is
 * referenced as an include file by client programs that use these functions and
 * implementations that provide their code; and that both clients and
 * implementations define Item, perhaps by including an Item. h file (which may
 * have a typedef or which may define a more general interface). The argument to
 * STACKinit specifies the maximum number of elements expected on the stack.
 */
void STACKinit(int);
int STACKempty();
void STACKpush(Item);
Item STACKpop();
int STACKcount();
