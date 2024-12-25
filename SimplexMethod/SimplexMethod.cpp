#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

void printTable(const vector<vector<double>>& table) {
    for (const auto& row : table) {
        for (const auto& val : row) {
            cout << setw(10) << val;
        }
        cout << endl;
    }
    cout << endl;
}

void simplexMethod(vector<vector<double>>& table) {
    int numRows = table.size() - 1;
    int numCols = table[0].size();

    while (true) {
        int pivotCol = 0;
        for (int j = 1; j < numCols - 1; ++j) {
            if (table[numRows][j] < table[numRows][pivotCol]) {
                pivotCol = j;
            }
        }

        if (table[numRows][pivotCol] >= 0) {
            break; 
        }

        int pivotRow = -1;
        double minRatio = numeric_limits<double>::max();
        for (int i = 0; i < numRows; ++i) {
            if (table[i][pivotCol] > 0) {
                double ratio = table[i][numCols - 1] / table[i][pivotCol];
                if (ratio < minRatio) {
                    minRatio = ratio;
                    pivotRow = i;
                }
            }
        }

        if (pivotRow == -1) {
            cout << "Unbounded solution!" << endl;
            return;
        }

        double pivotElement = table[pivotRow][pivotCol];

        for (int j = 0; j < numCols; ++j) {
            table[pivotRow][j] /= pivotElement;
        }

        for (int i = 0; i <= numRows; ++i) {
            if (i != pivotRow) {
                double factor = table[i][pivotCol];
                for (int j = 0; j < numCols; ++j) {
                    table[i][j] -= factor * table[pivotRow][j];
                }
            }
        }

        cout << "Updated Table:" << endl;
        printTable(table);
    }

    cout << "Optimal solution found!" << endl;
    cout << "Z = " << table[numRows][numCols - 1] << endl;
    for (int i = 0; i < numRows; ++i) {
        cout << "x" << i + 1 << " = " << table[i][numCols - 1] << endl;
    }
}

int main() {
    int numVariables, numConstraints;

    cout << "Enter the number of variables: ";
    cin >> numVariables;
    cout << "Enter the number of constraints: ";
    cin >> numConstraints;

    int numCols = numVariables + numConstraints + 1;
    int numRows = numConstraints + 1;

    vector<vector<double>> table(numRows, vector<double>(numCols, 0));

    cout << "Enter the coefficients of the objective function (maximize):" << endl;
    for (int j = 0; j < numVariables; ++j) {
        cin >> table[numRows - 1][j];
        table[numRows - 1][j] = -table[numRows - 1][j]; 
    }

    cout << "Enter the coefficients of the constraints (all <=):" << endl;
    for (int i = 0; i < numConstraints; ++i) {
        for (int j = 0; j < numVariables; ++j) {
            cin >> table[i][j];
        }
        cin >> table[i][numCols - 1]; 
        table[i][numVariables + i] = 1; 
    }

    cout << "Initial Table:" << endl;
    printTable(table);

    simplexMethod(table);

    return 0;
}