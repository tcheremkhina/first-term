#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cassert>

template <typename T>
struct vector
{
    typedef T* iterator;
    typedef T const* const_iterator;

    vector();                      // O(1) nothrow
    vector(vector const& other);            // O(N) strong
    vector& operator=(vector const& ); // O(N) strong

    ~vector();                              // O(N) nothrow

    T& operator[](size_t i);                // O(1) nothrow
    T const& operator[](size_t i) const;    // O(1) nothrow

    T* data();                              // O(1) nothrow
    T const* data() const;                  // O(1) nothrow
    size_t size() const;                    // O(1) nothrow

    T& front();                             // O(1) nothrow
    T const& front() const;                 // O(1) nothrow

    T& back();                              // O(1) nothrow
    T const& back() const;                  // O(1) nothrow
    void push_back(T const&);               // O(1)* strong
    void pop_back();                        // O(1) nothrow

    bool empty() const;                     // O(1) nothrow

    size_t capacity() const;                // O(1) nothrow
    void reserve(size_t);                   // O(N) strong
    void shrink_to_fit();                   // O(N) strong

    void clear();                           // O(N) nothrow

    void swap(vector&);                     // O(1) nothrow

    iterator begin();                       // O(1) nothrow
    iterator end();                         // O(1) nothrow

    const_iterator begin() const;           // O(1) nothrow
    const_iterator end() const;             // O(1) nothrow

    iterator insert(iterator pos, T const&); // O(N) weak
    iterator insert(const_iterator pos, T const&); // O(N) weak

    iterator erase(iterator pos);           // O(N) weak
    iterator erase(const_iterator pos);     // O(N) weak

    iterator erase(iterator first, iterator last); // O(N) weak
    iterator erase(const_iterator first, const_iterator last); // O(N) weak

private:
    size_t index(T* ptr);
    size_t index(T const* ptr);

private:
    T* data_;
    size_t size_;
    size_t capacity_;
};

template <typename T>
vector<T>::vector() {
    size_ = 0;
    capacity_ = 0;
    data_ = nullptr;
}

template <typename T>
void destroy_all(T* data, size_t n) {
    for (size_t i = n; i > 0; i--) {
        data[i - 1].~T();
    }
}

template <typename TT>
void my_memcpy(TT const* src, TT* dst, size_t len) {
    size_t i = 0;
    try {
        for (; i != len; i++)
            new(dst + i) TT(src[i]);
    } catch (...) {
        destroy_all(dst, i);
        throw;
    }
}

template <typename T>
vector<T>::vector(vector const& other) {
    T *new_data = nullptr;
    if (other.size_ > 0) {
        new_data = static_cast<T*>(operator new(other.size_ * sizeof(T)));
    }
    try {
        my_memcpy(other.data_, new_data, other.size_);
    } catch (...) {
        operator delete(new_data);
        throw;
    }
    size_ = other.size_;
    capacity_ = other.size_;
    data_ = new_data;
}

template <typename T>
void vector<T>::swap(vector<T> &v) {
    std::swap(size_, v.size_);
    std::swap(capacity_, v.capacity_);
    std::swap(data_, v.data_);
}

template <typename T>
vector<T>& vector<T>::operator=(vector const& other) {
    vector<T> v(other);
    swap(v);
    return *this;
}

template <typename T>
vector<T>::~vector() {
    destroy_all(this->data_, this->size_);
    operator delete(data_);
}

template <typename T>
T& vector<T>::operator[](size_t i) {
    assert(i < capacity_);
    return data_[i];
}

template <typename T>
T const& vector<T>::operator[](size_t i) const {
    assert(i < capacity_);
    return data_[i];
}

template <typename T>
T* vector<T>::data() {
    return data_;
}

template <typename T>
T const* vector<T>::data() const {
    return data_;
}

template <typename T>
size_t vector<T>::size() const {
    return size_;
}

template <typename T>
T& vector<T>::front() {
    assert(size_ > 0);
    return data_[0];
}

template <typename T>
T const& vector<T>::front() const {
    assert(size_ > 0);
    return data_[0];
}

template <typename T>
T& vector<T>::back() {
    assert(size_ > 0);
    return data_[size_ - 1];
}

template <typename T>
T const& vector<T>::back() const {
    assert(size_ > 0);
    return data_[size_ - 1];
}

template <typename T>
void vector<T>::push_back(T const& x) {
    if (size_ < capacity_) {
        new (data_ + size_) T(x);
        size_++;
        return;
    }
    T a = T(x);
    if (!capacity_) {
        reserve(1);
    } else {
        reserve(2 * capacity_);
    }
    push_back(a);
}

template <typename T>
void vector<T>::pop_back() {
    assert(size_ > 0);
    data_[size_ - 1].~T();
    size_--;
}

template <typename T>
bool vector<T>::empty() const {
    return (size_ == 0);
}

template <typename T>
size_t vector<T>::capacity() const {
    return capacity_;
}

template <typename T>
void vector<T>::reserve(size_t n) {
    if (n > capacity_) {
        T *old_data = data_;
        data_ = static_cast<T*>(operator new(n * sizeof(T)));
        try {
            my_memcpy(old_data, data_, size_);
        } catch (...) {
            operator delete (data_);
            data_ = old_data;
            throw;
        }
        capacity_ = n;
        destroy_all(old_data, size_);
        operator delete(old_data);
    }
}

template <typename T>
void vector<T>::shrink_to_fit() {
    if (size_ < capacity_) {
        T *old_data = data_;
        data_ = NULL;
        if (size_ != 0) {
            data_ = static_cast<T*>(operator new(size_ * sizeof(T)));
            try {
                my_memcpy(old_data, data_, size_);
            } catch (...) {
                operator delete(data_);
                data_ = old_data;
                throw;
            }
        }
        capacity_ = size_;
        destroy_all(old_data, size_);
        operator delete(old_data);
    }
}

template <typename T>
void vector<T>::clear() {
    destroy_all(data_, size_);
    size_ = 0;
}

template <typename T>
T* vector<T>::begin() {
    return data_;
}

template <typename T>
T* vector<T>::end() {
    return data_ + size_;
}

template <typename T>
T const* vector<T>::begin() const {
    return data_;
}

template <typename T>
T const* vector<T>::end() const {
    return data_ + size_;
}

template <typename T>
size_t vector<T>::index(T* ptr) {
    T const* pos = ptr;
    return index(pos);
}

template <typename T>
size_t vector<T>::index(T const* ptr) {
    T* b = begin();
    size_t j = 0;
    while (b != ptr) {
        b++;
        j++;
    }
    return j;
}

template <typename T>
T* vector<T>::insert(T* pos, T const& x) {
    T const* pos1 = pos;
    return insert(pos1, x);
}

template <typename T>
T* vector<T>::insert(T const* pos, T const& x) {
    size_t j = index(pos);
    push_back(x);
    size_t i = size_ - 1;
    for (; i > j; i--) {
        data_[i] = data_[i - 1];
    }
    data_[i] = x;
    return data_ + j;
}

template <typename T>
T* vector<T>::erase(T const* pos) {
    size_t j = index(pos);
    for (size_t i = j; i + 1 < size_; i++) {
        data_[i] = data_[i + 1];
    }
    pop_back();
    return data_ + j;
}

template <typename T>
T* vector<T>::erase(T* pos) {
    T const* pos1 = pos;
    return erase(pos1);
}

template <typename T>
T* vector<T>::erase(T const* first, T const* last) {
    size_t l = index(first), r = index(last);
    assert(r >= l);
    size_t len = r - l;
    for (size_t i = r; i < size_; i++) {
        data_[i - len] = data_[i];
    }
    destroy_all(data_ + size_ - len, len);
    size_ -= len;
    return data_ + size_ - 1;
}

template <typename T>
T* vector<T>::erase(T* first, T* last) {
    T const* f = first;
    T const* l = last;
    return erase(f, l);
}

#endif // VECTOR_H
