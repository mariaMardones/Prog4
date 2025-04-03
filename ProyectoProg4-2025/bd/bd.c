#include <stdio.h>
#include "bd.h"

#define BD_NOMBRE "hotel.db"

int inicializar_bd(sqlite3 **db) {
    int rc = sqlite3_open(BD_NOMBRE, db);
    if (rc) {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        return rc;
    }

    const char *sql_usuarios =
        "CREATE TABLE IF NOT EXISTS Usuario ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre TEXT NOT NULL, "
        "dni TEXT UNIQUE NOT NULL, "
        "contrasena TEXT NOT NULL, "
        "rol INTEGER NOT NULL);";  // 0 = Cliente, 1 = Administrador

    const char *sql_reservas =
        "CREATE TABLE IF NOT EXISTS Reserva ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "usuario_id INTEGER, "
        "habitacion INTEGER NOT NULL, "
        "fecha TEXT NOT NULL, "
        "FOREIGN KEY(usuario_id) REFERENCES Usuario(id));";

    char *err_msg = NULL;
    if (sqlite3_exec(*db, sql_usuarios, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Error al crear la tabla Usuario: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    if (sqlite3_exec(*db, sql_reservas, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Error al crear la tabla Reserva: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    return 0;
}

void cerrar_bd(sqlite3 *db) {
    sqlite3_close(db);
}
