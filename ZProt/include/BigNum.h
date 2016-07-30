#ifndef __BigNum_h__
#define __BigNum_h__

#define DIGIT	4
#define DEPTH	10000
#define MAX		200
typedef int		bignum_t[MAX + 1];

#define SGN(x)	((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#define ABS(x)	((x) > 0 ? (x) : -(x))


int					 read(bignum_t a, std::istream &is = std::cin);
int					 read(bignum_t a, int &sgn, std::istream &is = std::cin);
void				 write(const bignum_t a, std::ostream &os = std::cout);
int					 comp(const bignum_t a, const bignum_t b);
int					 comp(const bignum_t a, const int b);
int					 comp(const bignum_t a, const int c, const int d, const bignum_t b);
void				 add(bignum_t a, const bignum_t b);
void				 add(bignum_t a, const int b);
void				 sub(bignum_t a, const bignum_t b);
void				 sub(bignum_t a, const int b);
void				 sub(bignum_t a, const bignum_t b, const int c, const int d);
void				 mul(bignum_t c, const bignum_t a, const bignum_t b);
void				 mul(bignum_t a, const int b);
void				 mul(bignum_t b, const bignum_t a, const int c, const int d);
void				 div(bignum_t c, bignum_t a, const bignum_t b);
void				 div(bignum_t a, const int b, int &c);
void				 sqrt(bignum_t b, bignum_t a);
int					 length(const bignum_t a);
int					 digit(const bignum_t a, const int b);
int					 zeronum(const bignum_t a);
void				 comp(int *a, const int l, const int h, const int d);
void				 convert(int *a, const int h, bignum_t b);
void				 combination(bignum_t a, int m, int n);
void				 permutation(bignum_t a, int m, int n);


class CBigNum {
  public:
	inline CBigNum()
	{
		memset(num, 0, sizeof(bignum_t));
		num[0] = 1;
		sgn = 0;
	}

	inline CBigNum(const int a)
	{
		*this = a;
	}

	inline int operator!()
	{
		return num[0] == 1 && !num[1];
	}

	inline CBigNum &operator=(const CBigNum &a)
	{
		memcpy(num, a.num, sizeof(bignum_t));
		sgn = a.sgn;
		return *this;
	}

	inline CBigNum &operator=(const int a)
	{
		memset(num, 0, sizeof(bignum_t));
		num[0] = 1;
		sgn = SGN(a);
		add(num, sgn * a);
		return *this;
	}

	inline CBigNum &operator+=(const CBigNum &a)
	{
		if (sgn == a.sgn)
			add(num, a.num);
		else if (sgn && a.sgn)
		{
			int ret = comp(num, a.num);
			if (ret > 0)
				sub(num, a.num);
			else if (ret < 0)
			{
				bignum_t t;
				memcpy(t, num, sizeof(bignum_t));
				memcpy(num, a.num, sizeof(bignum_t));
				sub(num, t);
				sgn = a.sgn;
			}
			else
				memset(num, 0, sizeof(bignum_t)), num[0] = 1, sgn = 0;
		}
		else if (!sgn)
			memcpy(num, a.num, sizeof(bignum_t)), sgn = a.sgn;
		return *this;
	}

	inline CBigNum &operator+=(const int a)
	{
		if (sgn * a > 0)
			add(num, ABS(a));
		else if (sgn && a)
		{
			int ret = comp(num, ABS(a));
			if (ret > 0)
				sub(num, ABS(a));
			else if (ret < 0)
			{
				bignum_t t;
				memcpy(t, num, sizeof(bignum_t));
				memset(num, 0, sizeof(bignum_t));
				num[0] = 1;
				add(num, ABS(a));
				sgn = -sgn;
				sub(num, t);
			}
			else
				memset(num, 0, sizeof(bignum_t)), num[0] = 1, sgn = 0;
		}
		else if (!sgn)
			sgn = SGN(a), add(num, ABS(a));
		return *this;
	}

	inline CBigNum operator+(const CBigNum &a)
	{
		CBigNum	ret;
		memcpy(ret.num, num, sizeof(bignum_t));
		ret.sgn = sgn;
		ret += a;
		return ret;
	}

	inline CBigNum operator+(const int a)
	{
		CBigNum	ret;
		memcpy(ret.num, num, sizeof(bignum_t));
		ret.sgn = sgn;
		ret += a;
		return ret;
	}

	inline CBigNum &operator-=(const CBigNum &a)
	{
		if (sgn * a.sgn < 0)
			add(num, a.num);
		else if (sgn && a.sgn)
		{
			int ret = comp(num, a.num);
			if (ret > 0)
				sub(num, a.num);
			else if (ret < 0)
			{
				bignum_t t;
				memcpy(t, num, sizeof(bignum_t));
				memcpy(num, a.num, sizeof(bignum_t));
				sub(num, t);
				sgn = -sgn;
			}
			else
				memset(num, 0, sizeof(bignum_t)), num[0] = 1, sgn = 0;
		}
		else if (!sgn)
			add(num, a.num), sgn = -a.sgn;
		return *this;
	}

	inline CBigNum &operator-=(const int a)
	{
		if (sgn * a < 0)
			add(num, ABS(a));
		else if (sgn && a)
		{
			int ret = comp(num, ABS(a));
			if (ret > 0)
				sub(num, ABS(a));
			else if (ret < 0)
			{
				bignum_t t;
				memcpy(t, num, sizeof(bignum_t));
				memset(num, 0, sizeof(bignum_t));
				num[0] = 1;
				add(num, ABS(a));
				sub(num, t);
				sgn = -sgn;
			}
			else
				memset(num, 0, sizeof(bignum_t)), num[0] = 1, sgn = 0;
		}
		else if (!sgn)
			sgn = -SGN(a), add(num, ABS(a));
		return *this;
	}

	inline CBigNum operator-(const CBigNum &a)
	{
		CBigNum	ret;
		memcpy(ret.num, num, sizeof(bignum_t));
		ret.sgn = sgn;
		ret -= a;
		return ret;
	}

	inline CBigNum operator-(const int a)
	{
		CBigNum	ret;
		memcpy(ret.num, num, sizeof(bignum_t));
		ret.sgn = sgn;
		ret -= a;
		return ret;
	}

	inline CBigNum &operator*=(const CBigNum &a)
	{
		bignum_t t;
		mul(t, num, a.num);
		memcpy(num, t, sizeof(bignum_t));
		sgn *= a.sgn;
		return *this;
	}

	inline CBigNum &operator*=(const int a)
	{
		mul(num, ABS(a));
		sgn *= SGN(a);
		return *this;
	}

	inline CBigNum operator*(const CBigNum &a)
	{
		CBigNum	ret;
		mul(ret.num, num, a.num);
		ret.sgn = sgn * a.sgn;
		return ret;
	}

	inline CBigNum operator*(const int a)
	{
		CBigNum	ret;
		memcpy(ret.num, num, sizeof(bignum_t));
		mul(ret.num, ABS(a));
		ret.sgn = sgn * SGN(a);
		return ret;
	}

	inline CBigNum &operator/=(const CBigNum &a)
	{
		bignum_t t;
		div(t, num, a.num);
		memcpy(num, t, sizeof(bignum_t));
		sgn = (num[0] == 1 && !num[1]) ? 0 : sgn * a.sgn;
		return *this;
	}

	inline CBigNum &operator/=(const int a)
	{
		int t;
		div(num, ABS(a), t);
		sgn = (num[0] == 1 && !num[1]) ? 0 : sgn * SGN(a);
		return *this;
	}

	inline CBigNum operator/(const CBigNum &a)
	{
		CBigNum ret;
		bignum_t t;
		memcpy(t, num, sizeof(bignum_t));
		div(ret.num, t, a.num);
		ret.sgn = (ret.num[0] == 1 && !ret.num[1]) ? 0 : sgn * a.sgn;
		return ret;
	}

	inline CBigNum operator/(const int a)
	{
		CBigNum ret;
		int t;
		memcpy(ret.num, num, sizeof(bignum_t));
		div(ret.num, ABS(a), t);
		ret.sgn = (ret.num[0] == 1 && !ret.num[1]) ? 0 : sgn * SGN(a);
		return ret;
	}

	inline CBigNum &operator%=(const CBigNum &a)
	{
		bignum_t t;
		div(t, num, a.num);
		if (num[0] == 1 && !num[1]) sgn = 0;
		return *this;
	}

	inline int operator%=(const int a)
	{
		int t;
		div(num, ABS(a), t);
		memset(num, 0, sizeof(bignum_t));
		num[0] = 1;
		add(num, t);
		return t;
	}

	inline CBigNum operator%(const CBigNum &a)
	{
		CBigNum ret;
		bignum_t t;
		memcpy(ret.num, num, sizeof(bignum_t));
		div(t, ret.num, a.num);
		ret.sgn = (ret.num[0] == 1 && !ret.num[1]) ? 0 : sgn;
		return ret;
	}

	inline int operator%(const int a)
	{
		CBigNum ret;
		int t;
		memcpy(ret.num, num, sizeof(bignum_t));
		div(ret.num, ABS(a), t);
		memset(ret.num, 0, sizeof(bignum_t));
		ret.num[0] = 1;
		add(ret.num, t);
		return t;
	}

	inline CBigNum &operator++()
	{
		*this += 1;
		return *this;
	}

	inline CBigNum &operator--()
	{
		*this -= 1;
		return *this;
	};
	inline int operator>(const CBigNum &a)
	{
		return sgn > 0 ? (a.sgn > 0 ? comp(num, a.num) > 0 : 1) : (sgn < 0 ? (a.sgn < 0 ? comp(num, a.num) < 0 : 0) : a.sgn < 0);
	}

	inline int operator>(const int a)
	{
		return sgn > 0 ? (a > 0 ? comp(num, a) > 0 : 1) : (sgn < 0 ? (a < 0 ? comp(num, -a) < 0 : 0) : a < 0);
	}

	inline int operator>=(const CBigNum &a)
	{
		return sgn > 0 ? (a.sgn > 0 ? comp(num, a.num) >= 0 : 1) : (sgn < 0 ? (a.sgn < 0 ? comp(num, a.num) <= 0 : 0) : a.sgn <= 0);
	}

	inline int operator>=(const int a)
	{
		return sgn > 0 ? (a > 0 ? comp(num, a) >= 0 : 1) : (sgn < 0 ? (a < 0 ? comp(num, -a) <= 0 : 0) : a <= 0);
	}

	inline int operator < (const CBigNum &a)
	{
		return sgn < 0 ? (a.sgn < 0 ? comp(num, a.num) > 0 : 1) : (sgn > 0 ? (a.sgn > 0 ? comp(num, a.num) < 0 : 0) : a.sgn > 0);
	}
	inline int operator < (const int a)
	{
		return sgn < 0 ? (a < 0 ? comp(num, -a) > 0 : 1) : (sgn > 0 ? (a > 0 ? comp(num, a) < 0 : 0) : a > 0);
	}
	inline int operator<=(const CBigNum &a)
	{
		return sgn < 0 ? (a.sgn < 0 ? comp(num, a.num) >= 0 : 1) : (sgn > 0 ? (a.sgn > 0 ? comp(num, a.num) <= 0 : 0) : a.sgn >= 0);
	}

	inline int operator<=(const int a)
	{
		return sgn < 0 ? (a < 0 ? comp(num, -a) >= 0 : 1) : (sgn > 0 ? (a > 0 ? comp(num, a) <= 0 : 0) : a >= 0);
	}

	inline int operator==(const CBigNum &a)
	{
		return(sgn == a.sgn) ? !comp(num, a.num) : 0;
	}

	inline int operator==(const int a)
	{
		return(sgn * a >= 0) ? !comp(num, ABS(a)) : 0;
	}

	inline int operator!=(const CBigNum &a)
	{
		return(sgn == a.sgn) ? comp(num, a.num) : 1;
	}

	inline int operator!=(const int a)
	{
		return(sgn * a >= 0) ? comp(num, ABS(a)) : 1;
	}

	inline int operator[](const int a)
	{
		return digit(num, a);
	}

	friend inline std::istream &operator>>(std::istream &is, CBigNum &a)
	{
		read(a.num, a.sgn, is);
		return is;
	}

	friend inline std::ostream &operator<<(std::ostream &os, const CBigNum &a)
	{
		if (a.sgn < 0) os << '-';
		write(a.num, os);
		return os;
	}

	friend inline CBigNum sqrt(const CBigNum &a)
	{
		CBigNum ret;
		bignum_t t;
		memcpy(t, a.num, sizeof(bignum_t));
		sqrt(ret.num, t);
		ret.sgn = ret.num[0] != 1 || ret.num[1];
		return ret;
	}

	friend inline CBigNum sqrt(const CBigNum &a, CBigNum &b)
	{
		CBigNum	ret;
		memcpy(b.num, a.num, sizeof(bignum_t));
		sqrt(ret.num, b.num);
		ret.sgn = ret.num[0] != 1 || ret.num[1];
		b.sgn = b.num[0] != 1 || ret.num[1];
		return ret;
	}

	inline int length()
	{
		return ::length(num);
	}

	inline int zeronum()
	{
		return ::zeronum(num);
	}

	inline CBigNum C(const int m, const int n)
	{
		combination(num, m, n);
		sgn = 1;
		return *this;
	}

	inline CBigNum P(const int m, const int n)
	{
		permutation(num, m, n);
		sgn = 1;
		return *this;
	}

  private:
	bignum_t		 num;
	int				 sgn;
};

#endif
