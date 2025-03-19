#include "../domain/menus.h"
#include <stdio.h>

char mostrarMenuPrincipal() {
    char opcion;
        printf("\n--- Menú Principal ---\n");
        printf("1. Iniciar sesión\n");
        printf("2. Registrarse\n");
        printf("0. Salir\n");
        printf("Seleccione una opción: ");
        fflush(stdout);
        fflush(stdin);
        scanf(" %c", &opcion);

        return opcion;

}

char mostrarMenuCliente() {
    char opcion;
        printf("\n--- Menú Cliente ---\n");
        printf("1. Realizar reserva\n");
        printf("2. Cancelar reserva\n");
        printf("3. Ver habitaciones disponibles\n");
        printf("4. Filtrar habitaciones\n");
        printf("5. Consultar reservas activas\n");
        printf("0. Volver al menú principal\n");
        printf("Seleccione una opción: ");
        fflush(stdout);
        fflush(stdin);
        scanf(" %c", &opcion);
        return opcion;
}

char mostrarMenuAdministrador() {
    char opcion;

        printf("\n--- Menú Administrador ---\n");
        printf("1. Visualizar clientes\n");
        printf("2. Buscar cliente por DNI\n");
        printf("3. Cancelar reserva\n");
        printf("4. Eliminar cliente\n");
        printf("0. Volver al menú principal\n");
        printf("Seleccione una opción: ");
        fflush(stdout);
        fflush(stdin);
        scanf(" %c", &opcion);

        return opcion;
}
