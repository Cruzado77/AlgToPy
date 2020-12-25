#include "common.h"

/*
        Desenvolvido por: Raphael Rodrigues de Sena
*/

int entrada_console (char * str);

int entrada_arquivo (char * str);

int saida_console(texto text);

int saida_arquivo(texto text);
// MAIN
int main()
{
               char temp[BUFFER],sys_temp[FILE_BUFFER+BUFFER];
    printf ("Localidade corrente: %s\n", setlocale(LC_ALL,NULL) );
    printf("A localidade corrente agora é %s \n",setlocale(LC_ALL,""));
    int option,quit = 0;

    do{
               //system("cls");
               option = exibe_menu();
               switch(option)
               {
               case 1:
                              engine(entrada_console,saida_console);
                              break;
               case 2:
                              seleciona_arquivo();
                              if(!arq)break;
                              engine(entrada_arquivo,saida_console);
                              fclose(arq);
                              save_as();
                              break;
               case 3:
                              printf("\nDesenvolvido por Raphael Sena (Cruzado77)\nAracaju-SE, dezembro de 2020\n");
                              system("pause");
                              break;

               case 4:
                              quit = 1;
                              break;
               }

    }while(!quit);
    return 0;
}

int entrada_console (char * str)
{
               fgets(str,BUFFER-1,stdin);
               fflush(stdin);

               return 1;
}


int entrada_arquivo (char * str)
{
               fgets(str,BUFFER,arq);

               return 1;
}

int saida_console(texto text)
{
               exibe_texto(text);

               return 1;
}

int saida_arquivo(texto text)
{
               static int linha = 0;
               FILE *out;

               out = fopen("test.py","a");
               if(!out)exibe_erro(FILE_ERROR);

               fprintf(out,"%s",text[linha]);

               linha++;

               fclose(out);
               return 1;
}

