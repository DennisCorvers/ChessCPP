#pragma once

template<typename T>
class Bitmask {

protected:
	T bits;

	Bitmask() : bits(0)
	{}
	Bitmask(const T& bits) : bits(bits)
	{}

public:
	T getMask() const {
		return bits;
	}

	void setBits(const T& value) {
		bits = value;
	}

	void setAllBits() {
		bits = 0;
		bits = (~bits);
	}

	bool matches(const Bitmask<T>& bits, const T& relevant = 0) const {
		return(relevant ? ((bits.bits & relevant) == (bits & relevant)) : (bits.bits == bits));
	}

	bool getBit(const T& pos)const {
		ASSERT(pos <= sizeof(T), "Position exceeds sideof bitmask");
		return ((bits&(1 << pos)) != 0);
	}

	void turnOnBit(const T& pos) {
		ASSERT(pos <= sizeof(T), "Position exceeds sideof bitmask");
		bits |= 1 << pos;
	}

	void turnOnBits(const T& bits) {
		bits |= bits;
	}

	void clearBit(const T& pos) {
		ASSERT(pos <= sizeof(T), "Position exceeds sideof bitmask");
		bits &= ~(1 << pos);
	}

	void toggleBit(const T& pos) {
		ASSERT(pos <= sizeof(T), "Position exceeds sideof bitmask");
		bits ^= 1 << pos;
	}

	void clear() {
		bits = 0;
	}
};

using CharMask = Bitmask<unsigned char>;
using ShortMask = Bitmask<unsigned short>;
using IntMask = Bitmask<unsigned int>;
using LongMask = Bitmask<unsigned long>;