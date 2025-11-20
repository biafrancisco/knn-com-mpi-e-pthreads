/* Implementação do KNN
 * Autoras:
 * - Bianca Mendes Francisco
 * - Nathália Nogueira Alves */

#include "knn.h"
/* -------------------------- Funções auxiliares --------------------------*/
// Calcula a distância quadrática entre dois pontos
float distanceSquared(float firstPoint[], float secondPoint[], int dimensionality) {
    float distance, diff;

    distance = 0;
    diff = 0;
    for(int i = 0; i < dimensionality; i++) {
        diff = secondPoint[i] - firstPoint[i]; 
        distance += diff * diff;
    }

    return distance;
}

/* -------------------------- Core functions --------------------------*/
// Calcula os k-vizinhos mais próximos de cada ponto de Q em P.
// Recebe todos os parametros separados, para ser usado com apenas uma thread
int *knn(float Q[], int nq, float P[], int np, int d, int k) {
    int *result = allocateZeroedIntArray(nq * k);
    float *heap;
    int *heapIndices;
    int heapSize;
    float distance;
    float *qPoint, *pPoint;

    heap = allocateZeroedFloatArray(k);
    heapIndices = allocateZeroedIntArray(k);

    for (int i = 0; i < nq; i++) {
        heapSize = 0;

        qPoint = Q + i * d;

        // Se menos de k pontos foram vistos, insere na heap.
        // Quando k pontos já estão armazenados, aplica decreaseMax em cada um dos novos.
        for (int j = 0; j < np; j++) {
            pPoint = P + j * d;

            distance = distanceSquared(qPoint, pPoint, d);
            
            if (j < k) {
                insert(heap, &heapSize, distance, heapIndices, j);
            } else {
                decreaseMax(heap, heapSize, distance, heapIndices, j);
            }
        }

        // Copia os k-vizinhos mais próximos de Q[i] para a linha i da matriz resultado
        for (int p = 0; p < heapSize; p++) {
            result[i * k + p] = heapIndices[p];
        }

    }

    destroyArray(heap);
    destroyArray(heapIndices);

    return result;
}

// Corpo do calculo de KNN com uso de threads
// Recebe uma unica struct em vez de parametros separados e trabalha nela
void *knnThreadBody(void *voidArgs) {
    knnArgs *args = (knnArgs*)voidArgs;
    float *heap;
    int *heapIndices;
    int heapSize;
    float distance;
    float *qPoint, *pPoint;

    heap = allocateZeroedFloatArray(args->k);
    heapIndices = allocateZeroedIntArray(args->k);

    for (int i = args->start; i < args->end; i++) {
        heapSize = 0;
        qPoint = args->Q + i * args->d;

        for (int j = 0; j < args->np; j++) {
            pPoint = args->P + j * args->d;
            distance = distanceSquared(qPoint, pPoint, args->d);

            if (j < args->k) {
                insert(heap, &heapSize, distance, heapIndices, j);
            } else {
                decreaseMax(heap, heapSize, distance, heapIndices, j);
            }
        }

        for (int p = 0; p < heapSize; p++) {
            args->result[i * args->k + p] = heapIndices[p];
        }
    }

    destroyArray(heap);
    destroyArray(heapIndices);

    pthread_exit(NULL);
}

// Gerencia a divisão de Q por nThreads
int *knnThreads(float Q[], int nq, float P[], int np, int d, int k, int nThreads) {
    pthread_t threads[nThreads];
    knnArgs args[nThreads];
    
    int *result = allocateZeroedIntArray(nq * k);
        
    int elementsPerThread = nq / nThreads; // Divisão inteira
    int remainder = nq % nThreads; // Resto
    int start = 0;

    for (int i = 0; i < nThreads; i++) {
        int end = start + elementsPerThread + (i < remainder ? 1 : 0);

        args[i].Q = Q;
        args[i].start = start;
        args[i].end = end;
        args[i].P = P;
        args[i].np = np;
        args[i].d = d;
        args[i].k = k;
        args[i].result = result;

        pthread_create(&threads[i], NULL, knnThreadBody, (void*)&args[i]);

        start = end;
    }

    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    return result;
} 
