#pragma once

#include <vector>
#include <string>

class BigInt {
public:
	BigInt();
	BigInt(int);
	BigInt(int64_t);
	BigInt(std::string);
	BigInt(const BigInt&);
	~BigInt() = default;	

	BigInt& operator=(const BigInt&);

	BigInt operator~() const;

	BigInt& operator++();
	const BigInt operator++(int);
	BigInt& operator--();
	const BigInt operator--(int);

	BigInt& operator+=(const BigInt&);
	BigInt& operator*=(const BigInt&);
	BigInt& operator-=(const BigInt&);
	BigInt& operator/=(const BigInt&);
	BigInt& operator^=(const BigInt&);
	BigInt& operator%=(const BigInt&);
	BigInt& operator&=(const BigInt&);
	BigInt& operator|=(const BigInt&);

	BigInt operator+() const;
	BigInt operator-() const;

	bool operator==(const BigInt&) const;
	bool operator!=(const BigInt&) const;
	bool operator<(const BigInt&) const;
	bool operator>(const BigInt&) const;
	bool operator<=(const BigInt&) const;
	bool operator>=(const BigInt&) const;

	template<class TYPE>
	operator TYPE() const;
	operator std::string() const;

	size_t size() const;

	std::vector<int8_t> get_Num() const; //A method that returns a vector with a big number
	bool get_Sign() const; //A method that returns true if the big number is negative

private:
	std::vector<int8_t> bigNum; //A vector storing a number in a little-endian
	bool is_neg = false; //Flag indicating negative number

	BigInt& push_front(int8_t num); //A method for inserting a number at the beginning of a vector
	std::pair<BigInt, BigInt> div_mod(const BigInt&); //A method that returns the quotient and the remainder of a division
	
	BigInt bin_to_dec() const; //A method that converts the binary representation of a big number to decimal
	BigInt dec_to_bin() const; //A method that returns the binary representation of a big number

	BigInt& add_base(const BigInt&, int8_t base = 10); //A method that sums numbers in a specified number system
};

BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator^(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator|(const BigInt&, const BigInt&);

std::ostream& operator<<(std::ostream& o, const BigInt& i);

BigInt abs(BigInt&); //A function that returns the absolute value of a big number
int8_t inverse_bit(int8_t& num); //A function that returns the inverted bit stored in a big number vector