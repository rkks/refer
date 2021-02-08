/* Program 4.9: FIFO queue ADT interface
 * The interface is identical to the pushdown stack interface of Program 4.1,
 * except for the names of the structure. The two ADTs differ only in the
 * specification, which is not reflected in the code.
 *
 * Definition 4.3: A FIFO queue is an ADT that comprises two basic operations:
 * insert (put) a new item, and delete (get) the item that was least recently
 * inserted.
 */
void QUEUEinit(int);
int QUEUEempty();
void QUEUEput(Item);
Item QUEUEget();
