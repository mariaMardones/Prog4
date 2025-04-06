#include "habitacion.h"
#include <stdio.h>
#include <sqlite3.h>

void mostrarHabitacion(const Habitacion *h) {
    printf("HabitaciÃ³n %d - Precio: %.2fâ‚¬ - Reservada: %s\n", h->numero, h->precio, h->reservado ? "SÃ­" : "No");
}

void mostrar_habitaciones_disponibles(sqlite3 *db) {
    const char *sql = "SELECT numero, precio FROM habitaciones WHERE reservado = 0;";
    sqlite3_exec(db, sql, [](void *p, int argc, char **argv, char **azCol) {
        printf("HabitaciÃ³n %s - Precio: %sâ‚¬\n", argv[0], argv[1]);
        return 0;
    }, 0, NULL);
}

void filtrar_habitaciones_por_precio(sqlite3 *db, float max_precio) {
    char sql[128];
    snprintf(sql, sizeof(sql), "SELECT numero, precio FROM habitaciones WHERE reservado = 0 AND precio <= %.2f;", max_precio);
    sqlite3_exec(db, sql, [](void *p, int argc, char **argv, char **azCol) {
        printf("HabitaciÃ³n %s - Precio: %sâ‚¬\n", argv[0], argv[1]);
        return 0;
    }, 0, NULL);
}
