#include <iostream>
#include "src\pugixml.hpp"
#include "src\pugiconfig.hpp"
#include "src\pugixml.cpp"
#include <map>
#include <vector>
#include <sstream>
#include <set>

using namespace std;

void build_a_route(const pugi::xml_node object, map<pair<string, string>, vector<pair<double, double> > > &path) {

    string type = object.child("type_of_vehicle").text().as_string();
    cout<< type;
    string coordinates = object.child("coordinates").text().as_string();
    pair<double, double> stops;

    auto i = stops;
    auto cursor = istringstream(coordinates);          // читаем координаты
    string point;
    getline(cursor, point, ',');            //  записываем "пары" координат, разделенные запятой
    i.first = stod(point);
    getline(cursor, point, ',');
    i.second = stod(point);

    string routes = object.child_value("routes");
    char symbol;                                            // символ, который разделяет разные маршруты
    if (routes.find(',') != string::npos)            // возвращает положение запятой, npos - конец строки
        symbol = ',';
    else symbol = '.';
    vector<string> route;
    cursor = istringstream(routes);          // проходимся по маршрутам
    string number;                         // номер маршрута
    while (getline(cursor, number, symbol))
        route.push_back(number);                         // номера маршрутов
    for (auto &r: route)
        path[{type, r}].push_back({i.first, i.second});
}

double distant(double x1, double x2, double y1, double y2) {
    return sqrt(pow(abs(x2 - x1), 2) + pow(abs(y2 - y1), 2));
}

void who_is_longest(map<pair<string, string>, vector<pair<double, double> > > &path) {  // 1-тип транспорта, 2-номер мартшрута, 3,4-координаты

    map<string, pair<string, double> > maximum_path;
    for (auto& i : path) {
        pair<string, string> stops_pair;
        auto& j = stops_pair;
        auto cursor = istringstream(i.first.second);         // прекращает строку в поток

        getline(cursor, j.first, '.');
        getline(cursor, j.second, ' ');
        cout<<j.first<<" "<<j.second<<"\n";
        double S = 0;
        auto l = i.second[0];
        double x1 = l.first, x2 = l.second;
        for (auto& l : i.second) {
            if (x1 != l.first && x2 != l.second) {
                S += distant(x1, x2, l.first, l.second);
                x1 = l.first;
                x2 = l.second;
            }
        }

//         auto o = maximum_path[j.second];
//         if (S > o.second) {
//             o.second = S;
//             o.first = j.first;
//             maximum_path[j.second] = {o.first, o.second};
//             cout<<j.second;
//         }
//     }

//     cout << "\nЗадание 3:\n";

//     for (auto& i : maximum_path) {
//         auto j = i.second;
//         cout <<"Тип транспорта:" << i.first << "Маршрут: " << j.first << "\n" << "Длина: " << j.second << "\n";
//     }
}

int main() {
    system("chcp 65001");

    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file("trans.xml");
    cout << result.description() << "\n";                                            // проверка на нахождение файла

    map<pair<string, string>, int> number_of_stops;  // первое - вид транспорта, второе - номер маршрута, третье - количество станций

    auto set = document.child("dataset");
    for (auto station = set.child("transport_station"); station; station = station.next_sibling()) {
        vector<string> numbers;
        string number = station.child("routes").first_child().value();
        char symbol;                                            // символ, который разделяет разные маршруты
        if (number.find(',') != string::npos)            // возвращает положение запятой, npos - конец строки
            symbol = ',';
        else symbol = '.';

        auto cursor = istringstream(number);           //  поток
        string buffer;                                   // временное хранилище для номера транспорта
        while (getline(cursor, buffer, symbol))    // кладем в буфер значение взятое с "потока курсор" до "символа" и так каждый раз (символ разделяет номера маршрутов)
            numbers.push_back(buffer);

        string transport = station.child("type_of_vehicle").first_child().value();   // виды транспорта

        for (const auto &i : numbers)                    // считаем количество остановок каждого вида транспорта(и каждого маршрута)
            number_of_stops[{transport, i}]++;
    }

    cout << "\nЗадание 1:\n";
    pair<pair<string, string>, int> max_count;          // максимальное количество остановок (1-вид транспорта, 2-номер маршрута, 3-его длина)
    std::set<string> types_of_transport;                // все найденные виды транспорта
    for (const auto& i : number_of_stops) {
        string now_transport = i.first.first;             // вид текущего транспорта
        if (types_of_transport.find(now_transport) == types_of_transport.end()) {    // если в списке нет такого вида транспорта, тогда....
            max_count = {{now_transport, i.first.second}, i.second};    // пусть максимальное кол-во станций у этого транспорта
            types_of_transport.insert(now_transport);        // добавляем тип транспорта в список
            for (const auto &j : number_of_stops) {         // проходимся по всем маршрутам
                if (j.first.first == now_transport && max_count.second < j.second) {  // если встречаем такой же вид транспорта, но у которого остановок больше
                    max_count.second = j.second;                                            // то заменяем значение максимального
                    max_count.first.second = j.first.second;
                }
            }
            cout << max_count.first.first << " " << max_count.first.second << " имеет наибольшее количество остановок, равное " << max_count.second << ".\n";     // закончили прохождение по данному типу транспорта, можно вывести результат
        }
    }

    map<string, int> street_stops;   // 1-название улицы, 2-количество остановок
    for (auto station = set.child("transport_station"); station; station = station.next_sibling()) {
        string street = station.child_value("location");
        if (!street.empty()) {
            if (street.find(',') == string::npos)               // если 1 улица
                street_stops[street]++;                             // счёт сколько на этой улице остановок
            else {
                auto cursor = istringstream(street);
                string buffer;
                while (getline(cursor, buffer, ','))   // если несколько улиц, то разделяем
                    street_stops[buffer]++;
            }
        }
    }
    pair<string, int> max_count_of_street_stops = {"0", 0};
    for (auto& i : street_stops)
        if (i.second > max_count_of_street_stops.second) max_count_of_street_stops = {i.first, i.second};
    cout << "\nЗадание 2:\n" << max_count_of_street_stops.first << " - именно тут чаще всего останавливается транспорт. Целых " << max_count_of_street_stops.second << " раз.\n";

    map<pair<string, string>, vector<pair<double, double> > > coordinates;  // 1-название транспорта, 2-название маршрута, 3,4-координаты
    for (auto type = set.child("type_of_vehicle"); type; type = type.next_sibling())
        for (auto station = set.child("transport_station"); station; station = station.next_sibling()) {
            coordinates[{type.child_value(), station.child_value()}].push_back({0, 0});
            build_a_route(station, coordinates);
        }
    who_is_longest(coordinates);
}
