#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define TAM_BUFFER_LINEA 4096 // Segun google, el mismo que el de grep

char* leer_fichero (char* linea, FILE* f)
{
    // lee una linea entera de f y la guarda en linea
    char* ret = fgets(linea, TAM_BUFFER_LINEA, f);

    // ret contiene un puntero a linea si todo ha ido bien y NULL si no
    return ret;
}

int main (int argc, char *argv[]) {

    int error_code = 0;
    char linea[TAM_BUFFER_LINEA]; // buffer de cada linea


    if (argc == 1)
    {
        // No se han dado argumentos, no hay patron
        fprintf(stderr, "ERROR: Wrong amount of arguments\n");
        exit(1);
    } 
    else if (argc == 2)
    {
        // Se ha dado patron pero no ficheros, leemos y buscamos en stdin
        char* ret = leer_fichero(linea, stdin);

        while (ret != NULL) // not EOF
        {
            if (strstr(linea, argv[1]))
            {
                // Si el patron ( argv[1] ) esta en la linea leida, la imprimimos
                printf("%s", linea);
            }
            ret = leer_fichero(linea, stdin);
        }
        return 0;
    }

    // Hay patron y varios ficheros

    for (int i = 1; i < argc; i++)
    {
        FILE *f = fopen(argv[i], "r");

        if (f == NULL)
        {
            // ERROR: file not found
            // Ignorar este archivo

            //printf("Codigo de error: %d\n", errno);
            perror("ERROR");
            error_code = 1;
            continue; // Pasa al siguiente archivo
        }

	    //Declaramos el buffer y lo inicializamos.
        unsigned char buffer[100];
        size_t char_leidos; // esto indicara cuantos caracteres ha leido el buffer


        do {
            char_leidos = fread(buffer, sizeof(char), sizeof(buffer), f);
			fwrite(buffer, sizeof(char), char_leidos, stdout);
        } 
        while (char_leidos != 0); //Mientras se lea algun elemento (Char) imprimimos por pantalla
        /*
        if (ferror(f)){
            fprintf(stderr,"Ha ocurrido un ERROR en la lectura del fichero.\n");
        }
        */
        fclose(f);
    }

    return error_code;
}