#include "hash.h"
#include <stdio.h>
#include <string.h>

typedef enum { VACIO, OCUPADO, BORRADO } estado_t;
#define FACTOR_CARGA 0.5
#define FALLA_REHASH false
#define CASILLERO_BORRADO 0

typedef struct par {
	char *clave;
	void *valor;
	estado_t estado;
} par_t;

struct hash {
	par_t *tabla_hash;
	size_t capacidad;
	size_t cantidad;
	size_t (*funcion_hash)(const char *);
};

unsigned long funcion_hashing(const char *str)
{
	unsigned long hash = 5381;
	unsigned char c;

	while (*((unsigned char *)str) != '\0') {
		c = *((unsigned char *)str);
		hash = ((hash << 5) + hash) + c;
		str++;
	}

	return hash;
}

void liberar_claves(par_t *tabla, size_t capacidad)
{
	for (size_t i = 0; i < capacidad; i++) {
		if (tabla[i].estado == OCUPADO) {
			free(tabla[i].clave);
		}
	}
}

void inicializar_tabla(par_t *tabla, size_t capacidad)
{
	for (size_t i = 0; i < capacidad; i++) {
		tabla[i].estado = VACIO;
		tabla[i].clave = NULL;
		tabla[i].valor = NULL;
	}
}

size_t buscar_posicion(hash_t *hash, const char *clave)
{
	size_t pos = hash->funcion_hash(clave) % hash->capacidad;
	bool seguir_iterando = true;
	while (hash->tabla_hash[pos].estado != VACIO && seguir_iterando) {
		if (hash->tabla_hash[pos].estado == OCUPADO &&
		    strcmp(hash->tabla_hash[pos].clave, clave) == 0) {
			seguir_iterando = false;
		} else {
			pos = (pos + 1) % hash->capacidad;
		}
	}
	return pos;
}

hash_t *hash_crear(size_t capacidad_inicial)
{
	hash_t *tabla_hash =
		hash_crear_con_funcion(capacidad_inicial, funcion_hashing);
	if (tabla_hash == NULL) {
		return NULL;
	}
	return tabla_hash;
}

hash_t *hash_crear_con_funcion(size_t capacidad_inicial,
			       size_t (*f)(const char *))
{
	if (f == NULL) {
		return NULL;
	}
	hash_t *tabla = calloc(1, sizeof(hash_t));
	if (tabla == NULL) {
		return NULL;
	}
	size_t capacidad_aux = capacidad_inicial;
	if (capacidad_aux < 3) {
		capacidad_aux = 3;
	}
	tabla->capacidad = capacidad_aux;
	tabla->cantidad = 0;
	tabla->tabla_hash = malloc(tabla->capacidad * sizeof(par_t));
	if (tabla->tabla_hash == NULL) {
		free(tabla);
		return NULL;
	}
	inicializar_tabla(tabla->tabla_hash, tabla->capacidad);
	tabla->funcion_hash = f;

	return tabla;
}

bool rehash(hash_t *hash)
{
	par_t *claves_aux = hash->tabla_hash;
	size_t capacidad_antigua = hash->capacidad;
	size_t nueva_capacidad = (hash->capacidad) * 2;
	par_t *nueva_tabla = malloc(nueva_capacidad * sizeof(par_t));
	if (nueva_tabla == NULL) {
		return false;
	}
	hash->cantidad = 0;
	hash->tabla_hash = nueva_tabla;
	hash->capacidad = nueva_capacidad;
	inicializar_tabla(hash->tabla_hash, hash->capacidad);
	for (size_t i = 0; i < capacidad_antigua; i++) {
		if (claves_aux[i].estado == OCUPADO) {
			bool reinsertado =
				hash_insertar(hash, claves_aux[i].clave,
					      claves_aux[i].valor, NULL);
			if (!reinsertado) {
				liberar_claves(nueva_tabla, nueva_capacidad);
				free(nueva_tabla);
				hash->tabla_hash = claves_aux;
				return FALLA_REHASH;
			}
			free(claves_aux[i].clave);
		}
	}
	free(claves_aux);
	return true;
}

char *copiar_clave(const char *clave)
{
	char *copia = malloc((strlen(clave) + 1) * sizeof(char));
	if (copia == NULL) {
		return NULL;
	}
	strcpy(copia, clave);
	return copia;
}

bool completar_campo(hash_t *hash, size_t indice, const char *clave,
		     void *valor)
{
	char *copia = copiar_clave(clave);
	if (copia == NULL) {
		return false;
	}
	hash->tabla_hash[indice].clave = copia;
	hash->tabla_hash[indice].valor = valor;
	hash->tabla_hash[indice].estado = OCUPADO;
	return true;
}

size_t buscar_posicion_a_insertar(hash_t *hash, const char *clave)
{
	size_t pos = hash->funcion_hash(clave) % hash->capacidad;
	size_t pos_borrado = CASILLERO_BORRADO;
	bool seguir_iterando = true;
	while (hash->tabla_hash[pos].estado != VACIO && seguir_iterando) {
		if (hash->tabla_hash[pos].estado == OCUPADO &&
		    strcmp(hash->tabla_hash[pos].clave, clave) == 0) {
			seguir_iterando = false;
		} else if (hash->tabla_hash[pos].estado == BORRADO &&
			   pos_borrado == CASILLERO_BORRADO) {
			pos_borrado = pos;
		} else {
			pos = (pos + 1) % hash->capacidad;
		}
	}
	if (pos_borrado != CASILLERO_BORRADO) {
		return pos_borrado;
	}
	return pos;
}

void insertar_aux(hash_t *hash, const char *clave, void *valor, bool *insertado,
		  void **anterior)
{
	size_t indice = buscar_posicion_a_insertar(hash, clave);
	if (hash->tabla_hash[indice].estado == OCUPADO) {
		if (anterior) {
			*anterior = hash->tabla_hash[indice].valor;
		}
		hash->tabla_hash[indice].valor = valor;
		*insertado = true;
	} else {
		if (anterior) {
			*anterior = NULL;
		}
		if (completar_campo(hash, indice, clave, valor)) {
			hash->cantidad++;
			*insertado = true;
		}
	}
}

bool hash_insertar(hash_t *hash, const char *clave, void *valor,
		   void **anterior)
{
	if (hash == NULL || clave == NULL) {
		return false;
	}
	if (((float)hash->cantidad / (float)hash->capacidad) >= FACTOR_CARGA) {
		if (!rehash(hash)) {
			return false;
		}
	}
	bool insertado = false;
	insertar_aux(hash, clave, valor, &insertado, anterior);
	return insertado;
}

void sacar_clave(hash_t *hash, size_t posicion_actual)
{
	free(hash->tabla_hash[posicion_actual].clave);
	hash->tabla_hash[posicion_actual].valor = NULL;
	hash->tabla_hash[posicion_actual].estado = BORRADO;
	hash->cantidad--;
}

void *hash_sacar(hash_t *hash, const char *clave)
{
	if (hash == NULL || clave == NULL) {
		return NULL;
	}
	if (!hash_existe(hash, clave)) {
		return NULL;
	}
	size_t indice = buscar_posicion(hash, clave);
	void *valor_encontrado = hash->tabla_hash[indice].valor;
	sacar_clave(hash, indice);
	return valor_encontrado;
}

void *hash_buscar(hash_t *hash, const char *clave)
{
	if (hash == NULL || clave == NULL) {
		return NULL;
	}
	size_t pos = buscar_posicion(hash, clave);
	if (hash->tabla_hash[pos].estado == OCUPADO) {
		return hash->tabla_hash[pos].valor;
	}
	return NULL;
}

bool hash_existe(hash_t *hash, const char *clave)
{
	if (hash == NULL || clave == NULL) {
		return false;
	}
	size_t pos = buscar_posicion(hash, clave);
	return hash->tabla_hash[pos].estado == OCUPADO;
}

size_t hash_tamanio(hash_t *hash)
{
	if (hash == NULL) {
		return 0;
	}
	return hash->cantidad;
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (hash == NULL) {
		return;
	}
	for (size_t i = 0; i < hash->capacidad; i++) {
		if (hash->tabla_hash[i].estado == OCUPADO && destructor) {
			destructor(hash->tabla_hash[i].valor);
		}
	}
	liberar_claves(hash->tabla_hash, hash->capacidad);
	free(hash->tabla_hash);
	free(hash);
}

void hash_destruir(hash_t *hash)
{
	if (hash == NULL) {
		return;
	}
	liberar_claves(hash->tabla_hash, hash->capacidad);
	free(hash->tabla_hash);
	free(hash);
}

size_t hash_iterar_claves(hash_t *hash, bool (*f)(const char *, void *),
			  void *ctx)
{
	if (hash == NULL || f == NULL) {
		return 0;
	}
	size_t invocaciones = 0;
	bool seguir_iterando = true;
	for (size_t i = 0; i < hash->capacidad && seguir_iterando; i++) {
		if (hash->tabla_hash[i].estado == OCUPADO) {
			if (!f(hash->tabla_hash[i].clave, ctx)) {
				invocaciones++;
				seguir_iterando = false;
			} else {
				invocaciones++;
			}
		}
	}
	return invocaciones;
}