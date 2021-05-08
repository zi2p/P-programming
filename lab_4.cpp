#include <iostream>
#include <vector>

using namespace std;

template<class iterator, class predicate>
bool all_of(iterator first, iterator last, const predicate& condition) {
    while (first != last) {                                                 // если что-то не удовлетворяет устовию
        if (!condition(*first)) return false;                                    // то это ложно
        first++;
    }
    return true;                                                                  // иначе верно
}

template<class iterator, class predicate>
bool any_of(iterator first, iterator last, const predicate& condition) {
    while (first != last) {
        if (condition(*first)) return true;                   //  найден верный, то верно
        first++;
    }
    return false;                                                 // дошли до конца и ничего не нашли - неверно
}

template<class iterator, class predicate>
bool none_of(iterator first, iterator last, const predicate& condition) {
    return (!all_of(first, last, condition));     
}

template<class iterator, class predicate>
bool one_of(iterator first, iterator last, const predicate& condition) {
    int count = 0;
    while (first != last) {
        if (condition(*first)) count++;
        first++;
    }
    if (count == 1) return true;                  //  если верно ровно для одного, то верно
    return false;
}

template<class iterator, class criterial>
bool is_sorted(iterator first, iterator last, const criterial& condition) {  // диапазон задается началом и концом
    if (first == last) return true;
    iterator prev = first;
    for (first++; first != last; prev++, first++)
        if (!condition(*prev, *first)) return false;
    return true;
}

template<class iterator, class criterial>
bool is_partitioned(iterator first, iterator last, const criterial& condition) {  // диапазон задается началом и концом
    if (first == last) return false;      // если диапазон из 1 элемента, то мы не сможем произвести деление
    bool start = condition(*first);          // находим какое значение условия у начала
    bool end = !start;                        // у конца должно быть противоположное
    for (; first != last; first++)  // проходимся от начала до конца пока не найдем
        if (condition(*first) != start) break;     //   что условие НЕ ВЫПОЛНЯЕТСЯ
    if (first == last) return false;  // ломаться не должно в конце, ломаться должно в середине
    for (; first != last; first++) // получаем, что дальше идет другое условие
        if (condition(*first) != end) break;  // проверяем теперь его
    return first == last;   // если всё ОК, то мы должны дойти с этим условием до конца
}

template<typename iterator, typename element>
iterator find_not(iterator first, iterator last, const element& specified) {  // диапазон задается началом и концом
    for (; first != last; first++)   // находим первый элемент неравный заданному
        if (*first != specified) return first;
    return first;
}

template<typename iterator, typename element>
iterator find_backward(iterator first, iterator last, const element& specified) {  // диапазон задается началом и концом
    iterator first_last = last;
    for (; last != first;)  {           // находит первый с конца элемент == заданному
        last--;
        if (last == specified) return last;
    }
    return first_last;
}

template<typename iterator, typename criterial>
bool is_palindrome(iterator first, iterator last, const criterial& condition) {
    if (first == last) return true;   // из 1 элемента по-любому полиндром
    last--;
    for (iterator end = last; first != end; first++, last--)
        if (!condition(*first, *last)) return false;
    return true;
}

/////////////////////   ПРОВЕРКА    //////////////////////////////////////////

bool raven(int a, int b) {
    return a==b;
}

int main() {
    vector<int> M;
    for (int i = 0; i < 5; i++) {
        int b;
        cin >> b;
        M.push_back(b);
    }
    cout << is_palindrome(M.begin(), M.end(), raven);
};
