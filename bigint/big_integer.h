#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <iostream>
#include <functional>
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

    big_integer mult_short(uint x);
    void div_mod_short(uint x, bool mod);
    size_t const CEIL_SIZE = 8 * sizeof(uint);

private:
    storage_t data_;
    bool sign_; // 0 if +, 1 if -
    void del_zero();


    friend void translate_number(big_integer &a, size_t len);
    friend big_integer bit_operation(big_integer const& x, big_integer const& y,
            std::function<uint (uint, uint)> f);
    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);

    friend std::string to_string(big_integer a);
    friend big_integer div_mod(big_integer &a, big_integer &b, bool mod);
    friend uint32_t trial (big_integer &a, big_integer &b, size_t k, size_t m);
    friend void print_data(big_integer x, std::string str); // need for debug only
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