#include "bd.h"
#include <stdio.h>
#include <stdlib.h>

int conectar_bd(sqlite3 **db) {
    if (sqlite3_open("hotel.db", db) != SQLITE_OK) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        return 0;
    }
    return 1;
}

int crear_tablas(sqlite3 *db) {
    const char *sql_usuarios =
        "CREATE TABLE IF NOT EXISTS usuarios ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nombre TEXT NOT NULL, "
        "dni TEXT UNIQUE NOT NULL, "
        "contrasena TEXT NOT NULL, "
        "rol INTEGER NOT NULL);";

    const char *sql_habitaciones =
        "CREATE TABLE IF NOT EXISTS habitaciones ("
        "numero INTEGER PRIMARY KEY, "
        "precio REAL NOT NULL, "
        "reservado INTEGER DEFAULT 0);";

    const char *sql_reservas =
        "CREATE TABLE IF NOT EXISTS reservas ("
        "codigo INTEGER PRIMARY KEY AUTOINCREMENT, "
        "num_habitacion INTEGER NOT NULL, "
        "dni_usuario TEXT NOT NULL, "
        "FOREIGN KEY (num_habitacion) REFERENCES habitaciones(numero), "
        "FOREIGN KEY (dni_usuario) REFERENCES usuarios(dni));";

    char *errmsg = NULL;

    if (sqlite3_exec(db, sql_usuarios, 0, 0, &errmsg) != SQLITE_OK) {
        printf("Error creando tabla usuarios: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 0;
    }

    if (sqlite3_exec(db, sql_habitaciones, 0, 0, &errmsg) != SQLITE_OK) {
        printf("Error creando tabla habitaciones: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 0;
    }

    if (sqlite3_exec(db, sql_reservas, 0, 0, &errmsg) != SQLITE_OK) {
        printf("Error creando tabla reservas: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 0;
    }

    return 1;
}


int insertar_usuario(sqlite3 *db, const char *nombre, const char *dni, const char *contrasena, int rol) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO usuarios (nombre, dni, contrasena, rol) VALUES (?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, dni, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, contrasena, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, rol);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error insertando usuario: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int verificar_login(sqlite3 *db, const char *dni, const char *contrasena, int *rol) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT rol FROM usuarios WHERE dni = ? AND contrasena = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *rol = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return 1;
    } else {
        sqlite3_finalize(stmt);
        return 0;
    }
}

int insertar_habitacion(sqlite3 *db, int numero, float precio) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO habitaciones (numero, precio, reservado) VALUES (?, ?, 0);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, numero);
    sqlite3_bind_double(stmt, 2, precio);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error insertando habitación: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int listar_habitaciones(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT numero, precio, reservado FROM habitaciones;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("Habitaciones:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int reservado = sqlite3_column_int(stmt, 2);
        printf("Número: %d | Precio: %.2f | Reservado: %s\n",
            sqlite3_column_int(stmt, 0),
            sqlite3_column_double(stmt, 1),
            reservado ? "Sí" : "No");
    }

    sqlite3_finalize(stmt);
    return 1;
}

int filtrar_habitaciones_por_precio(sqlite3 *db, float max_precio) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, nombre, precio FROM Habitacion WHERE precio <= ?;";
    int rc;

    // Prepara la consulta
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 0; // Error al preparar la consulta
    }

    // Bind del parámetro max_precio
    sqlite3_bind_double(stmt, 1, max_precio);

    // Ejecutar la consulta y procesar los resultados
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *nombre = (const char *)sqlite3_column_text(stmt, 1);
        float precio = (float)sqlite3_column_double(stmt, 2);
        printf("ID: %d, Nombre: %s, Precio: %.2f\n", id, nombre, precio);
    }

    // Verifica si hubo error al ejecutar la consulta
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL step error: %s\n", sqlite3_errmsg(db));
    }

    // Finaliza la consulta
    sqlite3_finalize(stmt);
    return 1; // Éxito
}

int hacer_reserva(sqlite3 *db, const char *dni, int habitacion, const char *fecha) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO reservas (habitacion, dni, fecha) VALUES (?, ?, ?)";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, habitacion);  // Usar 'habitacion' que es el parámetro
    sqlite3_bind_text(stmt, 2, dni, -1, SQLITE_STATIC);  // Usar 'dni' que es el parámetro
    sqlite3_bind_text(stmt, 3, fecha, -1, SQLITE_STATIC);  // Usar 'fecha' que es el parámetro

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        return 1;  // Error al insertar
    }

    sqlite3_finalize(stmt);
    return 0;  // Reserva exitosa
}

int cancelar_reserva(sqlite3 *db, const char *dni, int habitacion) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM reservas WHERE dni_usuario = ? AND habitacion = ?;";

    // Preparar la sentencia SQL
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        return 0;  // Error al preparar la sentencia
    }

    // Vincular los parámetros (dni, habitacion)
    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, habitacion);

    // Ejecutar la sentencia
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        return 0;  // Error al ejecutar la sentencia
    }

    return 1;  // Reserva cancelada correctamente
}


int listar_reservas_usuario(sqlite3 *db, const char *dni_usuario) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT codigo, num_habitacion FROM reservas WHERE dni_usuario = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error consultando reservas: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, dni_usuario, -1, SQLITE_STATIC);

    printf("Reservas activas:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("Código: %d | Habitación: %d\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_int(stmt, 1));
    }

    sqlite3_finalize(stmt);
    return 1;
}


int consultar_usuarios(sqlite3 *db) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, nombre, dni, rol FROM usuarios;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("\nLista de Usuarios:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID: %d, Nombre: %s, DNI: %s, Rol: %d\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_text(stmt, 2),
               sqlite3_column_int(stmt, 3));
    }

    sqlite3_finalize(stmt);
    return 1;
}

int cerrar_bd(sqlite3 *db) {
    if (sqlite3_close(db) != SQLITE_OK) {
        printf("Error al cerrar la base de datos\n");
        return 0;
    }
    return 1;
}
