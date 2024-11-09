#include <iostream>
#include <cmath> 
using namespace std;
template<typename T> class Complex
{
private:
    T real_;
    T imag_;
    T modulus_ = sqrt(pow(real_, 2) + pow(imag_, 2));
public:
    Complex() : real_(0), imag_(0) {}
    Complex(T real, T imag) : real_(real), imag_(imag) {}

    T real() const { return real_; }
    T imag() const { return imag_; }
    T modulus() const { return modulus_; }
    Complex<T> operator-() const { return Complex<T>(-real_, -imag_); } //取负运算，实部虚部分别取负 

    // Binary operators
    Complex<T> operator+(const Complex<T>& other) const { //加法运算，实部虚部分别相加 
        return Complex<T>(real_ + other.real_, imag_ + other.imag_);
    }

    Complex<T> operator-(const Complex<T>& other) const { //减法运算，实部虚部分别相加 
        return Complex<T>(real_ - other.real_, imag_ - other.imag_);
    }

    Complex<T> operator*(const Complex<T>& other) const { //乘法运算，设z1=a+bi，z2=c+di(a、b、c、d∈R)是任意两个复数，那么它们的积(a+bi)(c+di)=(ac-bd)+(bc+ad)i。 
        return Complex<T>(real_ * other.real_ - imag_ * other.imag_,
            real_ * other.imag_ + imag_ * other.real_);
    }

    Complex<T> operator/(const Complex<T>& other) const { //除法运算，满足(c+di)(x+yi)=(a+bi)的复数x+yi(x,y∈R)叫复数a+bi除以复数c+di的商。 
        T divisor = other.real_ * other.real_ + other.imag_ * other.imag_;
        return Complex<T>((real_ * other.real_ + imag_ * other.imag_) / divisor,
            (imag_ * other.real_ - real_ * other.imag_) / divisor);
    }

    bool operator==(const Complex<T>& other) const { //判等运算，实部虚部均相等复数才相等 
        return real_ == other.real_ && imag_ == other.imag_;
    }

    bool operator!=(const Complex<T>& other) const { //判非等运算 
        return !(*this == other);
    }

    bool operator<(const Complex<T>& other) const { //判小于运算（按模大小排序） 
        if (modulus_ < other.modulus_)
            return modulus_ < other.modulus_;
        else if (modulus_ == other.modulus_)
            return real_ < other.real_;
    }

    bool operator>(const Complex<T>& other) const { //判大于运算（按模大小排序） 舍弃如果非大于非等于即小于的想法 
        if (modulus_ > other.modulus_)
            return modulus_ > other.modulus_;
        else if (modulus_ == other.modulus_)
            return real_ > other.real_;
    }

    void ComPrint() const { //打印复数 
        cout << real_ << "+" << imag_ << "i" << endl;
    }
};


