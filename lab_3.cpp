#include <iostream>
#include "src\pugixml.hpp"
#include "src\pugiconfig.hpp"
#include "src\pugixml.cpp"
#include <map>
#include <vector>
#include <sstream>
#include <set>
#include <unordered_map>

using namespace std;

vector<string> result_(string& number) {
    vector<string> numbers;
    char symbol;                                            // символ, который разделяет разные маршруты
    if (number.find(',') != string::npos)            // возвращает положение запятой, npos - конец строки
        symbol = ',';
    else symbol = '.';

    auto cursor = istringstream(number);           //  поток
    string buffer;                                   // временное хранилище для номера транспорта
    while (getline(cursor, buffer, symbol))    // кладем в буфер значение взятое с "потока курсор" до "символа" и так каждый раз (символ разделяет номера маршрутов)
        numbers.push_back(buffer);
    return numbers;
}

pair<double, double> result_coordinates(string& coordinates) {
    pair<double, double> stops;
    auto i = stops;
    auto cursor = istringstream(coordinates);          // читаем координаты
    string point;
    getline(cursor, point, ',');            //  записываем "пары" координат, разделенные запятой
    i.first = stod(point);
    getline(cursor, point, ',');
    i.second = stod(point);
    return {i.first, i.second};
}

double distanta(pair<double, double> point1, pair<double, double> point2) {
    return sqrt(pow(abs(point2.first - point1.first), 2) + pow(abs(point2.second - point1.second), 2));
}

void longest_route(unordered_map<string, vector<pair<double, double> > > &routes, string &type) {
    string max_number = " ";
    double max_route = 0;
    for (auto& [number, coordinates] : routes) {
        double length = 0;
        for (size_t i = 0; i < coordinates.size() - 1; i++) {
            length += distanta(coordinates[i], coordinates[i + 1]);
        }
        if (length > max_route) {
            max_route = length;
            max_number = number;
        }
    }
    cout << type << " " << max_number << " с наибольшей длиной маршрута.\n";
}

int main() {
    system("chcp 65001");

    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file("trans.xml");
    cout << result.description() << "\n";                                            // проверка на нахождение файла

// ЗАДАНИЕ 1
    map<pair<string, string>, int> number_of_stops;  // первое - вид транспорта, второе - номер маршрута, третье - количество станций
    auto set = document.child("dataset");
    for (auto station = set.child("transport_station"); station; station = station.next_sibling()) {
        string number = station.child("routes").first_child().value();
        vector<string> numbers = result_(number);
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
            for (const auto& j : number_of_stops) {         // проходимся по всем маршрутам
                if (j.first.first == now_transport && max_count.second < j.second) {  // если встречаем такой же вид транспорта, но у которого остановок больше
                    max_count.second = j.second;                                            // то заменяем значение максимального
                    max_count.first.second = j.first.second;
                }
            }
            cout << max_count.first.first << " " << max_count.first.second << " имеет наибольшее количество остановок, равное " << max_count.second << ".\n";     // закончили прохождение по данному типу транспорта, можно вывести результат
        }
    }
    types_of_transport.clear();

// ЗАДАНИЕ 2
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


// ЗАДАНИЕ 3
    vector<string> numbers;
    unordered_map<string, vector<pair<double, double> > > Routes;
    string last_type = "";
    string type = "";
    cout << "\nЗадание 3:\n";
    for (auto station = set.child("transport_station"); station; station = station.next_sibling()) {
        last_type = type;
        type = station.child("type_of_vehicle").first_child().value();
        if (types_of_transport.find(type) == types_of_transport.end()) {
            if (!types_of_transport.empty()) 
                longest_route(Routes, last_type);
            types_of_transport.insert(type);
            Routes.clear();
        }
        else {
            string routes = station.child("routes").first_child().value();
            numbers = result_(routes);
            string x_y = station.child("coordinates").first_child().value();
            pair<double, double> coordinate = result_coordinates(x_y);
            for (const auto& j : numbers) {
                Routes[j].push_back(coordinate);
            }
        }
    }
    longest_route(Routes, last_type);
}
