#include <iostream>

using namespace std;

template<class element>
class KB {
private:
    int size = 100;
public:
    bool working = false;
    int first = 0;
    element* elements = (element*) malloc(sizeof(element) * 100);  // выделям место под наш буфер
    int last = 0;
    void size_kb(int s) {
        if (!working) size = s;  // если в данный момент буфер пуст, то можно изменить его размер
        else throw out_of_range("don't change size during working");   // при работе менять размер не хочется....
    }
    void push_last(element e) {
        if (last < size - 1) {   // если не дошли до конца(есть свободные места, то продолжаем запись
            elements[last] = e;
            last++;
            if (last == first) {   // перезаписываем "сначала" уже имеющие
                first++;
                if (first > size - 1) first = 0;
            }
        }
        else {       //  иначе перезаписываем "сначала" уже имеющие
            elements[0] = e;
            last = 1;
        }
        working = true;
    }
    void push_first(element e) {
        if (first > 0) elements[first--] = e;
        else {
            elements[size - 1] = e;         // может возникнуть проблема удаления последнего элемента кольцевого буфера
            first = size - 1;
            if (last == first) {   // изменим указание на конец
                last--;
                if (last < 0) last = size - 1;
            }
        }
        working = true;
    }
    void del_first() {
        if (working) {
            first++;
            if (first > size - 1) first = 0;
        }
        else throw out_of_range("haven't elements");
        if (first == last) working = false;  // пусто
    }
    void del_last() {
        if (working) {
            last--;
            if (last < 0) last = size - 1;
        }
        else throw out_of_range("haven't elements");
        if (first == last) working = false;   // пусто
    }
};

int main() {

    KB<int> kb;
    // меняем размер
    kb.size_kb(3);
    // вводим элементы
    kb.push_first(1);
    kb.push_last(2);
    // вытаскиваем элемент
    kb.del_first();
    kb.del_last();
}
