Exercise 6.6: Give all sequence of five compare-exchange operations
that will sort four elements (see Program 6.1)

##### Say, int a[] = {6, 5, 3, 4};
The sort range is `l = 0` ~ `r = 3`.

```
i = 1
  j = 1
    compare-exchange(a[0], a[1]) -> a[] = {5, 6, 3, 4}
  j = 0
    break the loop
i = 2
  j = 2
    compare-exchange(a[1], a[2]) -> a[] = {5, 3, 6, 4}
  j = 1
    compare-exchange(a[0], a[1]) -> a[] = {3, 5, 6, 4}
  j = 0
    break the loop
i = 3
  j = 3
    compare-exchange(a[2], a[3]) -> a[] = {3, 5, 4, 6}
  j = 2
    compare-exchange(a[1], a[2]) -> a[] = {3, 4, 5, 6}
  j = 1
    compare-exchange(a[0], a[1]) -> a[] = {3, 4, 5, 6}
  j = 0
    break the loop
i = 4
  break the loop
```
