
## Randomized queue
It is required to implement template data structures: a randomized queue (RandomizedQueue). The following data structures are allowed: `std::list`, `std::vector`, `std::array`. You may use any other non-data-structure-related parts at your discretion.

A randomized queue is a collection that provides access to its elements in random order. Each "view" of this collection yields a random, independent permutation of the elements.
For example, if you take two iterators at the beginning of the collection and then traverse all elements to the end with each of them, these two traversals will result in two different, independent permutations.

Example: original collection = 1, 2, 3, 4, 5, 6

Permutations:

6, 2, 4, 1, 5, 3

5, 3, 2, 6, 4, 1

2, 6, 4, 5, 3, 1

The following code should work:
```cpp
RandomizedQueue<int> q;
for (int i = 0; i < 5; ++i) {
    q.enqueue(i);
}
auto b1 = q.begin();
auto e1 = q.end();
auto b2 = q.begin();
auto e2 = q.end();

std::vector<int> v11, v12;
std::copy(b1, e1, std::back_inserter(v11));
std::copy(b1, e1, std::back_inserter(v12));
assert(v11 == v12); // Two traversals with one iterator give the same sequence

std::vector<int> v21, v22;
std::copy(b2, e2, std::back_inserter(v21));
std::copy(b2, e2, std::back_inserter(v22));
assert(v21 == v22); // Two traversals with one iterator give the same sequence

assert(v11 != v21); // With a high probability, two different iterators result in different sequences

// Iterators did not affect the queue
while (!q.empty()) {
    std::cout << q.dequeue() << ' ';
}
```

## Randomized Queue Requirements
You need to create a templated implementation of `RandomizedQueue` that meets the following conditions:
* parameterless constructor
* method `empty` that checks whether the data structure is empty
* method `size` that returns the number of elements in the data structure
* iterators must be implemented (including ones that allow modifying elements in the queue)
* method `enqueue` adds an element to the data structure
* method `sample` returns a random element without removing it
* method `dequeue` returns a random element and removes it from the deque
* `begin`
* `end`
* `cbegin`
* `cend`

It is acceptable to assume that changing the size of the queue invalidates all its iterators.

## Client Program `subset`
You need to develop a utility called `subset` (add a meaningful implementation in `libraries/RandomizedQueue/src/Subset.cpp`), which takes a list of strings and returns `k` of them with uniform distribution. To do this, you need to use the developed data structures. A "string" is any sequence of printable characters, delimited by a newline character (`\n`).

Input: `printf '%s\n' A B C D E F G H I | ./subset 3`
Output:
```
C
G
A
```
