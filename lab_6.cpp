#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

enum rotation {
    horizontal_,
    vertical_,
    front_
};

void put_front(const vector<vector<unsigned short int> > &h, const vector<vector<unsigned short int> > &v, vector<vector<unsigned short int> > &f) {
    for (int i = 0; i < 3; i++) {
        f[i][0] = h[2][3 - i];
        f[i][1] = h[1][3 - i];
        f[i][2] = h[0][3 - i];
        f[i][3] = v[2][3 - i];
        f[i][4] = v[1][3 - i];
        f[i][5] = v[0][3 - i];
        f[i][6] = h[0][11 - i];
        f[i][7] = h[1][11 - i];
        f[i][8] = h[2][11 - i];
        f[i][9] = v[0][11 - i];
        f[i][10] = v[1][11 - i];
        f[i][11] = v[2][11 - i];
    }
}

class Cube {
private:
    vector<vector<unsigned short int> > horizontal;  // пояс горизонтальный [слой][грань.ячейка]
    vector<vector<unsigned short int> > vertical;       // пояс вертикальный [слой][грань.ячейка]
    vector<vector<unsigned short int> > front;

    static void rotation_plus(vector<unsigned short int> &arr) {
        unsigned short int buffer[3];
        for (int i = 9; i < 12; i++) {
            buffer[i - 9] = arr[i];
            arr[i] = arr[i - 3];
        }
        for (int i = 8; i > 2; i--)
            arr[i] = arr[i - 3];
        for (int i = 0; i < 3; i++)
            arr[i] = buffer[i];
    }

    static void rotation_minus(vector<unsigned short int> &arr) {
        unsigned short int buffer[3];
        for (int i = 0; i < 3; i++) {
            buffer[i] = arr[i];
            arr[i] = arr[i + 3];
        }
        for (int i = 3; i < 9; i++)
            arr[i] = arr[i + 3];
        for (int i = 9; i < 12; i++)
            arr[i] = buffer[i - 9];
    }

    void update_vertical(vector<vector<unsigned short int> > &h, vector<vector<unsigned short int> > &f) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++)
                    h[i][j] = vertical[j][2 - i];
                for (int j = 6; j < 9; j++)
                    h[i][j] = vertical[8 - j][6 - i];
                put_front(h, vertical, f);
            }
    }

    void update_front(vector<vector<unsigned short int> > &h, vector<vector<unsigned short int> > &v) {
        for (int i = 0; i < 3; i++) {
            for (int k = 3; k < 6; k++)
                v[i][k] = front[k - 3][5 - i];
            for (int k = 9; k <= 11; k++)
                v[i][k] = front[11 - k][9 + i];
            for (int k = 3; k < 6; k++)
                h[i][k] = front[k - 3][2 - i];
            for (int k = 9; k <= 11; k++)
                h[i][k] = front[2 - k][6 + i];
            }
    }

    void update_horizomtal(vector<vector<unsigned short int> > &v, vector<vector<unsigned short int> > &f) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++)
                v[i][j] = horizontal[2 - j][i];
            for (int j = 6; j < 9; j++)
                v[i][j] = horizontal[8 - j][6 + i];
            put_front(horizontal, v, f);
        }
    }

public:

    Cube(const vector<vector<unsigned short int>>& arr_h, const vector<vector<unsigned short int>>& arr_v, const vector<vector<unsigned short int>> & arr_f) :
        horizontal(arr_h),
        vertical(arr_v),
        front(arr_f) {}

    void rotation(rotation orientation, unsigned short int layer, string side_of_rotation) {
        // ориентация(горизонтальное вращение или вертикальное)
        // слой(1-верхний\левый, 2-средний, 3-нижний\правый)
        // в какую сторону(влево\вправо\вниз\вверх)

        if (orientation == horizontal_ && side_of_rotation[0] == 'r') {
            rotation_plus(horizontal[layer]);
            update_horizomtal(vertical, front);   // какие данные нужно обновить после поворота
        }

        if (orientation == horizontal_ && side_of_rotation[0] == 'l') {
            rotation_minus(horizontal[layer]);
            update_horizomtal(vertical, front);
        }

        if (orientation == vertical_ && side_of_rotation[0] == 'd') {
            rotation_minus(vertical[layer]);
            update_vertical(horizontal, front);
        }

        if (orientation == vertical_ && side_of_rotation[0] == 'u') {
            rotation_plus(vertical[layer]);
            update_vertical(horizontal, front);
        }

        if (orientation == front_ && side_of_rotation[0] == 'r') {
            rotation_plus(front[layer]);
            update_front(horizontal, vertical);
        }

        if (orientation == front_ && side_of_rotation[0] == 'l') {
            rotation_minus(front[layer]);
            update_front(horizontal, vertical);
        }
    }

    friend std::ostream& operator<<(std::ostream &out, const Cube &p);

};

std::ostream &operator<<(std::ostream &out, const Cube &cube) {                        // перегрузка оператора
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 3; j++)
            cout << cube.horizontal[j][i] << " ";
        cout << "\n";
        if ((i + 1) % 3 == 0) {
            cout << "-----------\n";
        }
    }
    cout << "--------\n";

    for (int i = 0; i < 12; i++) {
        if ((i >= 3 && i < 6) || (i >= 9 && i < 12)) {
            for (int j = 0; j < 3; j++)
                cout << cube.vertical[j][i] << " ";
            cout << "\n";
            if ((i + 1) % 3 == 0) {
                cout << "-----------\n";
            }
        }
    }
    return cout << "---------\n";
}

int main() {

    ifstream in("1.in");
    system("chcp 65001");
    vector<vector<unsigned short int> > h(3, vector<unsigned short int>(12));
    vector<vector<unsigned short int> > v(3, vector<unsigned short int>(12));
    vector<vector<unsigned short int> > f(3, vector<unsigned short int>(12));

    cout << "Введите Ваш кубик.\n Зафиксируйте одну из граней перед Вами. Назовем эту грань первой. "
            "Введите начиная с левого верхнего квадрата верхний \"слой\" цветов. Повторите процедуру трижды. ";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 12; j++)
            in >> h[i][j];

    cout << "Вы ввели горизонтальные слои Вашего кубика. Теперь сделаем подобное с вертикальными."
            " Начинаем с левого нижнего квадрата и идем вверх по контуру куба.\n";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 12; j++)
            in >> v[i][j];

    put_front(h, v, f);
    Cube cube(h, v, f);
    cout << cube;

    cube.rotation(horizontal_,1,"left");
    cout << "=========\n\n";
    cout << cube;

    cube.rotation(vertical_, 0, "down");
    cout << "=========\n\n";
    cout << cube;

    cout << "Ведите, какой Вы хотите поворот:\n Сначала введите какое хоттите вращение, затем, слой, затем, в какую сторону вращать.\n";

    string orientation, side;
    unsigned short int layer;
    // ориентация(горизонтальное вращение или вертикальное)
    // слой(1-верхний\левый, 2-средний, 3-нижний\правый)
    // в какую сторону(влево\вправо\вниз\вверх)
    cin >> orientation >> layer >> side;
    if (orientation[0] == 'h') cube.rotation(horizontal_, layer, side);
    else if (orientation[0] == 'v') cube.rotation(vertical_, layer, side);
    else cube.rotation(front_, layer, side);

}
