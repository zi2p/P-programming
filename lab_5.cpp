#include <iostream>
#include <algorithm>


#include <string>
#include <exception>
#include <cassert>

using namespace std;

template<class element>
class KB {

private:
    unsigned int size = 0;
    unsigned int capacity = 100;
    int first = 0;
    int last = 0;
    element* arr;

public:
    KB(size_t _capacity)
        : capacity(_capacity)
        , arr(new element[_capacity]) {}       // конструктор

    void capacity_kb(int s) {    // изменить размер
        element* arr1 = new element[s];
        if (s > capacity) {
            int i = s - 1;
            for (; i >= s - size; i--, last--) {
                if (last >= 0) arr1[i] = arr[last];
                else {
                    last = capacity - 1;
                    arr1[i] = arr[last];
                }
            }
            delete[] arr;
            arr = arr1;
            first = i + 1;
            last = s - 1;
            capacity = s;
        }
        else {
            int i = s - 1;
            for (; i >= max(0, s - size); i--, last--) {
                if (last >= 0) arr1[i] = arr[last];
                else {
                    last = capacity - 1;
                    arr1[i] = arr[last];
                }
            }
            delete[] arr;
            arr = arr1;
            first = 0;
            last = s - 1;
            capacity = s;
        }
    }

    void push_last(element e) {
        arr[last] = e;                             // добавляем новый элемент на место последней "свободной" ячейки
        if (size != capacity - 1) size++;            // если буфер еще не полон, то количество "заполненных" мест увеличиваем
        else if (first == capacity - 1) first = 0;   // если полон и первый на выход стоит на "конце", то следующий тот, кто в "начале" (при полном буфере last == first)
        else first++;                                  // если первый выходящий не в конце, то ++ к нему

        if (last == capacity - 1) last = 0;               // если указатель на последний вошедший стоит на "конце", то следующее место для записи - "начало"
        else last++;                               // иначе просто следующая ячейка
    }

    void push_first(element e) {  // первый на вылет
        if (size != capacity) {        // наполнился ли буфер, если нет, то
            if (first != 0) first--;       // если записываем не перед первым (нулевым) элементом, то записываем на месте предшествующего перед первым на выход
            else first = capacity - 1;    // иначе записываем на место в конце списка (его первым и выкинем)
            size++;     // количество увеличилось
        }
        arr[first] = e;    // первый на вылет ТЫ!!!!!!!!
    }

    void del_first() {  // первый на вылет
        if (size != 0) {   // если не пуст
            if (first != capacity - 1) first++;  // сдвинем на следующий
            else first = 0;           // иначе следующий "первый"
            size--;                 // уменьшили количество
        }
        else throw out_of_range("nothing to delete");
    }

    void del_last() {   // последний на вылет
        if (size != 0) {
            if (last != 0) last--;
            else last = capacity - 1;                    // аналогично del_first
            size--;
        }
        else throw out_of_range("nothing to delete");
    }

    element _first() {
        return arr[first];
    }

    element _last() {
        return arr[last - 1];  // так как указывает на следующую свободную или несвободную ячейку
    }

    element& operator[] (int a) {
        return arr[(first + a) % capacity];
    }


    template<typename type>
    class KB_iterator : public iterator <random_access_iterator_tag, type> {
        friend  KB<type>;
    private:
        type* iter;
        //KB<type> buffer;
        //using new_type = typename iterator <random_access_iterator_tag, type>;
    public:

        KB_iterator() : iter(0) {}
        ~KB_iterator() = default;
        explicit KB_iterator(type* t) : iter(t) {}

        type& operator++ () {
            if (iter != capacity - 1) return iter++;
            return 0;
        }

        type& operator-- () {
            if (iter != 0) return iter--;
            return capacity - 1;
        }

        type& operator+= (type iter2) {
            return iter = (iter + iter2) % capacity;
        }

        type& operator-= (type iter2) {
            return iter = (iter - iter2) % capacity;
        }

        type& operator+ (type iter2) {
            type iter3 = (iter + iter2) % capacity;
            return iter3;
        }

        type& operator- (type iter2) {
            type iter3 = (iter - iter2) % capacity;
            return iter3;
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

        KB_iterator begin() {
            return KB_iterator(first);
        }

        KB_iterator end() {
            return KB_iterator(last);
        }
    };
};

//int main() {
//
//    KB<int> kb;
//    // меняем размер
//    kb.size_kb(3);
//    // вводим элементы
//    kb.push_first(1);
//    kb.push_last(2);
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
    cb.push_first(1);
    cb.push_last(3);
    callAssert(equal_to<>(), cb._first(), 1);
    callAssert(equal_to<>(), cb._last(), 3);
}
