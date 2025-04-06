#include <stdio.h>
#include <string.h>
#include "usuario.h"
#include "../bd/bd.h"

int registrar_usuario(sqlite3 *db) {
    char nombre[100], dni[20], contrasena[50];
    int rol;

    printf("\n--- Registro de Usuario ---\n");
    printf("Nombre: ");
    scanf(" %[^\n]", nombre);

    printf("DNI: ");
    scanf(" %s", dni);

    printf("Contraseña: ");
    scanf(" %s", contrasena);

    printf("Rol (0 = Cliente, 1 = Administrador): ");
    scanf("%d", &rol);

    if (insertar_usuario(db, nombre, dni, contrasena, rol)) {
        printf("Registro exitoso.\n");
        return 1;
    } else {
        printf("Error en el registro.\n");
        return 0;
    }
}

int iniciar_sesion(sqlite3 *db, Usuario *usuario) {
    char dni[20], contrasena[50];
    int rol;

    printf("\n--- Iniciar Sesión ---\n");
    printf("DNI: ");
    scanf(" %s", dni);

    printf("Contraseña: ");
    scanf(" %s", contrasena);

    if (verificar_login(db, dni, contrasena, &rol)) {
        strcpy(usuario->dni, dni);
        usuario->rol = rol;
        printf("Inicio de sesión exitoso. Rol: %s\n", rol == ROL_ADMIN ? "Administrador" : "Cliente");
        return 1;
    } else {
        printf("Credenciales incorrectas.\n");
        return 0;
    }
}

void realizar_reserva_cliente(sqlite3 *db, const char *dni) {
    int habitacion;
    char fecha[20];

    printf("Ingrese el número de habitación: ");
    scanf("%d", &habitacion);

    printf("Ingrese la fecha de reserva (YYYY-MM-DD): ");
    scanf("%s", fecha);

    hacer_reserva(db, dni, habitacion, fecha);

    printf("Reserva realizada con éxito.\n");
}

void cancelar_reserva_cliente(sqlite3 *db, const char *dni) {
    int habitacion;

    printf("Ingrese el número de habitación a cancelar: ");
    scanf("%d", &habitacion);

    cancelar_reserva(db, dni, habitacion);

    printf("Reserva cancelada con éxito.\n");
}

void ver_habitaciones_disponibles_cliente(sqlite3 *db) {
    mostrar_habitaciones_disponibles(db);
}

void filtrar_habitaciones_cliente(sqlite3 *db) {
    float max_precio;

    printf("Ingrese el precio máximo: ");
    scanf("%f", &max_precio);

    filtrar_habitaciones_por_precio(db, max_precio);
}

void consultar_reservas_cliente(sqlite3 *db, const char *dni) {
    ver_reservas_activas(db, dni);
}

int callback_cliente(void *data, int argc, char **argv, char **azColName) {
    printf("Cliente: %s - DNI: %s\n", argv[0], argv[1]);
    return 0;
}

void visualizar_clientes_admin(sqlite3 *db) {
    const char *sql = "SELECT nombre, dni FROM Usuario WHERE rol = 0;";
    sqlite3_exec(db, sql, callback_cliente, 0, NULL);
}



void buscar_cliente_por_dni_admin(sqlite3 *db) {
    char dni[20];
    printf("Ingrese el DNI del cliente a buscar: ");
    scanf("%s", dni);

    const char *sql = "SELECT nombre, dni FROM Usuario WHERE dni = ?;";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, dni, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("Cliente encontrado: %s - DNI: %s\n",
               sqlite3_column_text(stmt, 0),
               sqlite3_column_text(stmt, 1));
    } else {
        printf("Cliente no encontrado.\n");
    }

    sqlite3_finalize(stmt);
}

void cancelar_reserva_admin(sqlite3 *db) {
    int habitacion;
    char dni[20];

    printf("Ingrese el DNI del cliente: ");
    scanf("%s", dni);

    printf("Ingrese el número de habitación a cancelar: ");
    scanf("%d", &habitacion);

    cancelar_reserva(db, dni, habitacion);

    printf("Reserva cancelada con éxito.\n");
}

void eliminar_cliente_admin(sqlite3 *db) {
    char dni[20];

    printf("Ingrese el DNI del cliente a eliminar: ");
    scanf("%s", dni);

    // Primero eliminamos las reservas del cliente
    const char *sql_reservas = "DELETE FROM Reserva WHERE usuario_id = (SELECT id FROM Usuario WHERE dni = ?);";
    sqlite3_stmt *stmt_reservas;
    sqlite3_prepare_v2(db, sql_reservas, -1, &stmt_reservas, 0);
    sqlite3_bind_text(stmt_reservas, 1, dni, -1, SQLITE_STATIC);
    sqlite3_step(stmt_reservas);
    sqlite3_finalize(stmt_reservas);

    // Luego eliminamos al cliente de la base de datos
    const char *sql_cliente = "DELETE FROM Usuario WHERE dni = ?;";
    sqlite3_stmt *stmt_cliente;
    sqlite3_prepare_v2(db, sql_cliente, -1, &stmt_cliente, 0);
    sqlite3_bind_text(stmt_cliente, 1, dni, -1, SQLITE_STATIC);
    sqlite3_step(stmt_cliente);
    sqlite3_finalize(stmt_cliente);

    printf("Cliente eliminado con éxito.\n");
}

void cerrar_sesion_admin() {
    printf("Sesión cerrada.\n");
}
