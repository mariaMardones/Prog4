#ifndef DOMAIN_MENUS_H_
#define DOMAIN_MENUS_H_

#include "../bd/sqlite3.h"

void menu_administrador();
void menu_cliente();
void mostrar_menu();

void manejar_menu_principal(sqlite3 *db);

#endif /* DOMAIN_MENUS_H_ */
