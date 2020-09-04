#ifndef BIGINT_BUFFER_H
#define BIGINT_BUFFER_H

#include <iostream>
#include <cstring>
#include <algorithm>
#include "my_vector.h"

struct buffer {
    buffer() = default;

    buffer(buffer const& other) {
        size_ = other.size_;
        if (size_ > MAX_STATIC) {
            dynamic_vec = other.dynamic_vec;
            dynamic_vec->inc_ref();
        } else {
            std::memcpy(static_vec, other.static_vec, sizeof(static_vec));
        }
    }

    buffer(size_t len, uint32_t const& x) {
        size_ = len;
        if (len > MAX_STATIC) {
            dynamic_vec = new my_vector(len, x);
        } else {
            memset(static_vec, 0, sizeof(static_vec));
            for (size_t i = 0; i < len; i++) {
                static_vec[i] = x;
            }
        }
    }

    buffer& operator= (buffer const& other) {
        if (*this == other) {
            return *this;
        }
        if (other.size_ > MAX_STATIC && dynamic_vec == other.dynamic_vec) {
            assert(0);
        }
        this->~buffer();
        size_ = other.size_;
        if (size_ > MAX_STATIC) {
            dynamic_vec = other.dynamic_vec;
            dynamic_vec->inc_ref();
        } else {
            std::memcpy(static_vec, other.static_vec, sizeof(static_vec));
        }
        return *this;
    }

    void resize(size_t new_size, uint32_t const& x = 0) {
        buffer tmp = (*this);
        // this->~buffer();
        *this = buffer(new_size, x);
        tmp.~buffer();
    }

    explicit buffer(size_t len) {
        if (len > MAX_STATIC) {
            dynamic_vec = new my_vector(len);
        } else {
            memset(static_vec, 0, sizeof(static_vec));
        }
        size_ = len;
    }

    ~buffer() {
        if (size_ > MAX_STATIC) {
            dynamic_vec->delete_vector();
        }
    }

    bool operator== (buffer const& other) const {
        if (size_ != other.size_) {
            return false;
        }
        if (size_ > MAX_STATIC) {
            return (dynamic_vec->vec == other.dynamic_vec->vec);
        } else {
            for (size_t i = 0; i < size_; i++) {
                if (static_vec[i] != other.static_vec[i]) {
                    return false;
                }
            }
        }
        return true;
    }

    void reverse() {
        if (size_ > MAX_STATIC) {
            unshare();
            std::reverse(dynamic_vec->vec.begin(), dynamic_vec->vec.end());
        } else {
            std::reverse(static_vec, static_vec + size_);
        }
    }

    uint32_t& operator[] (size_t i) {
        if (size_ > MAX_STATIC) {
            unshare();
            return (*dynamic_vec)[i];
        } else {
            return static_vec[i];
        }
    }

    uint32_t const& operator[] (size_t i) const {
        if (size_ > MAX_STATIC) {
            return (*dynamic_vec)[i];
        } else {
            return static_vec[i];
        }
    }

    void push_back(uint32_t const& x) {
        if (size_ > MAX_STATIC) {
            unshare();
            dynamic_vec->push_back(x);
        } else if (size_ < MAX_STATIC) {
            static_vec[size_] = x;
        } else {
            std::vector<uint32_t> tmp;
            for (size_t i = 0; i < size_; i++) {
                tmp.push_back(static_vec[i]);
            }
            tmp.push_back(x);
            dynamic_vec = new my_vector(tmp);
        }
        size_++;
    }

    uint32_t back() const {
        assert(size_ > 0);
        if (size_ > MAX_STATIC) {
            return (*dynamic_vec)[size_ - 1];
        } else {
            return static_vec[size_ - 1];
        }
    }

    void pop_back() {
        assert(size_ > 0);
        if (size_ > MAX_STATIC) {
            unshare();
            dynamic_vec->pop_back();
            if (size_ == MAX_STATIC + 1) {
                std::vector<uint32_t> tmp = dynamic_vec->vec;
                this->~buffer();
                for (size_t i = 0; i + 1 < size_; i++) {
                    static_vec[i] = tmp[i];
                }
                for (size_t i = size_ - 1; i < MAX_STATIC; i++) {
                    static_vec[i] = 0;
                }
            }
        } else {
            static_vec[size_ - 1] = 0;
        }
        size_--;
    }

    void unshare() {
        assert (size_ > MAX_STATIC);
        dynamic_vec = dynamic_vec->get_unique();
    }

    uint32_t size() const {
        return size_;
    }

private:
    size_t size_ = 0;
    static const size_t MAX_STATIC = sizeof(my_vector*) / sizeof(uint32_t);
    union {
        my_vector* dynamic_vec;
        uint32_t static_vec[MAX_STATIC];
    };
};

#endif //BIGINT_BUFFER_H
