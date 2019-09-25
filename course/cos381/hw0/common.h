#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

template<int N>
struct Value {
    unsigned long x : N;

    Value<N>(unsigned long a = 0) {
	x = a;
    }

    Value<N> operator+(const unsigned long a) const {
	Value<N> result { .x = x + a };

	return result;
    }

    Value<N> operator+(const Value<N> a) const {
	Value<N> result { .x = x + a.x };

	return result;
    }

    Value<N> operator++() {
	++x;

	return *this;
    }

    Value<N> operator-=(const unsigned long a) {
	x -= a;

	return *this;
    }

    Value<N> operator<<=(const unsigned long a) {
	x <<= a;

	return *this;
    }

    Value<N> operator|=(const unsigned long a) {
	x |= a;

	return *this;
    }

    Value<N> operator&=(const unsigned long a) {
	x &= a;

	return *this;
    }

    bool operator!=(const Value<N> a) const {
	return x != a.x;
    }

    operator unsigned long() const {
	return x;
    }
};

typedef Value<32> word;
extern long current_time;

#endif
