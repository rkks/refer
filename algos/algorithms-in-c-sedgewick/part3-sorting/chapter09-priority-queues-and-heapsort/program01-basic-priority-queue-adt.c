/* Definition 9.1: A __priority queue__ is a data structure of items
 * with keys that supports two basic operations:
 *   insert a new item, and
 *   delete the item with the largest key.
 */

/* Program 9.1: Basic priority-queue ADT
 * This interface defines operations for the simplest type of priority
 * queue: initialize, test if empty, add a new item, remove the
 * largest item. Elementary implementations of these functions using
 * arrays and linked lists can require linear time in the worst case,
 * but we shall see implementations in this chapter where all operations
 * are guaranteed to run in time at most proportional to the logarithm
 * of the number of items in the queue. The argument to `PQinit` specifies the maximum number of items expected in the queue.
 */
void PQinit(int);
int PQempty();
void PQinsert(Item);
Item PQdelmax();
