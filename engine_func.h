#ifndef ENGINE_FUNC_H_INCLUDED
#define ENGINE_FUNC_H_INCLUDED

#include "common.h"
#include "engine_var.h"
#include "engine.h"
#include "engine_str.h"
#include <inttypes.h>


#define BUFFER  180

#define OPTION_ERROR      -100
#define FILE_ERROR              -101
#define UNKNOW_ERROR  -102
#define MALLOC_ERROR    -103
#define DIR_ERROR               -104

typedef char **texto;

extern char *argumento;

const char funcoes_alg[][BUFFER] = {         //espaco para o strstr
                 "escreval",
                 "escreva",
                 "leia"
};

const char funcoes_alg_sp[][BUFFER] = {         //espaco para o strstr
                 "escreval ",
                 "escreva ",
                 "leia "
};

const char funcoes_py[][BUFFER] = {
                 "print",
                 "print",
                 "input"
};
const int valor_funcoes = 3;



int funcao(texto linha);

int traduz_funcao(texto linha);

int funcao(texto linha)
{
               int i,j, parentesis = 0;
               for(i = 0; linha[i]; i++)
               {
                              //printf("%s\n",linha[i]);
                              for(j = 0; j<strlen(linha[i]); j++)
                                             if(linha[i][j] == '(' )
                                             {
                                                            strtroca(linha[i],"("," @",-1);
                                                            parentesis++;
                                             }
                                             else{
                                                            if(linha[i][j] == ')' )
                                                            {
                                                                           strtroca(linha[i],")","¨ ",-1);
                                                                           parentesis++;
                                                            }
                                             }
               }

               for(i = 0; linha[i]; i++)
               {
                              for(j = 0; j<strlen(linha[i]); j++)
                                             if(linha[i][j] == '@' )linha[i][j] = '(';
                                             else if(linha[i][j] == '¨' )linha[i][j] = ')';
               }

               if(parentesis > 1) return 1;
               else return 0;
}

int traduz_funcao(texto linha)
{
               int i,j,func = -1,num = -1;

               //printf("Argumento (%s)\n",argumento);
               for(i = 0; linha[i]; i++)
               {
                              //printf("(%s)\n",linha[i]);
                              for(j = 0; j <valor_funcoes;j++)
                              {
                                             if(!strncmp(linha[i],funcoes_alg[j],BUFFER)){
                                                                           strncpy(linha[i],funcoes_py[j],BUFFER);
                                                                           func = j;
                                                                           num = i;
                                             }
                                             else{
                                                            if(strstr(linha[i],funcoes_alg_sp[j]) && linha[i][0] == funcoes_alg_sp[j][0])
                                                            {
                                                                           strtroca(linha[i],funcoes_alg[j],funcoes_py[j],1);
                                                                           func = j;
                                                                           num = i;
                                                            }
                                             }
                              }
               }
               if(argumento){
                              if(func == 2){
                                             strdel(linha[num],argumento);
                                             return 2;
                              }
                              else return func;

               }
               return -1;
}

#endif // ENGINE_FUNC_H_INCLUDED
