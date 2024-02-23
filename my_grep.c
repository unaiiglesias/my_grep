#include <stdio.h>
#include <errno.h>


int main (int argc, char *argv[]) {

    int error_code = 0;

    if (argc == 1)
    {
        // No se han dado argumentos, echo de lo que ponga el usuario
        char a;

        while (a != EOF)
        {
            a = getchar();
            if (a != EOF) printf("%c", a);
        }
        return 0;
    } 

    // Hay varios argumentos (archivos a leer)

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