/* Program 5.8: Divide and conquer to draw a ruler
 * To draw the marks on a ruler, we draw the marks on the left half,
 * then draw the longest mark in the middle, then draw the marks on the
 * right half. This program is intended to be used with `r - l` equal to
 * a power of 2 -- a property that it preserves in its recursive calls
 * (see Exercise 5.27).
 */
void rule(int l, int r, int h) {
  int m = (l+r)/2;
  
  if (h > 0) {
    rule(l, m, h-1);
    mark(m, h);
    rule(m, r, h-1);
  }
}
