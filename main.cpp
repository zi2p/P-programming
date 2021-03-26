//   Лабораторная работа 2. Классы. Перегрузка операторов
//  Спроектировать и реализовать класс для описания сущности многочлен (полином), раздела математики - Алгебра.
//  Реализовать конструктор(ы), конструктор копирования, деструктор, а также следующие операторы:
//  1. =
//  2. ==, !=
//  3. +, - (унарный и бинарный), +=, -=
//  4. * , / (на число), *=, /=
//  5. <<, >>
//  6. [] (для получения коэффициента i-го члена)

#include <iostream>
#include <map>
#include <string>

class polynomial {
private:
    std::map<unsigned int, double> poly;
public:
    polynomial()                        // конструктор по умолчанию
        : poly(0,0)
    {}
    explicit polynomial(const std::map<unsigned int, double> &p)  // создание
        : poly(p)
    {}
    polynomial(const polynomial &p)       // конструктор копирования
        : poly(p.poly)
    {}
    polynomial& operator=(const polynomial &p) {
        poly = p.poly;
    }
    ~polynomial()=default;
    bool operator==(const polynomial &p) {                            // равны
        return p.poly==poly;
    }
    bool operator!=(const polynomial &p) {                            // неравны
        return !operator==(p);
    }
    polynomial operator+(const polynomial &p) {                   // сложение полиномов
        polynomial p_rezult = *this;
        p_rezult += p;
        return p_rezult;
    }
    polynomial operator-() {                                                    // унарный минус
        polynomial p;
        for (auto i : poly)                                                      // проход по объектам
            p.poly[i.first] = -poly.at(i.first);
        return p;
    }
    polynomial operator-(const polynomial &p) {                                   // бинарный минус
        polynomial p_rezult = *this;
        p_rezult -= p;
        return p_rezult;
    }
    polynomial& operator+=(const polynomial &p) {
        for (auto i : p.poly) {                                                     // проход по объектам
            if (poly.count(i.first)) poly[i.first] = poly.at(i.first) + p.poly.at(i.first);
            else poly[i.first] = p.poly.at(i.first);
        }
        return *this;
    }
    polynomial& operator-=(const polynomial &p) {
        for (auto i : p.poly) {                                                     // проход по объектам
            if (poly.count(i.first)) poly[i.first] = poly.at(i.first) - p.poly.at(i.first);
            else poly[i.first] = -p.poly.at(i.first);
        }
        return *this;
    }
    polynomial operator*(const polynomial &p) {               // множение полиномов
        polynomial p_rezult = *this;
        p_rezult *= p;
        return p_rezult;
    }
    polynomial& operator*=(const polynomial &p) {
        for (auto i : p.poly)
            for (auto j : poly)
                poly[i.first + j.first] = p.poly.at(i.first) * poly.at(j.first);
        return *this;
    }
    polynomial operator/(int a) {                             // деление полинома на число
        polynomial p_rezult = *this;
        p_rezult /= a;
        return p_rezult;
    }
    polynomial& operator/=(const int a) {
        for (auto i : poly)
            poly[i.first] = poly.at(i.first) / a;
        return *this;
    }
    double operator[](const int a) {
        if (poly.count(a)) return poly.at(a);
        else return 0;
    }
    friend std::ostream& operator<<(std::ostream &out, const polynomial &p);
    friend std::istream& operator>>(std::istream &in, polynomial &p);
};

std::ostream &operator<<(std::ostream &out, const polynomial &p) {                        // перегрузка оператора
    for (auto i : p.poly) {                                                     // проход по объектам
        if (i.second) {
            if (i.second > 0) out << "+" << i.second << "^" << i.first;
            else out << i.second << "^" << i.first;
        }
    }
    return out;
}
std::istream &operator>>(std::istream &in, polynomial &p) {                        // перегрузка оператора
    std::cout << "Вводите многочлен х^2+6x-8 в виде 1 2 +6 1 -8 0. В конце поставьте ЕНТЕР";
    double x;
    int s;
    while (in >> x && in >> s) p.poly[s] = x;
    return in;
}

int main() {
    return 0;
}