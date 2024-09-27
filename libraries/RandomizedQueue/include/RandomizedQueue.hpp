#ifndef RANDOMIZEDQUEUE_HPP
#define RANDOMIZEDQUEUE_HPP

#include <random>
#include <vector>

template <class T>
class RandomizedQueue {
private:
    std::vector<T> randomized_queue;
    template <bool isConst>
    class QueueIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::conditional_t<isConst, const T, T>;
        using pointer           = value_type *;
        using reference         = value_type &;

        using items_type = std::conditional_t<isConst, const std::vector<T>, std::vector<T>>;

        QueueIterator() = default;
        QueueIterator(items_type *queue, std::size_t iterator_position)
            : iterator_position(iterator_position), queue(queue) {
            permutation.resize(queue->size());
            std::iota(permutation.begin(), permutation.end(), 0);
            std::random_shuffle(permutation.begin(), permutation.end());
        }
        friend bool operator==(const QueueIterator &lhs, const QueueIterator &rhs) {
            return (lhs.queue == rhs.queue && lhs.iterator_position == rhs.iterator_position);
        }

        friend bool operator!=(const QueueIterator &lhs, const QueueIterator &rhs) { return !(lhs == rhs); }

        reference operator*() const { return (*queue)[permutation[iterator_position]]; }

        pointer operator->() const { return &operator*(); }

        QueueIterator &operator++() {
            iterator_position++;
            return *this;
        }

        QueueIterator operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        QueueIterator &operator--() {
            iterator_position--;
            return *this;
        }

        QueueIterator operator--(int) {
            auto tmp = *this;
            operator--();
            return tmp;
        }

    private:
        friend class RandomizedQueue<T>;

        std::size_t iterator_position = 0;
        items_type *queue             = nullptr;
        std::vector<std::size_t> permutation;
    };

public:
    RandomizedQueue() = default;

    bool empty() const { return randomized_queue.empty(); }

    std::size_t size() const { return randomized_queue.size(); }

    void enqueue(T add) { randomized_queue.push_back(std::forward<T>(add)); }

    const T &sample() const { return randomized_queue[((double)rand() / RAND_MAX) * (randomized_queue.size() - 1)]; }

    T dequeue() {
        auto iterator_position = ((double)rand() / RAND_MAX) * (randomized_queue.size() - 1);
        std::swap(randomized_queue[iterator_position], randomized_queue[randomized_queue.size() - 1]);
        T ans = std::move(randomized_queue[randomized_queue.size() - 1]);
        randomized_queue.pop_back();
        return ans;
    }

    using iterator       = QueueIterator<false>;
    using const_iterator = QueueIterator<true>;

    iterator begin() { return iterator(&randomized_queue, 0); }
    iterator end() { return iterator(&randomized_queue, randomized_queue.size()); }

    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }
    const_iterator cbegin() const { return const_iterator(&randomized_queue, 0); }
    const_iterator cend() const { return const_iterator(&randomized_queue, randomized_queue.size()); }
};

#endif  // RANDOMIZEDQUEUE_HPP