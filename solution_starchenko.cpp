#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stdio.h>
#include <utility>

#include <atomic>

struct node_t;
struct queue_t;

class node_t {
public:
    uint64_t value = 0;
    std::atomic<node_t*> next;

    node_t(uint64_t value = 0, node_t* next = NULL) : value(value), next(next) {
    }
};

struct queue_t {
    std::atomic<node_t*> head;
    std::atomic<node_t*> tail;
};

class MyQueue : public IQueue {
public:
    MyQueue() {
        node_t* node = new node_t();

        queue.head = node;
        queue.tail = node;

        first_head = node;
    }

    ~MyQueue() {
        auto head = first_head;
        while (head->next) {
            auto next = head->next.load(std::memory_order_relaxed);
            delete head;
            head = next;
        }

        delete head;
    }

    void Push(uint64_t value) override {
        node_t* node = new node_t(value);

        node_t* tail;
        node_t* next;
        while (true) {
            tail = queue.tail.load(std::memory_order_relaxed);
            next = tail->next.load();
            if (tail == queue.tail) {
                if (next == NULL) {
                    if (std::atomic_compare_exchange_weak_explicit(
                            &(tail->next), &next, node,
                            std::memory_order_release,
                            std::memory_order_relaxed)) {
                        break;
                    } else {
                    }

                    if (std::atomic_compare_exchange_weak_explicit(
                            &(queue.tail), &tail, next,
                            std::memory_order_release,
                            std::memory_order_relaxed)) {
                    } else {
                    }
                }
            }
        }

        if (std::atomic_compare_exchange_weak_explicit(
                &(queue.tail), &tail, node, std::memory_order_release,
                std::memory_order_relaxed)) {
        } else {
        }
    }

    std::pair<bool, uint64_t> Pop() override {
        int64_t pvalue = 0;
        node_t* head;
        node_t* tail;
        node_t* next;
        while (true) {
            head = queue.head.load(std::memory_order_relaxed);
            tail = queue.tail.load(std::memory_order_relaxed);

            if (head == queue.head.load(std::memory_order_relaxed)) {
                if (head == tail) {
                    next = head->next;
                    if (next == NULL) {
                        return std::make_pair(false, 0);
                    }

                    if (std::atomic_compare_exchange_weak_explicit(
                            &(queue.tail), &tail, next,
                            std::memory_order_release,
                            std::memory_order_relaxed)) {
                    } else {
                    }
                } else {
                    next = head->next;
                    pvalue = next->value;
                    if (std::atomic_compare_exchange_weak_explicit(
                            &(queue.head), &head, next,
                            std::memory_order_release,
                            std::memory_order_relaxed)) {
                        break;
                    } else {
                    }
                }
            }
        }

        return std::make_pair(true, pvalue);
    }

private:
    queue_t queue;
    node_t* first_head;
};

using TheQueue = MyQueue;
