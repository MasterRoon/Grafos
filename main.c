#include "estrutura.c"
#include <string.h>


int main(int argc, char *argv[])
{
	Tgrafo *g=NULL;
	int i, j, op;
    char no[20], no2[20], custo[20];
    char default_file[] = "teste2.txt";
    argv[1] = default_file;


	do {

		printf("\n\n");
		printf("##############################################\n");
		printf("#\t 1 - Inserir nÃ³                      #\n");
		printf("#\t 2 - Inserir aresta                  #\n");
		printf("#\t 3 - Remover nÃ³                      #\n");
		printf("#\t 4 - Remover aresta                  #\n");
		printf("#\t 5 - Verificar se Ã© conexo           #\n");
		printf("#\t 6 - Imprimir grafo                  #\n");
		printf("#\t 7 - Libera grafo                    #\n");
		printf("#\t 8 - Árvore geradora mínima (Prim)   #\n");
		printf("#\t 9 - Caminho mínimo (Dijkstra)      #\n");
		printf("##############################################\n");
		printf("#                                            #\n");
		printf("#\t 0 - Para sair                       #\n");
		printf("#                                            #\n");
		printf("##############################################\n");
		printf("\n\n");


		scanf("%i", &op);


		switch (op){
			case 0:
				break;
			case 1:
				printf("Digite o valor do nÃ³ que deseja inserir\n");
				scanf(" %s", no);
				if (atoi(no)>0 && !busca_no(g, atoi(no)))
					g = ins_no(g, atoi(no));
				else
					printf("Erro\n");
				break;
			case 2:
				printf("Digite o 1Âº nÃ³ da aresta\n");
				scanf(" %s", no);
				printf("Digite o 2Âº nÃ³ da aresta\n");
				scanf(" %s", no2);
				printf("Digite o custo da aresta\n");
				scanf(" %s", custo);
				if (atoi(custo)>0)
				{
					ins_aresta(g, atoi(no), atoi(no2), atoi(custo));
					ins_aresta(g, atoi(no2), atoi(no), atoi(custo));
				} else
					printf("Custo deve ser maior que 0\n");
				break;
			case 3:
				printf("Digite o valor do nÃ³ que deseja remover\n");
				scanf(" %s", no);
				g = retira_no(g, atoi(no));
				break;
			case 4:
				printf("Digite o 1Âº nÃ³ da aresta\n");
				scanf(" %s", no);
				printf("Digite o 2Âº nÃ³ da aresta\n");
				scanf(" %s", no2);
				retira_aresta(g, atoi(no), atoi(no2));
				retira_aresta(g, atoi(no2), atoi(no));
				break;
			case 5:
			    //pega o conteudo do arquivo .txt e manda pra funÃ§Ã£o ler arquivo verificar se Ã© conexo//
                g = leiaArquivo(argv[1]);
                //Ele chama a função conectado que tem a responsabilidade de descobrir se é conexo ou não//
				if (Conectado(g))
            		printf("O grafo é conexo.\n");
        		else
            		printf("O grafo não é conexo.\n");
        		break;
			case 6:
				imprime(g);
				break;
			case 7:
				libera(&g);
				break;
			case 8:
				g = leiaArquivo(argv[1]);
                primMST(g, qtd_nos(g));// Adicione 'qtd_nos(g)' como segundo argumento
                break;
            case 9:
                printf("Digite o vértice de origem:\n");
        		int src;
        		scanf("%d", &src);
        		g = leiaArquivo(argv[1]);
        		dijkstra(g, src, qtd_nos(g));  // Adicione 'qtd_nos(g)' como terceiro argumento
        		break;
			default:
				printf("OpÃ§Ã£o InvÃ¡lida\n\n");
				break;
		}
	} while (op!=0);

	libera(&g);

	return 0;
}

