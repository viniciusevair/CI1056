#include <stdlib.h>
#include <string.h>
#include "ordenacao.h"

/* -------- Dados Aluno -------- */
void getNome(char nome[]){
    strncpy(nome, "Vinicius Evair da Silva", MAX_CHAR_NOME);
    nome[MAX_CHAR_NOME-1] = '\0';
}

unsigned int getGRR(){
    return 20221251;
}

// Funcao interna utilizada por diversas outras funcoes.
void trocar(int vetor[], int a, int b) {
    int aux;

    aux = vetor[a];
    vetor[a] = vetor[b];
    vetor[b] = aux;
}

/* -------- Busca Sequencial -------- */
int buscaSequencial(int vetor[], int tam, int valor, int* numComp){
    if(tam <= 0)
        return -1;

    (*numComp)++;
    if(valor == vetor[tam - 1])
        return tam - 1;      

    return buscaSequencial(vetor, tam - 1, valor, numComp);
}

/* -------- Busca Binaria -------- */
int buscaBinariaInterna(int vetor[], int ini, int fim, int valor, int* numComp)  {
    int meio;

    if(fim < ini)
        return -1;
    
    meio = (ini + fim) >> 1;

    (*numComp)++;
    if(valor == vetor[meio])
        return meio;
    if(valor < vetor[meio])
        return buscaBinariaInterna(vetor, ini, meio - 1, valor, numComp);
    return buscaBinariaInterna(vetor, meio + 1, fim, valor, numComp);
}

int buscaBinaria(int vetor[], int tam, int valor, int* numComp){
    return buscaBinariaInterna(vetor, 0, tam - 1, valor, numComp);
}

/* -------- Insertion Sort -------- */

/* 
 * Funcao interna do Insertion Sort.
 *
 * Recebe como entrada um vetor indexado por [ini..fim], 
 * com todos seus valores ordenados, com excessao do ultimo.
 * Retornar o indice em que um novo valor deve ser colocado
 * para que o vetor continue ordenado.
 *
 * Implementacao separada da versao acima da busca binaria usada nos testes,
 * para evitar a adicao de flags de controle na mesma.
 */
int buscar(int valor, int vetor[], int ini, int fim, int* numComp) {
    int meio;

    if(fim < ini)
        return ini - 1;

    meio = (ini + fim) >> 1;

    (*numComp)++;
    if(valor < vetor[meio]) {
        return buscar(valor, vetor, ini, meio - 1, numComp);
    }

    return buscar(valor, vetor, meio + 1, fim, numComp);
}

/*
 * Recebe um vetor indexado por [0..tam-1] com todos seus valores ordenados com
 * excessao do ultimo.
 * Ordena o vetor de maneira que fique completamente ordenado.
 * Retorna a quantidade de comparacoes feitas durante o processo.
 */ 
int insere(int vetor[], int tam) {
    int posicao, i, ini, fim, numComp;
    ini = 0;
    fim = tam - 1;
    numComp = 0;

    posicao = buscar(vetor[fim], vetor, ini, fim, &numComp);
    for(i = fim; i > posicao + 1; i--)
        trocar(vetor, i, i - 1);

    return numComp;
}

/*
 * Recebe um vetor indexado por [0..tam-1].
 * Ordena o vetor recebido.
 * Retorna a quantidade de comparacoes feitas no processo.
 */
int insertionSort(int vetor[], int tam){	
    int numComp;
    numComp = 0;

    if(tam <= 1)
        return 0;

    numComp += insertionSort(vetor, tam - 1);
    numComp += insere(vetor, tam);
    
    return numComp;
}

/* -------- Selection Sort -------- */
int maximo(int vetor[], int tam, int *numComp) {
    int i, max;
    max = 0;

    for(i = 1; i < tam; i++) {
        (*numComp)++;
        if(vetor[i] > vetor[max])
            max = i;
    }

    return max;
}

/*
 * Recebe um vetor indexado por [0..tam-1].
 * Ordena o vetor recebido.
 * Retorna a quantidade de comparacoes feitas no processo.
 */
int selectionSort(int vetor[], int tam){
    int numComp;
    if(tam <= 0)
        return 0;

    numComp = 0;
    trocar(vetor, maximo(vetor, tam, &numComp), tam - 1);
    numComp += selectionSort(vetor, tam - 1);

    return numComp;
}

/* -------- Merge Sort -------- */

/* 
 * Justapoe os dados do vetor auxiliar no vetor principal.
 * A memoria alocada para o vetor auxiliar e liberada logo em seguida, pois o
 * mesmo nao sera mais utilizado.
 */
int* copiar(int vetor[], int ini, int fim, int auxVetor[]) {
    int i = 0;

    while(i <= fim - ini) {
        vetor[ini+i] = auxVetor[i];
        i++;
    }

    free(auxVetor);

    return vetor;
}

/*
 * Recebe um vetor indexado por [ini..fim], onde vetor[ini..meio]
 * e vetor[meio+1..fim] sao dois vetores ordenados.
 * Retorna o vetor indexado por [ini..fim] modificado para estar ordenado.
 */
int* merge(int vetor[], int ini, int meio, int fim, int *numComp) {
    int k, i, j, p;
    int *auxVetor;

    /* Aloca dinamicamente a memoria para o vetor auxiliar, evitando um stack
     * overflow precoce.*/
    if(!(auxVetor = calloc(fim-ini+1, sizeof(int))))
        return NULL;

    k = 0;
    i = ini;
    j = meio+1;

    while(k <= fim - ini) {
        (*numComp)++;
        if(j > fim || (i <= meio && vetor[i] <= vetor[j])) {
            p = i;
            i++;
        }
        else {
            p = j;
            j++;
        }

        auxVetor[k] = vetor[p];
        k++;
    }

    return copiar(vetor, ini, fim, auxVetor);
}

/*
 * Recebe um vetor indexado por [ini..fim].
 * Ordena o vetor recebido.
 * Retorna a quantidade de comparacoes feitas no processo.
 *
 * Cuidado com o alto custo de memoria.
 */
int mergeSortInterna(int vetor[], int ini, int fim) {
    int meio, numComp;

    if(ini >= fim)
        return 0;

    meio = (ini + fim) / 2;
    numComp = 0;

    numComp += mergeSortInterna(vetor, ini, meio);
    numComp += mergeSortInterna(vetor, meio + 1, fim);
    merge(vetor, ini, meio, fim, &numComp);

    return numComp;
}

int mergeSort(int vetor[], int tam){
    return mergeSortInterna(vetor, 0, tam-1);
}

/* -------- Quick Sort -------- */

/*
 * Recebe um vetor indexado por [ini..fim].
 * Particiona o vetor em duas partes a partir do valor de vetor[fim].
 * Todos os valores menores ficam para a esquerda, e todos os valores maiores
 * ficam para a direita.
 * Retorna o indice em que o divisor das partes ficou.
 */
int particionar(int vetor[], int ini, int fim, int *numComp) {
    int indiceDivisor, i;

    indiceDivisor = ini;
    for(i = ini; i < fim; i++) {
        (*numComp)++;
        if(vetor[i] < vetor[fim]) {
            trocar(vetor, indiceDivisor, i);
            indiceDivisor++;
        }
    }

    trocar(vetor, indiceDivisor, fim);
    return indiceDivisor;        
}

/*
 * Recebe um vetor indexado por [0..tam-1].
 * Ordena o vetor recebido.
 * Retorna a quantidade de comparacoes feitas no processo.
 */
int quickSortInterna(int vetor[], int ini, int fim) {
    int indiceDivisor, numComp;
    if(ini >= fim)
        return 0;

    numComp = 0;
    indiceDivisor = particionar(vetor, ini, fim, &numComp);
    numComp += quickSortInterna(vetor, ini, indiceDivisor - 1);
    numComp += quickSortInterna(vetor, indiceDivisor + 1, fim);

    return numComp;
}

int quickSort(int vetor[], int tam){
    return quickSortInterna(vetor, 0, tam-1);
}

/* -------- Heap Sort -------- */
int indicePai(int i) {
    if(i == 2)
        return 0;
    return i/2;
}
int indiceFilhoEsquerdo(int i) {
    return (i << 1) + 1;
}
int indiceFilhoDireito(int i) {
    return (i << 1) + 2;
}

/*
 * Rearranja os dados de uma arvore binaria para transforma-la numa max heap.
 */
int maxHeapify(int vetor[], int indice, int fim) {
    int esq, dir, maiorIndice, numComp;

    esq = indiceFilhoEsquerdo(indice);
    dir = indiceFilhoDireito(indice);
    maiorIndice = indice;
    numComp = 0;

    if(esq <= fim) {
        numComp++;
        if(vetor[esq] > vetor[indice])
            maiorIndice = esq;
    }
    if(dir <= fim) {
        numComp++;
        if(vetor[dir] > vetor[maiorIndice])
            maiorIndice = dir;
    }

    if(maiorIndice != indice) {
        trocar(vetor, indice, maiorIndice);
        numComp += maxHeapify(vetor, maiorIndice, fim);
    }

    return numComp;
}

/*
 * Recebe um vetor indexado por [0..fim-1].
 * Transforma o vetor em uma arvore binaria do tipo max heap.
 * Retorna a quantidade de comparacoes feitas no processo.
 */
int construirMaxHeapify(int vetor[], int fim) {
    int i, numComp;

    numComp = 0;
    for(i = (fim >> 1); i >= 0; i--) {
        numComp += maxHeapify(vetor, i, fim);
    }

    return numComp;
}

/*
 * Recebe um vetor indexado por [0..tam-1].
 * Ordena o vetor recebido.
 * Retorna a quantidade de comparacoes feitas no processo.
 */
int heapSort(int *v, int tam) {
    int i, fim, numComp;
    fim = tam - 1;

    numComp = 0;
    numComp += construirMaxHeapify(v, fim);
    for(i = fim; i > 0; i--) {
        trocar(v, 0, i);
        fim--;
        numComp += maxHeapify(v, 0, fim);
    }

    return numComp;
}
