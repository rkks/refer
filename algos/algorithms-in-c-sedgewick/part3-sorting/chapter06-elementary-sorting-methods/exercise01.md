Exercise 6.1: A child's sorting toy has `i` cards that fit on a peg in
position `i` for `i` from 1 to 5. Write down the method that you use to
put the cards on the pegs, assuming that you cannot tell from the card
whether it fits on a peg (you have to try fitting it on).

##### My answer
What about mimicking binary search? With the first card, try the middle
one which is 3 so that the next step will be picking one of the split
parts. If it doesn't fit, select the one parts and so on. If the card
doesn't fit for the entire part, then move to the another part.

After the first card is put, then the second card naturally has to pick
one of the split parts to excavate. So the procedure can repeat.
