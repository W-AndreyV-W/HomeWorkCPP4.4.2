#include <iostream>
#include <stdexcept>
#include <locale.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>

struct ListNode
{
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    ListNode* prev;
    ListNode* next;
};

class List
{
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head))
    {
    }

    virtual ~List()
    {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }

    unsigned long Size() { return m_size; }

    void PushFront(int value)
    {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value)
    {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractPrev(current);
        }
    }

private:
    ListNode* extractPrev(ListNode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};

TEST_CASE("Checking the functions of adding and receiving items from a doubly linked list", "[Test methods]") {
    
    List list;

    SECTION("Test PushBack and PopBack") {

        INFO("Checking PushBack");
        list.PushBack(5);
        REQUIRE(list.Size() == 1);

        INFO("Checking PopBack");
        CHECK(list.PopBack() == 5);
        CHECK(list.Size() == 0);
    }

    SECTION("Test PushFront and PopFront") {

        INFO("Checking PushFront");
        list.PushFront(5);
        REQUIRE(list.Size() == 1);

        INFO("Checking PopFront");
        CHECK(list.PopFront() == 5);
        CHECK(list.Size() == 0);
    }

    SECTION("Check the correctness of the work on an empty list") {
        std::string str = "list is empty";
        INFO("Clearing PopFront");
        try {
            list.PopFront();
        }
        catch (const std::exception& ex) {
            CHECK(ex.what() == str);
        }

        INFO("Clearing PopBack");
        try {
            list.PopBack();
        }
        catch (const std::exception& ex) {
            CHECK(ex.what() == str);
        }
    }

    SECTION("Comprehensive testing of functions") {

        INFO("Checking PushBack and PushFront");
        list.PushBack(1);
        list.PushBack(2);
        list.PushFront(3);
        list.PushFront(4);
        list.PushBack(5);
        CHECK(list.Size() == 5);

        INFO("Checking PushBack(5) and PopBack");
        CHECK(list.PopBack() == 5);

        INFO("Checking PushBack(2) and PopBack");
        CHECK(list.PopBack() == 2);

        INFO("Checking PushFront(4) and PopFront");
        CHECK(list.PopFront() == 4);

        INFO("Checking PushFront(3) and PopFront");
        CHECK(list.PopFront() == 3);

        INFO("Checking PushBack(1) and PopFront");
        CHECK(list.PopFront() == 1);

        INFO("Checking PopBack and PopFront");
        CHECK(list.Size() == 0);
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    return Catch::Session().run(argc, argv);
}