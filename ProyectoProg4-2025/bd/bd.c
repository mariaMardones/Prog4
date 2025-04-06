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
    const char *sql = "SELECT numero, precio FROM habitaciones WHERE precio <= ? AND reservado = 0;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_double(stmt, 1, max_precio);

    printf("Habitaciones disponibles con precio <= %.2f:\n", max_precio);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("Número: %d | Precio: %.2f\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_double(stmt, 1));
    }

    sqlite3_finalize(stmt);
    return 1;
}

int hacer_reserva(sqlite3 *db, int num_habitacion, const char *dni_usuario) {
    const char *sql1 = "INSERT INTO reservas (num_habitacion, dni_usuario) VALUES (?, ?);";
    const char *sql2 = "UPDATE habitaciones SET reservado = 1 WHERE numero = ?;";

    sqlite3_stmt *stmt;

    // Insertar reserva
    if (sqlite3_prepare_v2(db, sql1, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error insertando reserva: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, num_habitacion);
    sqlite3_bind_text(stmt, 2, dni_usuario, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error ejecutando inserción: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);

    // Actualizar habitación
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error actualizando habitación: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, num_habitacion);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error ejecutando actualización: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int cancelar_reserva(sqlite3 *db, int codigo_reserva) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT num_habitacion FROM reservas WHERE codigo = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error seleccionando reserva: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, codigo_reserva);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        printf("Reserva no encontrada\n");
        sqlite3_finalize(stmt);
        return 0;
    }

    int num_habitacion = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    // Eliminar reserva
    const char *sql_del = "DELETE FROM reservas WHERE codigo = ?;";
    if (sqlite3_prepare_v2(db, sql_del, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error eliminando reserva: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    sqlite3_bind_int(stmt, 1, codigo_reserva);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Marcar habitación como no reservada
    const char *sql_upd = "UPDATE habitaciones SET reservado = 0 WHERE numero = ?;";
    if (sqlite3_prepare_v2(db, sql_upd, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error actualizando habitación: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    sqlite3_bind_int(stmt, 1, num_habitacion);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return 1;
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
