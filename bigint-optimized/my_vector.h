#ifndef BIGINT_MY_VECTOR_H
#define BIGINT_MY_VECTOR_H

#include <iostream>
#include <vector>
#include <cassert>

struct my_vector {
    my_vector();
    explicit my_vector(std::vector<uint32_t> const& vec);
    my_vector(my_vector const& other);
    explicit my_vector(size_t len, uint32_t x = 0);
    my_vector(uint32_t* const& src, size_t len);
    my_vector& operator=(my_vector const& other);
    uint32_t& operator[] (size_t i);
    uint32_t const& operator[] (size_t i) const;
    my_vector* get_unique();
    void push_back(uint32_t x);
    void pop_back();
    void delete_vector();
    void inc_ref();
    void reverse();
    ~my_vector() = default;
private:
    size_t ref_counter;
    std::vector<uint32_t> vec;
    friend bool operator== (my_vector const& a, my_vector const& b);
};

#endif //BIGINT_MY_VECTOR_H
