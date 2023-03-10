#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ordenacao.h"

typedef struct vetor {
    int *dado;
    int tamanho;
} vetor_t;

/*
 * Recebe um intervalo de valores inteiros
 * Devolve um valor aleatorio dentro do intervalo fechado.
 */
int aleat(int min, int max) {
    return rand() % (max - min + 1) + min;
}

/*
 * Recebe uma struct de vetor.
 * Devolve um aterramento para ponteiros.
 *
 * Libera toda a memoria alocada dinamicamente para a struct de vetor.
 */
vetor_t* liberaStructVetor(vetor_t *vetor) {
    free(vetor->dado);
    free(vetor);

    return NULL;
}

/*
 * Recebe um inteiro "tam" referente ao tamanho de um vetor e uma flag "tipo"
 * que controla se a ordenacao sera crescente ou decrescente.
 * Devolve uma struct para um vetor ordenado indexado por [0..tam-1].
 *
 * A memoria da struct e do vetor sao alocadas dinamicamente.
 */
vetor_t* inicializaVetorOrdenado(int tam, int tipo) {
    int i;
    vetor_t *vetor;

    if(!(vetor = malloc(sizeof(vetor_t)))) {
        printf("Falha fatal. Impossível alocar memoria.");
        return NULL;
    }
    vetor->tamanho = tam;
    if(!(vetor->dado = calloc(vetor->tamanho, sizeof(int)))) {
        printf("Falha fatal. Impossível alocar memoria.");
        free(vetor);
        return NULL;
    }

    if (tipo == 1) {
        for(i = 0; i < tam; i++)
            vetor->dado[i] = i;

        return vetor;
    }

    for(i = 0; i < tam; i++)
        vetor->dado[i] = tam - (i + 1);

    return vetor;
}

/*
 * Recebe um inteiro "tam" referente ao tamanho de um vetor.
 * Devolve uma struct para um vetor aleatorio indexado por [0..tam-1].
 *
 * A memoria da struct e do vetor sao alocadas dinamicamente.
 */
vetor_t* inicializaVetorAleatorio(int tam) {
    int i;
    vetor_t *vetor;

    if(!(vetor = malloc(sizeof(vetor_t)))) {
        printf("Falha fatal. Impossível alocar memoria.");
        return NULL;
    }
    vetor->tamanho = tam;
    if(!(vetor->dado = calloc(vetor->tamanho, sizeof(int)))) {
        printf("Falha fatal. Impossível alocar memoria.");
        free(vetor);
        return NULL;
    }

    for(i = 0; i < tam; i++) {
        vetor->dado[i] = aleat(-50000, 50000); //intervalo arbitrario
    }

    return vetor;
}

/*
 * Recebe como entrada um vetor indexado por [0..tam-1].
 * Devolve uma copia do vetor tambem indexado por [0..tam-1].
 *
 * A memoria da copia e alocada dinamicamente, e os dados sao copiados atraves
 * da memcpy.
 */
int* copiaVetor(int vetor[], int tam) {
    int *copia;

    if(!(copia = calloc(tam, sizeof(int)))) {
        printf("Falha fatal. Impossível alocar memoria.");
        return NULL;
    }
    memcpy(copia, vetor, tam * sizeof(int));

    return copia;
}

/*
 * Recebe como entrada um vetor aleatorio indexado por [0..tam-1] e uma flag
 * "escolhedor". 
 * Devolve na saida padrao o tempo de execucao, a quantidade de comparacoes e o
 * tamanho do vetor ordenado.
 *
 * A funcao realiza um unico teste de algum algoritmo de ordenacao controlado
 * pela flag "escolhedor". Os testes sao feitos em uma copia do vetor e portanto
 * o vetor original permanece desordenado.
 */
void testaAlgoritmoSort(int vetor[], int tam, int escolhedor) {
    clock_t start, end; //variáveis do tipo clock_t
    double total;
    int numComp;
    int *cpVetor;

    cpVetor = copiaVetor(vetor, tam);
    start = clock(); //start recebe o "ciclo" corrente
    switch(escolhedor) {
        case 1:
            numComp = insertionSort(cpVetor, tam);
            break;
        case 2:
            numComp = selectionSort(cpVetor, tam);
            break;
        case 3:
            numComp = mergeSort(cpVetor, tam);
            break;
        case 4:
            numComp = quickSort(cpVetor, tam);
            break;
        case 5:
            numComp = heapSort(cpVetor, tam);
            break;
    }
    end = clock();
    total = ((double)end - start)/CLOCKS_PER_SEC;
    printf("\t\t%05d\t", tam);
    printf("\t%.8f\t", total);
    printf("%d\n", numComp);

    free(cpVetor);
}

/* 
 * Recebe como entrada uma struct do tipo vetor.
 * Devolve na saida padrao uma serie de resultados com cinco tipos de 
 * algoritmos de ordenacao. O vetor original permanece desordenado.
 *
 * A funcao eh responsavel por organizar a saida dos dados, 
 * tambem gerencia a chamada da funcao que testa os algoritmos.
 * Os testes sao realizados em intervalos de tamanho 5000, com o limite sendo
 * o tamanho guardado dentro da struct de vetor. 
 */
void escreveTestesSort(vetor_t *vetor, int tipo) {
    int i, j;

    switch (tipo) {
        case 1:
            printf("Aleatorio:\n");
            break;
        case 2:
            printf("Crescente:\n");
            break;
        case 3:
            printf("Decrescente:\n");
            break;
    }
    for(i = 1; i <= 5; i++) {
        printf("------------------------------------------------------------------\n");
        
        switch(i) {
            case 1:
                printf("\tInsertion Sort:\n");
                break;
            case 2:
                printf("\tSelection Sort\n");
                break;
            case 3:
                printf("\tMerge Sort\n");
                break;
            case 4:
                printf("\tQuick Sort\n");
                break;
            case 5:
                printf("\tHeap Sort\n");
                break;
        }
    
        printf("\t\tTamanho\t");
        printf("\tTempo\t");
        printf("\tComparações\n");

        for(j = 5000; j <= vetor->tamanho; j += 5000)
            testaAlgoritmoSort(vetor->dado, j, i);
    }

}

/*
 * Recebe como entrada um vetor aleatorio indexado por [0..tam-1].
 * Devolve na saida padrao o tamanho do vetor utilizado e a 
 * quantidade de comparacao de cada um dos algoritmos.
 *
 * Os testes sao realizados em uma copia ordenada do vetor.
 */
void testaAlgoritmoBusca(int vetor[], int tam) {
    int *cpVetor;
    int idxBuscaSeq, idxBuscaBin;
    int numCompSeq, numCompBin;
    int valorBuscado;
    
    cpVetor = copiaVetor(vetor, tam);
    heapSort(cpVetor, tam);
    valorBuscado = cpVetor[aleat(0, tam)];

    numCompSeq = 0;
    idxBuscaSeq = buscaSequencial(vetor, tam, valorBuscado, &numCompSeq);
    numCompBin = 0;
    idxBuscaBin = buscaBinaria(vetor, tam, valorBuscado, &numCompBin);

    printf("%05d\t", tam);
    printf("\t%d\t\t", numCompSeq);
    printf("\t%d\t\t", numCompBin);
    printf("\t%d\t\t", idxBuscaSeq);
    printf("\t%d\n", idxBuscaBin);

    free(cpVetor);
}


/* 
 * Recebe como entrada uma struct do tipo vetor.
 * Devolve na saida padrao uma serie de resultados
 * com dois tipos de algoritmos de busca.
 *
 * A funcao eh responsavel por organizar a saida dos dados, 
 * tambem gerencia a chamada da funcao que testa os algoritmos.
 * Os testes sao feitos em intervalos de tamanho 5000, com o limite
 * sendo o tamanho guardado dentro da struct de vetor.
 */
void escreveTestesBusca(vetor_t *vetor) {
    int i;

    printf("Tamanho\t");
    printf("\tComparaçõesSeq\t");
    printf("\tComparaçõesBin\t");
    printf("\tIndiceSeq\t");
    printf("\tIndiceBin\n");

    for(i = 5000; i <= vetor->tamanho; i += 5000)
        testaAlgoritmoBusca(vetor->dado, i);
}

int main(){
    srand(time(NULL));

    char nome[MAX_CHAR_NOME];
    vetor_t *vetorAleatorio, *vetorCrescente, *vetorDecrescente;
    int tamanho;

    getNome(nome);
    printf("Trabalho de %s\n", nome);
    printf("GRR %u\n", getGRR());

    tamanho = 65000;
    if(!(vetorAleatorio = inicializaVetorAleatorio(tamanho))) {
        printf("Falha fatal. Impossível alocar memoria.");
        return 1;
    }
    if(!(vetorCrescente = inicializaVetorOrdenado(tamanho, 1))) {
        printf("Falha fatal. Impossível alocar memoria.");
        return 1;
    }
    if(!(vetorDecrescente = inicializaVetorOrdenado(tamanho, 2))) {
        printf("Falha fatal. Impossível alocar memoria.");
        return 1;
    }

    printf("*********************************************************************\n");
    printf("Todos os testes serao feitos com tamanhos variando entre 5000 e 65000\n");
    printf("*********************************************************************\n");
    printf("*********************************************************************\n");
    printf("Testes dos Algoritmos de Ordenação:\n\n");

    escreveTestesSort(vetorAleatorio, 1);
    escreveTestesSort(vetorCrescente, 2);
    escreveTestesSort(vetorDecrescente, 3);
    
    printf("\n*********************************************************************\n");
    printf("*********************************************************************\n");
    printf("Testes dos Algoritmos de Busca:\n\n");

    escreveTestesBusca(vetorCrescente);

    liberaStructVetor(vetorAleatorio);
    liberaStructVetor(vetorCrescente);
    liberaStructVetor(vetorDecrescente);

    return 0;
}
