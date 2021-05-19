#include <iostream>
#include <algorithm>


#include <string>
#include <exception>
#include <cassert>

using namespace std;

template<class element>
class KB {

private:

    unsigned int size_ = 0;
    unsigned int capacity_ = 100;
    int first = 0;
    int last = 0;
    element* arr;

public:

    KB(size_t _capacity)
        : capacity_(_capacity)
        , arr(new element[_capacity]) {}       // конструктор

    void change_capacity(int s) {    // изменить размер
        element* arr1 = new element[s];
        if (s > capacity_) {
            int i = s - 1;
            for (; i >= s - size_; i--, last--) {
                if (last >= 0) arr1[i] = arr[last];
                else {
                    last = capacity_ - 1;
                    arr1[i] = arr[last];
                }
            }
            delete[] arr;
            arr = arr1;
            first = i + 1;
            last = s - 1;
            capacity_ = s;
        }
        else {
            int i = s - 1;
            for (; i >= max(0, s - size_); i--, last--) {
                if (last >= 0) arr1[i] = arr[last];
                else {
                    last = capacity_ - 1;
                    arr1[i] = arr[last];
                }
            }
            delete[] arr;
            arr = arr1;
            first = 0;
            last = s - 1;
            capacity_ = s;
        }
    }

    unsigned int capacity() {
        return capacity_;
    }

    unsigned int size() {
        return size_;
    }

    void push_back(element e) {
        arr[last] = e;                             // добавляем новый элемент на место последней "свободной" ячейки
        if (size_ != capacity_ - 1) size_++;            // если буфер еще не полон, то количество "заполненных" мест увеличиваем
        else if (first == capacity_ - 1) first = 0;   // если полон и первый на выход стоит на "конце", то следующий тот, кто в "начале" (при полном буфере last == first)
        else first++;                                  // если первый выходящий не в конце, то ++ к нему

        if (last == capacity_ - 1) last = 0;               // если указатель на последний вошедший стоит на "конце", то следующее место для записи - "начало"
        else last++;                               // иначе просто следующая ячейка
    }

    void push_front(element e) {  // первый на вылет
        if (size_ != capacity_) {        // наполнился ли буфер, если нет, то
            if (first != 0) first--;       // если записываем не перед первым (нулевым) элементом, то записываем на месте предшествующего перед первым на выход
            else first = capacity_ - 1;    // иначе записываем на место в конце списка (его первым и выкинем)
            size_++;     // количество увеличилось
        }
        arr[first] = e;    // первый на вылет ТЫ!!!!!!!!
    }

    void pop_front() {  // первый на вылет
        if (size_ != 0) {   // если не пуст
            if (first != capacity_ - 1) first++;  // сдвинем на следующий
            else first = 0;           // иначе следующий "первый"
            size_--;                 // уменьшили количество
        }
        else throw out_of_range("nothing to delete");
    }

    void pop_back() {   // последний на вылет
        if (size_ != 0) {
            if (last != 0) last--;
            else last = capacity_ - 1;                    // аналогично del_first
            size_--;
        }
        else throw out_of_range("nothing to delete");
    }
//для константного и не константного отдельно
    element& front() {
        return arr[first];
    }

    element& front() const {
        return arr[first];
    }

    element& back() {
        return arr[last - 1];  // так как указывает на следующую свободную или несвободную ячейку
    }

    element& back() const {
        return arr[last - 1];  // так как указывает на следующую свободную или несвободную ячейку
    }

    element& operator[] (int a) {
        return arr[(first + a) % capacity_];
    }

    element& operator[] (int a) const {
        return arr[(first + a) % capacity_];
    }

    template<typename type>
    class KB_iterator : public iterator <random_access_iterator_tag, type> {
        type* iter;
        KB<type>& buffer;
        //using new_type = typename iterator <random_access_iterator_tag, type>;
    public:

        KB_iterator() : iter(nullptr) {}
        ~KB_iterator() = default;
        explicit KB_iterator(type* t, KB<type>& b) : iter(t), buffer(b) {}

        KB_iterator& operator++ () {
            if (iter != buffer.arr + buffer.capacity_ - 1) ++iter;
            else iter = buffer.arr;
            return *this;
        }

        KB_iterator operator++ (int) {
            KB_iterator tmp = *this;
            ++*this;
            return tmp;
        }

        KB_iterator& operator-- () {
            if (iter != buffer.arr) --iter;
            else iter = buffer.arr + buffer.capacity_ - 1;
            return *this;
        }

        KB_iterator operator-- (int) {
            KB_iterator tmp = *this;
            --*this;
            return tmp;
        }

        KB_iterator& operator+= (int rhs) {
            iter = (iter - buffer.arr + rhs) % capacity_ + buffer.arr;
            return *this;
        }

        KB_iterator& operator-= (int rhs) {
            iter = (iter - buffer.arr - rhs) % capacity_ + buffer.arr;
            return *this;
        }

        KB_iterator operator+ (int rhs) {
            KB_iterator iter3 = *this;
            return iter3 += rhs;
        }

        KB_iterator operator- (int rhs) {
            KB_iterator iter3 = *this;
            return iter3 -= rhs;
        }

        KB_iterator& operator= (KB_iterator iterator_) {
            this->iter = iterator_.iter;
            return *this;
        }

        bool operator== (KB_iterator other) const {
            return iter == other.iter;
        }

        bool operator!= (KB_iterator other) const {
            return !(*this == other);
        }

        bool operator< (KB_iterator other) const {
            if ((last > first && other.iter > iter) ||
                (last < first && (other.iter > iter && (other.iter < capacity_ - 1 || other.iter > 0)))) return true;
            return false;
        }

        bool operator>= (KB_iterator other) const {
            return !(this < other);
        }

        bool operator<= (KB_iterator other) const {
            if (this < other || this == other) return true;
            return false;
        }

        bool operator> (KB_iterator other) const {
            return !(this <= other);
        }
// сделать константный итератор
        type& operator* () {
            return *iter;
        }

    };

    KB_iterator<element> begin() {
        return KB_iterator<element>(arr + first, *this);
    }

    KB_iterator<element> end() {
        return KB_iterator<element>(arr + last, *this);
    }
};

//int main() {
//
//    KB<int> kb;
//    // меняем размер
//    kb.size_kb(3);
//    // вводим элементы
//    kb.push_first(1);
//    kb.push_back(2);
//    // вытаскиваем элемент
//    kb.del_first();
//    kb.del_last();
//}



template <auto& out=cout, typename T>
void print(const T& t)
{
    out << t << endl;
}

template<auto& out=cout, class T, class... Args>
void print(const T& el, Args... args)
{
    out << el << " ";
    print<out>(args...);
}


#define callAssert(...) assert(assertPrint(__VA_ARGS__))

template<class P, class... Args>
bool assertPrint(const P& pred, Args&&... args) {
    if (!pred(forward<Args>(args)...)) {
        print<cerr>(forward<Args>(args)...);
        return false;
    }
    return true;
}

int main(){
    KB<int> cb(4);
    cb.push_front(1);
    cb.push_back(3);
    for (int i : cb) cout << i << " ";
    callAssert(equal_to<>(), cb.front(), 1);
    callAssert(equal_to<>(), cb.back(), 3);
}
