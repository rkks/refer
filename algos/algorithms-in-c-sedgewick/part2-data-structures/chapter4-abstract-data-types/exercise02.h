/* Exercise 4.2: Give a definition for `Item` and `eq` that might
 * be used for points in the plane (see Section 3.1).
 */
typedef struct {
  int x;
  int y;
} Point;
#define eq(A, B) (A.x == B.x && A.y == B.y)
