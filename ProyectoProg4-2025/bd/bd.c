#include <stdio.h>
#include "bd.h"

#define BD_NOMBRE "hotel.db"

int inicializar_bd(sqlite3 **db) {
	int rc = sqlite3_open(BD_NOMBRE, db);
	    if (rc) {
	        fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(*db));
	        return rc;
	    }
	    // Eliminamos la creación de tablas si la base de datos ya existe.
	    printf("Base de datos abierta correctamente.\n");

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

int registrar_usuario(sqlite3 *db, const char *nombre, const char *dni, const char *contrasena, int rol) {
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO Usuario (nombre, dni, contrasena, rol) VALUES ('%s', '%s', '%s', %d);",
            nombre, dni, contrasena, rol);

    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Error al registrar usuario: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    printf("Usuario registrado correctamente en la base de datos.\n");  // Mensaje de depuración
    return 0;
}

int iniciar_sesion(sqlite3 *db, const char *dni, const char *contrasena, int *rol) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT rol FROM Usuario WHERE dni = ? AND contrasena = ?";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error en la preparación de la consulta: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contrasena, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        *rol = sqlite3_column_int(stmt, 0);  // Recuperamos el rol (0 = cliente, 1 = admin)
        sqlite3_finalize(stmt);
        return 0;  // Inicio de sesión exitoso
    } else {
        printf("Usuario o contraseña incorrectos.\n");
        sqlite3_finalize(stmt);
        return 1;  // Error de autenticación
    }
}

int hacer_reserva(sqlite3 *db, int usuario_id, int habitacion, const char *fecha) {
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO Reserva (usuario_id, habitacion, fecha) VALUES (%d, %d, '%s');",
             usuario_id, habitacion, fecha);

    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Error al hacer la reserva: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    return 0;
}

int cancelar_reserva(sqlite3 *db, int reserva_id) {
    char sql[256];
    snprintf(sql, sizeof(sql), "DELETE FROM Reserva WHERE id = %d;", reserva_id);

    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Error al cancelar la reserva: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    return 0;
}
