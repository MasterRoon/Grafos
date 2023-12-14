#include "estrutura.h"
#include "vizinho.h"
#include <limits.h>

Tgrafo* inicializa(void){
	return NULL;
}

Tgrafo* ins_no(Tgrafo *g, int no){
	Tgrafo *novo = (Tgrafo*) malloc(sizeof(Tgrafo));
	novo->no=no;
	novo->prim_viz=NULL;
	novo->prox=g;
	return novo;
}

void ins_aresta(Tgrafo *g, int no1, int no2, int custo){
	Tgrafo *p = busca_no(g, no1);
	if (!p || !busca_no(g,no2) || no1==no2) {
		printf("Erro\n");
		return;
	}
	Tviz *ant = NULL;
	Tviz *q = p->prim_viz;
	while(q){
		if (q->id == no2)
		{
			q->custo = custo;
			return;
		}
		ant = q;
		q = q->prox_viz;
	}
	Tviz *novo = (Tviz*) malloc (sizeof(Tviz));
	novo->prox_viz = NULL;
	novo->id = no2;
	novo->custo = custo;
	if (!ant)
		p->prim_viz = novo;
	else
		ant->prox_viz = novo;
}

Tgrafo* retira_no(Tgrafo *g, int no){
	Tgrafo *ant = NULL;
	Tgrafo *p = g;

	while ((p) && (p->no!=no) ){
		ant = p;
		p = p->prox;
	}

	if (!p){
		printf("Erro\n");
		return g;
	}
	Tviz *q= p->prim_viz;

	while (q) {
		int no2 = q->id;
		q = q->prox_viz;
		retira_aresta(g,no,no2);
		retira_aresta(g,no2,no);
	}

	if (!ant) 
		g = g->prox;
	else 
		ant->prox = p->prox;

	free(p);

	return g;
}

void retira_aresta(Tgrafo *g, int no1, int no2){
	Tgrafo *p = busca_no(g, no1);
	
	if (!p || !busca_no(g, no2)){
		printf("Erro\n");
		return;
	}
	
	Tviz *ant = NULL;
	Tviz *q = p->prim_viz;

	while( (q) && (q->id != no2) ){
		ant = q;
		q=q->prox_viz;
	}
	
	if (q){
		if (!ant)
			p->prim_viz = q->prox_viz;
		else
			ant->prox_viz = q->prox_viz;
	}

	free(q);
}

Tgrafo* leiaArquivo(char *file){
	FILE *arq = NULL;
	Tgrafo *g = inicializa();

	// Abre um arquivo TEXTO para LEITURA
	arq = fopen(file, "rt");
	if (arq == NULL)  // Se houve erro na abertura
	{
    	printf("Problemas na abertura do arquivo\n");
    	return NULL;
    }

    //printf("%s\n", file);

  	int nvertices, narestas, no1 , no2, custo, i;
  	//le o numero de vertices
  	fscanf(arq, "%i", &nvertices);

  	//le os vertices
  	for (i = 0; i < nvertices; i++){
  		fscanf(arq, "%i", &no1);
  		g = ins_no(g,no1);
  	}

  	//le o arquivo enquanto nao chega ao final
  	for (i = 0; !feof(arq) ; i++){
  		//le a aresta e insere nos dois nos
  		fscanf(arq, "%i %i %i",&no1, &no2, &custo);
  		ins_aresta(g, no1, no2, custo);
  		ins_aresta(g, no2, no1, custo);
  	}

  	fclose(arq);
  	return g;
}

void imprime(Tgrafo *g){
	if (!g){
		printf("\nGrafo Vazio\n");
		return;
	}

	Tgrafo *p = g;
	while (p) {
		printf("Nó: %i\n", p->no);
		Tviz *q = p->prim_viz;
		while(q){
			printf("Vizinho %d \tcusto %d\n", q->id,q->custo);
			q = q->prox_viz;
		}
		p=p->prox;
	}
}

void imprimeGrafo(Tgrafo *g, char *file){
	Tgrafo *p = inicializa();
	Tgrafo *ant = inicializa();
	Tviz *w = NULL;
	FILE *arq = NULL;
	int controle=0;

	// Abre um arquivo TEXTO para LEITURA
	arq = fopen(file, "w");
	if (arq == NULL)  // Se houve erro na abertura
	{
    	printf("Problemas na abertura do arquivo\n");
    	return;
    }

    fprintf(arq, "%i\n", qtd_nos(g) );
    p=g;
    while(p){
    	fprintf(arq, "%i\n", p->no );
    	p=p->prox;
    }

    p=g;
    while(p){
    	w = p->prim_viz;
    	while(w){
    		ant = g;
    		controle=0;
    		while(ant!=p){
    			if (w->id == ant->no)
	    			controle++;
	    		ant=ant->prox;
    		}
    		//imprime se houver nó anteriormente
    		if (controle>0)
    			fprintf(arq, "%i %i %i\n", p->no, w->id, w->custo );
    		w = w->prox_viz;
    	}
    	p=p->prox;
    }

    fclose(arq);
}

int qtd_nos(Tgrafo *g){
    Tgrafo *p = g;
	if (!g)
		return 0;

	int count=0;
	while (p){
		count++;
		p=p->prox;
	}

	return count;
}

// Fun��o para realizar a busca em profundidade (DFS)// 
void DFS(Tgrafo *g, int no, int visited[]) {
    Tgrafo *node = busca_no(g, no);// Busca o n� no grafo
    if (!node) return;// Se o n� n�o existir, retorna
    Tviz *v = node->prim_viz;// Obt�m o primeiro vizinho do n�
    while (v != NULL) {// Itera sobre todos os vizinhos do n�
        if (!visited[v->id]) {// Se o vizinho ainda n�o foi visitado
            visited[v->id] = 1;// Marca o vizinho como visitado
            DFS(g, v->id, visited);// Realiza a busca em profundidade a partir do vizinho
        }
        v = v->prox_viz;// Passa para o pr�ximo vizinho
    }
}
// Fun��o para verificar se o grafo � conexo
int Conectado(Tgrafo *g) {
	int i = 0;
    int no = qtd_nos(g);
    int visited[no];// Inicializa o array de n�s visitados
    for (i ; i < no; i++)
        visited[i] = 0;

    DFS(g, g->no, visited);// Realiza a busca em profundidade a partir do primeiro n�

    Tgrafo *p = g;// Itera sobre todos os n�s do grafo
    //while que verifica se tem alguem que n�o possivel de ser visitado e consequentemente faz n�o ser conexo
    while (p != NULL) {
        if (!visited[p->no])
            return 0;
        p = p->prox;// Passa para o pr�ximo n�
    }
    return 1;
}


// retorna null caso não encontre
Tgrafo* busca_no(Tgrafo *g, int no){
	Tgrafo *p = g;
	while ( (p) && (p->no != no))
		p = p->prox;
	return p;
}

void libera(Tgrafo **g){
	while (*g)
		*g = retira_no(*g,(*g)->no);
}


// Fun��o para encontrar o v�rtice com valor m�nimo, a partir do conjunto de v�rtices ainda n�o inclu�dos na �rvore geradora m�nima
Tgrafo* minKey(Tgrafo* nodes[], int key[], int mstSet[], int num_vertices) {
    int min = INT_MAX;
    int v = 0;
    Tgrafo* min_node = NULL;
    for (v; v < num_vertices; v++) {
        if (mstSet[v] == 0 && key[v] < min) {
            min = key[v];
            min_node = nodes[v];
        }
    }
    return min_node;
}

void printMST(int parent[], int key[], int num_vertices) {
    printf("Edge \tWeight\n");
    int i = 1;
    for (i ; i < num_vertices; i++)
        printf("%d - %d \t%d \n", parent[i], i, key[i]);
}

// Fun��o para construir e imprimir a �rvore geradora m�nima para um grafo representado usando matriz de adjac�ncia
void primMST(Tgrafo* g, int num_vertices) {
    Tgrafo* nodes[num_vertices];  // Array para armazenar os n�s
    int parent[num_vertices];  // Array para armazenar a MST
    int key[num_vertices];  // Valores-chave usados para escolher o peso m�nimo na corte
    int mstSet[num_vertices];  // Representa o conjunto de v�rtices inclu�dos na MST
	int count = 0;
	int i = 0;
	
    // Inicializa todas as chaves como infinito e mstSet[] como falso
    Tgrafo* node = g;
    for (i; i < num_vertices; i++) {
        nodes[i] = node;
        key[i] = INT_MAX;
        mstSet[i] = 0;
        node = node->prox;
    }

    // Sempre inclui o primeiro v�rtice. Faz a chave 0 para que esse v�rtice seja escolhido como o primeiro v�rtice.
    key[0] = 0;
    parent[0] = -1;  // O primeiro n� � sempre a raiz da MST

    // A MST ter� num_vertices v�rtices
    for (count; count < num_vertices - 1; count++) {
        // Escolhe o v�rtice de valor m�nimo, do conjunto de v�rtices ainda n�o inclu�dos na MST
        Tgrafo* u = minKey(nodes, key, mstSet, num_vertices);

        // Adiciona o v�rtice escolhido ao conjunto da MST
        mstSet[u->no] = 1;

        // Atualiza o valor da chave e o �ndice dos v�rtices adjacentes ao v�rtice escolhido. Considera apenas os v�rtices ainda n�o inclu�dos na MST.
        Tviz* v = u->prim_viz;
        while (v != NULL) {
            if (mstSet[v->id] == 0 && v->custo < key[v->id]) {
                parent[v->id] = u->no;
                key[v->id] = v->custo;
            }
            v = v->prox_viz;
        }
    }

    // imprime a MST constru�da
    printMST(parent, key, num_vertices);
}

// Fun��o para imprimir a solu��o
void printSolution(int dist[],int num_vertices) {
	int i = 0;
    // Imprime os v�rtices e suas dist�ncias da origem
    printf("Vertex \t\t Distance from Source\n");
    for (i; i < num_vertices; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

// Fun��o para encontrar o v�rtice com a menor dist�ncia, a partir do conjunto de v�rtices ainda n�o inclu�dos no caminho mais curto
int minDistance(int dist[], int sptSet[], int num_vertices) {
    int min = INT_MAX;
    int min_index = -1;
    int v = 0;
    // Percorre todos os v�rtices e retorna o �ndice do v�rtice com a menor dist�ncia
    for ( v  ; v < num_vertices; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Fun��o para implementar o algoritmo de Dijkstra
void dijkstra(Tgrafo* g, int src, int num_vertices) {
    Tgrafo* nodes[num_vertices];  // Array para armazenar os n�s
    int dist[num_vertices];  // Array para armazenar as dist�ncias m�nimas
    int sptSet[num_vertices];  // Array para armazenar os v�rtices inclu�dos no caminho mais curto
    int i = 0;
    int count = 0;
    Tgrafo* node = g;
    // Inicializa todas as dist�ncias como infinito e sptSet[] como falso
    for (i ; i < num_vertices; i++) {
        nodes[i] = node;
        dist[i] = INT_MAX;
        sptSet[i] = 0;
        node = node->prox;
    }

    // A dist�ncia do v�rtice de origem para si mesmo � sempre 0
    dist[src] = 0;

    // Encontra o caminho mais curto para todos os v�rtices
    for ( count ; count < num_vertices - 1; count++) {
        // Escolhe o v�rtice de dist�ncia m�nima do conjunto de v�rtices ainda n�o processados
        int u = minDistance(dist, sptSet, num_vertices);
        // Marca o v�rtice escolhido como processado
        sptSet[u] = 1;

        // Atualiza o valor da dist�ncia dos v�rtices adjacentes ao v�rtice escolhido
        Tviz* v = nodes[u]->prim_viz;
        while (v != NULL) {
            // Atualiza dist[v->id] apenas se n�o estiver em sptSet, h� uma aresta de u para v
            // e o peso total do caminho da origem para v atrav�s de u � menor que o valor atual de dist[v->id]
            if (!sptSet[v->id] && dist[u] != INT_MAX && dist[u] + v->custo < dist[v->id]) {
                dist[v->id] = dist[u] + v->custo;
            }
            v = v->prox_viz;
        }
    }

    // Imprime a matriz de dist�ncias
    printSolution(dist, num_vertices);
}









