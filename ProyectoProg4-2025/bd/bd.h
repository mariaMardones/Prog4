#ifndef DB_DB_H_
#define DB_DB_H_

#include "sqlite3.h"

int inicializar_bd(sqlite3 **db);

void cerrar_bd(sqlite3 *db);

#endif /* DB_DB_H_ */
