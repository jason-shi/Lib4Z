#include "StdAfx.h"
#include "RSACrypt.h"

using namespace std;

#define MAXL		 200 //定义大整数的最大位数，200完全可以满足p与q达到768bit位的要求，可以选择更大的数
#define GREAT		 1 //定义大整数比较时的返回值
#define EQUAL		 0
#define LOW			 -1

Lint				 ZERO, ONE, TWO; //定义常用大整数0、1、2

int Init_Lint(Lint *a, int n, unsigned int value)
{
	if (a == NULL)
		return 0;
	else
	{
		a->length = 0;
		for (int i = 0; i < MAXL; i++)
			a->n[i] = 0;
		for (int i = 0; i < n; i++)
			a->n[i] = value;
		a->length = n;
		return 1;
	}
}

void Set_Lint(Lint *a, int n, unsigned int value)
{
	if (a->length < n)
		a->length = n;
	a->n[n - 1] = value;
}

int Copy_Lint(Lint *a, Lint *b)
{
	if (a == NULL || b == NULL)
		return 0;
	if (b->length == 0)
	{
		Init_Lint(a, 0, 0);
		return 1;
	}
	for (int i = 0; i < MAX; i++)
		a->n[i] = b->n[i];
	a->length = b->length;
	return 1;
}

int Split_Lint(Lint *a, Lint *b, int n1, int n2)
{
	if (n1 < 1 || n2 > b->length)
		return 0;
	for (int i = n1 - 1; i < n2; i++)
		a->n[i - n1 + 1] = b->n[i];
	a->length = n2 - n1 + 1;
	return 1;
}

int Cmp_Lint(Lint a, Lint b)
{
	int i = a.length;
	while (1)
	{
		if (a.n[i - 1] == 0 && a.length > 1)
			a.length--;
		else
			break;
		i--;
	}
	i = b.length;
	while (1)
	{
		if (b.n[i - 1] == 0 && b.length > 1)
			b.length--;
		else
			break;
		i--;
	}
	if (a.length > b.length) return GREAT;
	if (a.length < b.length) return LOW;

	i = a.length - 1;
	while (i >= 0)
	{
		if (a.n[i] > b.n[i]) return GREAT;
		if (a.n[i] < b.n[i]) return LOW;
		i--;
	}
	return EQUAL;
}

void Lint_Add(Lint *a, Lint *b, Lint *c)
{
	unsigned long temp = 0;
	int i = 0, carry = 0;
	Init_Lint(c, 0, 0);
	while (i < a->length && i < b->length)
	{
		temp = a->n[i] + b->n[i] + carry;
		c->n[i] = temp & 0xFFFF;
		carry = temp >> 16;
		i++;
		c->length = i;
	}
	while (i < a->length)
	{
		temp = a->n[i] + carry;
		c->n[i] = temp & 0xFFFF;
		carry = temp >> 16;
		i++;
		c->length = i;
	}
	while (i < b->length)
	{
		temp = b->n[i] + carry;
		c->n[i] = temp & 0xFFFF;
		carry = temp >> 16;
		i++;
		c->length = i;
	}
	if (carry > 0)
	{ 
		c->n[i] = carry;
		c->length = i + 1;
	}
}

int Lint_Sub(Lint *a, Lint *b, Lint *c)
{
	unsigned long temp = 0;
	int i = 0, carry = 1;
	Init_Lint(c, 0, 0);
	if (Cmp_Lint(*a, *b) == LOW)
		return 0;
	while (i < b->length)
	{
		if (carry == 1)
			temp = a->n[i] - b->n[i] + 65536;
		else
			temp = a->n[i] - b->n[i] + 65535;
		c->n[i] = temp & 0xFFFF;
		carry = temp >> 16;
		i++;
		c->length = i;
	}
	while (i < a->length)
	{
		if (carry == 1)
			temp = a->n[i] + 65536;
		else
			temp = a->n[i] + 65535;
		c->n[i] = temp & 0xFFFF;
		carry = temp >> 16;
		i++;
		c->length = i;
	}
	while (1)
	{
		if (c->n[i - 1] == 0 && c->length > 1)
			c->length--;
		else
			break;
		i--;
	}
	return 1;
}

void Lint_Mul(Lint *a, Lint *b, Lint *c)
{
	int i, j;
	unsigned long temp1 = 0, temp2 = 0, carry = 0; 
	Init_Lint(c ,0, 0);
	for (i = 0; i < a->length; i++)
	{
		for (j = 0, carry = 0; j < b->length; j++)
		{
			temp1 = a->n[i] * b->n[j] + c->n[i + j] + carry;
			c->n[i + j] = temp1 & 0xFFFF;
			carry = temp1 >> 16;
		}
		if (carry > 0)
		{
			c->n[i + b->length] = carry;
		}
	}
	c->length = a->length + b->length;
	i = c->length;
	while (1)
	{
		if (c->n[i - 1] == 0 && c->length > 1)
			c->length--;
		else
			break;
		i--;
	}
}

int Lint_Div(Lint *a, Lint *b, Lint *q, Lint *r)
{
	Lint R1, R2, B1, B2;
	Lint temp_Lint0, temp_Lint1, temp_Lint2;
	Lint Q1, Q_Lint, SubB, SubR, d;
	unsigned int i, j, n, Q;
	int flag1, flag2;
	unsigned long temp;

	Init_Lint(&d, 0, 0);
	Init_Lint(&R1, 0, 0);
	Init_Lint(&R2, 0, 0);
	Init_Lint(&Q1, 0, 0);
	Init_Lint(&B1, 0, 0);
	Init_Lint(&B2, 0, 0);
	Init_Lint(&temp_Lint0, 0, 0);
	Init_Lint(&temp_Lint1, 0, 0);
	Init_Lint(&temp_Lint2, 0, 0);
	Init_Lint(&Q_Lint, 0, 0);
	Init_Lint(&SubB, 0, 0);
	Init_Lint(&SubR, 0, 0);
	Init_Lint(q, 0, 0);
	Init_Lint(r, 0, 0);
	Copy_Lint(&R1, a);
	Set_Lint(&R1, a->length + 1, 0);
	Copy_Lint(&B1, b);
	Copy_Lint(&B2, b);
	Set_Lint(&d, 1, 1);
	Set_Lint(&d, 1, 1);
	i = R1.length - 1;
	j = a->length - b->length;
	n = B1.length;
	flag1 = Cmp_Lint(*b, ZERO);
	if (a == NULL || b == NULL || flag1 == 0)
		return 0;
	if (Cmp_Lint(B1, ZERO) == 0)
		return 0;
	if (Cmp_Lint(R1, B1) < 0)
	{
		Copy_Lint(q, &ZERO);
		Copy_Lint(r, a);
		return 1;
	}
	if (Cmp_Lint(R1, B1) == 0)
	{
		Copy_Lint(q, &ONE);
		Copy_Lint(r, &ZERO);
		return 1;
	}
	while (i >= n)
	{
		if (Cmp_Lint(R1, B1) <= 0)
		{
			Q = 0;
		}
		else
		{
			temp = (R1.n[i] * 65536 + R1.n[i - 1]) / B1.n[n - 1];
			temp = temp < 65535 ? temp : 65535;
			Split_Lint(&SubR, &R1, i - n + 1, i + 1);
			do
			{
				Set_Lint(&Q_Lint, 1, temp);
				Lint_Mul(&B1, &Q_Lint, &temp_Lint1);
				flag1=Cmp_Lint(SubR, temp_Lint1);
				Set_Lint(&Q_Lint, 1, temp + 1);
				Lint_Mul(&B1, &Q_Lint, &temp_Lint1);
				flag2=Cmp_Lint(SubR, temp_Lint1);
				if (flag1 == 0) { Q = temp; break; }
				if (flag2 == 0) { Q = temp + 1; break; }
				if (flag1 < 0 && flag2 < 0) temp--;
				if (flag1 > 0 && flag2 < 0) { Q = temp; break; }
				if (flag1 > 0 && flag2 > 0) temp++;
			} while (1);

			Set_Lint(&Q_Lint, 1, Q);
			Lint_Mul(&B1, &Q_Lint, &temp_Lint2);
			Lint_Sub(&SubR, &temp_Lint2, &temp_Lint0);
			for (int k = 0; k <= n; k++)
				R1.n[i - n + k] = temp_Lint0.n[k];
		}
		Set_Lint(&Q1, j + 1, Q);
		i--;
		j--;
	}
	i = R1.length;
	while (1)
	{
		if (R1.n[i - 1] == 0 && R1.length > 1)
			R1.length--;
		else
			break;
		i--;
	}
	i = Q1.length;
	while (1)
	{
		if (Q1.n[i - 1] == 0 && Q1.length > 1)
			Q1.length--;
		else
			break;
		i--;
	}
	Copy_Lint(r, &R1);
	Copy_Lint(q, &Q1);
	return 1;
}

//实现大整数的模减运算
int Mod_Sub(Lint a, Lint b, Lint *c, Lint *m)
{
	Lint temp0,temp1;
	if (Cmp_Lint(a,b)>=0)
	{
		Lint_Sub(&a,&b,&temp0);
		Lint_Div(&temp0,m,&a,c);
	}
	else
	{
		Lint_Sub(&b,&a,&temp0);
		Lint_Div(&temp0,m,&a,&temp1);
		if (Cmp_Lint(temp1,ZERO)>0)
			Lint_Sub(m,&temp1,c);
		else
			Copy_Lint(c,&ZERO);
	}
	return 1;
} 

//用扩展欧几里德算法求a模n的乘法逆元v，和a与n的最大公约数
int Euclid(Lint n, Lint a, Lint *v, Lint *gcd)
{
	Lint u, g, v1, v3, q, t1, t3, temp1, temp2, temp3;
	int i;
	Copy_Lint(&u, &ONE);
	Copy_Lint(&t1, &ONE);
	Copy_Lint(&v1, &ZERO);
	Copy_Lint(&g, &a);
	Copy_Lint(&v3, &n);
	i = Cmp_Lint(v3, ZERO);
	if (i == 0)
		return 0;
	while (i > 0)
	{
		Lint_Div(&g, &v3, &q, &t3);
		Lint_Mul(&q, &v1, &temp1);
		Lint_Div(&temp1, &n, &temp3, &temp2);
		Mod_Sub(u, temp2, &t1, &n);
		Copy_Lint(&u, &v1);
		Copy_Lint(&v1, &t1);
		Copy_Lint(&g, &v3);
		Copy_Lint(&v3, &t3);
		i = Cmp_Lint(v3, ZERO);
	}
	Copy_Lint(v, &u);
	Copy_Lint(gcd, &g);
	return 1;
}

//用二进制算法进行模幂运算
int Mexp_Lint(Lint a, Lint e, Lint *p, Lint m)
{
	unsigned int r, bit[] = {
		1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768,
	};
	int i, n, s, t;
	Lint temp0, temp1, temp2;
	Init_Lint(&temp0, 0, 0);
	Init_Lint(&temp1, 0, 0);
	Init_Lint(&temp2, 0, 0);

	if (Cmp_Lint(m, ZERO) == 0)
		return 0;
	if (Cmp_Lint(m, ONE) == 0)
	{
		Copy_Lint(p, &ZERO);
		return 1;
	}
	else if (Cmp_Lint(a, ZERO) == 0)
	{
		if (Cmp_Lint(e, ZERO) == 0)
		{
			Copy_Lint(p, &ONE);
			return 1;
		}
		if (Cmp_Lint(e, ZERO) > 0)
		{
			Copy_Lint(p, &ZERO);
			return 1;
		}
	}
	n = e.length * 16;
	r = e.n[e.length - 1] & bit[15];
	if (r > 0)
		Copy_Lint(p, &a);
	else
		Copy_Lint(p, &ONE);
	i = n - 2;
	while (i >= 0)
	{
		Lint_Mul(p, p, &temp1);
		Lint_Div(&temp1, &m, &temp0, p);
		s = i >> 4;
		t = i & 0x0F;
		r = e.n[s] & bit[t];
		if (r > 0)
		{
			Lint_Mul(p, &a, &temp0);
			Lint_Div(&temp0, &m, &temp1, p);
		}
		i--;
	}
	return 1;
}

//伪随机生成n位大整数p
int Rand_Lint(Lint *p, int n)
{
	unsigned long a = 65539, b = 65539, seed1, seed0;
	unsigned long temp;
	unsigned long m = 65536;
	seed1 = rand();
	seed0 = rand();
	for (int i = 0; i < n; i++)
	{
		temp = (a * seed1 + b * seed0) % m;
		Set_Lint(p, i + 1, (unsigned short)temp);
		seed1 = rand();
		seed0 = temp;
	}
	return 1;
}

//利用费马定理判断大整数是否是素数
int IsPrime(Lint n)
{
	int x[4] = { 2, 3, 4, 5 };
	Lint temp0, temp1, temp2;

	Init_Lint(&temp0, 0, 0);
	Init_Lint(&temp1, 0, 0);
	Init_Lint(&temp2, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		Lint_Sub(&n, &ONE, &temp0);
		Set_Lint(&temp2, 1, x[i]);
		Mexp_Lint(temp2, temp0, &temp1, n);
		if (Cmp_Lint(temp1, ONE) != 0)
			return 0;
	}
	return 1;
}

//仿照PGP中确定大质数的方法生成n位大素数p
int GenPrime(Lint *p, int n)
{
	unsigned int primelist[64] = {
		2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43,
		47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101,
		103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
		157, 163, 167, 173, 179, 181, 191, 193, 197, 199,
		211, 223, 227, 229, 233, 239, 241, 251, 257, 263,
		269, 271, 277, 281, 283, 293, 307, 311,
	};

	//质数筛选表
	Lint temp0, temp1, temp2, temp3;

	Init_Lint(&temp0, 0, 0);
	Init_Lint(&temp1, 0, 0);
	Init_Lint(&temp2, 0, 0);
	Init_Lint(&temp3, 0, 0);
	Rand_Lint(&temp0, n);
	temp0.n[0] = temp0.n[0] | 1; //把随机生成的大整数的最后一个比特位通过或运算置1，保证它是奇数

	int flage;
	while (1)
	{
		do
		{
			flage = 0;
			for (int i = 0; i < 64; i++) //利用筛选表进行筛选
			{
				Set_Lint(&temp1, 1, primelist[i]);
				Lint_Div(&temp0, &temp1, &temp2, &temp3);
				if (Cmp_Lint(temp3, ZERO) == 0)
				{
					flage = 1;
					break;
				}
			}
			if (flage == 1)
			{
				Lint_Add(&temp0, &TWO, &temp2); //如果所选的大整数没有通过筛选，则加2，重新筛选
				Copy_Lint(&temp0, &temp2);
			}
		} while (flage == 1);

		//判断是否是素数，若是返回大整数
		if (IsPrime(temp0) == 1)
		{
			Copy_Lint(p, &temp0);
			return 1;
		}
		else
		{
			Lint_Add(&temp0, &TWO, &temp2); //如果所选的大整数不是素数，则加2，重新筛选
			Copy_Lint(&temp0, &temp2);
		}
	}
}

int GenKey(Lint p, Lint q, Lint e, Lint *d, Lint *n)
{
	Lint z, temp;
	Init_Lint(&z, 0, 0);
	Init_Lint(&temp, 0, 0);
	Lint_Mul(&p, &q, n);
	Lint_Sub(&p, &ONE, &temp);
	Copy_Lint(&p, &temp);
	Lint_Sub(&q, &ONE, &temp);
	Copy_Lint(&q, &temp);
	Lint_Mul(&p, &q, &z);
	Euclid(z, e, d, &temp);
	if (temp.n[0]!=1)
		return 0;
	return 1;
}

int Encrypt(Lint M, Lint e, Lint n, Lint *C)
{
	Mexp_Lint(M, e, C, n); //对明文模幂运算，求出密文
	return 1;
}

int Decrypt(Lint C, Lint d, Lint n, Lint *M)
{
	Mexp_Lint(C, d, M, n); //对密文模幂运算，求出明文
	return 1;
}

//实现加解密之前的初始化，主要是设置ZERO,ONE,TWO变量
int Reset()
{
	Init_Lint(&ZERO, 1, 0);
	Init_Lint(&ONE, 1, 1);
	Init_Lint(&TWO, 1, 2);
	srand((int)time(NULL));
	return 1;
}

int PrintLint(Lint a, char *ch)
{
	printf("%s(", ch);
	for (int i = a.length - 1; i > 0; i--)
		printf("%d,", a.n[i]);
	printf("%d)\n\n", a.n[0]);
	return 1;
}

CRSACrypt::CRSACrypt(bool bGenKey)
{
	unsigned short sh_e[] = {
		42977, 17289, 7541,
	};
#ifdef _DEBUG
	unsigned short sh_d[] = {
		19217, 41613, 4834, 9141, 20646, 8385, 4014, 64639, 29096, 45518, 33748,
		52639, 27557, 54525, 32515, 46488, 22021, 5552, 707, 4314, 40031, 63970,
		37125, 61060, 2715, 48431, 4985, 21607, 44137, 32539, 44934, 18084, 50048,
		23284, 9209, 16619, 51854, 47055, 61856, 8181, 25278, 5528, 55754, 30787,
		6769, 61767, 25713, 55473, 22617, 61926, 14498, 46739, 8064, 51858, 35797,
		24941, 51968, 45477, 40005, 33338, 39274, 47458, 60411, 19293, 3434,
	};
#endif
	unsigned short sh_n[] = {
		7831, 4719, 19133, 22132, 10111, 5907, 11481, 27531, 28556, 28746, 34700,
		676, 58910, 27155, 55264, 56403, 64222, 33233, 29916, 26794, 28876, 29422,
		49846, 61029, 49931, 36737, 9749, 30583, 48707, 27902, 57617, 33772, 57616,
		28724, 40367, 13740, 15755, 24445, 54810, 60703, 31811, 7906, 28553, 39827,
		30374, 8419, 7725, 62259, 12789, 17308, 26607, 15585, 33407, 53154, 54938,
		13519, 907, 23848, 21757, 9720, 56947, 1065, 57876, 41555, 39514,
	};

	//必须初始化
	Init_Lint(&ZERO, 1, 0);
	Init_Lint(&ONE, 1, 1);
	Init_Lint(&TWO, 1, 2);

	//加载固有密钥
	Init_Lint(&m_e, 0, 0);
	Init_Lint(&m_d, 0, 0);
	Init_Lint(&m_n, 0, 0);
	m_e.length = sizeof(sh_e) / sizeof(sh_e[0]);
	for (int i = 0; i < m_e.length; i++)
		m_e.n[i] = sh_e[i];
#ifdef _DEBUG
	m_d.length = sizeof(sh_d) / sizeof(sh_d[0]);
	for (int i = 0; i < m_d.length; i++)
		m_d.n[i] = sh_d[i];
#endif
	m_n.length = sizeof(sh_n) / sizeof(sh_n[0]);
	for (int i = 0; i < m_n.length; i++)
		m_n.n[i] = sh_n[i];

	//重新生成密钥
	if (bGenKey)
	{
		Generate();
		SaveKey();
	}
}

CRSACrypt::~CRSACrypt()
{
}

void CRSACrypt::Generate()
{
	Lint p, q;
	Reset();
	Init_Lint(&p, 0, 0);
	Init_Lint(&q, 0, 0);
	Init_Lint(&m_e, 0, 0);
	Init_Lint(&m_d, 0, 0);
	Init_Lint(&m_n, 0, 0);
	GenPrime(&p, PL);
	GenPrime(&q, PL - 1);
	GenPrime(&m_e, 3);
	GenKey(p, q, m_e, &m_d, &m_n); //计算n与d，产生密钥
}

bool CRSACrypt::SaveKey()
{
	char szBuff[8];
	FILE *fp = fopen("绝密\\新密钥.txt", "w+");

	//保存m_n
	fputs("m_n:", fp);
	for (int i = 0; i < m_n.length; i++)
	{
		sprintf(szBuff, "%d, ", m_n.n[i]);
		fputs(szBuff, fp);
	}
	fputs("\n", fp);
	//保存m_e
	fputs("m_e:", fp);
	for (int i = 0; i < m_e.length; i++)
	{
		sprintf(szBuff, "%d, ", m_e.n[i]);
		fputs(szBuff, fp);
	}
	fputs("\n", fp);
	//保存m_d
	fputs("m_d:", fp);
	for (int i = 0; i < m_d.length; i++)
	{
		sprintf(szBuff, "%d, ", m_d.n[i]);
		fputs(szBuff, fp);
	}
	fputs("\n", fp);

	fclose(fp);
	return true;
}

string CRSACrypt::Encrypt(const string& Ms)
{
	union
	{
		char ch[2 * MAX];
		unsigned short l[MAX];
	} buffer;

	string Cs; //输出明文
	Lint C, M;
	int block = 2 * (PL - 1);

	int cur = 0;
	while (1)
	{
		//读入密文
		int len = min(block, Ms.size() - cur);
		if (len == 0)
			break;
		for (int i = 0; i < len; i++)
			buffer.ch[i] = Ms[cur + i];
		cur += len;

		M.length = len / 2;
		for (int i = 0; i < M.length; i++)
			M.n[i] = buffer.l[i];

		//加密并输出
		::Encrypt(M, m_e, m_n, &C);
		for (int i = 0; i < C.length; i++)
			buffer.l[i] = C.n[i];
		for (int i = 0; i < C.length * 2; i++)
			Cs += buffer.ch[i];
	}
	return Cs;
}

string CRSACrypt::Decrypt(const string& Cs)
{
	union
	{
		char ch[2 * MAX];
		unsigned short l[MAX];
	} buffer;

	string Ms; //输出密文
	Lint C, M;
	int block = m_n.length * 2;

	int cur = 0;
	while (1)
	{
		//读入明文
		if (cur == Cs.size())
			break;
		for (int i = 0; i < block; i++)
			buffer.ch[i] = Cs[cur + i];
		cur += block;

		C.length = block / 2;
		for (int i = 0; i < C.length; i++)
			C.n[i] = buffer.l[i];

		//解密并输出
		::Decrypt(C, m_d, m_n, &M);
		for (int i = 0; i < M.length; i++)
			buffer.l[i] = M.n[i];
		for (int i = 0; i < M.length * 2; i++)
			Ms += buffer.ch[i];
	}
	return Ms;
}
