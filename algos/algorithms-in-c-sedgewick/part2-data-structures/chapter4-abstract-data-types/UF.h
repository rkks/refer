/* Program 4.6: Equivalence-relations ADT interface
 * The ADT interface mechanism makes it convenient for us to encode
 * precisely our decision to consider the connectivity algorithm in terms
 * of three abstract operations: initialize, find whether two nodes are con
 * nected, and perform a union operation to consider them connected
 * henceforth.
 */
void UFinit(int);
int UFfind(int, int);
void UFunion(int, int);
