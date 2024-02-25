#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define TAM_BUFFER_LINEA 4096 // Segun google, el mismo que el de grep

size_t leer_fichero (char* linea, size_t* size, FILE* f)
{
    // lee una linea entera de f y la guarda en linea
    size_t charLeidos = getline(&linea, size, f);

    /*
        Adicionalmente, si la linea a leer es de mas de TAM_BUFFER_LINEA caracteres (que no deberia, peor bueno)
        reserva más memoria y nos asegura que siempre va a poder almacenar la linea (sea del tamaño que sea).
    */

    // charLeidos contienen el nº de caracteres leidos, si hay error o EOF devuelve -1
    return charLeidos;
}

int main (int argc, char *argv[]) {

    int error_code = 1; // partimos de que no ha encontrado ninguna linea coincidente (ver especificacion)
    char* linea = malloc(TAM_BUFFER_LINEA); // buffer de cada linea
    size_t buffSize = sizeof(linea);
    size_t leidos;
    //printf("argc = %d\n", argc); // DEBUG

    if (argc == 1)
    {
        // No se han dado argumentos, no hay patron
        fprintf(stderr, "ERROR: Wrong amount of arguments\n");
        exit(2);
    } 
    else if (argc == 2)
    {
        // Se ha dado patron pero no ficheros, leemos y buscamos en stdin
        leidos = leer_fichero(linea, &buffSize, stdin);

        while (leidos != -1) // not EOF or error
        {
            if (strstr(linea, argv[1]))
            {
                // Si el patron ( argv[1] ) esta en la linea leida, la imprimimos
                printf("%s", linea);
                error_code = 0;
            }
            leidos = leer_fichero(linea, &buffSize, stdin);
        }
        return error_code;
    }

    // Hay patron y varios ficheros
    int i;
    for (i = 2; i < argc; i++)
    {
        FILE *f = fopen(argv[i], "r");

        if (f == NULL)
        {
            // ERROR: file not found
            // Ignorar este archivo

            //printf("Codigo de error: %d\n", errno);
            perror("ERROR");
            error_code = 2;
            continue; // Pasa al siguiente archivo
        }

        // bucle principal de lectura
        leidos = leer_fichero(linea, &buffSize, f);
        while (leidos != -1) // not EOF or error
        {
            if (strstr(linea, argv[1]))
            {
                // Si el patron ( argv[1] ) esta en la linea leida, la imprimimos
                printf("%s", linea);
                error_code = 0;
            }
            leidos = leer_fichero(linea, &buffSize, f);
        }

        /*
        if (ferror(f)){
            fprintf(stderr,"Ha ocurrido un ERROR en la lectura del fichero.\n");
        }
        */
        fclose(f);
        
    }
    free(linea); // como linea la reservamos dinámicamente entre malloc y getline luego hay que liberarla
    return error_code;
}