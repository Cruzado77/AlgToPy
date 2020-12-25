#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

/*
        Desenvolvido por: Raphael Rodrigues de Sena
*/

#include "common.h"
#include "engine_var.h"
#include "engine.h"
#include "engine_str.h"
#include "engine_func.h"


#define BUFFER  180

#define TEMP   "file.temp"

#define OPTION_ERROR      -100
#define FILE_ERROR              -101
#define UNKNOW_ERROR  -102
#define MALLOC_ERROR    -103
#define DIR_ERROR               -104
#define SYNTAX_ERROR      -105

#define EXIT                               -200

//globais////////////////////////////
extern int valor_variaveis;
unsigned int identacao = 0;

int delta_identacao = 10;

char *argumento;

int interrupt = 0;

int linhas_outfile = 0;

/////////////////////////////////////
//typedefs//////////////////////////
typedef struct {
               char nome[BUFFER];
               char tipo[BUFFER];
               char identificador[BUFFER];
} variavel;

typedef char **texto;

variavel *variaveis = NULL;

///////////////////////////////////////
//externs//////////////////////////////

extern variavel *inicializa_var(void);

extern variavel *reconhece_variaveis(texto str,variavel *banco);

extern variavel *encontra_var (const char *nome,const variavel *banco);

extern void destroi_var(variavel *banco);

extern void exibe_erro (int erro);

//globas2//////////////////////////////////////////////////////////////

const char no_effect[][BUFFER] = {
               "var",
               "const",
               "inicio",
               "salvar",
               "executar"
};
const int valor_noeffects = 5;



const char comandos_alg[][BUFFER] = {
               "sair",
               "enquanto",
               "para",
               "se",
               "senao",
               "e",
               "ou",
               "nao",
               "algoritmo"
};
const char comandos_py[][BUFFER] = {
                 "exit()",
                 "while",
                 "for",
                 "if",
                 "else:",
                 "&",
                 "|",
                 "not",
                 "##"
};
const int valor_comandos = 9;


const char icontent_alg[][BUFFER] = {
                 "faca",
                 "entao" //outros que sugiram :
};
const int valor_icontent = 2;


const char fcontent_alg[][BUFFER] = {
                 "fimalgoritmo",
                 "fimfuncao",
                 "fimse",
                 "fimpara",
                 "fimenquanto"
};
const int valor_fcontent = 5;


const char types_alg[][BUFFER] = {
               "caracter",
               "inteiro",
               "real"
};
const char types_py[][BUFFER] = {
               "str",
               "int",
               "float"
};
const int valor_types = 3;


const char operador_py[][BUFFER] = {"=", "==", "!=", "#"};
const char operador_alg[][BUFFER] = {"<-", " = ", "<>","//"};
const int valor_operador = 4;


///////////////////////////////////////////////////////////////////////////
//funcoes/////////////////////////////////////////////////////////////////

void exibe_texto (texto str);

texto aloca_texto(int linhas, size_t buffer);

texto realoca_texto(texto p, int linhas, size_t buffer);

int engine (int  (*entrada)  (char *str),int  (*saida)  (texto text));           // flag saida unica ou multipla

int strtroca(char *str, const char *anterior, const char *nova, int ocorrencias);

int tira_espaco(char *str);

int tira_linha(char *str);

static int cria_for(texto argumento) ;

static int tabulador(char *str, int valor);

int strdel(char *str,const char *del);

static int traduz (texto  str,char *espaco_futuro, char *espaco_anterior);

int desaloca_texto(texto text);

int salva_arquivo (texto text,const char *dest, const char *mode);

char *retorna_argumento (const char *str);
//FUNÇÕES/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int engine (int  (*entrada)  (char *str),int  (*saida)  (texto text))
{
               char command_line[BUFFER],py_line[BUFFER] ="#!/usr/bin/python\n# -*- coding: latin-1 -*-\n###AlgToPy v1.1###",espacos[BUFFER] = "",espaco_anterior[BUFFER] = "", *str;
               char delimitador[] = " ,:";
               texto matrix, text;

               strings *banco_strings;
               banco_strings = init_banco_str();

               register int i;

               int quit = 0, linha = 1;

               text = aloca_texto(linha,sizeof(char[BUFFER]));
               if(!text)exibe_erro(MALLOC_ERROR);


               variaveis = inicializa_var();

               do{

                              // Controla linha, junta linha de comando traduzia py_line ao espaço identado

                              snprintf(text[linha-1],BUFFER,"%s%s\n",espaco_anterior,py_line);


                              (*saida) (text);
                              text = realoca_texto(text,linha+1,sizeof(char[BUFFER]));

                              linha++;

                              printf(">>%s",espacos);

                              strncpy(espaco_anterior,espacos,BUFFER);

                              (*entrada) (command_line);


                              if(!tira_linha(command_line)){
                                             strncpy(py_line,command_line,BUFFER);
                                             continue;
                              }

                              retorna_argumento(command_line);

                              reconhece_string(command_line,banco_strings);
                              strtolower(command_line);


                              // TOKEN
                              str = strtok(command_line,delimitador);
                              if(!str)continue;
                              matrix = aloca_texto(1,sizeof(char[BUFFER]));
                              for(i = 1; str; i++)
                              {
                                             strncpy(matrix[i-1],str,BUFFER);
                                             matrix = realoca_texto(matrix,i+1,sizeof(char[BUFFER]));
                                             str = strtok(NULL,delimitador);
                              }
                              i--;

                              if(matrix[i])free(matrix[i]);
                              matrix[i] = NULL;

                              i = traduz(matrix,espacos,espaco_anterior);

                              if(i == EXIT)quit = 1;
                              if(i<0 && i != -100)printf("<<<< !ATENCAO! Expressao indefinida! >>>>\n");

                              if(interrupt)
                              {
                                             interrupt = 0;
                                             strncpy(py_line,"",BUFFER);
                                             continue;
                              }

                              for(register int j = 0; matrix[j]; j++){
                                             restaura_strings(matrix[j],banco_strings);
                                             strncat(matrix[j]," ",BUFFER);
                                             if(j) strncat(matrix[0],matrix[j],BUFFER);
                              }

                              strncpy(py_line,matrix[0],BUFFER);

                              desaloca_texto(matrix);
               }while(!quit);

               destroi_var(variaveis);
               valor_variaveis = 0;

               destroi_banco_str(banco_strings);
               strncpy(id_str,"?0?",BUFFER-1);
               desaloca_texto(text);
               identacao = 0;
               return 0;
}

int tira_espaco(char *str) {
	char saida[BUFFER];

	int i, j = 0;

	if (!strlen(str))
		return 0;

	for (i = 0; i <= strlen(str); i++) {
		if (str[i] == ' ' && (str[i + 1] == ' '))
			continue;
		else {
			saida[j] = str[i];
			j++;
			continue;
		}
	}

	// printf("\nCom espaco :%s\nSem espaco :%s\n",str,saida);

	if (!strlen(saida))
		return 0;
	strcpy(str, saida);

	return j;
}

int tira_linha(char *str) {
	int i;

	if(str[0] == '\n') return 0;

	for (i = 0; i < strlen(str); i++) {
		if (str[i] == '\n')
			str[i] = '\0';
	}

	return i;
}


static int tabulador(char *str, int valor) {
	int i;
	if ((valor) != 0) {
		identacao = identacao + valor;
	} else
		return identacao;
	if (!identacao) {
		strncpy(str,"",BUFFER);
		return 0;
	}

	for (i = 0; i < identacao; i++) {
		str[i] = ' ';
	}
	str[i - 1] = '\0';
	return i;
}

int strdel(char *str,const char *del)
{
                int i,j = 0,iFim,iInicio = 0,encontrado = 0;
                char saida[BUFFER];

                for (i = 0; i < strlen(str); i++) {
                              if (str[i] == del[j]) {
                                             iInicio = i;
                                             for (j = 0; j < strlen(del); j++) {
                                                            if (del[j] == str[i]) {
                                                                           encontrado = 1;
                                                                           i++;
                                                                           continue;
                                                            } else {
                                                                           encontrado = 0;
                                                                           j = 0;
                                                                           break;
                                                            }
                                             }
                                             if (encontrado) {
                                                            iFim = (i - 1);
                                                            break;
                                             } else
                                                            continue;
                              }
               }

               if (!encontrado)
                              return 0;

               j = 0;
               for (i = 0; i < (strlen(str)); i++) {
                              if (i != iInicio) {
                                             saida[j] = str[i];
                                             j++;
                              } else {
                                             i = iFim;
                              }
               }

               saida[j ] = '\0';

               strncpy(str, saida,BUFFER);

               if(!strlen(str)){
                              strncpy(str,"",BUFFER);
                              return 1;
               }

               return iInicio + 1; // evita o 0;
}

int strtroca(char *str, const char *anterior, const char *nova,int ocorrencias) {

	int iInicio = 1, i = 0, j = 0,count = 0; // count evita loop infinito
	char one[BUFFER], two[BUFFER];

               if(!strncmp(nova,anterior,BUFFER) || !strlen(str))return 0;                 // evita loop infinito

               if(!strncmp(str,anterior,BUFFER)){
                              strncpy(str,nova,BUFFER);
                              return strlen(str);
               }
	while (iInicio) {
		iInicio = strdel(str, anterior);
		if (iInicio) {
			for (i = 0; i < iInicio - 1; i++) {
				one[i] = str[i];
			}
			one[i] = '\0';

			for (j = 0; j < (strlen(str) - iInicio + 1); j++) {
				two[j] = str[i];
				i++;
			}
			two[j] = '\0';

			snprintf(str, BUFFER, "%s%s%s", one, nova, two);

                                             count++;
                                             if(count == ocorrencias)break;
		}

	}

	return i;
}

static int cria_for(texto argumento) {

	char x[4][BUFFER];
	int i, j =1;

	for(i = 0; i<4; i++)
               {
                              strncpy(x[i],argumento[j],BUFFER);
                              strncpy(argumento[j],"",BUFFER);
                              if(i<3)strncpy(argumento[j+1],"",BUFFER);
                              else{
                                             strncpy(argumento[2],"faca",BUFFER);
                                             strncpy(argumento[8],"",BUFFER);
                              }

                              j += 2;
               }

	snprintf(argumento[1], BUFFER, "%s in range(%s,%s,%s)", x[0], x[1], x[2], x[3]);

	return 1;
}


static int traduz (texto  str,char *espaco_futuro, char *espaco_anterior)
{
               register  int i,j;
               static int var_referencia = 0, inicio = 0;                              //1 = var, 2 = const 0 = nada
               int invoke_func = -1,contador = -1;
               char temp[BUFFER];
               variavel *p_var;

               for(j = 0; str[j]; j++){
                              if(!strncmp(str[j],"=",BUFFER))
                              {
                                             strncpy(str[j]," = ",BUFFER);
                              }
                              if(strstr(str[j],"=") && !strstr(str[j]," = ")){
                                             strtroca(str[j],"="," = ",1);
                              }
               }

               if(var_referencia == 1){
                              reconhece_variaveis(str,variaveis);
                              contador++;
               }

               if(inicio){
                              if(funcao(str)){
                                             invoke_func = traduz_funcao(str);
                                             if(invoke_func == 2){

                                                            p_var = encontra_var(argumento,variaveis);
                                                            if(!p_var || strlen(argumento) <= 2){
                                                                           exibe_erro(SYNTAX_ERROR);
                                                                           interrupt = 1;
                                                            }
                                                            else {
                                                                           if(strncmp(p_var->tipo,"str",BUFFER) != 0){
                                                                                          snprintf(str[0],BUFFER,"%s = %s(input())",p_var->nome,p_var->tipo);
                                                                           }
                                                                           else snprintf(str[0],BUFFER,"%s = input()",p_var->nome);
                                                                           for(j = 1; str[j]; j++)strncpy(str[j],"",BUFFER);
                                                            }


                                                            return 1;
                                             }
                                             else{
                                                            if(invoke_func == 1 || invoke_func == 0 ){
                                                                           snprintf(str[0],BUFFER,"%s%s",funcoes_py[invoke_func],argumento);
                                                                           for(j = 1; str[j]; j++)strncpy(str[j],"",BUFFER);
                                                                           return 1;
                                                            }
                                             }
                              }
               }

               for(i = 0; str[i]; i++)
               {
                              for(j = 0;j<valor_fcontent;j++){
                                             if(!strncmp(str[i],fcontent_alg[j],BUFFER)){
                                                            strtroca(str[i],fcontent_alg[j],"",-1);
                                                            tabulador(espaco_futuro,-delta_identacao);
                                                            if(j == 0){
                                                                           strtroca(str[i],fcontent_alg[j],comandos_py[0],-1);
                                                                           return EXIT;
                                                            }
                                                            contador++;
                                             }
                              }
                              for(j = 0; j<valor_comandos;j++){
                                             if(!strncmp(str[i],comandos_alg[j],BUFFER)){
                                                            strtroca(str[i],comandos_alg[j],comandos_py[j],-1);
                                                            if(j == 2){
                                                                           if(!encontra_var(str[1],variaveis)){
                                                                                          exibe_erro(SYNTAX_ERROR);
                                                                                          interrupt = 1;
                                                                                          contador++;
                                                                                          return contador;
                                                                           }
                                                                           else cria_for(str);                                                                 //laço for
                                                            }
                                                            if(j == 4) {                                                                                        //else
                                                                           tabulador(espaco_anterior,-delta_identacao);
                                                                           tabulador(espaco_futuro,delta_identacao);
                                                            }
                                                            contador++;
                                             }
                              }
                              for(j = 0; j<valor_icontent;j++)
                              {
                                             if(!strncmp(str[i],icontent_alg[j],BUFFER)){
                                                            strtroca(str[i],icontent_alg[j],":",-1);
                                                            tabulador(espaco_futuro,delta_identacao);
                                                            contador++;
                                             }
                              }

                              for(j = 0; j<valor_noeffects; j++)
                              {
                                             if(!i){
                                                            if(!strncmp(str[i],no_effect[j],BUFFER)){
                                                                           strncpy(str[i],"",BUFFER);
                                                                           if(j == 0)var_referencia =1;
                                                                           else{
                                                                                          if(j == 1)var_referencia =2;
                                                                                          else{
                                                                                                         var_referencia = 0;
                                                                                                         if(j == 2)inicio = 1;
                                                                                                         else{
                                                                                                                        if(j == 3){
                                                                                                                                       strncpy(file_name,"",FILE_BUFFER-1);
                                                                                                                                       save_as();
                                                                                                                        }
                                                                                                                        if(j == 4){
                                                                                                                                       snprintf(temp,BUFFER,"py %s",TEMP);
                                                                                                                                       system(temp);
                                                                                                                        }
                                                                                                         }
                                                                                          }
                                                                           }
                                                                           contador++;
                                                            }
                                             }
                              }

                              for(j = 0; j<valor_operador; j++)
                              {
                                             if(strstr(str[i],operador_alg[j])){
                                                            if(var_referencia != 2)
                                                                           strtroca(str[i],operador_alg[j],operador_py[j],-1);
                                                            contador++;

                                             }
                              }

               }

               return contador;
}


void exibe_texto (texto str)
{
               FILE *out = fopen(TEMP,"w");
               printf("\n");
               for(register int i = 0; str[i];i++)
               {
                              printf(">>%s",str[i]);
                              fprintf(out,"%s",str[i]);
               }
               fclose(out);
}


texto aloca_texto(int linhas, size_t buffer)
{
               char ** saida;
               register int i;
               if(!linhas || !buffer)return NULL;

               saida = (char **) malloc(sizeof(char *) * (linhas+1));
               if(!saida)exibe_erro(MALLOC_ERROR);

               for(i = 0; i<linhas; i++)
                              saida[i] = (char *) malloc(buffer);
               saida[i] = NULL;
               return saida;
}

int desaloca_texto(texto text)
{
               int i;

               for(i=0; text[i]; i++)
                              free(text[i]);
               free(text);

               return 0;
}

texto realoca_texto(texto p, int linhas, size_t buffer)
{
               int i;
               char **saida;

               saida = aloca_texto(linhas,buffer);

               for(i = 0;p[i];i++){
                              strncpy(saida[i],p[i],buffer/sizeof(char));
               }
               desaloca_texto(p);

               p = saida;

               return saida;

}


char *retorna_argumento (const char *str)
{
               static char saida[BUFFER];
               register int i;

               strncpy(saida,str,BUFFER-1);

               for(i = 0; i<valor_funcoes; i++){
                              if(strstr(saida,funcoes_alg[i]))strdel(saida,funcoes_alg[i]);
               }

               argumento = saida;

               return saida;
}

#endif // ENGINE_H_INCLUDED
