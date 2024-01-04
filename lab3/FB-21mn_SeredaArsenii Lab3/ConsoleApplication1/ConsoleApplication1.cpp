#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#if __cplusplus >= 201103L
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdalign>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cwchar>
#include <cwctype>
#endif

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif
#include <C:/Users/Арсений/Desktop/Учеба 13 семестр/МРКМ/Lab 1 InfInt/InfInt/InfInt.h>
#include <chrono>
#include "windows.h"
#include <ios>
#include <fstream>
#include <string>
#include "math.h"
using namespace std::chrono;
using namespace std;

const int N = 32; ///change 


bool IsPrime(InfInt n)
{
	cout << "IsPrime:";
	if (n == 0 || n == 1) {
		return false;
	}

	for (InfInt i = 2; i <= n / i; ++i)
	{
		if (n % i == 0) {
			return false;
		}
	}

	return true;
}

//////////
InfInt modulo(InfInt base, InfInt e, InfInt mod) {
	cout << "Modulo:";
	InfInt a = 1;
	InfInt b = base;
	while (e > 0) {
		if (e % 2 == 1)
			a = (a * b) % mod;
		b = (b * b) % mod;
		e = e / 2;
	}
	return a % mod;
}


/*bool Fermat(InfInt m, int iterations) {
	cout << "Fermat:";
	if (m == 1) {
		return false;
	}
	for (int i = 0; i < iterations; i++) {
		InfInt x = GetRandom() % (m - 1) + 1;
		if (modulo(x, m - 1, m) != 1) {
			return false;
		}
	}
	return true;
}*/
////////////

InfInt GetPrime()
{
	
	int mas[N];
	

GetRandom:
	//cout << "GetRandom:";
	for (int i = 0; i < N; i++)
	{
		mas[i] = 0;
	}


	int a = 0;
	for (int i = 0; i < N; i++)
	{
		a = rand();
		mas[i] = a % 2;
	}

	InfInt b = 0;
	InfInt c = 1;
	InfInt last_c = 1;
	int last_i = 0;

	for (int i = 0; i < N; i++)
	{
		if (mas[i] == 1)
		{
			//cout << i << ":";
			c = last_c;
			for (int j = last_i; j < i; j++)
			{
				c = c * 2;
			}
			b = b + c;
			last_i = i;
			last_c = c;
		}
	}
	//cout << b;

	cout << "GetPrime:";
	while (1)
	{
		for (InfInt i = 2; i < b; i++) {
			if (b % i == 0) 
			{
				break;
			}

			if (i * i >= b)
			{
				cout << b << endl;
				return b;
			}
			/*if (Fermat(i, 2)) {
				if (Fermat(i, 5))
				{
					return i;
				}
			}*/
		}
		b++;
	}
	
}

/*InfInt Maurer()
{
	cout << "Maurer:";
	InfInt prime_mas[100];
	int counter = 0;
	for (InfInt i = 0; ; i++)
	{
		if (IsPrime(i))
		{
			prime_mas[counter] = i;
			counter++;
		}
	}

	for (int i = 0; i < 100; i++)
	{
		cout << prime_mas[i] << " ";
	}
}
*/

InfInt gcd(InfInt a, InfInt h)
{
	InfInt temp;
	while (1) {
		temp = a % h;
		if (temp == 0)
			return h;
		a = h;
		h = temp;
	}
}

int main(void)
{
	srand(time(0));
	InfInt p = GetPrime();
	InfInt q;

check:
	q = GetPrime();

	if (p == q)
	{
		cout << endl;
		cout << "ERROR EQUAL PRIME NUMBERS" << endl;
		goto check;
	}
	

	cout << endl;
	cout << "p = " << p << endl;
	cout << "q = " << q << endl;


	p = 19;
	q = 41;



	InfInt n = p * q;
	cout << "n = " << n << endl;

	InfInt e = 2;
	InfInt phi = (p - 1) * (q - 1);
	while (e < phi) {
		if (gcd(e, phi) == 1)
			break;
		else
			e++;
	}
	cout << "e = " << e << endl;


	e = 691;

	cout << "phi = " << phi << endl;


	// вот тут херня
	InfInt d;
	InfInt h; 
	InfInt r;
	InfInt a = phi;
	InfInt b = e;
	InfInt x;
	InfInt y;
	InfInt x1 = 0;
	InfInt x2 = 1;
	InfInt y1 = 1;
	InfInt y2 = 0;
	cout << endl << endl;

	while (1) 
	{
		if (b > 0)
		{
			cout << endl << "?????????????????????????????" << endl;
			h = a / b;
			cout << "h = " << h << endl;
			r = a - (h * b);
			cout << "r = " << r << endl;
			x = x2 - (h * x1);
			cout << "x = " << x << endl;
			y = y2 - (h * y1);
			cout << "y = " << y << endl;
			a = b;
			cout << "a = " << a << endl;
			b = r;
			cout << "b = " << b << endl;
			x2 = x1;
			cout << "x2 = " << x2 << endl;
			x1 = x;
			cout << "x1 = " << x1 << endl;
			y2 = y1;
			cout << "y2 = " << y2 << endl;
			y1 = y;
			cout << "y1 = " << y1 << endl;
		}
		else
		{
			if (x2 < y2)
			{
				if (x2 < 0)
				{
					x2 = x2 * -1;
				}
				d = phi - x2;
			}
			else
			{
				if (y2 < 0)
				{
					y2 = y2 * -1;
				}
				d = phi - y2;
			}

			break;
		}
	}





	cout << endl << endl;
	cout << "e = " << e << endl;
	cout << "d = " << d << endl;
	cout << "n = " << n << endl;






	InfInt msg = 321;

	cout << endl;
	cout << "Message data = " << msg << endl;

	InfInt c = msg;

	for (InfInt i = 1; i < e; i++)
	{
		c = c * msg;
	}

	c = c - ((c / n) * n); 
	
	cout << "Encrypted data = " << c << endl;
	InfInt m = c;
	for (InfInt i = 1; i < d; i++)
	{
		m = m * c;
	}
	m = m % n;

	cout << "Original Message Sent = " << m << endl;

	return 0;
}

