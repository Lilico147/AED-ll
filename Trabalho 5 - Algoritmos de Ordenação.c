#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>

struct stat infoArquivo;

void trocar(int32_t *a, int32_t *b) {
    int32_t aux = *a;
    *a = *b;
    *b = aux; 
}

void bubbleSortOriginal(int32_t *lista, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n - 1; j++) {
            if(lista[j] > lista[j + 1])
                trocar(&lista[j], &lista[j + 1]);
        }
    }
}

void bubbleSortMelhorado(int32_t *lista, int n) {
    int mudou = 1, j, N = n - 1, guarda = n;

    while(mudou) {
        j = 0;
        mudou = 0;

        while(j < N) {
            if(lista[j] > lista[j + 1]) {
                trocar(&lista[j], &lista[j + 1]);
                mudou = 1;
                guarda = j;
            }
            j++;
        }
        N = guarda;
    }
}

void insertionSort(int32_t *lista, int n) {
    int32_t prov, valor;
    int j;

    for(int i = 1; i < n; i++) {
        prov = lista[i];
        valor = lista[i];
        j = i - 1;

        while(j >= 0 && valor < lista[j]) {
            lista[j + 1] = lista[j];
            j = j - 1;
        }
        lista[j + 1] = prov;
    }
}

void intercalar(int32_t *lista, int ini1, int ini2, int fim2) {
    int fim1 = ini2 + 1, nro = 0, ind = ini1, tam = fim2 - ini1 + 1, inicio = ini1;
    int32_t *Tmp = (int32_t*)malloc(tam * sizeof(int32_t));

    while(inicio <= ini2 && fim1 <= fim2) {
        if(lista[inicio] < lista[fim1]) {
            Tmp[nro] = lista[inicio];
            inicio++;
        } else {
            Tmp[nro] = lista[fim1];
            fim1++;
        }
        ind++;
        nro++;
    }
    while(inicio <= ini2) {
        Tmp[nro] = lista[inicio];
        inicio++;
        ind++;
        nro++;
    }
    while(fim1 <= fim2) {
        Tmp[nro] = lista[fim1];
        fim1++;
        ind++;
        nro++;
    }
    for(int i = ini1; i <= fim2; i++)
        lista[i] = Tmp[i - ini1];

    free(Tmp);
}

void mergeSort(int32_t *lista, int esq, int dir) {
    if(esq < dir) {
        int centro = (esq + dir) / 2;
        mergeSort(lista, esq, centro);
        mergeSort(lista, centro + 1, dir);
        intercalar(lista, esq, centro, dir);
    }
}

void quickSortUltimoElemento(int32_t *lista, int ini, int fim) {
    int i, j, chave;

    if(fim - ini < 2) {
        if(fim - ini == 1) {
            if(lista[ini] > lista[fim])
                trocar(&lista[ini], &lista[fim]);
        }
    } else {
        i = ini;
        j = fim - 1;
        chave = fim;

        while(j >= i) {
            while(lista[i] < lista[chave])
                i++;
            while(j >= ini && lista[j] >= lista[chave])
                j--;

            if(j >= i) {
                trocar(&lista[i], &lista[j]);
                i++;
                j--;
            }
        }
        trocar(&lista[i], &lista[chave]);

        if(ini < i - 1)
            quickSortUltimoElemento(lista, ini, i - 1);
        if(i + 1 < fim)
            quickSortUltimoElemento(lista, i + 1, fim);
    }
}

void quickSortElementoAleatorio(int32_t *lista, int ini, int fim) {
    int i, j, chave;

    if(fim - ini < 2) {
        if(fim - ini == 1) {
            if(lista[ini] > lista[fim])
                trocar(&lista[ini], &lista[fim]);
        }
    } else {
        srand(time(NULL));
        int aleatorio = ini + rand() % (fim - ini); 
        
        trocar(&lista[aleatorio], &lista[fim]);
        i = ini;
        j = fim - 1;
        chave = lista[fim];

        while(j >= i) {
            while(lista[i] < chave)
                i++;
            while(j >= ini && lista[j] >= chave)
                j--;

            if(j >= i) {
                trocar(&lista[i], &lista[j]);
                i++;
                j--;
            }
        }
        trocar(&lista[i], &lista[fim]);
        
        if(ini < i - 1)
            quickSortElementoAleatorio(lista, ini, i - 1);
        if(i + 1 < fim)
            quickSortElementoAleatorio(lista, i + 1, fim);
    }
}

// Obtém o pivô do array por mediana de 3, especificamente para o QuickSort.
void PIVOT(int32_t *lista, int ini, int fim, int *mediana) {
    int mid = floor((ini + fim) / 2);

    if((lista[ini] > lista[mid]) ^ (lista[ini] > lista[fim]))
        *mediana = ini;
    else if((lista[mid] < lista[ini]) ^ (lista[mid] < lista[fim]))
        *mediana = mid;
    else
        *mediana = fim; 
}

void quickSortMediana(int32_t *lista, int ini, int fim) {
    int i, j, chave;

    if(fim - ini < 2) {
        if(fim - ini == 1) {
            if(lista[ini] > lista[fim])
                trocar(&lista[ini], &lista[fim]);
        }
    } else {
        int mediana;

        PIVOT(lista, ini, fim, &mediana);
        trocar(&lista[mediana], &lista[fim]);
        i = ini;
        j = fim - 1;
        chave = lista[fim];

        while(j >= i) {
            while(lista[i] < chave)
                i++;
            while(lista[j] > chave)
                j--;

            if(j > i) {
                trocar(&lista[i], &lista[j]);
                i++;
                j--;
            } else
                break;
        }
        trocar(&lista[i], &lista[fim]);

        quickSortMediana(lista, ini, i - 1);
        quickSortMediana(lista, i + 1, fim); 
    }
}

void descer(int32_t *lista, int i, int n) {
    int j = 2 * i;

    if(j <= n) {
        if(j < n) {
            if(lista[j + 1] > lista[j])
                j++;
        }
        if(lista[i] < lista[j]) {
            trocar(&lista[i], &lista[j]);
            descer(lista, j, n);
        }
    }
}

void arranjar(int32_t *lista, int n) {
    for(int i = n / 2; i >= 0; i--)
        descer(lista, i, n - 1);
}

void heapSort(int32_t *lista, int n) {
    arranjar(lista, n);
    int m = n - 1;

    while(m >= 0) {
        trocar(&lista[0], &lista[m]);
        m--;
        descer(lista, 0, m);
    }
}

int main(int argc, char *argv[]) {
    if(argc == 4) {
        unsigned short algoritmoEscolhido = atoi(argv[1]);
        char *nomeArquivoEntrada = argv[2];
        char *nomeArquivoSaida = argv[3];

        FILE *arquivoEntrada, *arquivoSaida;

        int i = 0, tamanhoArray;

        clock_t inicio, fim;

        // Abre os arquivos de entrada e saída.
        arquivoEntrada = fopen(nomeArquivoEntrada, "rb");
        arquivoSaida = fopen(nomeArquivoSaida, "wb");

        // Obtém a quantidade de bytes do arquivo de entrada e cria um array para armazenar os valores.
        fstat(fileno(arquivoEntrada), &infoArquivo);
        int32_t *array = malloc(infoArquivo.st_size);
        tamanhoArray = infoArquivo.st_size / sizeof(int32_t);

        // Verificação caso haja erro na abertura dos arquivos.
        if(!arquivoEntrada) {
            printf("Este arquivo de entrada não existe!\n");
            fclose(arquivoEntrada);
            fclose(arquivoSaida);
            return 0;
        }
        if(!arquivoSaida) {
            printf("Não foi possível abrir o arquivo de saída!\n");
            fclose(arquivoEntrada);
            fclose(arquivoSaida);
            return 0;
        }

        // Lê os dados do arquivo de entrada.
        while(fread(&array[i], sizeof(int32_t), 1, arquivoEntrada)) {
            i++;
        }

        // Ordena o array de acordo com o algoritmo escolhido e mede o tempo de execução.
        inicio = clock();

        switch(algoritmoEscolhido) {
            case 1:
                bubbleSortOriginal(array, tamanhoArray);
                break;
            case 2:
                bubbleSortMelhorado(array, tamanhoArray);
                break;
            case 3:
                insertionSort(array, tamanhoArray);
                break;
            case 4:
                mergeSort(array, 0, tamanhoArray - 1);
                break;
            case 5:
                quickSortUltimoElemento(array, 0, tamanhoArray - 1);
                break;
            case 6:
                quickSortElementoAleatorio(array, 0, tamanhoArray - 1);
                break;
            case 7:
                quickSortMediana(array, 0, tamanhoArray - 1);
                break;
            case 8:
                heapSort(array, tamanhoArray);
                break;
            default:
                printf("Escolha uma opção válida!\n");
                break;
        }

        fim = clock();

        // Escreve os dados ordenados no arquivo de saída.
        fwrite(array, sizeof(int32_t), tamanhoArray, arquivoSaida);

        // Libera a memória alocada.
        free(array);
        fclose(arquivoEntrada);
        fclose(arquivoSaida);

        printf("Tempo de execução: %.3f segundos\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
    } else {
        printf("O número de argumentos fornecidos é inválido.\n");
    }

    return 0;
}

