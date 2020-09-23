#ifndef BIGINT_BUFFER_H
#define BIGINT_BUFFER_H

#include <iostream>
#include <cstring>
#include <algorithm>
#include "my_vector.h"

struct buffer {
    buffer();
    buffer(buffer const& other);
    buffer(size_t len, uint32_t x = 0);
    buffer& operator= (buffer const& other);
    void resize(size_t new_size, uint32_t x = 0);
    ~buffer();
    bool operator== (buffer const& other) const;
    void reverse();
    uint32_t& operator[] (size_t i);
    uint32_t const& operator[] (size_t i) const;
    void push_back(uint32_t const& x);
    uint32_t back() const;
    void pop_back();
    size_t size() const;
private:
    void unshare();
    size_t size_ = 0;
    static constexpr size_t MAX_STATIC = sizeof(my_vector*) / sizeof(uint32_t);
    bool small = true;
    union {
        my_vector* dynamic_vec;
        uint32_t static_vec[MAX_STATIC];
    };
};

#endif //BIGINT_BUFFER_H
