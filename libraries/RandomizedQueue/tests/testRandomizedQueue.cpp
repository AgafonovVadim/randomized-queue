#include <algorithm>
#include <string>
#include <vector>

#include "RandomizedQueue.hpp"
#include "gtest/gtest.h"
#include "testIterator.hpp"

namespace {

class NonCopyable {
    int m_data = 0;

public:
    NonCopyable() = default;
    NonCopyable(const int data) : m_data(data) {}
    NonCopyable(const NonCopyable &)       = delete;
    NonCopyable(NonCopyable &&)            = default;
    NonCopyable &operator=(NonCopyable &&) = default;

    operator int() const { return m_data; }
    NonCopyable &operator=(const int value) {
        m_data = value;
        return *this;
    }

    NonCopyable &operator*=(const NonCopyable &other) {
        m_data *= other.m_data;
        return *this;
    }

    friend bool operator==(const int lhs, const NonCopyable &rhs) { return lhs == rhs.m_data; }
};

template <class T>
struct RandomizedQueueTest: ::testing::Test {
    RandomizedQueue<T> queue;

    const RandomizedQueue<T> &const_queue() { return queue; }

    // Initialize sample data for generic iterator tests
    RandomizedQueue<T> &not_empty_container() {
        if (sample.empty()) {
            sample.enqueue(1);
            sample.enqueue(2);
            sample.enqueue(3);
            sample.enqueue(33);
            sample.enqueue(190);
        }
        return sample;
    }

    RandomizedQueue<T> sample;
};

using TestedTypes = ::testing::Types<int, NonCopyable>;
TYPED_TEST_SUITE(RandomizedQueueTest, TestedTypes);

}  // anonymous namespace

TEST(RandomizedQueueEnqueueTest, enqueue) {
    RandomizedQueue<std::string> queue;
    std::string s1       = "Hello, world";
    const std::string s2 = "Bonjour monde";
    queue.enqueue(s1);
    queue.enqueue(s2);
    queue.enqueue("Hallo wereld");
    EXPECT_EQ("Hello, world", s1);
    for (auto &s : queue) {
        s += '!';
    }
    std::vector<std::string> elements;
    for (const auto &s : queue) {
        elements.push_back(s);
    }
    ASSERT_EQ(3, elements.size());
    std::sort(elements.begin(), elements.end());
    EXPECT_EQ("Bonjour monde!", elements[0]);
    EXPECT_EQ("Hallo wereld!", elements[1]);
    EXPECT_EQ("Hello, world!", elements[2]);
}

TEST(RandomizedQueueRandTest, compare_randomness) {
    RandomizedQueue<int> q1, q2, q3;
    RandomizedQueue<std::string> q4, q5;
    for (int i = 0; i < 50; ++i) {
        q1.enqueue(i);
        q2.enqueue(i);
        q3.enqueue(i);
        q4.enqueue(std::to_string(i));
        q5.enqueue(std::to_string(i));
    }
    int count = 0;
    int e1 = q1.sample(), e2 = q2.sample(), e3 = q3.sample();
    int e4 = std::stoi(q4.sample()), e5 = std::stoi(q5.sample());
    if (e1 == e2) {
        ++count;
    }
    if (e1 == e3) {
        ++count;
    }
    if (e1 == e4) {
        ++count;
    }
    if (e1 == e5) {
        ++count;
    }
    if (e2 == e3) {
        ++count;
    }
    if (e4 == e5) {
        ++count;
    }
    EXPECT_LT(count, 3) << "Too much for coincidence";
}

TYPED_TEST(RandomizedQueueTest, types) {
    auto it                 = this->queue.begin();
    auto const_it           = this->const_queue().begin();
    const bool is_ref       = std::is_same_v<TypeParam &, decltype(*it)>;
    const bool is_const_ref = std::is_same_v<const TypeParam &, decltype(*const_it)>;
    EXPECT_TRUE(is_ref);
    EXPECT_TRUE(is_const_ref);
    const bool is_ptr       = std::is_same_v<TypeParam *, decltype(it.operator->())>;
    const bool is_const_ptr = std::is_same_v<const TypeParam *, decltype(const_it.operator->())>;
    EXPECT_TRUE(is_ptr);
    EXPECT_TRUE(is_const_ptr);
}

TYPED_TEST(RandomizedQueueTest, iterators) {
    this->queue.enqueue(-1);
    this->queue.enqueue(139);
    auto it1        = this->queue.begin();
    auto it2        = it1;
    const auto b_it = it1;
    EXPECT_EQ(it1, it2);
    EXPECT_EQ(*it1, *it2);
    it1++;
    ++it2;
    EXPECT_EQ(it1, it2);
    EXPECT_EQ(*it1, *it2);
    EXPECT_EQ(1, std::distance(b_it, it1));
    EXPECT_EQ(1, std::distance(b_it, it2));
}

TYPED_TEST(RandomizedQueueTest, empty) {
    EXPECT_TRUE(this->queue.empty());
    EXPECT_EQ(0, this->queue.size());
    std::size_t count = 0;
    for ([[maybe_unused]] const auto &x : this->queue) {
        ++count;
    }
    EXPECT_EQ(0, count);
}

TYPED_TEST(RandomizedQueueTest, singleton) {
    this->queue.enqueue(0);
    EXPECT_FALSE(this->queue.empty());
    EXPECT_EQ(1, this->queue.size());
    EXPECT_EQ(0, this->queue.sample());
    std::size_t count = 0;
    for (const auto &x : this->queue) {
        EXPECT_EQ(0, x);
        ++count;
    }
    EXPECT_EQ(1, count);

    const auto x = this->queue.dequeue();
    EXPECT_EQ(0, x);
}

TYPED_TEST(RandomizedQueueTest, many) {
    const std::vector<int> etalon_sorted = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (auto i : etalon_sorted) {
        this->queue.enqueue(i);
    }
    EXPECT_FALSE(this->const_queue().empty());
    EXPECT_EQ(etalon_sorted.size(), this->const_queue().size());
    std::size_t count = 0;
    for ([[maybe_unused]] const auto &x : this->const_queue()) {
        ++count;
    }
    EXPECT_EQ(etalon_sorted.size(), count);

    const auto b1 = this->queue.cbegin();
    const auto e1 = this->queue.cend();
    const auto b2 = this->const_queue().begin();
    const auto e2 = this->const_queue().end();

    std::vector<int> v11, v12, v21, v22;
    std::copy(b1, e1, std::back_inserter(v11));
    std::copy(b2, e2, std::back_inserter(v21));
    std::copy(b1, e1, std::back_inserter(v12));
    std::copy(b2, e2, std::back_inserter(v22));
    EXPECT_EQ(etalon_sorted.size(), v11.size());
    EXPECT_EQ(v11, v12);
    std::sort(v12.begin(), v12.end());
    EXPECT_EQ(etalon_sorted, v12);
    EXPECT_EQ(etalon_sorted.size(), v21.size());
    EXPECT_EQ(v21, v22);
    EXPECT_NE(v11, v21);

    for (int i = 0; i < 100; ++i) {
        EXPECT_NE(etalon_sorted.end(),
                  std::find(etalon_sorted.begin(), etalon_sorted.end(), this->const_queue().sample()));
    }

    std::vector<int> v;
    v.reserve(this->queue.size());
    while (!this->queue.empty()) {
        v.push_back(this->queue.dequeue());
    }
    EXPECT_EQ(etalon_sorted.size(), v.size());
    std::sort(v.begin(), v.end());
    EXPECT_EQ(etalon_sorted, v);
}

TYPED_TEST(RandomizedQueueTest, iterator_modify) {
    const std::vector<int> initial_values = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const std::vector<int> etalon_sorted  = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100};
    for (auto i : initial_values) {
        this->queue.enqueue(i);
    }
    EXPECT_FALSE(this->queue.empty());
    EXPECT_EQ(initial_values.size(), this->queue.size());
    for (auto &x : this->queue) {
        x *= x;
    }
    std::vector<int> values;
    values.reserve(this->queue.size());
    for (const auto &x : this->queue) {
        values.push_back(x);
    }
    EXPECT_EQ(initial_values.size(), values.size());
    std::sort(values.begin(), values.end());
    EXPECT_EQ(etalon_sorted, values);
}

TYPED_TEST(RandomizedQueueTest, lot_of_modifications) {
    int first            = 1234;
    int second           = 1234 + 70000;
    int third            = second + 70000;
    int fourth           = third + 70000;
    const std::size_t n1 = second - first;
    const std::size_t n2 = third - first;
    const std::size_t n3 = fourth - first;
    for (int i = first; i < second; ++i) {
        this->queue.enqueue(i);
    }
    EXPECT_EQ(n1, this->queue.size());
    for (int i = second; i < third; ++i) {
        this->queue.enqueue(i);
    }
    EXPECT_EQ(n2, this->queue.size());

    {
        std::size_t count = 0;
        for (const auto &x : this->queue) {
            ++count;
            EXPECT_LE(first, x);
            EXPECT_GE(third, x);
        }
        EXPECT_EQ(n2, count);
    }

    for (std::size_t i = 0; i < n1; ++i) {
        const int x = this->queue.dequeue();
        EXPECT_LE(first, x);
        EXPECT_GE(third, x);
    }
    EXPECT_EQ(n2 - n1, this->queue.size());
    for (int i = third; i < fourth; ++i) {
        this->queue.enqueue(i);
    }

    {
        std::size_t count = 0;
        for (const auto &x : this->queue) {
            ++count;
            EXPECT_LE(first, x);
            EXPECT_GE(fourth, x);
        }
        EXPECT_EQ(n3 - n1, count);
    }

    std::size_t count = 0;
    while (!this->queue.empty()) {
        ++count;
        const int x = this->queue.dequeue();
        EXPECT_LE(first, x);
        EXPECT_GE(fourth, x);
    }
    EXPECT_EQ(n3 - n1, count);
}

using TypesToTest = ::testing::Types<RandomizedQueueTest<int>>;
INSTANTIATE_TYPED_TEST_SUITE_P(RandomizedQueue, IteratorTest, TypesToTest);
