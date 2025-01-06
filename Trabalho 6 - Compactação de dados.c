#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALTURA_MAX_ARVORE 256

// Estrutura para armazenar nós da árvore de Huffman
struct NoHeapMin {
    unsigned char dado;
    unsigned frequencia;
    struct NoHeapMin *esquerda, *direita;
};

// Estrutura para armazenar uma heap mínima de nós
struct HeapMin {
    unsigned tamanho;
    unsigned capacidade;
    struct NoHeapMin** array;
};

// Cria um novo nó da árvore de Huffman
struct NoHeapMin* novoNo(unsigned char dado, unsigned frequencia) {
    struct NoHeapMin* temp = (struct NoHeapMin*)malloc(sizeof(struct NoHeapMin));
    temp->esquerda = temp->direita = NULL;
    temp->dado = dado;
    temp->frequencia = frequencia;
    return temp;
}

// Cria uma heap mínima (fila de prioridade)
struct HeapMin* criaHeapMin(unsigned capacidade) {
    struct HeapMin* heapMin = (struct HeapMin*)malloc(sizeof(struct HeapMin));
    heapMin->tamanho = 0;
    heapMin->capacidade = capacidade;
    heapMin->array = (struct NoHeapMin**)malloc(heapMin->capacidade * sizeof(struct NoHeapMin*));
    return heapMin;
}

// Troca dois nós da heap
void trocaNosHeapMin(struct NoHeapMin** a, struct NoHeapMin** b) {
    struct NoHeapMin* t = *a;
    *a = *b;
    *b = t;
}

// Corrige a heap para manter a propriedade de min-heap
void heapMinifica(struct HeapMin* heapMin, unsigned int idx) {
    unsigned int menor = idx;
    unsigned int esquerda = 2 * idx + 1;
    unsigned int direita = 2 * idx + 2;

    if (esquerda < heapMin->tamanho && heapMin->array[esquerda]->frequencia < heapMin->array[menor]->frequencia)
        menor = esquerda;

    if (direita < heapMin->tamanho && heapMin->array[direita]->frequencia < heapMin->array[menor]->frequencia)
        menor = direita;

    if (menor != idx) {
        trocaNosHeapMin(&heapMin->array[menor], &heapMin->array[idx]);
        heapMinifica(heapMin, menor);
    }
}

// Verifica se a heap tem tamanho 1
int tamanhoUm(struct HeapMin* heapMin) {
    return (heapMin->tamanho == 1);
}

// Remove o menor nó da heap
struct NoHeapMin* extraiMin(struct HeapMin* heapMin) {
    struct NoHeapMin* temp = heapMin->array[0];
    heapMin->array[0] = heapMin->array[heapMin->tamanho - 1];
    --heapMin->tamanho;
    heapMinifica(heapMin, 0);
    return temp;
}

// Insere um novo nó na heap e corrige a propriedade da heap
void insereHeapMin(struct HeapMin* heapMin, struct NoHeapMin* noHeapMin) {
    ++heapMin->tamanho;
    int i = heapMin->tamanho - 1;
    while (i && noHeapMin->frequencia < heapMin->array[(i - 1) / 2]->frequencia) {
        heapMin->array[i] = heapMin->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heapMin->array[i] = noHeapMin;
}

// Função para construir a heap mínima inicial
void constroiHeapMin(struct HeapMin* heapMin) {
    int n = heapMin->tamanho - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        heapMinifica(heapMin, i);
}

// Função para verificar se o nó é uma folha
int ehFolha(struct NoHeapMin* raiz) {
    return !(raiz->esquerda) && !(raiz->direita);
}

// Cria uma heap mínima com capacidade igual ao número de caracteres e insere todos os nós na heap
struct HeapMin* criaEConstroiHeapMin(unsigned char dados[], int frequencias[], int tamanho) {
    struct HeapMin* heapMin = criaHeapMin(tamanho);
    for (int i = 0; i < tamanho; ++i)
        heapMin->array[i] = novoNo(dados[i], frequencias[i]);
    heapMin->tamanho = tamanho;
    constroiHeapMin(heapMin);
    return heapMin;
}

// Função para construir a árvore de Huffman
struct NoHeapMin* constroiArvoreHuffman(unsigned char dados[], int frequencias[], int tamanho) {
    struct NoHeapMin *esquerda, *direita, *topo;
    struct HeapMin* heapMin = criaEConstroiHeapMin(dados, frequencias, tamanho);
    while (!tamanhoUm(heapMin)) {
        esquerda = extraiMin(heapMin);
        direita = extraiMin(heapMin);
        topo = novoNo('$', esquerda->frequencia + direita->frequencia);
        topo->esquerda = esquerda;
        topo->direita = direita;
        insereHeapMin(heapMin, topo);
    }
    return extraiMin(heapMin);
}

// Função para salvar os códigos de Huffman em um array de char
void salvaCodigos(struct NoHeapMin* raiz, int arr[], int topo, int codigos[ALTURA_MAX_ARVORE][ALTURA_MAX_ARVORE]) {
    if (raiz->esquerda) {
        arr[topo] = 0;
        salvaCodigos(raiz->esquerda, arr, topo + 1, codigos);
    }
    if (raiz->direita) {
        arr[topo] = 1;
        salvaCodigos(raiz->direita, arr, topo + 1, codigos);
    }
    if (ehFolha(raiz)) {
        for (int i = 0; i < topo; i++)
            codigos[raiz->dado][i] = arr[i];
        codigos[raiz->dado][topo] = -1;
    }
}

// Função para salvar a árvore no arquivo compactado usando pré-ordem
void salvaArvore(struct NoHeapMin *raiz, FILE *saida) {
    if (ehFolha(raiz)) {
        fputc('1', saida);
        fputc(raiz->dado, saida);
    } else {
        fputc('0', saida);
        if (raiz->esquerda) salvaArvore(raiz->esquerda, saida);
        if (raiz->direita) salvaArvore(raiz->direita, saida);
    }
}

// Função para ler a árvore de Huffman do arquivo compactado
struct NoHeapMin* leArvore(FILE *entrada) {
    int bit = fgetc(entrada);
    if (bit == '1') {
        unsigned char dado = fgetc(entrada);
        return novoNo(dado, 0);
    } else {
        struct NoHeapMin* esquerda = leArvore(entrada);
        struct NoHeapMin* direita = leArvore(entrada);
        struct NoHeapMin* no = novoNo('$', 0);
        no->esquerda = esquerda;
        no->direita = direita;
        return no;
    }
}

// Função para compactar um arquivo usando o algoritmo de Huffman
void compactaArquivo(const char *arquivo_entrada, const char *arquivo_saida) {
    FILE *entrada = fopen(arquivo_entrada, "rb");
    if (!entrada) {
     perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    // Contagem de frequências de caracteres
    int frequencias[256] = {0};
    unsigned char ch;
    while (fread(&ch, 1, 1, entrada) == 1) {
        frequencias[ch]++;
    }

    unsigned char dados[256];
    int frequencias_dados[256];
    int tamanho = 0;

    // Coleta os caracteres com frequências não zero
    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            dados[tamanho] = (unsigned char)i;
            frequencias_dados[tamanho] = frequencias[i];
            tamanho++;
        }
    }

    // Cria a árvore de Huffman
    struct NoHeapMin* raiz = constroiArvoreHuffman(dados, frequencias_dados, tamanho);

    // Gera os códigos de Huffman
    int codigos[ALTURA_MAX_ARVORE][ALTURA_MAX_ARVORE] = {{-1}};
    int arr[ALTURA_MAX_ARVORE], topo = 0;
    salvaCodigos(raiz, arr, topo, codigos);

    // Reescreve o arquivo compactado no formato Huffman
    FILE *saida = fopen(arquivo_saida, "wb");
    if (!saida) {
       perror("Erro ao criar o arquivo de saída");
        fclose(entrada);
        return;
    }

    // Volta para o início do arquivo de entrada
    fseek(entrada, 0, SEEK_SET);
    
    // Escreve a árvore de Huffman no início do arquivo compactado
    salvaArvore(raiz, saida);

    unsigned char buffer = 0;
    int contador_bits = 0;

    // Escreve os dados compactados
    while (fread(&ch, 1, 1, entrada) == 1) {
        for (int i = 0; codigos[ch][i] != -1; i++) {
            buffer <<= 1;
            buffer |= codigos[ch][i];
            contador_bits++;
            if (contador_bits == 8) {
                fwrite(&buffer, 1, 1, saida);
                contador_bits = 0;
                buffer = 0;
            }
        }
    }

    // Escreve o buffer restante
    if (contador_bits > 0) {
        buffer <<= (8 - contador_bits);
        fwrite(&buffer, 1, 1, saida);
    }

    fclose(entrada);
    fclose(saida);
}

// Função para descompactar um arquivo usando o algoritmo de Huffman
void descompactaArquivo(const char *arquivo_entrada, const char *arquivo_saida) {
    FILE *entrada = fopen(arquivo_entrada, "rb");
    if (!entrada) {
     perror("Erro ao abrir o arquivo compactado");
        return;
    }

    FILE *saida = fopen(arquivo_saida, "wb");
    if (!saida) {
       perror("Erro ao criar o arquivo de saída");
        fclose(entrada);
        return;
    }

    // Reconstrói a árvore de Huffman a partir do arquivo compactado
    struct NoHeapMin *raiz = leArvore(entrada);
    struct NoHeapMin *atual = raiz;
    unsigned char byte;
    while (fread(&byte, 1, 1, entrada) == 1) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;
            if (bit == 0) {
                atual = atual->esquerda;
            } else {
                atual = atual->direita;
            }

            if (ehFolha(atual)) {
                fwrite(&atual->dado, 1, 1, saida);
                atual = raiz;
            }
        }
    }

    fclose(entrada);
    fclose(saida);
}

// Função principal que gerencia a entrada da linha de comando
int main(int argc, char *argv[]) {
    if (argc != 3) {
         fprintf(stderr, "Uso: %s <c|d> <arquivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char opcao = argv[1][0];
    const char *arquivo_entrada = argv[2];
    char arquivo_saida[256];

    if (opcao == 'c') {
        snprintf(arquivo_saida, sizeof(arquivo_saida), "%s.cmp", arquivo_entrada);
        compactaArquivo(arquivo_entrada, arquivo_saida);
    } else if (opcao == 'd') {
        snprintf(arquivo_saida, sizeof(arquivo_saida), "%s.dcmp", arquivo_entrada);
        descompactaArquivo(arquivo_entrada, arquivo_saida);
    } else {
        fprintf(stderr, "Opção inválida. Use 'c' para compactar ou 'd' para descompactar.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

