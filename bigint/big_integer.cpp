#include "big_integer.h"

#include <iostream>
#include <cstring>
#include <cassert>
#include <climits>
#include <algorithm>

uint64_t const BASE = (uint64_t)UINT_MAX + (uint64_t)1;

void my_memcpy(std::vector<uint> &dst, std::vector<uint> const& src, size_t len) {
    for (size_t i = 0; i != len;i++) {
        dst[i] = src[i];
    }
}

big_integer::big_integer() : data_(1, 0), sign_(false) {}

big_integer::~big_integer() = default;

big_integer::big_integer(uint const& a) : data_(1, a), sign_(false) {}

big_integer::big_integer(int const& a) : data_(1, std::abs(static_cast<int64_t>(a))), sign_(a < 0) {}

const big_integer NUM[11] = {big_integer(0), big_integer(1), big_integer(2),
                             big_integer(3), big_integer(4), big_integer(5),
                             big_integer(6), big_integer(7), big_integer(8),
                             big_integer(9), big_integer(10)};

big_integer& big_integer::operator=(big_integer const& other) {
    sign_ = other.sign_;
    data_ = other.data_;
    return *this;
}

big_integer::big_integer(big_integer const& other) = default;

void big_integer::del_zero() {
    while (size() > 1 && data_.back() == 0) {
        data_.pop_back();
    }
}

big_integer::big_integer(std::string const& str) {
    data_ = storage_t(1, 0);
    sign_ = false;
    for (char i : str) {
        if (i > '9' || i < '0') {
            continue;
        }
        int x = i - '0';
        *this *= NUM[10];
        *this += NUM[x];
    }
    sign_ = (str[0] == '-');
}

inline size_t big_integer::size() const {
    return data_.size();
}

inline uint big_integer::data(size_t i) const {
    if (i >= size()) {
        return 0;
    }
    return data_[i];
}

inline bool big_integer::sign() const {
    return sign_;
}

big_integer abs(big_integer const& a) {
    if (a.sign()) {
        return -a;
    }
    return a;
}

big_integer& big_integer::operator-=(big_integer const& a) {
    if (sign_ != a.sign_) {
        *this += (-a);
        return *this;
    }

    if ((*this != a) && (*this < a) != sign_) {
        *this = (a - *this);
        sign_ ^= true;
        return *this;
    }
    uint x = 0;
    for (size_t i = 0; i < size(); i++) {
        uint y = 0;
        if (data_[i] < a.data(i) + x) {
            y = 1;
        }
        data_[i] -= (a.data(i) + x);
        x = y;
    }
    assert(x == 0);
    del_zero();
    return *this;
}

big_integer& big_integer::operator+=(big_integer const& a) {
    if (sign_ != a.sign_) {
        *this -= (-a);
        return *this;
    }
    if (size() < a.size()) {
        *this = (a + *this);
        return *this;
    }
    uint x = 0;
    for (size_t i = 0; i < size(); i++) {
        uint y = 0;
        if (UINT_MAX - data_[i] < a.data(i) + x) {
            y = 1;
        }
        data_[i] += (a.data(i) + x);
        x = y;
    }
    if (x) {
        data_.push_back(x);
    }
    return *this;
}

big_integer& big_integer::operator++() {
    *this += NUM[1];
    return *this;
}

big_integer& big_integer::operator--() {
    *this -= NUM[1];
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer tmp(*this);
    *this += NUM[1];
    return tmp;
}

big_integer big_integer::operator--(int) {
    big_integer tmp(*this);
    *this -= NUM[1];
    return tmp;
}

big_integer big_integer::mult_short(uint x) {
    big_integer b = *this;
    uint64_t t = 0;
    for (size_t i = 0; i < size(); i++) {
        uint64_t a = (uint64_t)b.data_[i] * (uint64_t)x;
        uint c = 0;
        if (UINT_MAX - t < a % BASE) {
            c = 1;
        }
        b.data_[i] = (a % BASE) + t;
        t = a / BASE + c;
    }
    if (t > 0) {
        b.data_.push_back(t);
    }
    return b;
}

big_integer& big_integer::operator*=(big_integer const& a) {
    if (a.size() == 0) {
        *this = a;
        return *this;
    }
    big_integer b(*this);
    big_integer c(b.mult_short(a.data(0)));
    for (size_t i = 1; i < a.size(); i++) {
        big_integer h = (b.mult_short(a.data(i)) << (CEIL_SIZE * i));
        c += h;
    }
    c.sign_ = (a.sign() != b.sign());
    *this = c;
    return *this;
}

big_integer div_mod(big_integer &a, big_integer &b, bool mod) {
    assert(b != NUM[0]);
    if (a == NUM[0]) {
        return a;
    }
    if (a < b) {
        return mod ? a : NUM[0];
    }
    int shift = 0;
    if (b.data_.back() < UINT_MAX / 2 + 1) {
        uint bb = b.data_.back();
        while (bb < UINT_MAX / 2 + 1) {
            bb <<= 1;
            shift++;
        }
        a <<= shift;
        b <<= shift;
    }
    size_t m = a.size() - b.size();
    big_integer q(0);
    if (a >= (b << static_cast<int32_t>(m * a.CEIL_SIZE))) {
        q = (NUM[1] << static_cast<int32_t>(m * a.CEIL_SIZE));
        a -= (b << static_cast<int32_t>(m * a.CEIL_SIZE));
    }
    while (q.size() < m) {
        q.data_.push_back(0);
    }
    for (int j = static_cast<int>(m) - 1; j >= 0; j--) {
        q.data_[j] += std::min(static_cast<uint64_t>(UINT_MAX),
                (static_cast<uint64_t>(a.data(b.size() + j)) * BASE + static_cast<uint64_t>(a.data(b.size() + j - 1)))
                                                    / static_cast<uint64_t>(b.data(b.size() - 1)));
        assert(q.size() > j);
        a -= ((big_integer(q.data_[j]) * b) << static_cast<int32_t>(j * a.CEIL_SIZE));
        while (a < NUM[0]) {
            q.data_[j]--;
            assert(q.data_[j] >= 0);
            a += (b << static_cast<int32_t>(j * a.CEIL_SIZE));
        }
    }
    if (mod) {
        return (a >> shift);
    }
    return q;
}

big_integer& big_integer::operator/=(big_integer const& a) {
    big_integer b = *this;
    big_integer x = abs(b), y = abs(a);
    *this = div_mod(x, y, false);
    sign_ = (b.sign_ != a.sign_);
    return *this;
}

big_integer& big_integer::operator%=(big_integer const& a) {
    big_integer b = *this;
    big_integer x = abs(b), y = abs(a);
    *this = div_mod(x, y, true);
    sign_ = b.sign_;
    return *this;
}

big_integer& big_integer::operator<<=(int shift) {
    if (shift == 0) {
        return *this;
    }
    assert(shift >= 0);
    if (*this == NUM[0]) {
        return *this;
    }
    int n = shift / CEIL_SIZE;
    for (int i = 0; i < n; i++) {
        data_.push_back(0);
    }
    for (int i = (int)size() - 1; i >= n; i--) {
        data_[i] = data_[i - n];
    }
    for (int i = 0; i < n; i++) {
        data_[i] = 0;
    }
    n = shift % CEIL_SIZE;
    if (n) {
        if ((UINT_MAX >> n) < data_.back()) {
            data_.push_back(0);
        }
        uint x = 0;
        for (size_t i = 0; i < size(); i++) {
            uint y = (data_[i] >> (CEIL_SIZE - n));
            data_[i] = (data_[i] << (uint)n) + x;
            x = y;
        }
    }
    return *this;
}

big_integer& big_integer::operator>>=(int shift) {
    assert(shift >= 0);
    if (*this == NUM[0]) {
        return *this;
    }
    size_t n = std::min(size(), shift / CEIL_SIZE);
    for (size_t i = 0; i < size() - n; i++) {
        data_[i] = data_[i + n];
    }
    for (size_t i = 0; i < n; i++) {
        data_.pop_back();
    }
    n = shift % CEIL_SIZE;
    if (n) {
        uint x = 0;
        for (int i = (int)size() - 1; i >= 0; i--) {
            uint y = (data_[i] << (CEIL_SIZE - n));
            data_[i] = (data_[i] >> n) + x;
            x = y;
        }
    }
    if (sign_ ) {
        *this -= NUM[1];
    }
    del_zero();
    return *this;
}

void translate_number(big_integer &a, size_t len) {
    while (a.size() < len) {
        a.data_.push_back(0);
    }
    if (a.sign()) {
        for (auto &i : a.data_) {
            i = ~i;
        }
        a--;
    }
}

big_integer bit_operation(big_integer const& x, big_integer const& y, std::function<uint(uint, uint)> f) {
    size_t len = std::max(x.size(), y.size());
    big_integer a(x), b(y);
    translate_number(a, len);
    translate_number(b, len);
    big_integer ans;
    ans.data_.resize(len);
    for (size_t i = 0; i < len; i++) {
        ans.data_[i] = f(a.data_[i], b.data_[i]);
    }
    ans.del_zero();
    ans.sign_ = f(x.sign_, y.sign_);
    translate_number(ans, len);
    ans.del_zero();
    return ans;
}

big_integer& big_integer::operator&=(big_integer const& a) {
    *this = bit_operation(*this, a, [] (uint a, uint b) {return a & b;});
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& a) {
    *this = bit_operation(*this, a, [] (uint a, uint b) {return a | b;});
    return *this;
}

big_integer& big_integer::operator^=(big_integer const& a) {
    *this = bit_operation(*this, a, [] (uint a, uint b) {return a ^ b;});
    return *this;
}

big_integer operator+(big_integer a, big_integer const& b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b) {
    return a %= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

big_integer operator&(big_integer a, big_integer const& b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b) {
    return a ^= b;
}

big_integer big_integer::operator-() const {
    big_integer b = *this;
    b.sign_ ^= true;
    return b;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator~() const {
    return (-*this - NUM[1]);
}

std::string to_string (big_integer const& a) {
    if (a < NUM[0]) {
        return "-" + to_string(-a);
    }
    std::string str;
    big_integer x = a;
    while (x > NUM[0]) {
        big_integer r = (x % NUM[10]);
        int n = 0;
        if (r > NUM[0]) {
            n = r.data_[0];
        }
        x /= NUM[10];
        str.push_back(n + '0');
    }
    if (str.empty()) {
        str = "0";
    }
    std::reverse(str.begin(), str.end());
    return str;
}

bool operator==(big_integer const& a, big_integer const& b) {
    return ((a.sign_ == b.sign_ && a.data_ == b.data_) ||
            (a.data_ == b.data_  && a.data_ == NUM[0].data_));
}

bool operator>(big_integer const& a, big_integer const& b) {
    return !(a <= b);
}

bool operator<(big_integer const& a, big_integer const& b) {
    if (a == b) {
        return false;
    }
    if (a.sign_ != b.sign_) {
        return !b.sign_;
    }
    if (a.size() != b.size()) {
        return (a.sign_ != (a.size() < b.size()));
    }
    for (int j = (int)a.size() - 1; j >= 0; j--) {
        if (a.data_[j] != b.data_[j]) {
            return (a.sign_ != (a.data_[j] < b.data_[j]));
        }
    }
    return false;
}

bool operator!=(big_integer const& a, big_integer const& b) {
    return !(a == b);
}

bool operator<=(big_integer const& a, big_integer const& b) {
    return (a < b || a == b);
}

bool operator>=(big_integer const& a, big_integer const& b) {
    return !(a < b);
}