#include "habitacion.h"
#include <stdio.h>
#include "../bd/sqlite3.h"

void mostrarHabitacion(const Habitacion *h) {
    printf("HabitaciÃ³n %d - Precio: %.2fâ‚¬ - Reservada: %s\n", h->numero, h->precio, h->reservado ? "SÃ­" : "No");
}

int callback(void *NotUsed, int argc, char **argv, char **azCol);

int mostrar_habitaciones_disponibles_callback(void *NotUsed, int argc, char **argv, char **azCol) {
    printf("Habitación: %s - Precio: %s\n", argv[0], argv[1]);
    return 0; // Devuelve 0 para continuar la ejecución de sqlite3_exec
}

void mostrar_habitaciones_disponibles(sqlite3 *db) {
    const char *sql = "SELECT numero, precio FROM Habitacion WHERE disponible = 1;";
    char *errMsg = 0;

    int rc = sqlite3_exec(db, sql, mostrar_habitaciones_disponibles_callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

int filtrar_habitaciones_por_precio_callback(void *NotUsed, int argc, char **argv, char **azCol) {
    printf("Habitación: %s - Precio: %s\n", argv[0], argv[1]);
    return 0;
}

//void filtrar_habitaciones_por_precio(sqlite3 *db, float max_precio) {
//    const char *sql = "SELECT numero, precio FROM Habitacion WHERE precio <= 100 AND disponible = 1;";
//    char *errMsg = 0;
//
//    int rc = sqlite3_exec(db, sql, filtrar_habitaciones_por_precio_callback, 0, &errMsg);
//    if (rc != SQLITE_OK) {
//        fprintf(stderr, "SQL error: %s\n", errMsg);
//        sqlite3_free(errMsg);
//    }
//}
