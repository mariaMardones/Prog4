#include "reserva.h"
#include <stdio.h>
#include <sqlite3.h>

void hacer_reserva(sqlite3 *db, const char *dni, int habitacion, const char *fecha) {
    char sql[256];
    snprintf(sql, sizeof(sql),
        "INSERT INTO Reserva (usuario_id, habitacion, fecha) "
        "SELECT id, %d, '%s' FROM Usuario WHERE dni = '%s';",
        habitacion, fecha, dni);
    sqlite3_exec(db, sql, 0, 0, NULL);
}

void cancelar_reserva(sqlite3 *db, const char *dni, int habitacion) {
    char sql[256];
    snprintf(sql, sizeof(sql),
        "DELETE FROM Reserva WHERE habitacion = %d AND usuario_id = (SELECT id FROM Usuario WHERE dni = '%s');",
        habitacion, dni);
    sqlite3_exec(db, sql, 0, 0, NULL);
}

void ver_reservas_activas(sqlite3 *db, const char *dni) {
    const char *sql =
        "SELECT R.id, R.habitacion, R.fecha FROM Reserva R "
        "JOIN Usuario U ON R.usuario_id = U.id WHERE U.dni = ?;";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("Reserva ID: %d | HabitaciÃ³n: %d | Fecha: %s\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_int(stmt, 1),
               sqlite3_column_text(stmt, 2));
    }
    sqlite3_finalize(stmt);
}
