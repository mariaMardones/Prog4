#include <stdio.h>
#include "menus.h"

void mostrar_menu_principal() {
    printf("\n--- Menú Principal ---\n");
    printf("1. Iniciar sesión\n");
    printf("2. Registrarse\n");
    printf("3. Salir\n");
    printf("Seleccione una opción: ");
}

int mostrar_menu_cliente() {
    int opcion;
    printf("\n--- Menú Cliente ---\n");
    printf("1. Realizar reserva\n");
    printf("2. Cancelar reserva\n");
    printf("3. Visualizar habitaciones\n");
    printf("4. Filtrar habitaciones por precio\n");
    printf("5. Consultar mis reservas\n");
    printf("6. Cerrar sesión\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opcion);
    return opcion;
}

int mostrar_menu_admin() {
    int opcion;
    printf("\n--- Menú Administrador ---\n");
    printf("1. Visualizar clientes\n");
    printf("2. Buscar cliente por DNI\n");
    printf("3. Cancelar reserva\n");
    printf("4. Eliminar cliente\n");
    printf("5. Cerrar sesión\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opcion);
    return opcion;
}
