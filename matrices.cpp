#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

// Clase para manejar matrices
class Matrix {
private:
    vector<vector<double>> data;
    int rows, cols;

public:
    // Constructor
    Matrix(int r, int c) : rows(r), cols(c), data(r, vector<double>(c, 0)) {}

    // Obtener dimensiones
    int getRows() const { return rows; }
    int getCols() const { return cols; }

    // Acceso a elementos
    double& at(int r, int c) { return data[r][c]; }
    const double& at(int r, int c) const { return data[r][c]; }

    // Leer matriz desde entrada estándar
    void input() {
        cout << "Introduce los elementos de la matriz (" << rows << "x" << cols << "):\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cin >> data[i][j];
            }
        }
    }

    // Mostrar matriz
    void print() const {
        for (const auto& row : data) {
            for (double val : row) {
                cout << val << " ";
            }
            cout << endl;
        }
    }

    // Sumar matrices
    Matrix operator+(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("Las dimensiones de las matrices no coinciden para la suma.");
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.at(i, j) = this->at(i, j) + other.at(i, j);
            }
        }
        return result;
    }

    // Restar matrices
    Matrix operator-(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("Las dimensiones de las matrices no coinciden para la resta.");
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.at(i, j) = this->at(i, j) - other.at(i, j);
            }
        }
        return result;
    }

    // Multiplicar matrices
    Matrix operator*(const Matrix& other) {
        if (cols != other.rows) {
            throw invalid_argument("El número de columnas de la primera matriz debe ser igual al número de filas de la segunda matriz para la multiplicación.");
        }
        Matrix result(rows, other.cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < other.cols; ++j) {
                for (int k = 0; k < cols; ++k) {
                    result.at(i, j) += this->at(i, k) * other.at(k, j);
                }
            }
        }
        return result;
    }

    // Calcular la inversa de una matriz
    Matrix inverse() const {
        if (rows != cols) {
            throw invalid_argument("Solo las matrices cuadradas tienen inversa.");
        }

        int n = rows;
        Matrix augmented(n, 2 * n);

        // Construir la matriz aumentada [A|I]
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                augmented.at(i, j) = this->at(i, j);
                augmented.at(i, j + n) = (i == j) ? 1 : 0;
            }
        }

        // Realizar eliminación gaussiana
        for (int i = 0; i < n; ++i) {
            if (augmented.at(i, i) == 0) {
                throw invalid_argument("La matriz es singular y no tiene inversa.");
            }

            double diag = augmented.at(i, i);
            for (int j = 0; j < 2 * n; ++j) {
                augmented.at(i, j) /= diag;
            }

            for (int k = 0; k < n; ++k) {
                if (k != i) {
                    double factor = augmented.at(k, i);
                    for (int j = 0; j < 2 * n; ++j) {
                        augmented.at(k, j) -= factor * augmented.at(i, j);
                    }
                }
            }
        }

        // Extraer la inversa de la matriz aumentada
        Matrix inverse(n, n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                inverse.at(i, j) = augmented.at(i, j + n);
            }
        }

        return inverse;
    }

    // Dividir matrices (A / B = A * B^-1)
    Matrix operator/(const Matrix& other) {
        return *this * other.inverse();
    }
};

// Función principal
int main() {
    cout << "===========================================\n";
    cout << "            CALCULADORA DE MATRICES        \n";
    cout << "===========================================\n";
    cout << "Seleccione una opción:\n";
    cout << "1. Sumar matrices\n";
    cout << "2. Restar matrices\n";
    cout << "3. Multiplicar matrices\n";
    cout << "4. Dividir matrices\n";
    cout << "5. Salir\n";
    cout << "===========================================\n";

    int opcion;
    cout << "Opción: ";
    cin >> opcion;

    if (opcion == 1 || opcion == 2 || opcion == 3 || opcion == 4) {
        int r1, c1, r2, c2;
        cout << "Introduce las dimensiones de la primera matriz (filas columnas): ";
        cin >> r1 >> c1;
        Matrix A(r1, c1);
        cout << "Matriz A:\n";
        A.input();

        cout << "Introduce las dimensiones de la segunda matriz (filas columnas): ";
        cin >> r2 >> c2;
        Matrix B(r2, c2);
        cout << "Matriz B:\n";
        B.input();

        try {
            if (opcion == 1) {
                if (r1 != r2 || c1 != c2) throw invalid_argument("Las dimensiones deben coincidir para la suma.");
                Matrix C = A + B;
                cout << "Resultado de la suma:\n";
                C.print();
            } else if (opcion == 2) {
                if (r1 != r2 || c1 != c2) throw invalid_argument("Las dimensiones deben coincidir para la resta.");
                Matrix C = A - B;
                cout << "Resultado de la resta:\n";
                C.print();
            } else if (opcion == 3) {
                if (c1 != r2) throw invalid_argument("El número de columnas de A debe ser igual al número de filas de B para la multiplicación.");
                Matrix C = A * B;
                cout << "Resultado de la multiplicación:\n";
                C.print();
            } else if (opcion == 4) {
                if (c1 != r2 || r2 != c2) throw invalid_argument("La matriz B debe ser cuadrada y compatible para la división.");
                Matrix C = A / B;
                cout << "Resultado de la división:\n";
                C.print();
            }
        } catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }

    cout << "===========================================\n";
    cout << "           PROGRAMA TERMINADO              \n";
    cout << "===========================================\n";
    return 0;
}
