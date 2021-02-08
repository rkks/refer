Exercise 6.5: Give all sequence of three compare-exchange operations
that will sort three elements (see Program 6.1)

##### Say, int a[] = {5, 3, 4};
The sort range is `l = 0` ~ `r = 2`.

```
i = 1
  j = 1
    compare-exchange(a[0], a[1]) -> a[] = {3, 5, 4}
  j = 0
    break the loop
i = 2
  j = 2
    compare-exchange(a[1], a[2]) -> a[] = {3, 4, 5}
  j = 1
    compare-exchange(a[0], a[2]) -> a[] = {3, 4, 5}
  j = 0
    break the loop
i = 3
  break the loop
```
