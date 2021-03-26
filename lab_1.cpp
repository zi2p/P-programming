//   Лабораторная работа 1. ООП. Классы. Наследование
//  Спроектировать и реализовать следующие классы:
//  1. Точка
//  2. Ломаная
//  3. Замкнутая ломаная
//  4. Многоугольник
//  5. Треугольник
//  6. Трапеция
//  7. Правильный многоугольник
//
//  Для каждого из классов реализовать следующие методы:
//  1. Конструктор(ы)
//  2. Конструктор копирования
//  3. Оператор присваивания
//  4. Расчет периметра, если применимо
//  5. Расчет площади, если применимо
//  6. Другие приватные и публичные метода по усмотрению.
//      Организовать иерархию классов, там, где это имеет смысл.
//  !Подумать. Какие объекты этих классов могут быть объединены в один массив, где
//  применим динамический полиморфизм? Продемонстрировать это.

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class point                                          //точка
{
private:
    double x;
    double y;
public:
    point()                                // коструктор по умолчанию
        : x(0)
        , y(0)
    {}
    explicit point(double a=0, double b=0)    //конструктор создания
        : x(a)
        , y(b)
    {}
    point(const point &p)                 //конструктор копирования
        : x(p.x)
        , y(p.y)
    {}
    point& operator= (const point &p) {      //оператор присваивания
        x=p.x;
        y=p.y;
    }
    void write()const {
        std::cout << "x = " << x << " y = " << y;
    }
    double return_x()const {
        return x;
    }
    double return_y()const {
        return y;
    }
    void change_x(double a) {
        x=a;
    }
    void change_y(double a) {
        y=a;
    }
    ~point()= default;
};
class polyline_curve                               //ломанная кривая
{
protected:
    std::vector<point> Arr;
public:
    explicit polyline_curve(const std::vector<point> &p)         // конструктор создания
        : Arr(p)
    {}
    polyline_curve(const polyline_curve &l)             //конструктор копирования
        : Arr(l.Arr)
    {}
    polyline_curve& operator=(const polyline_curve &l) {   //оператор присваивания
        Arr = l.Arr;
    }
    bool operator==(const polyline_curve &l)const {         //оператор сравнения
        if (Arr.size()==l.Arr.size()){
            for (int i = 0; i < Arr.size(); i++)
                if (Arr[i].return_x() != l.Arr[i].return_x()) return false;
            return true;
        }
        return false;
    }
    bool operator!=(const polyline_curve &l)const {
        if (Arr.size()!=l.Arr.size()) return true;
        else {
            for (int i = 0; i < Arr.size(); i++)
                if (Arr[i].return_x() != l.Arr[i].return_x()) return true;
            return true;
        }
    }
    int size()const {     // количество точек в ломанной
        return Arr.size();
    }
    virtual double show_length()const {
        double length = 0;
        for (int i = 0; i < Arr.size()-1; i++){
            length += pow((pow(Arr[i].return_x()-Arr[i+1].return_x(), 2)+pow(Arr[i].return_y()-Arr[i+1].return_y(), 2)), 1/2);
        }
        return length;
    }
    ~polyline_curve()= default;
};

class closed_polyline : public polyline_curve       //замкнутая ломанная
{
public:
    explicit closed_polyline(const std::vector<point> &p)         // конструктор создания
            : polyline_curve(p)
    {}
    closed_polyline(const closed_polyline &l)             //конструктор копирования
            : polyline_curve(l)
    {}
    closed_polyline& operator=(const closed_polyline &l) {   //оператор присваивания
        Arr = l.Arr;
    }
    double show_length()const override {
        double length = 0;
        for (int i = 0; i < Arr.size()-1; i++){
            length += pow((pow(Arr[i].return_x()-Arr[i+1].return_x(), 2)+pow(Arr[i].return_y()-Arr[i+1].return_y(), 2)), 1/2);
        }
        length += pow((pow(Arr[Arr.size()-1].return_x()-Arr[0].return_x(), 2)+pow(Arr[Arr.size()-1].return_y()-Arr[0].return_y(), 2)), 1/2);
        return length;
    }
    const point &operator[] (unsigned int a) const {
        return Arr[a];
    }
    ~closed_polyline()= default;
};

class polygon        // многоугольник
{
protected:
    closed_polyline polyline;
public:
    explicit polygon(const std::vector<point> &p)         // конструктор создания
            : polyline(p)
    {}
    polygon(const polygon &p)             //конструктор копирования
            : polyline(p.polyline)
    {}
    polygon& operator=(const polygon &p) {  //оператор присваивания
        polyline = p.polyline;
    }
    bool convex() {                    // выпуклость
        int sign;
        if (((polyline[1].return_x() - polyline[0].return_x()) * (polyline[2].return_y() - polyline[1].return_y()) - (polyline[1].return_y() - polyline[0].return_y()) * (polyline[2].return_x() - polyline[1].return_x())) < 0) sign = -1;
        else sign = 1;
        for (int i = 1; i < polyline.size() - 2; i++) {
            double v = (polyline[i+1].return_x() - polyline[i].return_x()) * (polyline[i+2].return_y() - polyline[i+1].return_y()) - (polyline[i+1].return_y() - polyline[i].return_y()) * (polyline[i+2].return_x() - polyline[i+1].return_x());
            if (v < 0 && sign > 0) return false;
            if (v > 0 && sign < 0) return false;
        }
        return true;
    }
    virtual bool correct() {                    // проверка на правильность
        double a, cos_a;
        double x1 = polyline[0].return_x()-polyline[1].return_x(), x2 = polyline[2].return_x()-polyline[1].return_x();
        double y1 = polyline[0].return_y()-polyline[1].return_y(), y2 = polyline[2].return_y()-polyline[1].return_y();
        a = pow((pow(polyline[0].return_x()-polyline[1].return_x(),2)+pow(polyline[0].return_y()-polyline[1].return_y(),2)),(1/2));
        cos_a = (abs(x1*x2+y1*y2)/(pow(pow(x1,2)+pow(y1,2),(1/2))*pow(pow(x2,2)+pow(y2,2),(1/2))));
        for (int i = 1; i < polyline.size() - 2; i++) {
            double b = pow((pow(polyline[i+0].return_x()-polyline[i+1].return_x(),2)+pow(polyline[i+0].return_y()-polyline[i+1].return_y(),2)),(1/2));
            if (a!=b) return false;
            x1 = polyline[i].return_x()-polyline[i+1].return_x(), x2 = polyline[i+2].return_x()-polyline[i+1].return_x();
            y1 = polyline[i].return_y()-polyline[i+1].return_y(), y2 = polyline[i+2].return_y()-polyline[i+1].return_y();
            double cos_b = (abs(x1*x2+y1*y2)/(pow(pow(x1,2)+pow(y1,2),(1/2))*pow(pow(x2,2)+pow(y2,2),(1/2))));
            if (cos_a!=cos_b) return false;
        }
        return true;
    }
    double diagonals()const {                                         // диагонали выпуклого многоугольника
        return polyline.size() * (polyline.size() - 3) / 2;
    }
    virtual double perimeter()const {                              // периметр
        return polyline.show_length();
    }
    virtual double area()const {                                             // площадь многоугольника
        double s1, s=0;
        point t(polyline[0].return_x()-polyline[2].return_x(),polyline[0].return_y()-polyline[2].return_y()); // внутренняя точка n-угольника
        for (int i = 0; i < polyline.size()-1; i++) {
            s1 = abs((t.return_x()-polyline[i].return_x())*(polyline[i+1].return_y()-polyline[i].return_y())-(polyline[i+1].return_x()-polyline[i].return_x())*(polyline[i+1].return_y()-t.return_y()))/2;
            s = s + s1;
        }
        return s;
    }
    ~polygon()= default;
};
class triangle : public polygon                // треугольник
{
public:
    explicit triangle(const std::vector<point> &p) : polygon(p) {
        if (p.size() != 3 || !convex()) {
            std::cout << "IT'S_NOT_A_TRIANGLE";
        }
    }
    std::string the_form()const {                  // вид треугольника
        double cos_a;
        for (int i = 0, count = 1; count <= 3; i++, count++) {
            double x1 = polyline[count%3].return_x()-polyline[i].return_x(), x2 = polyline[2-i].return_x()-polyline[i].return_x();
            double y1 = polyline[count%3].return_y()-polyline[i].return_y(), y2 = polyline[2-i].return_y()-polyline[i].return_y();
            cos_a = (abs(x1*x2+y1*y2)/(pow(pow(x1,2)+pow(y1,2),(1/2))*pow(pow(x2,2)+pow(y2,2),(1/2))));
            if (cos_a == 0) return "rectangular";
            if (cos_a < 0) return "obtuse_angle";
        }
        return "sharp_eyed";
    }
    double area()const override {
        double a, b, c; // стороны
        a = pow(pow(polyline[0].return_x()-polyline[1].return_x(), 2)+pow(polyline[0].return_y()-polyline[1].return_y(), 2),(1/2));
        b = pow(pow(polyline[2].return_x()-polyline[1].return_x(), 2)+pow(polyline[2].return_y()-polyline[1].return_y(), 2),(1/2));
        c = pow(pow(polyline[0].return_x()-polyline[2].return_x(), 2)+pow(polyline[0].return_y()-polyline[2].return_y(), 2),(1/2));
        return pow((perimeter()-a)*(perimeter()-b)*(perimeter()*c), (1/2));
    }
    ~triangle()= default;
};
class trapezoid : public polygon               // трапеция
{
public:
    explicit trapezoid(const std::vector<point> &p) : polygon(p) {
        if (p.size() != 4 || !check_trapezoid() || !convex()) {
            std::cout << "IT'S_NOT_A_TRAPEZOID";
        }
    }
    bool check_trapezoid()const {
        double x1 = polyline[0].return_x()-polyline[1].return_x(), x2 = polyline[2].return_x()-polyline[3].return_x();
        double y1 = polyline[0].return_y()-polyline[1].return_y(), y2 = polyline[2].return_y()-polyline[3].return_y();
        if (abs(abs(x1*x2+y1*y2)/(pow(pow(x1,2)+pow(y1,2),(1/2))*pow(pow(x2,2)+pow(y2,2),(1/2)))) == 1) return true;
        else {
            x1 = polyline[2].return_x()-polyline[1].return_x(), x2 = polyline[0].return_x()-polyline[3].return_x();
            y1 = polyline[2].return_y()-polyline[1].return_y(), y2 = polyline[0].return_y()-polyline[3].return_y();
            if (abs(abs(x1*x2+y1*y2)/(pow(pow(x1,2)+pow(y1,2),(1/2))*pow(pow(x2,2)+pow(y2,2),(1/2)))) == 1) return true;
        }
        return false;
    }
    std::string the_form()const {
        double cos_a[4];
        for (int i = 0, count = 1; count <= 4; i++, count++) {
            double x1 = polyline[count%4].return_x()-polyline[i].return_x(), x2 = polyline[3-i].return_x()-polyline[i].return_x();
            double y1 = polyline[count%4].return_y()-polyline[i].return_y(), y2 = polyline[3-i].return_y()-polyline[i].return_y();
            cos_a[i] = (abs(x1*x2+y1*y2)/(pow(pow(x1,2)+pow(y1,2),(1/2))*pow(pow(x2,2)+pow(y2,2),(1/2))));
            if (cos_a[i] == 0) return "rectangular";
        }
        for (int i = 0; i < 3; i++) {
            if (cos_a[i] == cos_a[i+1]) return "isosceles";
        }
        if ((cos_a[0]<0 && cos_a[2]<0) || (cos_a[1]<0 && cos_a[3])) return "obtuse_angle";
        return "sharp_eyed";
    }
    bool correct() override {
        std::cout << "THE_TRAPEZOID_CAN'T_BE_CORRECT";
        return false;
    }
    ~trapezoid()= default;
};
class correct_polygon : public polygon         // правильный многоугольник
{
public:
    explicit correct_polygon(const std::vector<point> &p) : polygon(p) {
        if (!correct()) {
            std::cout << "IT'S_NOT_A_CORRECT_POLYGON";
        }
    }
    double area()const override {
        double PI = 3.141592653589793238463;
        double a = pow(pow(polyline[0].return_x()-polyline[1].return_x(), 2)+pow(polyline[0].return_y()-polyline[1].return_y(), 2),(1/2));
        return (pow(a, 2) * polyline.size() / (4 * tan(PI / polyline.size())));
    }
    ~correct_polygon()= default;
};

int main() {
    return 0;
}
