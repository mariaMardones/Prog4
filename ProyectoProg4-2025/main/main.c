#include <stdio.h>
#include "../domain/menus.h"
#include "../domain/usuario.h"

int main() {
	Usuario usuario;
    int rolUsuario;
    int opcionPrincipal;
    int opcionMenu;

    do {
        opcionPrincipal = mostrarMenuPrincipal();

        switch (opcionPrincipal) {
        //INICIAR SESION
            case '1':
            	printf("Bienvenido, %s \n", usuario.nombre);
                //ADMMIN
                if (rolUsuario == ADMINISTRADOR) {
                    do {
                        opcionMenu = mostrarMenuAdministrador();
                        switch (opcionMenu) {
                            case '1':
                                printf("Función de visualizar clientes aquí.\n");
                                break;
                            case '2':
                                printf("Función de buscar cliente por DNI aquí.\n");
                                break;
                            case '3':
                                printf("Función de cancelar reserva aquí.\n");
                                break;
                            case '4':
                                printf("Función de eliminar cliente aquí.\n");
                                break;
                            case '0':
                                printf("Volviendo al menú principal...\n");
                                break;
                            default:
                                printf("Opción no válida, intente de nuevo.\n");
                        }
                    } while (opcionMenu != '0');
                    //CLIENTE
                } else if (rolUsuario == CLIENTE) {
                    do {
                        opcionMenu = mostrarMenuCliente();
                        switch (opcionMenu) {
                            case '1':
                                printf("Función de realizar reserva aquí.\n");
                                break;
                            case '2':
                                printf("Función de cancelar reserva aquí.\n");
                                break;
                            case '3':
                                printf("Función de ver habitaciones aquí.\n");
                                break;
                            case '4':
                                printf("Función de filtrar habitaciones aquí.\n");
                                break;
                            case '5':
                                printf("Función de consultar reservas aquí.\n");
                                break;
                            case '0':
                                printf("Volviendo al menú principal...\n");
                                break;
                            default:
                                printf("Opción no válida, intente de nuevo.\n");
                        }
                    } while (opcionMenu != '0');
                }

                break;
            //REGISTRO
            case '2':
            	registrarUsuario();
                break;

            case '0':
                printf("Saliendo...\n");
                break;

            default:
                printf("Opción no válida, intente de nuevo.\n");
        }
    } while (opcionPrincipal != '0');

    return 0;
}
