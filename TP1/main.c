#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseArg.h"


#define MAX_DATA  40    // Max size of the auxiliary matrix 
#define STR_SIZE  40    

#define MAX_MATRIX 30

enum{OK = 0, ERROR_1, ERROR_2, ERROR_CALLBACK}; // enums that belongs to the callback
enum{STATE_0, STATE_1};


typedef enum{NO_ERROR, DATA_OVERFLOW, WRONG_ARG} errorStates_t;    // calback variables 
typedef enum{VOID, PARAMETRO, OPCION} tipoDeArgumento_t; 


typedef struct 
{
    tipoDeArgumento_t tipoDeArgumento;
    errorStates_t errorState;
    union{
        char parametro[STR_SIZE];
        char clave[STR_SIZE];
    };
    int valor;
}userDataInput_t;

int parseCallback(char * key, char * value, void * userData);

int main()
{
    userDataInput_t myData[MAX_DATA];


    printf("\nTEST BORRABLE BEGIN \n\n");

    char * argv3[MAX_MATRIX][MAX_MATRIX] = {{"./myProgram","-div", "3", "param1", "param2", "-", NULL},
                          {"./myProgram","-div", "3", "param1", "param2", "param3", "-sum", "2", "-div", "-4", "-res", "2", "param3", NULL  },
                          {"./myProgram","-div", "3", NULL},
                          {"./myProgram","-div", "3", NULL},
                          {"./myProgram", "-hola", NULL} ,
                          {"./myProgram", "-", "7", NULL},
                          {"./myProgram", NULL},
                          {"./myProgram", "param1", "mal_escrito", NULL},
                          {NULL}  };

    int fils = 0;
    while(argv3[fils][0] != NULL)
        fils++;

    if(fils == 30)
    {
        printf("ERROR DEL TESTER\n");
    }
    else
    {
        for (int j = 0; j < fils; j++)
        {
            printf("\n\n#####TEST NRO %d ha comenenzado#####\n\n", j + 1);
    
            int argc3 = 0;
    
            while(argv3[j][argc3] != NULL && argc3 < 30)
                argc3++;
    
            if(argc3 < 30 && argv3[j][argc3] == NULL)
            {
            
                for (int i = 0; i < MAX_DATA; i++)
                {
                    myData[i].tipoDeArgumento = VOID;
                    myData[i].errorState = NO_ERROR;
                }    
                int state = parseCmdLine(argc3, argv3[j], &parseCallback, myData );
                if( state != -1)
                {
                    printf("La cantidad de opciones y parametros ingresada fue de = %d\n", state);
    
                    for (int i = 0; i < MAX_DATA && myData[i].parametro != VOID; i++)
                    {
                        if(myData[i].errorState == DATA_OVERFLOW || myData[i].errorState == WRONG_ARG )
                        {
                            printf("ERROR DE INPUT %d\n", i);                  // Esto habria que sacarlo porque si entra aca es porque no hay errores
                        }
                        else if(myData[i].tipoDeArgumento == OPCION)
                        {
                            printf("CLAVE = %s\n", myData[i].clave);
                            printf("VALOR = %d\n\n", myData[i].valor);
                        }
                        else if(myData[i].tipoDeArgumento == PARAMETRO)
                        {
                            printf("PARAMETRO = %s\n\n", myData[i].parametro);
                        }
                    }
                }
                else
                {
                //Chequeo si el error es de la callback
                if(myData[0].errorState == WRONG_ARG )
                    printf("WRONG ARGUMENT\n");
                else if(myData[0].errorState == DATA_OVERFLOW)
                    printf("DEMASIADOS ARGUMENTOS INGRESADOS\n");
                }
            }
            else
            {
                printf("ERROR SISTEMA DE TESTEO\n");
            }
        }
    }    
}


int parseCallback(char * key, char * value, void * userData)
{
    userDataInput_t * pUserData = userData;
    int error = 0;

    int i = 0;
    while(pUserData[i].tipoDeArgumento != VOID && i < MAX_DATA)
        i++;

    if(i == MAX_DATA)
    {
        pUserData[0].errorState = DATA_OVERFLOW;
        error = 1;
    }
    else
    {
        if(key == NULL)
        {
            pUserData[i].tipoDeArgumento = PARAMETRO;
            if(strcmp(value, "param1") == 0)
            {
                strcpy(pUserData[i].parametro, "param1");
            }
            else if(strcmp(value, "param2") == 0)
            {
                strcpy(pUserData[i].parametro, "param2");
            }
            else if(strcmp(value, "param3") == 0)
            {
                strcpy(pUserData[i].parametro, "param3");
            }
            else if(strcmp(value, "param4") == 0)
            {
                strcpy(pUserData[i].parametro, "param4");
            }
            else
            {
                pUserData[0].errorState = WRONG_ARG; 
                error = 1;
            }
        }
        else if( strcmp(key, "-div") == 0)
        {
            pUserData[i].tipoDeArgumento = OPCION;
            strcpy(pUserData[i].clave, "div");
            pUserData[i].valor = atof(value);
        }
        else if( strcmp(key, "-sum") == 0)
        {
            pUserData[i].tipoDeArgumento = OPCION;
            strcpy(pUserData[i].clave, "sum");
            pUserData[i].valor = atof(value);
        }
        else if( strcmp(key, "-res") == 0)
        {
            pUserData[i].tipoDeArgumento = OPCION;
            strcpy(pUserData[i].clave, "res");
            pUserData[i].valor = atof(value);       // VER SI METIO MAL EL NUMERO por el atof !!!!!!
        }
        else
        {
            pUserData[0].errorState = WRONG_ARG;
            error = 1;
        }
    }
    return !error;   
}