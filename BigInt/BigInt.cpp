#pragma once

#include "BigInt.h"
#include <iostream>
#include <algorithm>

BigInt::BigInt() {

	this->BigInt::BigInt(0);
}

BigInt::BigInt(int num) {

	if (num < 0)
	{
		this->is_neg = true;
		num = -num;
	}

	do
	{
		this->bigNum.push_back(num % 10);
		num /= 10;

	} while (num);

}

BigInt::BigInt(int64_t num) {

	if (num < 0)
	{
		this->is_neg = true;
		num = -num;
	}

	do
	{
		this->bigNum.push_back(num % 10);
		num /= 10;

	} while (num);

}

BigInt::BigInt(std::string num) {

	if (num.empty())
		this->BigInt::BigInt(0);

	num.erase(remove(num.begin(), num.end(), ' '), num.end());

	auto rend = num.rend() - 1;

	if ((*rend) == '-' || (*rend) == '+')
		this->is_neg = (*rend == '-' ? true : false);
	else
		rend++;

	try
	{
		for (auto rit = num.rbegin(); rit != rend; rit++)
		{
			if (!isdigit(*rit))
			{
				this->bigNum.clear();
				this->is_neg = false;

				throw std::invalid_argument("String " + num + " is not a decimal integer\n");
			}

			this->bigNum.push_back((*rit) - '0');
		}
	}

	catch (std::invalid_argument e)
	{
		std::cerr << e.what() << std::endl;
	}
}

BigInt::BigInt(const BigInt& num) {

	this->BigInt::operator=(num);
}


BigInt& BigInt::operator=(const BigInt& num) {

	if (this == &num)
		return *this;

	if (this->bigNum.size() != num.bigNum.size())
		this->bigNum.resize(num.bigNum.size());

	copy(num.bigNum.begin(), num.bigNum.end(), this->bigNum.begin());

	this->is_neg = num.is_neg;

	return *this;
}


BigInt BigInt::operator~() const {

	BigInt bits = this->dec_to_bin();

	for (auto& bit : bits.bigNum)
		bit = inverse_bit(bit);

	bits.is_neg = bits.bigNum.back() ? true : false;

	return bits.bin_to_dec();
}


BigInt& BigInt::operator++() {

	return *this += 1;
}

const BigInt BigInt::operator++(int) {

	return (BigInt)(++(*this));
}

BigInt& BigInt::operator--() {

	return *this -= 1;
}

const BigInt BigInt::operator--(int) {

	BigInt res(*this);
	--(*this);

	return res;
}


BigInt& BigInt::operator+=(const BigInt& num) {

	return this->add_base(num);
}

BigInt& BigInt::operator*=(const BigInt& num) {

	BigInt zero;

	if (*this == zero || num == zero)
		return *this = zero;

	BigInt res;
	BigInt num1(*this);
	BigInt num2(num);

	if (abs(num1) < abs(num2))
		std::swap(num1, num2);

	for (size_t i = 0; i < num2.bigNum.size(); i++)
	{
		int8_t carry = 0;
		BigInt tmp;

		for (size_t j = 0; j < num1.bigNum.size() || carry; j++)
		{
			if (i + j > tmp.bigNum.size() - 1)
				tmp.bigNum.resize(i + j + 1);

			tmp.bigNum[i + j] = (j > num1.bigNum.size() - 1 ? 0 : num1.bigNum[j]) * num2.bigNum[i] + carry;
			carry = 0;

			if (tmp.bigNum[i + j] > 9)
			{
				carry = tmp.bigNum[i + j] / 10;
				tmp.bigNum[i + j] %= 10;
			}
		}

		res += tmp;
	}

	if (num1.is_neg != num2.is_neg)
		res.is_neg = true;

	return *this = res;
}

BigInt& BigInt::operator-=(const BigInt& num) {

	if (this->is_neg != num.is_neg)
		return *this += -num;

	bool carry = false;

	BigInt num1(*this);
	BigInt num2(num);

	if (abs(num1) < abs(num2))
	{
		std::swap(num1, num2);
		num1 = -num1;
	}

	for (size_t i = 0; i < num2.bigNum.size() || carry; i++)
	{
		if (i > num1.bigNum.size() - 1)
			num1.bigNum.push_back(0);

		num1.bigNum[i] -= (i > num2.bigNum.size() - 1 ? 0 : num2.bigNum[i]);

		if (carry)
		{
			carry = false;

			if (!num1.bigNum[i])
			{
				num1.bigNum[i] = 9;
				carry = true;
			}

			else
				num1.bigNum[i]--;
		}

		if (num1.bigNum[i] < 0)
		{
			carry = true;
			num1.bigNum[i] += 10;
		}
	}

	while (!num1.bigNum.back() && num1.bigNum.size() > 1)
		num1.bigNum.pop_back();

	if (!num1.bigNum[0] && num1.bigNum.size() == 1)
		num1.is_neg = false;

	return *this = num1;
}

BigInt& BigInt::operator/=(const BigInt& num) {

	return *this = this->div_mod(num).first;
}

BigInt& BigInt::operator^=(const BigInt& num) {

	BigInt num1(this->dec_to_bin());
	BigInt num2(num.dec_to_bin());

	if (num1.bigNum.size() < num2.bigNum.size())
		std::swap(num1, num2);

	for (size_t i = 0; i < num1.bigNum.size(); i++)
		num1.bigNum[i] = num2.bigNum.size() - 1 < i ? num1.bigNum[i] ^ 0 : num1.bigNum[i] ^ num2.bigNum[i];

	num1.is_neg = num1.bigNum.back() ? true : false;

	return *this = num1.bin_to_dec();
}

BigInt& BigInt::operator%=(const BigInt& num) {

	return *this = this->div_mod(num).second;
}

BigInt& BigInt::operator&=(const BigInt& num) {

	BigInt num1(this->dec_to_bin());
	BigInt num2(num.dec_to_bin());

	if (num1.bigNum.size() < num2.bigNum.size())
		std::swap(num1, num2);

	for (size_t i = 0; i < num1.bigNum.size(); i++)
		num1.bigNum[i] = num2.bigNum.size() - 1 < i ? num1.bigNum[i] & 0 : num1.bigNum[i] & num2.bigNum[i];

	num1.is_neg = num1.bigNum.back() ? true : false;

	return *this = num1.bin_to_dec();
}

BigInt& BigInt::operator|=(const BigInt& num) {

	BigInt num1(this->dec_to_bin());
	BigInt num2(num.dec_to_bin());

	if (num1.bigNum.size() < num2.bigNum.size())
		std::swap(num1, num2);

	for (size_t i = 0; i < num1.bigNum.size(); i++)
		num1.bigNum[i] = num2.bigNum.size() - 1 < i ? num1.bigNum[i] | 0 : num1.bigNum[i] | num2.bigNum[i];

	num1.is_neg = num1.bigNum.back() ? true : false;

	return *this = num1.bin_to_dec();
}


BigInt BigInt::operator+() const {

	return *this;
}

BigInt BigInt::operator-() const {

	BigInt tmp(*this);
	tmp.is_neg = !tmp.is_neg;

	return tmp;
}


bool BigInt::operator==(const BigInt& num) const {

	if (this == &num)
		return true;

	if (this->is_neg != num.is_neg)
		return false;

	if (this->bigNum.size() != num.bigNum.size())
		return false;

	for (size_t i = 0; i < this->bigNum.size(); i++)
		if (this->bigNum[i] != num.bigNum[i])
			return false;

	return true;
}

bool BigInt::operator!=(const BigInt& num) const {

	return !(*this == num);
}

bool BigInt::operator<(const BigInt& num) const {

	if (*this == num)
		return false;

	if (this->is_neg && !num.is_neg)
		return true;

	if (!this->is_neg && num.is_neg)
		return false;

	if (this->bigNum.size() != num.bigNum.size())
	{
		if (this->is_neg)
			return !(this->bigNum.size() < num.bigNum.size());

		return this->bigNum.size() < num.bigNum.size();
	}

	for (int64_t i = this->bigNum.size() - 1; i >= 0; i--)
		if (this->bigNum[i] != num.bigNum[i])
		{
			if (this->is_neg)
				return !(this->bigNum[i] < num.bigNum[i]);

			return this->bigNum[i] < num.bigNum[i];
		}
}

bool BigInt::operator>(const BigInt& num) const {

	return !(*this <= num);
}

bool BigInt::operator<=(const BigInt& num) const {

	return (*this < num || *this == num);
}

bool BigInt::operator>=(const BigInt& num) const {

	return (*this > num || *this == num);
}

BigInt::operator std::string() const {

	std::string str;

	if (this->is_neg)
		str.push_back('-');

	for (auto rit = this->bigNum.rbegin(); rit != this->bigNum.rend(); rit++)
		str.push_back((*rit) + '0');

	return str;
}

template<class TYPE>
BigInt::operator TYPE() const {

	BigInt bits = this->dec_to_bin();
	int8_t cnt = sizeof(TYPE) * 8;

	if (bits.bigNum.size() >= cnt)
		bits.is_neg = bits.bigNum[cnt - 1];

	bits.bigNum.resize(cnt);

	bits = bits.bin_to_dec();

	TYPE res = 0;

	for (size_t i = 0; i < bits.bigNum.size(); i++)
		res += bits.bigNum[i] * std::pow(10, i);

	return (bits.is_neg ? -res : res);
}

size_t BigInt::size() const {

	return sizeof(BigInt) + this->bigNum.size() * sizeof(int8_t);
}


std::vector<int8_t> BigInt::get_Num() const {

	return this->bigNum;
}

bool BigInt::get_Sign() const {

	return this->is_neg;
}


BigInt& BigInt::push_front(int8_t num) {

	if (!(this->bigNum.size() == 1 && !this->bigNum[0]))
		this->bigNum.push_back(0);

	std::copy(this->bigNum.begin(), this->bigNum.end() - 1, this->bigNum.begin() + 1);

	this->bigNum[0] = num;

	return *this;
}

std::pair<BigInt, BigInt> BigInt::div_mod(const BigInt& num) {

	const BigInt zero;

	if (num == zero)
		throw "Division by zero";

	std::pair<BigInt, BigInt> res;

	BigInt num1(*this);
	BigInt num2(num);

	num1.is_neg = false;
	num2.is_neg = false;

	if (this->is_neg != num.is_neg)
		res.first.is_neg = true;

	if (num1 < num2)
	{
		res.first = zero;
		res.second = *this;

		return res;
	}

	bool carry = false;

	while (1)
	{
		while (res.second < num2 || carry)
		{
			if (!num1.bigNum.size())
			{
				if (this->is_neg && res.second != zero)
					res.second.is_neg = true;

				return res;
			}

			res.second.push_front(num1.bigNum.back());
			num1.bigNum.pop_back();

			if (carry)
			{
				if (res.second < num2)
					res.first.push_front(0);
				else
					carry = false;
			}
		}
		int8_t val = 0;

		while (res.second >= num2)
		{
			val++;
			res.second -= num2;
		}

		res.first.push_front(val);
		carry = true;
	}
}


BigInt BigInt::bin_to_dec() const {

	BigInt num;
	num.is_neg = this->is_neg;

	bool is_begin = false;

	for (auto rit = this->bigNum.rbegin(); rit != this->bigNum.rend(); rit++)
	{
		if (!is_begin)
		{
			if (!(*rit))
				continue;

			is_begin = true;

			if (this->is_neg)
				continue;
		}

		num *= 2;

		if (!num.is_neg && (*rit))
			++num;

		else if (num.is_neg && !(*rit))
			--num;

	}

	return num.is_neg ? --num : num;
}

BigInt BigInt::dec_to_bin() const {

	BigInt bits;
	bits.bigNum.pop_back();

	std::vector<int8_t> tmp = this->bigNum;

	if (this->is_neg)
		bits.is_neg = true;

	auto check = [&]()
	{
		for (auto i : tmp)
			if (i)
				return true;

		return false;
	};

	auto div = [&]()
	{
		int8_t res = tmp[0] % 2;
		tmp[0] /= 2;

		for (size_t i = 1; i < tmp.size(); i++)
		{
			if (tmp[i] % 2)
				tmp[i - 1] += 5;

			tmp[i] /= 2;
		}

		return (this->is_neg ? inverse_bit(res) : res);
	};

	while (check() || !bits.bigNum.size() || bits.bigNum.size() % 8)
		bits.bigNum.push_back(div());

	if (!bits.bigNum.back() && bits.is_neg)
		bits.bigNum.push_back(1);

	return this->is_neg ? bits.add_base(-1, 2) : bits;
}


BigInt& BigInt::add_base(const BigInt& num, int8_t base) {

	if (this->is_neg != num.is_neg)
	{
		if (this->is_neg)
			return *this = num - (-*this);

		else if (num.is_neg)
			return *this -= -num;
	}

	bool carry = false;
	size_t len = std::max(this->bigNum.size(), num.bigNum.size());

	for (size_t i = 0; i < len || carry; i++)
	{
		if (i > this->bigNum.size() - 1)
			this->bigNum.push_back(0);

		this->bigNum[i] += (i > num.bigNum.size() - 1 ? 0 : num.bigNum[i]);

		if (carry)
		{
			this->bigNum[i]++;
			carry = false;
		}

		if (this->bigNum[i] > base - 1)
		{
			this->bigNum[i] -= base;
			carry = true;
		}
	}

	return *this;
}


BigInt operator+(const BigInt& num1, const BigInt& num2) {

	return BigInt(num1) += num2;
}

BigInt operator-(const BigInt& num1, const BigInt& num2) {

	return BigInt(num1) -= num2;
}

BigInt operator*(const BigInt& num1, const BigInt& num2) {

	BigInt res(num1);

	return res *= num2;
}

BigInt operator/(const BigInt& num1, const BigInt& num2) {

	BigInt res(num1);

	return res /= num2;
}

BigInt operator^(const BigInt& num1, const BigInt& num2) {

	BigInt res(num1);

	return res ^= num2;
}

BigInt operator%(const BigInt& num1, const BigInt& num2) {

	BigInt res(num1);

	return res %= num2;
}

BigInt operator&(const BigInt& num1, const BigInt& num2) {

	BigInt res(num1);

	return res &= num2;
}

BigInt operator|(const BigInt& num1, const BigInt& num2) {

	BigInt res(num1);

	return res |= num2;
}


std::ostream& operator<<(std::ostream& o, const BigInt& i) {

	std::vector<int8_t> vec = i.get_Num();

	if (i.get_Sign())
		o << '-';

	for (int64_t i = vec.size() - 1; i >= 0; i--)
		o << (int)vec[i];

	return o;
}


BigInt abs(BigInt& num) {

	return (num < (BigInt)0 ? -num : num);
}

int8_t inverse_bit(int8_t& bit) {

	return (!bit ? 1 : 0);
}

