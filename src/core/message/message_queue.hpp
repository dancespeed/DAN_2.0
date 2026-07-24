#pragma once

#include <stdint.h>

#include "message.hpp"

template<uint8_t Capacity>
class MessageQueue
{
    static_assert(
        Capacity > 0,
        "MessageQueue capacity must be greater than zero"
    );

public:

    MessageQueue()
    {
        Clear();
    }

    void Clear()
    {
        head_ = 0;
        tail_ = 0;
        count_ = 0;
    }

    bool Push(const Message& message)
    {
        if (IsFull())
        {
            return false;
        }

        messages_[tail_] = message;

        tail_++;

        if (tail_ >= Capacity)
        {
            tail_ = 0;
        }

        count_++;

        return true;
    }

    bool Pop(Message& message)
    {
        if (IsEmpty())
        {
            return false;
        }

        message = messages_[head_];

        head_++;

        if (head_ >= Capacity)
        {
            head_ = 0;
        }

        count_--;

        return true;
    }

    bool IsEmpty() const
    {
        return count_ == 0;
    }

    bool IsFull() const
    {
        return count_ == Capacity;
    }

    uint8_t Count() const
    {
        return count_;
    }

    constexpr uint8_t GetCapacity() const
    {
        return Capacity;
    }

private:

    Message messages_[Capacity];

    uint8_t head_ = 0;
    uint8_t tail_ = 0;
    uint8_t count_ = 0;
};