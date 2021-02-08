/* Program 9.8: First-class priority-queue ADT
 * This interface for a priority-queue ADT provides handles to items
 * (which allow client programs to delete items and to change
 * priorities) and handlesto priority queues (which allow clients to
 * maintain multiple priority queues and to merge queues together).
 * These types, `PQlink` and `PQ` respectively, are pointers to
 * structures that are to be specified in the implementation (see
 * Section 4.8).
 */
typedef struct pq* PQ;
typedef struct PQnode* PQlink;

PQ PQinit();
int PQempty(PQ);
PQlink PQinsert(PQ, Item);
Item PQdelmax(PQ);
void PQchange(PQ, PQlink, Item);
void PQdelete(PQ, PQlink);
void PQjoin(PQ, PQ);
