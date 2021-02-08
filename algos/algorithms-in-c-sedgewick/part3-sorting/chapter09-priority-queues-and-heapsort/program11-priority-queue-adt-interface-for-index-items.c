/* Program 9.11: Priority queue ADT interface for index items
 * Instead of building a data structure from the items themselves,
 * this interface provides for building a priority queue using indices
 * into a client array. The _insert_, _delete the maximum_,
 * _change priority_, and _delete_ routines all uses a handle
 * consisting of an array index. The client supplies a `less` routine
 * to compare two records. For example, the client program might define
 * `less(i, j)` to be the result of comparing `data[i].grade` and
 * `data[j].grade`.
 */
int less(int, int);
void PQinit();
int PQempty();
void PQinsert(int);
int PQdelmax();
void PQchange(int);
void PQdelete(int);
