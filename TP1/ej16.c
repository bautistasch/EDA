#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA  40
#define STR_SIZE  40

enum{OK = 0, ERROR_1, ERROR_2, ERROR_CALLBACK};
enum{STATE_0, STATE_1};


typedef enum{NO_ERROR, DATA_OVERFLOW, WRONG_ARG} errorStates_t;
typedef enum{VOID, PARAMETRO, OPCION} tipoDeArgumento_t; 

typedef int (*pCallback) (char *, char*, void *);

int parseCallback(char * key, char * value, void *userData);
int parseCmdLine(int argc, char * argv[], pCallback p, void * userData);


typedef struct 
{
    tipoDeArgumento_t tipoDeArgumento;
    errorStates_t errorState;
    struct{
        char parametro[STR_SIZE];
        char clave[STR_SIZE];
    };
        int valor;
}userDataInput;


int main()
{
    userDataInput myData[MAX_DATA];

    char * argv[] = {"./myProgram","-div", "3", "param1", "param2", "param3", "-sum", "2", "-div", "-4", "-", "2"};
    int argc = sizeof(argv)/sizeof(char *);

    for (int i = 0; i < MAX_DATA; i++)
    {
        myData[i].tipoDeArgumento = VOID;
        myData[i].tipoDeArgumento = NO_ERROR;
    }
    int state = parseCmdLine(argc, argv, &parseCallback, myData );
    if( state != OK)
    {
        switch (state)
        {
        case ERROR_1:
            printf("ERROR 1\n");
            break;
        case ERROR_2:
            printf("ERROR 2\n");
            break;
        case ERROR_CALLBACK:
            printf("ERROR CALLBACK\n");
            break;
        }
    }
    else
    {
        for (int i = 0; i < MAX_DATA && myData[i].parametro != VOID; i++)
        {
            if(myData[i].errorState == DATA_OVERFLOW || myData[i].errorState == WRONG_ARG )
            {
                printf("ERROR DE INPUT\n");
            }
            else if(myData[i].tipoDeArgumento == OPCION)
            {
                printf("CLAVE = %s\n", myData[i].clave);
                printf("VALOR = %d\n\n\n", myData[i].valor);
            }
            else if(myData[i].tipoDeArgumento == PARAMETRO)
            {
                printf("PARAMETRO = %s\n\n\n", myData[i].parametro);
            }
        }
    }

}

int parseCmdLine(int argc, char * argv[], pCallback p, void * userData)
{
    int errorState = OK;
    int state = STATE_0;
    for (int i = 1; i < argc && errorState == OK ; i++)
    {
        switch (state)
        {
        case STATE_0:
            if(argv[i][0] == '-')
            {
               if(argv[i][1] == '\0')
               {
                   errorState = ERROR_1; // NOT SURE
               }
               else
               {
                   state = STATE_1;
               }
            }
            else
            {
                if(p( NULL, argv[i], userData) == 0)
                    errorState = ERROR_CALLBACK;
            }
            break;

        case STATE_1:     // tomo el valor de la opcion
            state = STATE_0;
            if(p(argv[i - 1], argv[i], userData) == 0 )
                errorState = ERROR_CALLBACK;
            break;
        }
    }
    if(state == STATE_1)
    {
        errorState = ERROR_2;
    }
    return errorState;
}

int parseCallback(char * key, char * value, void * userData)
{
    userDataInput * pUserData = userData;
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