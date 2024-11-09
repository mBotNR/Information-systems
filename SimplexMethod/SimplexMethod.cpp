#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// Функция для ввода данных
void inputData(vector<vector<double>>& A, vector<double>& b, vector<double>& c) {
    int m, n;
    cout << "Введите количество переменных: ";
    cin >> n;
    cout << "Введите количество ограничений: ";
    cin >> m;

    A.resize(m, vector<double>(n + m));
    b.resize(m);
    c.resize(n + m);

    cout << "Введите коэффициенты целевой функции (максимизация): ";
    for (int j = 0; j < n; ++j) {
        cin >> c[j];
    }

    cout << "Введите коэффициенты ограничений (левая часть): " << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> A[i][j];
        }
    }

    cout << "Введите правые части ограничений: ";
    for (int i = 0; i < m; ++i) {
        cin >> b[i];
    }

    // Добавляем искусственные переменные
    for (int i = 0; i < m; ++i) {
        A[i][n + i] = 1;
        c[n + i] = 0;
    }
}

// Функция для вывода таблицы симплекс-метода
void printTable(const vector<vector<double>>& A, const vector<double>& b, const vector<double>& c, const vector<int>& basis) {
    int m = A.size();
    int n = A[0].size();

    cout << setw(10) << "Базис" << setw(10) << "c_b" << setw(10);
    for (int j = 0; j < n; ++j) {
        cout << "x" << j + 1 << setw(10);
    }
    cout << setw(10) << "b" << endl;

    for (int i = 0; i < m; ++i) {
        cout << setw(10) << "x" << basis[i] + 1 << setw(10) << c[basis[i]] << setw(10);
        for (int j = 0; j < n; ++j) {
            cout << A[i][j] << setw(10);
        }
        cout << b[i] << endl;
    }

    cout << setw(20) << "z" << setw(10);
    for (int j = 0; j < n; ++j) {
        double z_j = 0;
        for (int i = 0; i < m; ++i) {
            z_j += c[basis[i]] * A[i][j];
        }
        cout << z_j - c[j] << setw(10);
    }
    cout << endl;
}

// Функция для нахождения ведущего столбца
int findPivotColumn(const vector<vector<double>>& A, const vector<double>& c, const vector<int>& basis) {
    int m = A.size();
    int n = A[0].size();
    for (int j = 0; j < n; ++j) {
        double z_j = 0;
        for (int i = 0; i < m; ++i) {
            z_j += c[basis[i]] * A[i][j];
        }
        if (z_j - c[j] < 0) {
            return j;
        }
    }
    return -1;
}

// Функция для нахождения ведущей строки
int findPivotRow(const vector<vector<double>>& A, const vector<double>& b, int pivotCol) {
    int m = A.size();
    int pivotRow = -1;
    double minRatio = numeric_limits<double>::max();
    for (int i = 0; i < m; ++i) {
        if (A[i][pivotCol] > 0) {
            double ratio = b[i] / A[i][pivotCol];
            if (ratio < minRatio) {
                minRatio = ratio;
                pivotRow = i;
            }
        }
    }
    return pivotRow;
}

// Функция для решения задачи симплекс-методом
void simplexMethod(vector<vector<double>>& A, vector<double>& b, vector<double>& c) {
    int m = A.size();
    int n = A[0].size();
    vector<int> basis(m);

    // Инициализация базиса искусственными переменными
    for (int i = 0; i < m; ++i) {
        basis[i] = n - m + i;
    }

    while (true) {
        printTable(A, b, c, basis);

        // Находим ведущий столбец
        int pivotCol = findPivotColumn(A, c, basis);
        if (pivotCol == -1) {
            cout << "Решение найдено." << endl;
            break;
        }

        // Находим ведущую строку
        int pivotRow = findPivotRow(A, b, pivotCol);
        if (pivotRow == -1) {
            cout << "Задача не ограничена. Решение не существует." << endl;
            break;
        }

        // Обновляем базис
        basis[pivotRow] = pivotCol;

        // Обновляем таблицу
        double pivotElement = A[pivotRow][pivotCol];
        for (int j = 0; j < n; ++j) {
            A[pivotRow][j] /= pivotElement;
        }
        b[pivotRow] /= pivotElement;

        for (int i = 0; i < m; ++i) {
            if (i != pivotRow) {
                double factor = A[i][pivotCol];
                for (int j = 0; j < n; ++j) {
                    A[i][j] -= factor * A[pivotRow][j];
                }
                b[i] -= factor * b[pivotRow];
            }
        }
    }

    // Выводим решение
    cout << "Оптимальное решение: " << endl;
    for (int j = 0; j < n - m; ++j) {
        bool isBasis = false;
        for (int i = 0; i < m; ++i) {
            if (basis[i] == j) {
                cout << "x" << j + 1 << " = " << b[i] << endl;
                isBasis = true;
                break;
            }
        }
        if (!isBasis) {
            cout << "x" << j + 1 << " = 0" << endl;
        }
    }

    double objectiveValue = 0;
    for (int i = 0; i < m; ++i) {
        objectiveValue += c[basis[i]] * b[i];
    }
    cout << "Значение целевой функции: " << objectiveValue << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<vector<double>> A;
    vector<double> b;
    vector<double> c;

    inputData(A, b, c);
    simplexMethod(A, b, c);

    return 0;
}