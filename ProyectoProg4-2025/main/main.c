#include <stdio.h>
#include <stdlib.h>
#include "../domain/menus.h"
#include "../bd/bd.h"
#include "../domain/usuario.h"

int main() {
    sqlite3 *db;
    Usuario usuario;

    if (!conectar_bd(&db)) {
        return 1;
    }

    crear_tablas(db);

    int opcion;
    int sesion_iniciada = 0;

    do {
        mostrar_menu_principal();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                if (iniciar_sesion(db, &usuario)) {
                    sesion_iniciada = 1;

                    int subopcion;
                    if (usuario.rol == ROL_CLIENTE) {
                        do {
                            subopcion = mostrar_menu_cliente();
                            switch (subopcion) {
                                case 1:
                                    realizar_reserva_cliente(db, usuario.dni);
                                    break;
                                case 2:
                                    cancelar_reserva_cliente(db, usuario.dni);
                                    break;
                                case 3:
                                    ver_habitaciones_disponibles_cliente(db);
                                    break;
                                case 4:
                                    filtrar_habitaciones_cliente(db);
                                    break;
                                case 5:
                                    consultar_reservas_cliente(db, usuario.dni);
                                    break;
                                case 6:
                                    printf("Sesión cerrada.\n");
                                    break;
                                default:
                                    printf("Opción no válida.\n");
                            }
                        } while (subopcion != 6);
                    } else if (usuario.rol == ROL_ADMIN) {
                        do {
                            subopcion = mostrar_menu_admin();
                            switch (subopcion) {
                                case 1:
                                    visualizar_clientes_admin(db);
                                    break;
                                case 2:
                                    buscar_cliente_por_dni_admin(db);
                                    break;
                                case 3:
                                    cancelar_reserva_admin(db);
                                    break;
                                case 4:
                                    eliminar_cliente_admin(db);
                                    break;
                                case 5:
                                    cerrar_sesion_admin();
                                    break;
                                default:
                                    printf("Opción no válida.\n");
                            }
                        } while (subopcion != 5);
                    }

                    sesion_iniciada = 0;
                }
                break;
            case 2:
                registrar_usuario(db);
                break;
            case 3:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 3);

    cerrar_bd(db);
    return 0;
}
