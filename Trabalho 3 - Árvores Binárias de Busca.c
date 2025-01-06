//    AUTOR: Alexandre Cordeiro Arruda    RGM:43551
//    Faça um programa em C para Linux que suporte as seguintes operações sobre uma árvore binária de busca que armazena chaves inteiras:
//    Busca pela maior e menor chave na árvore;
//    Busca por uma chave com valor x;
//    Busca pelo sucessor e predecessor de uma chave com valor x;
//    Inserção e remoção de uma chave com valor x;
//    Visualização da representação hierárquica da árvore em modo gráfico usando a biblioteca gfx;
//    Leitura e gravação da árvore em arquivo formato binário usando percurso em pré-ordem. A árvore não deve ser reconstruída através de operações de inserção;
//    Desalocação da árvore.

#include "gfx/gfx.h"
#include <stdio.h>  /* printf */
#include <SDL/SDL.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 1024
#define HEIGHT 768

struct s_arq_no
{
    int32_t chave:30;
    uint32_t esq:1;
    uint32_t dir:1;
};

typedef struct No {
	int chave;
	struct No *esq;
	struct No *dir;
} No;

//operacoes
int procurar (int, No*, No**);
void inserir(int, No**);
No *criar(int);                        //operacoes
void desalocaArvore(No*);
void removeNode(No*, No**);

No *getAncestral(No*, No*);
No *getMin(No*);
No *getMax(No*);                     //getters
No *getSucessor(No*, int);
No *getAntecessor(No*, int);

void paintNode(No*, int, int, int);    //utils
void pause();

void lerArvore(No**, char*);
void preorderLer(No*, FILE*, int);     //manipulacao de arquivos
void preorderSalvar(No*, FILE*);
void salvaArvore(No*, char*);

int main() {
	
	No *raiz = NULL;
	char str[30];

	gfx_init(WIDTH, HEIGHT, "");

	int escolha, val;
	No *ptr;

	do {
		gfx_clear();
		paintNode(raiz, WIDTH/2, 90, 1);
		gfx_paint();
		printf("1. Inserir\n2. Remover\n3. Buscar maior e menor\n4. Buscar sucessor e predecessor\n5. Ler arquivo\n6. Salvar arquivo\n7. Sair\n--> ");
		scanf("%d", &escolha);
		switch (escolha){
			case 1: //insercao
				printf("\nInforme o valor a ser inserido na arvore: \n --> ");
				scanf("%d", &val);
				inserir(val, &raiz);
			break;

			case 2: //remocao
				printf("\nInforme o valor a ser removido da arvore:\n --> ");
				scanf("%d", &val);
				if (procurar(val, raiz, &ptr) == 1) {
					removeNode(ptr, &raiz);
					printf("[SUCESSO] Elemento %d removido da arvore\n", val);
				}
				else printf("[ERRO] Elemento %d nao existe na arvore\n", val);
			break;

			case 3:
				ptr = getMax(raiz);
				if (ptr)
					printf("\nMaior valor da arvore: %d\n", ptr->chave);
				ptr = getMin(raiz);
				if (ptr)
					printf("Menor valor da arvore: %d\n\n", ptr->chave);
				else printf("[ERRO] Arvore vazia\n");
			break;

			case 4:
				printf("\nInforme o valor para descobrir seu sucessor e predecessor: ");
				scanf("%d", &val);
				if (procurar(val, raiz, &ptr) != 1) {
					printf("[ERRO] Elemento %d nao existe na arvore", val);
					
				} else {
					ptr = getSucessor(raiz, val);
					if (ptr) printf("Sucessor de %d: %d\n", val, ptr->chave);
					else printf("[ERRO] %d nao possui sucessor\n", val);

					ptr = getAntecessor(raiz, val);
					if (ptr) printf("Predecessor de %d: %d\n", val, ptr->chave);
					else printf("[ERRO] %d nao possui predecessor\n", val);
				}
			break;

			case 5:
				printf("\nInforme o nome do arquivo:\n --> ");
				scanf("%s", str);
				lerArvore(&raiz, str);
			break;

			case 6:
				printf("\nInforme o nome do arquivo:\n --> ");
				scanf("%s", str);
				salvaArvore(raiz, str);
			break;

			case 7:
			break;

			default:
				printf("[ERRO] Opcao inexistente");
		}
		printf("\n\n");
		if (escolha != 7) //nao pedir para pressionar tecla quando for sair
			pause();
	} while (escolha != 7);
	gfx_quit();
	if (raiz)
		desalocaArvore(raiz);
	

	return 1;
}

/*  Procura um valor val e armazena o ponteiro para o no em ret
	retorna um valor f:
	0 para arvore vazia
	1 para elemento encontrado
	2 para val MENOR que a chave do no
	3 para val MAIOR que a chave do no
*/
int procurar(int val, No *no, No **ret) {
	if (!no) return 0;
	*ret = no;

	if (no->chave == val) return 1; //encontrado o elemento exato

	if (val < no->chave && no->esq == NULL) return 2; //menor elemento
	else if (val < no->chave && no->esq) return procurar(val, no->esq, ret); //buscar o menor

	if (val > no->chave && no->dir == NULL) return 3;
	
	else if (val > no->chave && no->dir) return procurar(val, no->dir, ret);

	return 0;
}

//cria um nó com chave
No *criar(int chave){
	No *newNode = malloc(sizeof(No));
	newNode->chave = chave;
	newNode->esq = NULL;
	newNode->dir = NULL;
	return newNode;
}

//insere um no com chave `chave` na arvore `raiz` 
void inserir(int chave, No **raiz){

	No *newNode = criar(chave);
	No *pt = *raiz;
	int status = procurar(chave, pt, &pt); //os codigos da busca (1 para igual, 2 para menor, etc)

	switch(status) {
		case 1:
			printf("[ERRO] %d ja existe na arvore\n", chave);
			break;
		case 0:
			*raiz = newNode;
			printf("Arvore criada com raiz %d\n", chave);
			break;
		case 2:
			pt->esq = newNode;
			printf("No %d adicionado a arvore\n", chave);
			break;
		case 3:
			pt->dir = newNode;
			printf("No %d adicionado a arvore\n", chave);
			break;
	}
}

//desaloca um no no e todos seus filhos
void desalocaArvore(No *no) {
	if (no->esq)
		desalocaArvore(no->esq);
	if (no->dir)
		desalocaArvore(no->dir);
	free(no);
}

//encontrar o no pai de um no `filho` partindo de um no 'nó'
No *getAncestral(No *no, No *filho) {
	No *pai = NULL;
	while (no != filho) {
		pai = no;
		no = no->chave >= filho->chave ? no->esq : no->dir;
	}
	return pai;
}

//encontra o valor minimo de uma arvore/subarvore
No *getMin(No *no) {
	while (no->esq)
		no = no->esq;
	return no;
}

//encontra o valor maximo de uma arvore/subarvore
No *getMax(No *no) {
	while (no->dir)
		no = no->dir;
	return no;
}

//encontra o sucessor de um no de valor 'chave' partindo de um nó
No *getSucessor(No *no, int chave){
	No *successor = NULL;
	No *ptr = NULL; //apontar para o no que contem a chave
	if (!no || procurar(chave, no, &ptr) != 1)
		return NULL; //ARVORE nao existe ou entao o valor nao existe na arvore
	
	if (ptr->dir) successor = getMin(ptr->dir); //tem subarvore direita

	else while (no != NULL) {
		if (chave >= no->chave) {
			no = no->dir;
		}
		else {
			successor = no;
			no = no->esq;
		}
	}
	return successor;
}

//encontra o predecessor de um no de valor `chave` partindo de um nó
No *getAntecessor(No *no, int chave){
	No *predecessor = NULL;
	No *ptr = NULL;
	if (!no || procurar(chave, no, &ptr) != 1)
		return NULL; //ARVORE nao existe ou entao o valor nao existe na arvore
	
	if (ptr->esq) predecessor = getMax(ptr->esq); //tem subarvore esquerda

	else while (no != NULL) {
		if (chave <= no->chave) {
			no = no->esq;
		}
		else {
			predecessor = no;
			no = no->dir;
		}
	}
	return predecessor;
}

//remove um nó de uma arvore `raiz`
void removeNode(No *no, No **raiz) {
	if (!no) return;

	No *ancestral = getAncestral(*raiz, no);
	No *successor = getSucessor(*raiz, no->chave);
	int isLeftNode;
	if (ancestral)
		isLeftNode = ancestral->esq == no; //variavel para saber se nó eh um filho da direita ou esquerda

	if (!no->esq && !no->dir) { //nao tem subarvores
		if (ancestral && isLeftNode) //setar como lambda no ancestral
			ancestral->esq = NULL;
		else if (ancestral) 
			ancestral->dir = NULL;
		if (no == *raiz)
			*raiz = NULL;
	}

	else if (!no->esq) {//caso (a) (nao tem filho esquerdo)
		if (ancestral && isLeftNode) //caso for um filho da esquerda do ancestral, substituir o filho da esquerda do ancestral
			ancestral->esq = no->dir;
		else if (ancestral)
			ancestral->dir = no->dir;
		if (no == *raiz)
			*raiz = no->dir;
	}

	else if (no->esq && !no->dir) {//caso (b) (apenas um filho a direita)
		if (ancestral && isLeftNode)
			ancestral->esq = no->esq;
		else if (ancestral)
			ancestral->dir = no->esq;
		if (no == *raiz)
			*raiz = no->esq;
	}
	
	else if (no->dir && no->dir == successor) { //caso c
		successor->esq = no->esq; //herdar a subarvore esquerda, mesmo se for lambda, successor nunca vai ter subarvore esquerda
		if (ancestral && ancestral->chave > no->chave) //z eh a chave esquerda do ancestral
			ancestral->esq = successor;
		else if (ancestral) ancestral->dir = successor; //chave direita
		if (no == *raiz)
			*raiz = successor;
	}

	//caso d
	else {
		No *successor_ancestor = getAncestral(*raiz, successor);
		if (successor_ancestor) // SEMPRE o sucessor vai ser um filho da esquerda
			successor_ancestor->esq = successor->dir; //sucessor NUNCA vai ter uma subarvore esquerda
		if (ancestral)
			ancestral->dir = successor;
		successor->esq = no->esq;
		successor->dir = no->dir;
		if (no == *raiz)
			*raiz = successor;
	}
	free(no);

}

//printa a arvore na tela
void paintNode(No *no, int x, int y, int depth) {
	char text[30];
	int text_w, text_h;
	if (!no) { //arvore vazia
		gfx_set_font_size(40);
		gfx_get_text_size("Arvore Vazia", &text_w, &text_h);
		gfx_text(WIDTH/2 - text_w/2, HEIGHT/2 - text_h/2, "Arvore Vazia");
		return;
	}
	gfx_set_font_size(10);

	//pintar em pos ordem
	if (no->esq) { 
		gfx_line(x, y, x - (200/depth), y + 80);
		paintNode(no->esq, x - (200/depth), y + 80, depth + 1);
	}
	if (no->dir) {
		gfx_line(x, y, x + (200/depth), y + 80);
		paintNode(no->dir, x + (200/depth), y + 80, depth + 1);
	}
	gfx_set_color(0, 0, 0);
	gfx_filled_ellipse(x, y, 22, 22);
	gfx_set_color(255,255,255);
	gfx_ellipse(x, y, 22, 22);
	sprintf(text, "%d", no->chave);
	gfx_get_text_size(text, &text_w, &text_h);
	gfx_text(x - text_w/2, y - text_h/2, text);
}

//funcao recursiva para construir a arvore
void preorderLer(No *ancestral, FILE *ptr, int isLeft) {
	struct s_arq_no *nodeBinario = malloc(sizeof(struct s_arq_no));
	fread(nodeBinario, sizeof(struct s_arq_no), 1, ptr);

	No *newNode = malloc(sizeof(No)); //alocar o novo nó
	newNode->chave = nodeBinario->chave;
	newNode->esq = NULL;
	newNode->dir = NULL;

	if (isLeft)	ancestral->esq = newNode;
	else ancestral->dir = newNode;
	

	if (nodeBinario->esq) {
		preorderLer(newNode, ptr, 1);
	}
	if (nodeBinario->dir) {
		preorderLer(newNode, ptr, 0);
	}
	free(nodeBinario);
}

//funcao para abrir o arquivo `filename` e setar a raiz de uma arvore `raiz`
void lerArvore(No **raiz, char *filename){
	FILE *ptr = fopen(filename, "rb");
	if (!ptr) {
		printf("Arquivo nao encontrado\n");
		return;
	}

	fseek(ptr, 0, SEEK_END); //saber se tem UM nó no arquivo (no minimo)
	size_t size = ftell(ptr);
	fseek(ptr, 0, SEEK_SET);

	if (sizeof(struct s_arq_no) > size) { //carregar um arquivo vazio significa desalocar a arvore
		desalocaArvore(*raiz);
		*raiz = NULL;
		printf("Arquivo vazio\n");
		return;
	}

	struct s_arq_no *nodeBinario = malloc(sizeof(struct s_arq_no));
	fread(nodeBinario, sizeof(struct s_arq_no), 1, ptr);

	//desalocar a raiz antiga (caso exista)
	if (*raiz)
		desalocaArvore(*raiz);
	
	*raiz = malloc(sizeof(No));
	*raiz = criar(nodeBinario->chave);
	if (nodeBinario->esq)
		preorderLer(*raiz, ptr, 1);
	if (nodeBinario->dir)
		preorderLer(*raiz, ptr, 0);
	
	free(nodeBinario);
	fclose(ptr);
	printf("Arquivo %s carregado\n", filename);
}

void preorderSalvar(No *no, FILE *ptr) {
	struct s_arq_no *nodeBinario = malloc(sizeof(struct s_arq_no));
	nodeBinario->chave = no->chave;
	nodeBinario->esq = no->esq != NULL;
	nodeBinario->dir = no->dir != NULL;
	fwrite(nodeBinario, sizeof(struct s_arq_no), 1, ptr);
	if (no->esq) preorderSalvar(no->esq, ptr);
	if (no->dir) preorderSalvar(no->dir, ptr);
	free(nodeBinario);
}

void salvaArvore(No *raiz, char *filename){

	FILE *ptr = fopen(filename, "wb+");
	if (!ptr) return;
	if (!raiz) fputc(0, ptr); //arquivo vazio
	else preorderSalvar(raiz, ptr);


	fclose(ptr);
	printf("Arquivo %s salvo\n", filename);
}

void pause(){
	fflush(stdout);
	system("read -p \"Pressione uma tecla para continuar\" var");
}
