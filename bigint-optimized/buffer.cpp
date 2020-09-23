#include "buffer.h"

#include <algorithm>
#include <vector>
#include <cstring>

buffer::buffer() {
    size_ = 0;
    small = true;
}

buffer::buffer(buffer const& other) : size_(other.size_), small(other.small) {
    if (small) {
        std::copy(other.static_vec, other.static_vec + size_, static_vec);
    } else {
        dynamic_vec = other.dynamic_vec;
        dynamic_vec->inc_ref();
    }
}

buffer::buffer(size_t len, uint32_t x) : size_(len), small(len <= MAX_STATIC) {
    if (small) {
        std::fill(static_vec, static_vec + len, x);
    } else {
        dynamic_vec = new my_vector(len, x);
    }
}

buffer& buffer::operator= (buffer const& other) {
    if (this == &other) {
        return *this;
    }
    this->~buffer();
    size_ = other.size_;
    small = other.small;
    if (small) {
        std::copy(other.static_vec, other.static_vec + size_, static_vec);
    } else {
        dynamic_vec = other.dynamic_vec;
        dynamic_vec->inc_ref();
    }
    return *this;
}

void buffer::resize(size_t new_size, uint32_t x) {
    while (new_size > size_) {
        push_back(x);
    }
    while (new_size < size_) {
        pop_back();
    }
}

buffer::~buffer() {
    if (!small) {
        dynamic_vec->delete_vector();
    }
}

bool buffer::operator== (buffer const& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (size_t i = 0; i < size_; i++) {
        if ((*this)[i] != other[i]) {
            return false;
        }
    }
    return true;
}

void buffer::reverse() {
    if (small) {
        std::reverse(static_vec, static_vec + size_);
    } else {
        unshare();
        dynamic_vec->reverse();
    }
}

uint32_t& buffer::operator[] (size_t i) {
    if (small) {
        return static_vec[i];
    } else {
        unshare();
        return (*dynamic_vec)[i];
    }
}

uint32_t const& buffer::operator[] (size_t i) const {
    if (small) {
        return static_vec[i];
    } else {
        return (*dynamic_vec)[i];
    }
}

void buffer::push_back(uint32_t const& x) {
    if (!small) {
        unshare();
        dynamic_vec->push_back(x);
    } else if (size_ < MAX_STATIC) {
        static_vec[size_] = x;
    } else {
        my_vector* tmp = new my_vector(static_vec, static_vec + size_);
        try {
            tmp->push_back(x);
        } catch (...) {
            delete tmp;
            throw;
        }
        dynamic_vec = tmp;
        small = false;
    }
    size_++;
}

uint32_t buffer::back() const {
    assert(size_ > 0);
    if (small) {
        return static_vec[size_ - 1];
    } else {
        return (*dynamic_vec)[size_ - 1];
    }
}

void buffer::pop_back() {
    assert(size_ > 0);
    if (!small) {
        unshare();
        dynamic_vec->pop_back();
    }
    size_--;
}

void buffer::unshare() {
    assert(!small);
    dynamic_vec = dynamic_vec->get_unique();
}

size_t buffer::size() const {
    return size_;
}
