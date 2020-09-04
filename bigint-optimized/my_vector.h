#ifndef BIGINT_MY_VECTOR_H
#define BIGINT_MY_VECTOR_H

#include <iostream>
#include <vector>
#include <cassert>

struct my_vector {

    my_vector() : ref_counter(1) {};

    explicit my_vector(std::vector<uint32_t> const& vec) : vec(vec), ref_counter(1) {};

    my_vector(my_vector const& other) : vec(other.vec), ref_counter(1) {};

    explicit my_vector(size_t const& len, uint32_t const& x = 0) {
        vec.resize(len, x);
        ref_counter = 1;
    }

    my_vector& operator=(my_vector const& other) {
        ref_counter = 1;
        vec = other.vec;
        return *this;
    }

    uint32_t& operator[] (size_t i) {
        return vec[i];
    }

    uint32_t const& operator[] (size_t i) const {
        return vec[i];
    }

    my_vector* get_unique() {
        if (ref_counter == 1) {
            return this;
        }
        assert(ref_counter > 0);
        ref_counter--;
        return new my_vector(vec);
    }

    void push_back(uint32_t const& x) {
        vec.push_back(x);
    }

    void pop_back() {
        vec.pop_back();
    }

    void delete_vector() {
        assert(ref_counter);
        if (ref_counter == 1) {
            delete this;
        } else {
            ref_counter--;
        }
    }

    void inc_ref() {
        ref_counter++;
    }

    ~my_vector() = default;

    std::vector<uint32_t> vec;
private:
    size_t ref_counter;

};

#endif //BIGINT_MY_VECTOR_H
