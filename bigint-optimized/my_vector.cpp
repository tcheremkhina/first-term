#include "my_vector.h"

#include <iostream>
#include <cstring>
#include <cassert>
#include <algorithm>

my_vector::my_vector() : ref_counter(1) {}

my_vector::my_vector(std::vector<uint32_t> const& vec) : ref_counter(1), vec(vec) {}

my_vector::my_vector(my_vector const& other) : ref_counter(1), vec(other.vec) {}

my_vector::my_vector(uint32_t* first, uint32_t* last) : ref_counter(1), vec(std::vector<uint32_t> (first, last)) {}

my_vector::my_vector(size_t len, uint32_t x) : ref_counter(1), vec(std::vector<uint32_t> (len, x)) {}

uint32_t& my_vector::operator[] (size_t i) {
    return vec[i];
}

uint32_t const& my_vector::operator[] (size_t i) const {
    return vec[i];
}

my_vector* my_vector::get_unique() {
    if (ref_counter == 1) {
        return this;
    }
    assert(ref_counter > 0);
    ref_counter--;
    return new my_vector(vec);
}

void my_vector::push_back(uint32_t x) {
    vec.push_back(x);
}

void my_vector::pop_back() {
    vec.pop_back();
}

void my_vector::delete_vector() {
    assert(ref_counter > 0);
    if (ref_counter == 1) {
        delete this;
    } else {
        ref_counter--;
    }
}

void my_vector::inc_ref() {
    ref_counter++;
}

bool operator== (my_vector const& a, my_vector const& b) {
    return a.vec == b.vec;
}

void my_vector::reverse() {
    std::reverse(vec.begin(), vec.end());
}
