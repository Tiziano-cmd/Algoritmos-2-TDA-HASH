#include "pa2m.h"
#include "src/hash.h"

bool funcion(const char *clave, void *contexto)
{
	return clave != NULL && contexto == NULL;
}

bool funcion_1(const char *clave, void *contexto)
{
	return clave == NULL && contexto == NULL;
}

void crear_hash_es_correcto()
{
	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash != NULL, "El hash se creo correctamente");
	size_t tamanio = hash_tamanio(hash);
	pa2m_afirmar(tamanio == 0,
		     "El tamanio del hash es 0 (valor obtenido: %ld)", tamanio);
	hash_destruir(hash);
}

void insertar_en_hash_es_valido()
{
	hash_t *hash = hash_crear(10);
	for (int i = 0; i < 1; i++) {
		pa2m_afirmar(hash_insertar(hash, "Pedro", NULL, NULL) == true,
			     "La clave pedro fue insertada correctamente");
		pa2m_afirmar(hash_insertar(hash, "Lucas", NULL, NULL) == true,
			     "La clave Lucas fue insertada correctamente");
		pa2m_afirmar(hash_insertar(hash, "P", NULL, NULL) == true,
			     "La clave P fue insertada correctamente");
		pa2m_afirmar(hash_insertar(hash, "P", NULL, NULL) == true,
			     "La clave P fue actualizada correctamente");
	}

	size_t tamanio = hash_tamanio(hash);
	pa2m_afirmar(tamanio == 3,
		     "La cantidad de claves es 3 (valor obtenido: %ld)",
		     tamanio);
	for (int i = 0; i < 1; i++) {
		pa2m_afirmar(hash_insertar(hash, "Perro", NULL, NULL) == true,
			     "La clave Perro fue insertada correctamente");
		pa2m_afirmar(hash_insertar(hash, "Tiziano", NULL, NULL) == true,
			     "La clave Tiziano fue insertada correctamente");
	}

	size_t tamanio_1 = hash_tamanio(hash);
	pa2m_afirmar(tamanio_1 == 5,
		     "La cantidad de claves es 5 (valor obtenido: %ld)",
		     tamanio_1);
	int edad = 21;
	void *ptr = NULL;
	void **anterior = &ptr;
	for (int i = 0; i < 1; i++) {
		pa2m_afirmar(hash_insertar(hash, "Argentina", NULL, NULL) ==
				     true,
			     "La clave Argentina fue insertada correctamente");
		pa2m_afirmar(hash_insertar(hash, "Brenda", NULL, NULL) == true,
			     "La clave Brenda fue insertada correctamente");
		pa2m_afirmar(hash_insertar(hash, "Luciana", NULL, NULL) == true,
			     "La clave Luciana fue insertada correctamente");
		pa2m_afirmar(hash_insertar(hash, "Luciano", NULL, NULL) == true,
			     "La clave Luciano fue insertada correctamente");
		pa2m_afirmar(hash_insertar(hash, "Tiziano", &edad, anterior) ==
				     true,
			     "La clave Tiziano fue actualizada correctamente");
		pa2m_afirmar(*anterior == NULL, "anterior es igual a NULL");
	}

	size_t tamanio_2 = hash_tamanio(hash);
	pa2m_afirmar(tamanio_2 == 9,
		     "La cantidad de claves es 9 (valor obtenido: %ld)",
		     tamanio_2);
	void *valor = hash_buscar(hash, "Tiziano");
	if (valor == NULL) {
		printf("El valor asociado es NULL\n");
	}
	pa2m_afirmar(*((int *)valor) == 21,
		     "La clave Tiziano tiene el valor 21 (valor obtenido: %d)",
		     *((int *)valor));
	void *valor_1 = hash_buscar(hash, "Jorge");
	pa2m_afirmar(valor_1 == NULL,
		     "La clave Jorge no se encuentra en el hash");
	hash_insertar(hash, "Brenda", NULL, NULL);
	size_t tamanio_3 = hash_tamanio(hash);
	pa2m_afirmar(tamanio_3 == 9,
		     "El tamanio del hash sigue siendo 9 (valor obtenido: %ld)",
		     tamanio_3);
	bool existe = hash_existe(hash, "Luciana");
	pa2m_afirmar(existe == true, "La clave Luciana existe en el hash");
	hash_destruir(hash);
}

void eliminar_en_hash_es_valido()
{
	hash_t *hash = hash_crear(10);
	int edad = 21;
	int edad_1 = 29;
	int edad_2 = 18;
	int edad_3 = 15;
	for (int i = 0; i < 1; i++) {
		hash_insertar(hash, "Pedro", NULL, NULL);
		hash_insertar(hash, "Lucas", &edad_1, NULL);
		hash_insertar(hash, "P", NULL, NULL);
		hash_insertar(hash, "P", NULL, NULL);
		hash_insertar(hash, "Perro", NULL, NULL);
		hash_insertar(hash, "Tiziano", NULL, NULL);
		hash_insertar(hash, "Argentina", NULL, NULL);
		hash_insertar(hash, "Brenda", &edad_2, NULL);
		hash_insertar(hash, "Luciana", &edad_3, NULL);
		hash_insertar(hash, "Luciano", NULL, NULL);
		hash_insertar(hash, "Tiziano", &edad, NULL);
	}
	size_t tamanio = hash_tamanio(hash);
	pa2m_afirmar(tamanio == 9,
		     "La cantidad de claves es 9 (valor obtenido: %ld)",
		     tamanio);
	for (int i = 0; i < 1; i++) {
		void *sacado = hash_sacar(hash, "Lucas");
		void *sacado_1 = hash_sacar(hash, "Lucas");
		void *sacado_2 = hash_sacar(hash, "Brenda");
		void *sacado_3 = hash_sacar(hash, "Tiziano");
		void *sacado_4 = hash_sacar(hash, "Luciana");
		pa2m_afirmar(
			*((int *)sacado) == 29,
			"La clave Lucas fue eliminada (valor obtenido: %d)",
			*((int *)sacado));
		pa2m_afirmar(
			sacado_1 == NULL,
			"Eliminar la clave Lucas cuando ya se habia eliminado devuelve NULL");
		pa2m_afirmar(
			*((int *)sacado_2) == 18,
			"Eliminar la clave Brenda devuelve el valor 18 (valor obtenido: %d)",
			*((int *)sacado_2));
		pa2m_afirmar(
			*((int *)sacado_3) == 21,
			"Eliminar la clave Tiziano devuelve el valor 21 (valor obtenido: %d)",
			*((int *)sacado_3));
		pa2m_afirmar(
			*((int *)sacado_4) == 15,
			"Eliminar la clave Luciana devuelve el valor 15 (valor obtenido: %d)",
			*((int *)sacado_4));
	}
	pa2m_afirmar(
		hash_tamanio(hash) == 5,
		"La cantidad de claves se redujo a 5 (valor obtenido: %ld)",
		hash_tamanio(hash));
	for (int i = 0; i < 1; i++) {
		pa2m_afirmar(hash_existe(hash, "Lucas") == false,
			     "La clave Lucas ya no existe en el hash");
		pa2m_afirmar(hash_existe(hash, "Luciana") == false,
			     "La clave Luciana ya no existe en el hash");
		void *buscado_1 = hash_buscar(hash, "Lucas");
		void *buscado_2 = hash_buscar(hash, "Luciana");
		pa2m_afirmar(
			buscado_1 == NULL,
			"Buscar la clave Lucas al eliminarla en el hash devuelve NULL");
		pa2m_afirmar(
			buscado_2 == NULL,
			"Buscar la clave Luciana al eliminarla devuelve NULL");
	}
	hash_destruir(hash);
}

void iterar_claves_es_correcto()
{
	hash_t *hash = hash_crear(10);
	hash_insertar(hash, "Argentina", NULL, NULL);
	hash_insertar(hash, "Brenda", NULL, NULL);
	hash_insertar(hash, "Luciana", NULL, NULL);
	hash_insertar(hash, "Luciano", NULL, NULL);
	size_t iterados = hash_iterar_claves(hash, funcion, NULL);
	pa2m_afirmar(
		iterados == 4,
		"La cantidad de claves iteradas es 4 (valor obtenido: %ld)",
		iterados);
	size_t iterados_1 = hash_iterar_claves(hash, funcion_1, NULL);
	pa2m_afirmar(
		iterados_1 == 1,
		"La cantidad de claves iteradas es 1 (valor obtenido: %ld)",
		iterados_1);
	hash_destruir(hash);
}

void hash_invalido()
{
	hash_t *hash = hash_crear(2);
	pa2m_afirmar(hash != NULL, "El hash se creo correctamente");
	pa2m_afirmar(hash_insertar(hash, NULL, NULL, NULL) == false,
		     "Tratar de insertar una clave NULL es invalido");
	void *sacado = hash_sacar(hash, NULL);
	pa2m_afirmar(sacado == NULL, "Eliminar una clave NULL devuelve NULL");
	void *buscado = hash_buscar(hash, NULL);
	pa2m_afirmar(buscado == NULL,
		     "Tratar de buscar una clave NULL devuelve NULL");
	pa2m_afirmar(hash_existe(hash, NULL) == false,
		     "Tratar de saber si existe una clave NULL devuelve NULL");
	pa2m_afirmar(hash_tamanio(hash) == 0,
		     "El tamanio del hash es 0 (valor obtenido: %ld)",
		     hash_tamanio(hash));
	pa2m_afirmar(
		hash_iterar_claves(hash, NULL, NULL) == 0,
		"Tratar de iterar las claves con una funcion NULL devuelve 0 (valor obtenido: %ld)",
		hash_iterar_claves(hash, NULL, NULL));
	hash_destruir(hash);
}

void operar_con_capacidad_es_valido()
{
	hash_t *hash = hash_crear(0);
	pa2m_afirmar(hash != NULL, "El hash se creo correctamente");
	hash_insertar(hash, "Conejo", NULL, NULL);
	hash_insertar(hash, "Gato", NULL, NULL);
	size_t tamanio = hash_tamanio(hash);
	pa2m_afirmar(tamanio == 2,
		     "El tamanio del hash es 2 (valor obtenido: %ld)", tamanio);
	hash_insertar(hash, "Tigre", NULL, NULL);
	size_t tamanio_2 = hash_tamanio(hash);
	pa2m_afirmar(tamanio_2 == 3,
		     "El tamanio del hash ahora es 3 (valor obtenido: %ld)",
		     tamanio_2);
	pa2m_afirmar(hash_existe(hash, "Conejo") == true,
		     "La clave Conejo existe en el hash");
	void *buscado = hash_buscar(hash, "Conejo");
	pa2m_afirmar(buscado == NULL,
		     "El valor asociado a la clave Conejo es NULL");
	for (int i = 0; i < 1; i++) {
		void *sacado = hash_sacar(hash, "Conejo");
		void *sacado_1 = hash_sacar(hash, "Gato");
		void *sacado_2 = hash_sacar(hash, "Tigre");
		pa2m_afirmar(
			sacado == NULL && sacado_1 == NULL && sacado_2 == NULL,
			"Las tres claves fueron eliminadas correctamente y tenian el valor NULL asociado");
	}
	pa2m_afirmar(
		hash_tamanio(hash) == 0,
		"La cantidad de claves se redujo a 0 (valor obtenido: %ld)",
		hash_tamanio(hash));

	hash_destruir(hash);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de creacion de hash");
	crear_hash_es_correcto();
	pa2m_nuevo_grupo("Pruebas de insercion");
	insertar_en_hash_es_valido();
	pa2m_nuevo_grupo("Pruebas de eliminacion");
	eliminar_en_hash_es_valido();
	pa2m_nuevo_grupo("Pruebas de iteracion");
	iterar_claves_es_correcto();
	pa2m_nuevo_grupo("Pruebas con punteros nulos");
	hash_invalido();
	pa2m_nuevo_grupo("Pruebas con capacidad menor a 3");
	operar_con_capacidad_es_valido();

	return pa2m_mostrar_reporte();
}
