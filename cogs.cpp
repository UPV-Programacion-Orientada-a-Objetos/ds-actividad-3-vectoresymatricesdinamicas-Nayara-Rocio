#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
using namespace std;

const int PRODUCTOS = 5;
const int MATERIAS = 4;
const int SEMANAS_INICIALES = 4;

int requerimientos[PRODUCTOS][MATERIAS] = {
    {2, 1, 0, 3},
    {1, 2, 1, 0},
    {3, 0, 2, 1},
    {0, 1, 1, 2},
    {2, 1, 1, 1}
};

void limpiarEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int leerEntero(string texto, int min, int max) {
    int num;
    while (true) {
        cout << texto;
        if (cin >> num && num >= min && num <= max) {
            limpiarEntrada();
            return num;
        }
        cout << "Entrada invalida, intenta de nuevo.\n";
        limpiarEntrada();
    }
}

float leerDecimal(string texto, float min = 0) {
    float num;
    while (true) {
        cout << texto;
        if (cin >> num && num >= min) {
            limpiarEntrada();
            return num;
        }
        cout << "Entrada invalida, intenta de nuevo.\n";
        limpiarEntrada();
    }
}

float** crearMatriz(int filas, int columnas) {
    float** m = new float*[filas];
    for (int i = 0; i < filas; i++) {
        m[i] = new float[columnas];
        for (int j = 0; j < columnas; j++) {
            m[i][j] = 0;
        }
    }
    return m;
}

void liberarMatriz(float**& m, int filas) {
    for (int i = 0; i < filas; i++) {
        delete[] m[i];
    }
    delete[] m;
    m = nullptr;
}

float** agregarSemana(float** matriz, int filas, int columnas) {
    float** nueva = crearMatriz(filas, columnas + 1);
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            nueva[i][j] = matriz[i][j];
        }
    }
    liberarMatriz(matriz, filas);
    cout << "Nueva semana agregada. Total de semanas: " << columnas + 1 << "\n";
    return nueva;
}

void mostrarPlan(float** matriz, int filas, int columnas) {
    cout << "\nPlan de Produccion:\n";
    cout << setw(10) << "Producto";
    for (int j = 0; j < columnas; j++)
        cout << setw(10) << "Semana" << j;
    cout << "\n";

    for (int i = 0; i < filas; i++) {
        cout << setw(10) << "P" << i;
        for (int j = 0; j < columnas; j++) {
            cout << setw(10) << matriz[i][j];
        }
        cout << "\n";
    }
}

void consumoMP(float** matriz, int semana, float* consumo) {
    for (int m = 0; m < MATERIAS; m++)
        consumo[m] = 0;

    for (int p = 0; p < PRODUCTOS; p++) {
        for (int m = 0; m < MATERIAS; m++) {
            consumo[m] += matriz[p][semana] * requerimientos[p][m];
        }
    }
}

void calcularCOGS(float** matriz, int columnas, float* inventario, float* costoMP, float* costoProd) {
    int semana = leerEntero("Selecciona semana (0-" + to_string(columnas - 1) + "): ", 0, columnas - 1);

    float consumo[MATERIAS];
    consumoMP(matriz, semana, consumo);

    float totalProd = 0;
    for (int p = 0; p < PRODUCTOS; p++) {
        totalProd += matriz[p][semana] * costoProd[p];
    }

    float inventarioInicial = 0, inventarioFinal = 0;
    for (int m = 0; m < MATERIAS; m++) {
        inventarioInicial += inventario[m] * costoMP[m];
        inventarioFinal += (inventario[m] - consumo[m]) * costoMP[m];
        if (inventarioFinal < 0) inventarioFinal = 0;
    }

    float compras = leerDecimal("Valor de compras de MP (si no hubo, 0): ");
    float COGS = inventarioInicial + compras - inventarioFinal;

    cout << fixed << setprecision(2);
    cout << "\n--- Reporte Semana " << semana << " ---\n";
    cout << "Costo Total de Produccion: $" << totalProd << "\n";
    cout << "Inventario Inicial: $" << inventarioInicial << "\n";
    cout << "Inventario Final: $" << inventarioFinal << "\n";
    cout << "Compras: $" << compras << "\n";
    cout << "COGS (Inv. Inicial + Compras - Inv. Final): $" << COGS << "\n";

    cout << "\nConsumo de Materia Prima:\n";
    for (int m = 0; m < MATERIAS; m++)
        cout << "MP-" << (m + 1) << ": " << consumo[m] << " unidades\n";
}

void modificarProduccion(float** matriz, int columnas) {
    int producto = leerEntero("Producto (0-4): ", 0, PRODUCTOS - 1);
    int semana = leerEntero("Semana (0-" + to_string(columnas - 1) + "): ", 0, columnas - 1);
    float cantidad = leerDecimal("Cantidad a producir: ");
    matriz[producto][semana] = cantidad;
    cout << "Produccion actualizada correctamente.\n";
}

void mostrarCostos(float* costosProd, float* inventario, float* costoMP) {
    cout << "\nCostos Unitarios de Produccion:\n";
    for (int i = 0; i < PRODUCTOS; i++) {
        cout << "Producto " << i << ": $" << costosProd[i] << "\n";
    }
    cout << "\nInventario y Costo de Materia Prima:\n";
    for (int i = 0; i < MATERIAS; i++) {
        cout << "MP-" << (i + 1) << ": " << inventario[i] << " unidades | Costo: $" << costoMP[i] << "\n";
    }
}

int main() {
    cout << "--- Sistema de Planificacion y Costos (COGS) ---\n";

    int columnas = SEMANAS_INICIALES;
    float** plan = crearMatriz(PRODUCTOS, columnas);

    float* costoProd = new float[PRODUCTOS]{10, 15, 20, 25, 30};
    float* inventario = new float[MATERIAS]{1000, 800, 600, 400};
    float costoMP[MATERIAS] = {2.5, 3.0, 1.2, 5.0};

    for (int i = 0; i < PRODUCTOS; i++) {
        for (int j = 0; j < columnas; j++) {
            plan[i][j] = (i + 1) * 100;
        }
    }

    int opcion;
    do {
        cout << "\n--- Menu Principal ---\n";
        cout << "1. Ver Plan de Produccion\n";
        cout << "2. Agregar Nueva Semana\n";
        cout << "3. Modificar Produccion\n";
        cout << "4. Calcular COGS\n";
        cout << "5. Ver Costos e Inventario\n";
        cout << "6. Salir\n";
        opcion = leerEntero("Opcion: ", 1, 6);

        switch (opcion) {
            case 1: mostrarPlan(plan, PRODUCTOS, columnas); break;
            case 2: plan = agregarSemana(plan, PRODUCTOS, columnas++); break;
            case 3: modificarProduccion(plan, columnas); break;
            case 4: calcularCOGS(plan, columnas, inventario, costoMP, costoProd); break;
            case 5: mostrarCostos(costoProd, inventario, costoMP); break;
            case 6: cout << "Saliendo del sistema...\n"; break;
        }

    } while (opcion != 6);

    liberarMatriz(plan, PRODUCTOS);
    delete[] costoProd;
    delete[] inventario;

    cout << "Memoria liberada correctamente.\n";
    return 0;
}
