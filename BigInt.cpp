#include <vector>
#include<iostream>
#include "BigInt.h"


BigInt::BigInt() {
    this->value = new std::vector<uint8_t>;
    this->length = 0;
    this->sign = false;
};

BigInt::BigInt(int x) {
    this->value = new std::vector<uint8_t>;
    this->length = 0;
    this->sign = false;
    if (x < 0) {
        this->sign = true;
        x *= -1;
    }
    if (x > 0)
        while (x > 0) {
            this->value->insert(this->value->begin(), uint8_t(x % 10));
            this->length++;
            x /= 10;
        }
    else {
        this->length = 1;
        this->value->push_back(0);
    }
}

BigInt::BigInt(long long int x) {
    this->value = new std::vector<uint8_t>;
    this->length = 0;
    this->sign = false;
    if (x < 0) {
        this->sign = true;
        x *= -1;
    }
    if (x > 0)
        while (x > 0) {
            this->value->insert(this->value->begin(), uint8_t(x % 10));
            this->length++;
            x /= 10;
        }
    else {
        this->length = 1;
        this->value->push_back(0);
    }
}

BigInt::BigInt(std::string x) {
    this->value = new std::vector<uint8_t>;
    unsigned long long l = x.size();
    this->length = l;
    int k = 0;
    if (x[0] == '-') {
        this->sign = true;
        k = 1;
        this->length--;
    } else this->sign = false;

    for (int i = k; i < l; i++) {
        if (48 <= x[i] && x[i] <= 57)
            this->value->push_back(uint8_t(x[i]) - 48);
        else throw std::invalid_argument("can't convert to BigInt");
    }
}


BigInt::BigInt(const BigInt &x) {
    this->value = new std::vector<uint8_t>();
    this->value->resize(x.size());
    this->value = x.value;
    this->sign = x.sign;
    this->length = x.length;
}

BigInt::~BigInt() = default;

size_t BigInt::size() const {
    return (size_t) length;
}

BigInt &BigInt::operator=(const BigInt &x) {
    if (this->value != x.value || this->sign != x.sign) {
        this->value = x.value;
        this->sign = x.sign;
        this->length = x.length;
        return *this;
    }
    return *this;
}

BigInt BigInt::operator+() const {
//    BigInt a = *this;
//    a.sign = false;
//    return a;
      return *this;
}

BigInt BigInt::operator-() const {
//    BigInt a = *this;
//    a.sign = true;
//    return a;
      BigInt a= *this;
      a.sign = !this->sign;
      return a;
}

bool BigInt::operator>(const BigInt &b) const {
    if (!this->sign && this->sign != b.sign)
        return true;
    else if (this->sign && this->sign != b.sign)
        return false;
    else {
        if (this->length > b.length)
            return !this->sign;
        else if (this->length < b.length)
            return b.sign;
        else {
            for (int i = 0; i < this->length; i++)
                if (this->value->at(i) > b.value->at(i))
                    return !this->sign;
                else if(this->value->at(i) < b.value->at(i))
                    return this->sign;
            return false;
        }
    }
}

bool BigInt::operator==(const BigInt &b) const {
    if (this->sign != b.sign)
        return false;
    else {
        if (this->length != b.length)
            return false;
        else {
            for (int i = 0; i < this->length; i++)
                if (this->value->at(i) != b.value->at(i))
                    return false;
            return true;
        }
    }
}

bool BigInt::operator!=(const BigInt &b) const {
    return !(*this == b);
}

bool BigInt::operator>=(const BigInt &b) const {
    return (*this > b) || (*this == b);
}

bool BigInt::operator<(const BigInt &b) const {
    return !(*this >= b);
}

bool BigInt::operator<=(const BigInt &b) const {
    return !(*this > b);
}

void BigInt::_remove_leading_zeros() {
    while (this->value->at(0) == 0 && this->length > 1) {
        this->value->erase(this->value->begin());
        this->length--;
    }
    if(*this == BigInt("-0"))
        this->sign = false;
}
BigInt operator+(const BigInt &a, const BigInt &b) {
    if (a.sign == b.sign) {
        BigInt c;
        c.value = new std::vector<uint8_t>;
        c.length = 0;
        c.sign = a.sign;
        unsigned long long l;
        uint8_t x, p = 0;
        BigInt t;
        if(a == BigInt(0)){
            c = b;
            return c;
        }
        else if(b == BigInt(0)){
            c= a;
            return c;
        }
        if (a.length >= b.length) {
            l = b.length;
            t = a;}
        else {l = a.length;
            t = b;
        }
        for (int i = 0; i < l; i++) {
            x = (a.value->at(a.length - i - 1) + b.value->at(b.length - 1 - i)) + p;
            p = 0;
            if (x >= 10) {
                p = x / 10;
                x %= 10;
            }
            c.value->insert(c.value->begin(), x);
            c.length += 1;
        }

        for (int i = l; i < t.length; i++) {
            x = t.value->at(t.length - 1 - i) + p;
            p = 0;
            if (x >= 10) {
                p = x / 10;
                x %= 10;
            }
            c.value->insert(c.value->begin(), x);
            c.length += 1;
        }
        if(p>0){
            c.value->insert(c.value->begin(), p);
            c.length += 1;
        }

//        if (a.length == b.length) {
//            }
//         else{
//            c.value->at(c.length - l - 1) = c.value->at(c.length - l - 1);
//         }

        c._remove_leading_zeros();
        return c;
    } else if (a.sign) return b - (-a);
    else return a - (-b);
}

BigInt& BigInt::operator+=(const BigInt&b){
    BigInt t = b;
    return *this = (*this + t);
}

BigInt operator-(const BigInt &a, const BigInt &b) {
    if (a.sign == b.sign) {
        BigInt c;
        c.value = new std::vector<uint8_t>;
        c.length = 0;
        unsigned long long len;
        uint8_t x, p = 0;
        BigInt l;
        BigInt r;
        bool ka = a>b;
        if (a > b && !a.sign || a < b && a.sign) {
            l = a;
            len = b.length;
            r = b;
            c.sign = l.sign;
        } else if (a > b && a.sign || a < b && !a.sign) {
            len = a.length;
            l = b;
            r = a;
            c.sign = !l.sign;
        } else {
            c.value->push_back(0);
            c.length = 1;
            c.sign = false;
            return c;
        }
        for (int i = 0; i < len; i++) {
            x = (l.value->at(l.length - i - 1) - r.value->at(r.length - 1 - i));
            c.value->insert(c.value->begin(), x);
            c.length += 1;
        }
        for (unsigned long long i = len; i < l.length; i++) {
            c.value->insert(c.value->begin(), l.value->at(l.length - i - 1));
            c.length += 1;
        }
        for (int i = c.length - 1; i >= 0; i--) {
            if (c.value->at(i) > 200) {
                c.value->at(i) = 10 - (256 - c.value->at(i)) - p;
                p = 1;
                if (c.value->at(i) == 255)
                    c.value->at(i) = 9;
            } else {
                c.value->at(i) = (c.value->at(i)) - p;
                p = 0;
                if (c.value->at(i) == 255){
                    c.value->at(i) = 9;
                    p=1;
                }
            }
        }
        c._remove_leading_zeros();
        return c;
    }
//    else if (a.sign)
//        return a + (-b);
    else return a + (-b);
}

BigInt& BigInt::operator-=(const BigInt&b){
    return *this = (*this - b);
}

BigInt operator*(const BigInt&a, const BigInt&b){
    BigInt c;
    if(a == BigInt(0) || b== BigInt(0)){
        c.value->push_back(0);
        c.length = 1;
        c.sign = false;
        return c;
    }
    c.value->resize(a.length + b.length);
    c.length = a.length + b.length;
    c.sign = a.sign ^ b.sign;
    for (long long i = b.length-1; i >=0 ; i--) {
        for (long long j = a.length-1; j >=0; j--) {
            uint8_t k = a.value->at(j) * b.value->at(i);
            c.value->at(i+j+1) += a.value->at(j) * b.value->at(i);
        }
        for (long long j = c.length-1; j >= 0; j--) {
            if (c.value->at(j) >= 10) {
                c.value->at(j - 1) += c.value->at(j) / 10;
                c.value->at(j) = c.value->at(j) % 10;
            }
        }
    }
    c._remove_leading_zeros();
    return c;
}

BigInt& BigInt::operator*=(const BigInt&b){
    return *this = (*this * b);
}

void BigInt::_shift_left(){
    this->value->push_back(0);
    this->length++;
}

BigInt operator/(const BigInt&a, const BigInt&b){
    if (b == BigInt(0)) throw std::invalid_argument("Division by zero");
    BigInt right = b;
    right.sign = false;
    BigInt res, cur;
    res.value->resize(a.length);
    res.length = a.length;
    for(long long i =0; i <a.length;i++){
        cur._shift_left();
        cur.value->at(cur.length-1)=a.value->at(i);
        cur._remove_leading_zeros();
        int x =0, l =0, r = 10;
        while(l<=r){
            int m = (l+r)/2;
            BigInt t = right*BigInt(m);
            if(t <= cur){
                x = m;
                l = m+1;
            }
            else r = m-1;
        }
        res.value->at(i) = x;
        cur = cur - right*BigInt(x);
    }
    res.sign = a.sign!=b.sign;
    res._remove_leading_zeros();
    return res;
}

BigInt& BigInt::operator/=(const BigInt&b){
    return *this = (*this / b);
}

BigInt operator%(const BigInt&a, const BigInt&b){
    BigInt res = a - (a/b)*b;
    return res;
}
BigInt& BigInt::operator%=(const BigInt&b){
    return *this = (*this % b);
}
BigInt::operator std::string() const {
    std::string res;
    if(this->sign)
        res.push_back('-');
    for(long long i = 0;i< this->length;i++ )
        res.push_back(this->value->at(i)+48);
    return res;
}

BigInt::operator long long int()const{
    return std::atoll(std::string(*this).c_str());
}
BigInt::operator int()const{
    return std::atoi(std::string(*this).c_str());
}

BigInt operator&(const BigInt&a, const BigInt&b){
    BigInt res=0,l,r;
    BigInt imax = std::string("1073741824");
    if(a == b){
        res =a;
        return res;
    }
    l=a;
    r=b;
    if(a.sign&b.sign){
        l = ~l;
        r = ~r;
    }
    int cnt=0;
    int lt,rt;
    int t;

    while(l!=BigInt(0)&& r!=BigInt(0)){
        lt =int(l%imax);
        rt =int(r%imax);
        if(a.sign&b.sign)
            t = int(lt|rt);
        else t = int(lt&rt);
        BigInt BIt =t;
        for(int i = 0; i< cnt;i ++){
            BIt*=imax;
        }
        res+=BIt;
        cnt++;

        l /= imax;
        r/= imax;
    }
    BigInt t0 =0;
    int f =0;
    if(l != BigInt(0)){
        t0 = l;
        if(b.sign)
            f =-1;
    }
    else if(r != BigInt(0)){
        t0 = r;
        if(a.sign)
            f =-1;
    }
    while(t0!=BigInt(0)){
        t =int(t0%imax);
        t =t&f;
        BigInt BIt =t;
        for(int i = 0; i< cnt;i ++){
            BIt*=imax;
        }
        res+=BIt;
        cnt++;
        t0/=imax;
    }
    if(a.sign&b.sign)
        res= ~(res);
    if(res == BigInt(0))
        return res;
    res.sign = a.sign & b.sign;
    return res;
}

BigInt& BigInt::operator&=(const BigInt& b){
    return *this = *this&b;
}

BigInt operator|(const BigInt&a, const BigInt&b){
    BigInt res=0,l,r;
    BigInt imax = std::string("1073741824");
    if(a == b){
        res =a;
        return res;
    }
    l=a;
    r=b;

//    if(b.sign)
//        r = ~r;
    if(a.sign|b.sign){
        l = ~l;
        r = ~r;
    }
    int cnt=0;
    int lt,rt;
    int t;

    while(l!=BigInt(0)&& r!=BigInt(0)){
        lt =int(l%imax);
        rt =int(r%imax);
        if(a.sign|b.sign )
            t = int(lt&rt);
        else
            t = int(lt|rt);
        BigInt BIt =t;
        for(int i = 0; i< cnt;i ++){
            BIt*=imax;
        }
        res+=BIt;
        cnt++;

        l /= imax;
        r/= imax;
    }
    BigInt t0 =0;
    int f =-1;
    if(l != BigInt(0)){
        t0 = l;
        if(b.sign)
            f =0;
    }
    else if(r != BigInt(0)){
        t0 = r;
        if(a.sign)
            f =0;
    }
    while(t0!=BigInt(0)){
        t =int(t0%imax);
        t =t&f;
        BigInt BIt =t;
        for(int i = 0; i< cnt;i ++){
            BIt*=imax;
        }
        res+=BIt;
        cnt++;
        t0/=imax;
    }
    if(a.sign|b.sign)
        res= ~(res);
    if(res == BigInt(0))
        return res;
    res.sign = a.sign | b.sign;
    return res;

}

BigInt& BigInt::operator|=(const BigInt& b){
    return *this = *this|b;
}

BigInt operator^(const BigInt&a, const BigInt&b){
    BigInt res=0,l,r;
    BigInt imax = std::string("2147483648");
    if(a == b){
        res = BigInt(0);
        return res;
    }
    l=a;
    r=b;
    if(a.sign)
        l = ~l;
    if(b.sign)
        r = ~r;
    l.sign = false;
    r.sign = false;

    int cnt=0;
    int lt,rt, t;

    while(l!=BigInt(0)&& r!=BigInt(0)){
        lt= int(l%imax);
        rt = int(r%imax);
        t = lt^rt;
        BigInt BIt =t;
        for(int i = 0; i< cnt;i ++)
            BIt*=imax;
        res+=BIt;
        cnt++;
        l/=imax;
        r/=imax;
    }
    BigInt t0 =0;
    if(l > BigInt(0))
        t0 = l;
    else if(r > BigInt(0))
        t0 = r;
    while(t0> BigInt(0)){
        t = int(t0%imax);
        BigInt BIt = t;
        for(int i = 0; i< cnt;i ++)
            BIt*=imax;
        res+=BIt;
        cnt++;
        t0/=imax;
    }
    if(res == BigInt(0))
        return res;
    res.sign = a.sign ^ b.sign;
    return res;
}

BigInt& BigInt::operator^=(const BigInt& b){
    return *this = *this^b;
}

BigInt& BigInt::operator++() {
    return *this = *this +BigInt(1);
}
BigInt BigInt::operator++() const{
    return *this + BigInt(1);
}

BigInt& BigInt::operator--(){
    return *this = *this -BigInt(1);
}
BigInt BigInt::operator--() const{
    return *this - BigInt(1);
}

BigInt BigInt::operator~() const {
    BigInt res = *this;
    return -(res+BigInt(1));
}

std::ostream& operator<<(std::ostream& o, const BigInt& i){
    return o<<std::string(i);
}