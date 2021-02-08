/* Program 5.6: Divide-and-conquer to find the maximum
 * This function divides a file `a[1]`, ..., `a[r]` into
 * `a[1]`, ..., `a[m]` and `a[m+1]`, ..., `a[r]`, finds the maximum
 * elements in the two parts (recursively), and returns the larger of
 * the two as the maximum element in the whole file. It assumes that
 * `Item` is a first-class type for which `>` is defined. If the file
 * size is even, the two parts are equal in size; if the file size is
 * odd, the size of the first part is 1 greater than the size of the
 * second part.
 *
 * Property 5.1: A recursive function that divide a problem of size
 * N into two independent (nonempty) parts that is solves recursively
 * calls itself less than N times.
 */
Item max(Item a[], int l, int r) {
  Item u, v;
  int m = (l+r)/2;

  if (l == r)
    return a[l];
  u = max(a, l, m);
  v = max(a, m+1, r);
  if (u > v)
    return u;
  else
    return v;
}
