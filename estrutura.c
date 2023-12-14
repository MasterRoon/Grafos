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
		printf("NÃ³: %i\n", p->no);
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
    		//imprime se houver nÃ³ anteriormente
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

// Função para realizar a busca em profundidade (DFS)// 
void DFS(Tgrafo *g, int no, int visited[]) {
    Tgrafo *node = busca_no(g, no);// Busca o nó no grafo
    if (!node) return;// Se o nó não existir, retorna
    Tviz *v = node->prim_viz;// Obtém o primeiro vizinho do nó
    while (v != NULL) {// Itera sobre todos os vizinhos do nó
        if (!visited[v->id]) {// Se o vizinho ainda não foi visitado
            visited[v->id] = 1;// Marca o vizinho como visitado
            DFS(g, v->id, visited);// Realiza a busca em profundidade a partir do vizinho
        }
        v = v->prox_viz;// Passa para o próximo vizinho
    }
}
// Função para verificar se o grafo é conexo
int Conectado(Tgrafo *g) {
	int i = 0;
    int no = qtd_nos(g);
    int visited[no];// Inicializa o array de nós visitados
    for (i ; i < no; i++)
        visited[i] = 0;

    DFS(g, g->no, visited);// Realiza a busca em profundidade a partir do primeiro nó

    Tgrafo *p = g;// Itera sobre todos os nós do grafo
    //while que verifica se tem alguem que não possivel de ser visitado e consequentemente faz não ser conexo
    while (p != NULL) {
        if (!visited[p->no])
            return 0;
        p = p->prox;// Passa para o próximo nó
    }
    return 1;
}


// retorna null caso nÃ£o encontre
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


// Função para encontrar o vértice com valor mínimo, a partir do conjunto de vértices ainda não incluídos na árvore geradora mínima
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

// Função para construir e imprimir a árvore geradora mínima para um grafo representado usando matriz de adjacência
void primMST(Tgrafo* g, int num_vertices) {
    Tgrafo* nodes[num_vertices];  // Array para armazenar os nós
    int parent[num_vertices];  // Array para armazenar a MST
    int key[num_vertices];  // Valores-chave usados para escolher o peso mínimo na corte
    int mstSet[num_vertices];  // Representa o conjunto de vértices incluídos na MST
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

    // Sempre inclui o primeiro vértice. Faz a chave 0 para que esse vértice seja escolhido como o primeiro vértice.
    key[0] = 0;
    parent[0] = -1;  // O primeiro nó é sempre a raiz da MST

    // A MST terá num_vertices vértices
    for (count; count < num_vertices - 1; count++) {
        // Escolhe o vértice de valor mínimo, do conjunto de vértices ainda não incluídos na MST
        Tgrafo* u = minKey(nodes, key, mstSet, num_vertices);

        // Adiciona o vértice escolhido ao conjunto da MST
        mstSet[u->no] = 1;

        // Atualiza o valor da chave e o índice dos vértices adjacentes ao vértice escolhido. Considera apenas os vértices ainda não incluídos na MST.
        Tviz* v = u->prim_viz;
        while (v != NULL) {
            if (mstSet[v->id] == 0 && v->custo < key[v->id]) {
                parent[v->id] = u->no;
                key[v->id] = v->custo;
            }
            v = v->prox_viz;
        }
    }

    // imprime a MST construída
    printMST(parent, key, num_vertices);
}

// Função para imprimir a solução
void printSolution(int dist[],int num_vertices) {
	int i = 0;
    // Imprime os vértices e suas distâncias da origem
    printf("Vertex \t\t Distance from Source\n");
    for (i; i < num_vertices; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

// Função para encontrar o vértice com a menor distância, a partir do conjunto de vértices ainda não incluídos no caminho mais curto
int minDistance(int dist[], int sptSet[], int num_vertices) {
    int min = INT_MAX;
    int min_index = -1;
    int v = 0;
    // Percorre todos os vértices e retorna o índice do vértice com a menor distância
    for ( v  ; v < num_vertices; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Função para implementar o algoritmo de Dijkstra
void dijkstra(Tgrafo* g, int src, int num_vertices) {
    Tgrafo* nodes[num_vertices];  // Array para armazenar os nós
    int dist[num_vertices];  // Array para armazenar as distâncias mínimas
    int sptSet[num_vertices];  // Array para armazenar os vértices incluídos no caminho mais curto
    int i = 0;
    int count = 0;
    Tgrafo* node = g;
    // Inicializa todas as distâncias como infinito e sptSet[] como falso
    for (i ; i < num_vertices; i++) {
        nodes[i] = node;
        dist[i] = INT_MAX;
        sptSet[i] = 0;
        node = node->prox;
    }

    // A distância do vértice de origem para si mesmo é sempre 0
    dist[src] = 0;

    // Encontra o caminho mais curto para todos os vértices
    for ( count ; count < num_vertices - 1; count++) {
        // Escolhe o vértice de distância mínima do conjunto de vértices ainda não processados
        int u = minDistance(dist, sptSet, num_vertices);
        // Marca o vértice escolhido como processado
        sptSet[u] = 1;

        // Atualiza o valor da distância dos vértices adjacentes ao vértice escolhido
        Tviz* v = nodes[u]->prim_viz;
        while (v != NULL) {
            // Atualiza dist[v->id] apenas se não estiver em sptSet, há uma aresta de u para v
            // e o peso total do caminho da origem para v através de u é menor que o valor atual de dist[v->id]
            if (!sptSet[v->id] && dist[u] != INT_MAX && dist[u] + v->custo < dist[v->id]) {
                dist[v->id] = dist[u] + v->custo;
            }
            v = v->prox_viz;
        }
    }

    // Imprime a matriz de distâncias
    printSolution(dist, num_vertices);
}









