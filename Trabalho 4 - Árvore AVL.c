#include "gfx/gfx.h"
#include <stdio.h>  
#include <SDL/SDL.h>
#include <stdlib.h>
#include <string.h>

#define LARGURA 1024
#define ALTURA 768

struct s_arq_no {
    int32_t chave:28;
    int32_t bal:2;
    uint32_t esq:1;
    uint32_t dir:1;
};

typedef struct s_no {
    int32_t chave:28;
    int32_t bal:2;
    int32_t reservado:2; /* sem uso */
    struct s_no* esq;
    struct s_no* dir;
} No;

int buscar (int, No*, No**);
void inserir(int, No**, int*);
No *criar(int);							//operacoes
void desalocaArvore(No*);
void removerNo(No*, No**, No*, int*, No*);

No *getAntecessor(No*, No*);
No *getMin(No*);
No *getMax(No*);						//getters
No *getSuccessor(No*, int);
No *getPredecessor(No*, int);

int rotacao(No **, int);				//rotacao

void pintaNo(No*, int, int, int);		//utils
void pause();

//manipulacao de arquivos
void lerArvore(No**, char*);
void antecipaLeitura(No*, FILE*, int);	//manipulacao de arquivos
void antecipaSave(No*, FILE*);
void salvarArvore(No*, char*);

int main() {

	No *raiz = NULL;
	char str[30];
	int h = 0;

	gfx_init(LARGURA, ALTURA, "");

	int escolha, val;
	No *ptr;

	do {
		gfx_clear();
		pintaNo(raiz, LARGURA/2, 90, 1);
		gfx_paint();
		printf("1- Inserir\n2- Remover\n3- Buscar maior e menor\n4- Buscar sucessor e predecessor\n5- Ler uma arvore\n6- Salvar esta arvore\n7- Sair\n--> ");
		scanf("%d", &escolha);
		switch (escolha){
			case 1: //insercao
				printf("\nInforme o valor a ser inserido na arvore: \n --> ");
				scanf("%d", &val);
				inserir(val, &raiz, &h);
			break;

			case 2: //remocao
				printf("\nInforme o valor a ser removido da arvore:\n --> ");
				scanf("%d", &val);
				if (buscar(val, raiz, &ptr) == 1) {
					removerNo(ptr, &raiz, NULL, &h, raiz);
					printf("Elemento %d removido da arvore\n", val);
				}
				else printf("Não foi possível remover o elemento %d pois ele nao existe na arvore\n", val);
				h = 0;
			break;

			case 3:
				ptr = getMax(raiz);
				if (ptr)
					printf("\nMaior valor da arvore: %d\n", ptr->chave);
				ptr = getMin(raiz);
				if (ptr)
					printf("Menor valor da arvore: %d\n\n", ptr->chave);
				else printf("A Arvore se encontra vazia\n");
			break;

			case 4:
				printf("\nInforme o valor para descobrir seu sucessor e predecessor: ");
				scanf("%d", &val);
				if (buscar(val, raiz, &ptr) != 1) {
					printf("Nao foi possivel localizar o elemento %d pois ele nao existe na arvore", val);
					
				} else {
					ptr = getSuccessor(raiz, val);
					if (ptr) printf("Sucessor de %d: %d\n", val, ptr->chave);
					else printf("O elemento %d nao possui sucessor\n", val);

					ptr = getPredecessor(raiz, val);
					if (ptr) printf("Predecessor de %d: %d\n", val, ptr->chave);
					else printf("O elemento %d nao possui predecessor\n", val);
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
				salvarArvore(raiz, str);
			break;

			case 7:
			break;

			default:
				printf("Opcao inexistente");
		}
		printf("\n\n");
		if (escolha != 7) //sair do codigo
			pause();
	} while (escolha != 7);
	gfx_quit();
	if (raiz)
		desalocaArvore(raiz);
	

	return 1;
}

int buscar(int val, No *no, No **ret) {
	if (!no) return 0;
	*ret = no;

	if (no->chave == val) return 1; //encontrado o elemento exato

	if (val < no->chave && no->esq == NULL) return 2; //menor elemento
	else if (val < no->chave && no->esq) return buscar(val, no->esq, ret); //buscar o menor

	if (val > no->chave && no->dir == NULL) return 3; //maior elemento
	
	else if (val > no->chave && no->dir) return buscar(val, no->dir, ret);

	return 0;
}

//cria um no com chave `chave`
No *criar(int chave){
	No *novoNo = malloc(sizeof(No));
	novoNo->chave = chave;
	novoNo->esq = NULL;
	novoNo->dir = NULL;
	novoNo->bal = 0;
	return novoNo;
}

int altura(No *pt) {
	if (!pt) return 0;
	int left = altura(pt->esq);
	int direita = altura(pt->dir);
	if (left > direita) return left + 1;
	else return direita + 1;

}

void rotacaoDireita(No **pt) {
	No *ptu = (*pt)->esq;
	if (ptu->bal == -1 || ptu->bal == 0) {
		(*pt)->esq = ptu->dir;
		ptu->dir = *pt;
		(*pt)->bal = altura((*pt)->dir) - altura((*pt)->esq); //para a remocao
		*pt = ptu;
		
	}
	else {
		No *ptv = ptu->dir;
		ptu->dir = ptv->esq;
		ptv->esq = ptu;
		(*pt)->esq = ptv->dir;
		ptv->dir = *pt;
		(*pt)->bal = altura((*pt)->dir) - altura((*pt)->esq); //tambem para remocao
		ptu->bal = altura(ptu->dir) - altura(ptu->esq);  //remocao
		*pt = ptv;
	}
	(*pt)->bal = altura((*pt)->dir) - altura((*pt)->esq);
}

void rotacaoEsquerda(No **pt) {
	No *ptu = (*pt)->dir;
	if (ptu->bal == 1 || ptu->bal == 0) {
		printf("rotacao simples em %d\n", (*pt)->chave);
		(*pt)->dir = ptu->esq;
		ptu->esq = *pt;
		(*pt)->bal = altura((*pt)->dir) - altura((*pt)->esq);
		*pt = ptu;
	}
	else {
		printf("rotacao dupla\n");
		No *ptv = ptu->esq;
		ptu->esq = ptv->dir;
		ptv->dir = ptu;
		(*pt)->dir = ptv->esq;
		ptv->esq = *pt;
		(*pt)->bal = altura((*pt)->dir) - altura((*pt)->esq);
		ptu->bal = altura(ptu->dir) - altura(ptu->esq);
		*pt = ptv;
	}
	(*pt)->bal = altura((*pt)->dir) - altura((*pt)->esq);
}

//insere um nó com chave  na arvore
void inserir(int chave, No **pt, int *h){
	if (!(*pt)) {
		*pt = criar(chave);
		*h = 1;
	}
	if ((*pt)->chave == chave) return;
	if (chave < (*pt)->chave) {
		inserir(chave, &((*pt)->esq), h);
		if (*h) 
			*h = rotacao(pt, 0);
		 
	}
	else {
		inserir(chave, &((*pt)->dir), h);
		if (*h)
			*h = rotacao(pt, 1);
	}
}

//desaloca um nó e todos seus filhos
void desalocaArvore(No *no) {
	if (no->esq)
		desalocaArvore(no->esq);
	if (no->dir)
		desalocaArvore(no->dir);
	free(no);
}

//encontrar o nó pai de um nó filho partindo de um nó
No *getAntecessor(No *no, No *filho) {
	No *parent = NULL;
	while (no != filho) {
		parent = no;
		no = no->chave >= filho->chave ? no->esq : no->dir;
	}
	return parent;
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

//encontra o sucessor de um no de valor chave partindo de um nó
No *getSuccessor(No *no, int chave){
	No *successor = NULL;
	No *ptr = NULL; //apontar para o nó que contem a chave
	if (!no || buscar(chave, no, &ptr) != 1)
		return NULL; //arvore nao existe ou entao o valor nao existe na arvore
	
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

//encontra o predecessor de um nó de valor chave partindo de um nó
No *getPredecessor(No *no, int chave){
	No *predecessor = NULL;
	No *ptr = NULL;
	if (!no || buscar(chave, no, &ptr) != 1)
		return NULL; //arvore nao existe ou o valor nao existe na arvore
	
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

//retorna h 
int rotacao(No **pt, int pos) { //pos pra saber se adiciona 1 ou subtrai 1 do bal
	switch ((*pt)->bal) { //switch case
		case -1: //se for -1 e estiver subtraindo 1, precisa rotacionar
			if (!pos)
				rotacaoDireita(pt);
			else (*pt)->bal = 0; //se nao rodar, volta pra 0 (pq adicionou 1)
			return 0;
		break;

		case 0:
			(*pt)->bal = pos ? 1 : -1;
			return 1;
		break;

		case 1: //rodar caso for adicionar 1
			if (pos) rotacaoEsquerda(pt);
			else (*pt)->bal = 0;
			return 0;
		break;
	}
	return 0;
}

void updateBal(No **no, No *ancestral, int aEsquerda) { //pra remocao
	short int h = altura((*no)->dir) - altura((*no)->esq);

	if (h != -2 && h != 2) {
		(*no)->bal = h;
		return;
	}

	if (h == -2) 
		rotacaoDireita(no);
		
	
	else if (h == 2) 
		rotacaoEsquerda(no);

	if (aEsquerda && ancestral)
		ancestral->esq = *no;
	else if (ancestral)
		ancestral->dir = *no;
}

//remove um no `no`
void removerNo(No *no, No **pt, No *ancestral, int *h, No *raiz) {
	if (!no) return;
	if (no != *pt && no->chave < (*pt)->chave) { 
		removerNo(no, &(*pt)->esq, *pt, h, raiz);
		if (*h) {
			int aEsquerda;
			if (ancestral) aEsquerda = ancestral->esq == (*pt);
			updateBal(pt, ancestral, aEsquerda);
		}
	}
	else if (no != *pt && no->chave > (*pt)->chave) {
		removerNo(no, &(*pt)->dir, *pt, h, raiz);
		if (*h) {
			int aEsquerda;
			if (ancestral) aEsquerda = ancestral->esq == (*pt);
			updateBal(pt, ancestral, aEsquerda);
		}
	}
	else {
		No *successor = getSuccessor(*pt, no->chave);
		int noEsq;
		if (ancestral)
			noEsq = ancestral->esq == no; //variavel para saber se `no` eh um filho da direita ou esquerda

		if (!no->esq && !no->dir) { //nao tem subarvores
			if (ancestral && noEsq) //setar como lambda no ancestral
				ancestral->esq = NULL;
			else if (ancestral) 
				ancestral->dir = NULL;
			if (ancestral) //se for a raiz nao precisa checar rotacoes
				*h = 1;
			*pt = NULL; 
		}

		else if (!no->esq) {//caso A (nao tem filho a esquerda)
			if (ancestral && noEsq) //caso for um filho da esquerda do ancestral, substituir o filho da esquerda do ancestral
				ancestral->esq = no->dir;
			else if (ancestral)
				ancestral->dir = no->dir;
			*pt = no->dir;
			if (ancestral) *h = 1;
		}

		else if (no->esq && !no->dir) {//caso B (apenas um filho a direita)
			if (ancestral && noEsq)
				ancestral->esq = no->esq;
			else if (ancestral)
				ancestral->dir = no->esq;
			*pt = no->esq;
			if (ancestral) *h = 1;
		}
		
		else if (no->dir && no->dir == successor) { //caso c
			if (successor->dir != NULL) *h = 1;
			successor->bal = no->bal; //herda o balanço e subtrai 1 (pq a subarvore esquerda diminuiu 1)
			successor->esq = no->esq; //herdar a subarvore esquerda, mesmo se for lambda, successor nunca vai ter subarvore esquerda
			rotacao(&successor, 0); // bal -1
			if (ancestral && ancestral->chave > no->chave) //z eh a chave esquerda do ancestral
				ancestral->esq = successor;
			else if (ancestral) ancestral->dir = successor; //chave direita
			*pt = successor;
		}

		//caso d
		else {
			int aEsquerda;
			No *successor_ancestor = getAntecessor(raiz, successor);
			No *successor_ancestor_ancestor = successor_ancestor ? getAntecessor(raiz, successor_ancestor) : NULL; //mudar o esq ou dir depois de fazer a rotacao!
			//se o no a ser removido eh o ancestral do ancestral do sucessor, entao quando for substituido sera o proprio sucessor
			if (no == successor_ancestor_ancestor) successor_ancestor_ancestor = successor; 
			if (successor_ancestor_ancestor)
				aEsquerda = successor_ancestor_ancestor->esq == successor_ancestor;

			if (successor_ancestor->dir != NULL) *h = 1;
			if (successor_ancestor) {// o sucessor sempre vai ser um filho da esquerda
				successor_ancestor->esq = successor->dir; //o sucessor nunca vai ter uma subarvore esquerda

			}
			if (ancestral)
				ancestral->dir = successor;
			successor->esq = no->esq;
			successor->dir = no->dir;
			*pt = successor;

			//nao necessita de if pois sempre vai existir neste caso
			updateBal(&successor_ancestor, successor_ancestor_ancestor, aEsquerda);

			
			if (successor_ancestor_ancestor)
				successor_ancestor_ancestor->bal = altura(successor_ancestor_ancestor->dir) - altura(successor_ancestor_ancestor->esq);
			
		}
		if (*pt) 
			(*pt)->bal = altura((*pt)->dir) - altura((*pt)->esq);
		free(no);
	}
}

//printa a arvore na tela
void pintaNo(No *no, int x, int y, int profundidade) {
	char text[30];
	int text_w, text_h;
	if (!no) { //arvore vazia
		return;
	}
	gfx_set_font_size(10);

	//pintar em pos ordem
	if (no->esq) { 
		gfx_line(x, y, x - (200/profundidade), y + 80);
		pintaNo(no->esq, x - (200/profundidade), y + 80, profundidade + 1);
	}
	if (no->dir) {
		gfx_line(x, y, x + (200/profundidade), y + 80);
		pintaNo(no->dir, x + (200/profundidade), y + 80, profundidade + 1);
	}
	gfx_set_color(0, 0, 0);
	gfx_filled_ellipse(x, y, 22, 22);
	gfx_set_color(255,255,255);
	gfx_ellipse(x, y, 22, 22);
	sprintf(text, "%d", no->chave);
	gfx_get_text_size(text, &text_w, &text_h);
	gfx_text(x - text_w/2, y - text_h/2, text);

	sprintf(text, "%d", no->bal);
	gfx_get_text_size(text, &text_w, &text_h);
	gfx_text(x - text_w/2, y - text_h/2 + 10, text);

}

//funcao recursiva para construir a arvore
void antecipaLeitura(No *ancestral, FILE *ptr, int aEsquerda) {
	struct s_arq_no *noBinario = malloc(sizeof(struct s_arq_no));
	fread(noBinario, sizeof(struct s_arq_no), 1, ptr);

	No *novoNo = malloc(sizeof(No)); //alocar o novo no
	novoNo->chave = noBinario->chave;
	novoNo->esq = NULL;
	novoNo->dir = NULL;
	novoNo->bal = noBinario->bal;

	if (aEsquerda)	ancestral->esq = novoNo;
	else ancestral->dir = novoNo;
	

	if (noBinario->esq) {
		antecipaLeitura(novoNo, ptr, 1);
	}
	if (noBinario->dir) {
		antecipaLeitura(novoNo, ptr, 0);
	}
	free(noBinario);
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

	struct s_arq_no *noBinario = malloc(sizeof(struct s_arq_no));
	fread(noBinario, sizeof(struct s_arq_no), 1, ptr);

	//desalocar a raiz antiga (caso exista)
	if (*raiz)
		desalocaArvore(*raiz);
	
	*raiz = malloc(sizeof(No));
	*raiz = criar(noBinario->chave);
	(*raiz)->bal = noBinario->bal;
	if (noBinario->esq)
		antecipaLeitura(*raiz, ptr, 1);
	if (noBinario->dir)
		antecipaLeitura(*raiz, ptr, 0);
	
	free(noBinario);
	fclose(ptr);
	printf("Arquivo %s carregado\n", filename);
}

void antecipaSave(No *no, FILE *ptr) {
	struct s_arq_no *noBinario = malloc(sizeof(struct s_arq_no));
	noBinario->chave = no->chave;
	noBinario->esq = no->esq != NULL;
	noBinario->dir = no->dir != NULL;
	noBinario->bal = no->bal;

	fwrite(noBinario, sizeof(struct s_arq_no), 1, ptr);
	if (no->esq) antecipaSave(no->esq, ptr);
	if (no->dir) antecipaSave(no->dir, ptr);
	free(noBinario);
}

void salvarArvore(No *raiz, char *filename){

	FILE *ptr = fopen(filename, "wb+");
	if (!ptr) return;
	if (!raiz) fputc(0, ptr); //arquivo vazio
	else antecipaSave(raiz, ptr);


	fclose(ptr);
	printf("Arquivo %s salvo\n", filename);
}

void pause(){
	fflush(stdout);
	system("read -p \"Pressione uma tecla para continuar\" var");
}

