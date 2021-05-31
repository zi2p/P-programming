#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void put_front(vector<vector<unsigned short int> > h, const vector<vector<unsigned short int> > v, vector<vector<unsigned short int> > &f) {
    for (int i = 0; i < 3; i++) {
//        f[i][0] = v[0][3 - i];
//        f[i][1] = v[1][3 - i];
//        f[i][2] = v[2][3 - i];
//        f[i][3] = h[0][11 - i];
//        f[i][4] = h[1][11 - i];
//        f[i][5] = h[2][11 - i];
//        f[i][6] = v[2][11 - i];
//        f[i][7] = v[1][11 - i];
//        f[i][8] = v[0][11 - i];
//        f[i][9] = h[2][3 - i];
//        f[i][10] = h[1][3 - i];
//        f[i][11] = h[0][3 - i];
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

    static void rotation_plus(vector<vector<unsigned short int> > &arr, unsigned short int layer) {
        unsigned short int buffer[3];
        for (int i = 9; i < 12; i++) {
            buffer[i - 9] = arr[layer][i];
            arr[layer][i] = arr[layer][i - 3];
        }
        for (int i = 8; i > 2; i--)
            arr[layer][i] = arr[layer][i - 3];
        for (int i = 0; i < 3; i++)
            arr[layer][i] = buffer[i];
    }

    static void rotation_minus(vector<vector<unsigned short int> > &arr, unsigned short int layer) {
        unsigned short int buffer[3];
        for (int i = 0; i < 3; i++) {
            buffer[i] = arr[layer][i];
            arr[layer][i] = arr[layer][i + 3];
        }
        for (int i = 3; i < 9; i++)
            arr[layer][i] = arr[layer][i + 3];
        for (int i = 9; i < 12; i++)
            arr[layer][i] = buffer[i - 9];
    }

    void update(vector<vector<unsigned short int> > &arr1, vector<vector<unsigned short int> > &arr2) {
        if (arr1 != front && arr2 != front) {
            for (int i = 0; i < 3; i++) {                   // 1 - горизонтал, 2 - вертикал
                for (int k = 3; k < 6; k++)
                    arr2[i][k] = front[k - 3][5 - i];
                for (int k = 9; k <= 11; k++)
                    arr2[i][k] = front[11 - k][9 + i];
                for (int k = 3; k < 6; k++)
                    arr1[i][k] = front[k - 3][2 - i];
                for (int k = 9; k <= 11; k++)
                    arr1[i][k] = front[2 - k][6 + i];
            }
        }

        if (arr1 != vertical && arr2 != vertical) {
            for (int i = 0; i < 3; i++) {                        // 1 - горизонт, 2 - фронт
                for (int j = 0; j < 3; j++)
                    arr1[i][j] = vertical[j][2 - i];
                for (int j = 6; j < 9; j++)
                    arr1[i][j] = vertical[8 - j][6 - i];
                put_front(arr1, vertical, arr2);
//                for (int j = 3; j < 6; j++)
//                    arr2[i][j] = vertical[5 - j][3 + i];
//                for (int j = 11; j >= 9; j--)
//                    arr2[i][j] = vertical[j - 9][11 - i];
            }
        }

        if (arr1 != horizontal && arr2 != horizontal) {
            for (int i = 0; i < 3; i++) {                       // 1 - вертикал, 2 - фронт
                for (int j = 0; j < 3; j++)
                    arr1[i][j] = horizontal[2 - j][i];
                for (int j = 6; j < 9; j++)
                    arr1[i][j] = horizontal[8 - j][6 + i];
                put_front(horizontal, arr1, arr2);
//                for (int j = 0; j < 3; j++)
//                    arr2[i][j] = horizontal[2 - j][3 + i];
//                for (int j = 8; j >= 6; j--)
//                    arr2[i][j] = horizontal[j - 6][11 - i];
            }
        }
    }

public:

    void input_cube(vector<vector<unsigned short int> > arr_h, vector<vector<unsigned short int> > arr_v, vector<vector<unsigned short int> > arr_f) {  // ввод куба
        horizontal.resize(3, vector<unsigned short int>(12));
        vertical.resize(3, vector<unsigned short int>(12));
        front.resize(3, vector<unsigned short int>(12));
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 12; j++) {
                horizontal[i][j] = arr_h[i][j];
                vertical[i][j] = arr_v[i][j];
                front[i][j] = arr_f[i][j];
            }
    }

    void rotation(string orientation, unsigned short int layer, string side_of_rotation) {
        // ориентация(горизонтальное вращение или вертикальное)
        // слой(1-верхний\левый, 2-средний, 3-нижний\правый)
        // в какую сторону(влево\вправо\вниз\вверх)

        if (orientation[0] == 'h' && side_of_rotation[0] == 'r') {
            rotation_plus(horizontal, layer);
            update(vertical, front);   // какие данные нужно обновить после поворота
        }

        if (orientation[0] == 'h' && side_of_rotation[0] == 'l') {
            rotation_minus(horizontal, layer);
            update(vertical, front);
        }

        if (orientation[0] == 'v' && side_of_rotation[0] == 'd') {
            rotation_minus(vertical, layer);
            update(horizontal, front);
        }

        if (orientation[0] == 'v' && side_of_rotation[0] == 'u') {
            rotation_plus(vertical, layer);
            update(horizontal, front);
        }

        if (orientation[0] == 'f' && side_of_rotation[0] == 'r') {
            rotation_plus(front, layer);
            update(horizontal, vertical);
        }

        if (orientation[0] == 'f' && side_of_rotation[0] == 'l') {
            rotation_minus(front, layer);
            update(horizontal, vertical);
        }
    }

    void write() {
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 3; j++)
                cout << horizontal[j][i] << " ";
            cout << "\n\n";
        }

        for (int i = 0; i < 12; i++) {
            if ((i >= 3 && i < 6) || (i >= 9 && i < 12)) {
                for (int j = 0; j < 3; j++)
                    cout << vertical[j][i] << " ";
                cout << "\n\n";
            }
        }
    }
};

int main() {

    ifstream in("1.in");
    system("chcp 65001");
    Cube cube;
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

    cube.input_cube(h, v, f);
//     cube.write();

//     cube.rotation("horis",1,"left");
//     cout << "=========\n\n";
//     cube.write();

//     cube.rotation("vertic",0,"down");
//     cout << "=========\n\n";
//     cube.write();
}

