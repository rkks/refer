/* Program 4.18: First-class ADT interface for queues
 * We provide handles for queues in precisely the same manner as we did for
 * complex numbers in Program 4.16: A handle is a pointer to a structure that
 * is unspecified except for the tag name.
 */
typedef struct queue *Q;
void QUEUEdump(Q);
Q QUEUEinit(int maxN);
int QUEUEempty(Q);
void QUEUEput(Q, Item);
Item QUEUEget(Q);
