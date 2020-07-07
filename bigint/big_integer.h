#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <iostream>
#include <vector>

struct big_integer
{
    using uint = uint32_t;
    using storage_t = std::vector<uint>;
    big_integer();
    big_integer(big_integer const& other);
    big_integer(int const& val);
    explicit big_integer(uint const& val);
    ~big_integer();
    explicit big_integer(std::string const& str);
    big_integer& operator=(big_integer const& other);

    big_integer operator-() const;
    big_integer operator+() const;
    big_integer operator~() const;

    big_integer& operator+=(big_integer const& a);
    big_integer& operator-=(big_integer const& a);
    big_integer& operator*=(big_integer const& a);
    big_integer& operator/=(big_integer const& a);
    big_integer& operator%=(big_integer const& a);
    big_integer& operator<<=(int shift);
    big_integer& operator>>=(int shift);
    big_integer& operator&=(big_integer const& a);
    big_integer& operator|=(big_integer const& a);
    big_integer& operator^=(big_integer const& a);
    big_integer& operator++();
    big_integer& operator--();
    big_integer operator++(int);
    big_integer operator--(int);

    size_t size() const;
    uint data(size_t i) const;
    bool sign() const;
    /*
    size_t capacity() const;
    void inc_capacity();
    void inc_size(uint x);
     */
    big_integer mult_short(uint x);
    size_t const CEIL_SIZE = 8 * sizeof(uint);

private:
    storage_t data_;
    // size_t size_;
    // size_t capacity_;
    bool sign_; // 0 if +, 1 if -
    void del_zero();


    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);

    friend std::string to_string(big_integer const& a);
    friend big_integer div_mod(big_integer a, big_integer b, bool mod);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);
big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);
big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer abs(big_integer const& a);
#endif // BIG_INTEGER_H