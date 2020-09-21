#include "buffer.h"

#include <algorithm>
#include <vector>
#include <cstring>

buffer::buffer() {
    size_ = 0;
    small = true;
}

buffer::buffer(buffer const& other) : size_(other.size_), small(other.small) {
    if (!small) {
        dynamic_vec = other.dynamic_vec;
        dynamic_vec->inc_ref();
    } else {
        std::copy(other.static_vec, other.static_vec + size_, static_vec);
    }
}

buffer::buffer(size_t len, uint32_t x) : size_(len), small(len <= MAX_STATIC) {
    if (!small) {
        dynamic_vec = new my_vector(len, x);
    } else {
        std::fill(static_vec, static_vec + len, x);
        /*if (len < MAX_STATIC) {
            std::fill(static_vec + len, static_vec + MAX_STATIC, 0);
        }*/
    }
}

buffer& buffer::operator= (buffer const& other) {
    if (this == &other) {
        return *this;
    }
    this->~buffer();
    size_ = other.size_;
    small = other.small;
    if (!small) {
        dynamic_vec = other.dynamic_vec;
        dynamic_vec->inc_ref();
    } else {
        std::copy(other.static_vec, other.static_vec + size_, static_vec);
    }
    return *this;
}

void buffer::resize(size_t new_size, uint32_t x) {
    if (!small) {
        buffer tmp = (*this);
        *this = buffer(new_size, x);
        tmp.~buffer();
    } else {
        *this = buffer(new_size, x);
    }
}

buffer::buffer(size_t len) : size_(len), small(len <= MAX_STATIC) {
    if (!small) {
        dynamic_vec = new my_vector(len);
    } else {
        std::fill(static_vec, static_vec + size_, 0);
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
    if (!small) {
        unshare();
        dynamic_vec->reverse();
    } else {
        std::reverse(static_vec, static_vec + size_);
    }
}

uint32_t& buffer::operator[] (size_t i) {
    if (!small) {
        unshare();
        return (*dynamic_vec)[i];
    } else {
        return static_vec[i];
    }
}

uint32_t const& buffer::operator[] (size_t i) const {
    if (!small) {
        return (*dynamic_vec)[i];
    } else {
        return static_vec[i];
    }
}

void buffer::push_back(uint32_t const& x) {
    if (!small) {
        unshare();
        dynamic_vec->push_back(x);
    } else if (size_ < MAX_STATIC) {
        static_vec[size_] = x;
    } else {
        dynamic_vec = new my_vector(static_vec, size_);
        dynamic_vec->push_back(x);
        small = false;
    }
    size_++;
}

uint32_t buffer::back() const {
    assert(size_ > 0);
    if (!small) {
        return (*dynamic_vec)[size_ - 1];
    } else {
        return static_vec[size_ - 1];
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
