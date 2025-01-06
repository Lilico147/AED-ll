#include "gfx.h"
#include <stdio.h>
#include <stdlib.h>

struct Lsea
{
    int valor;
    struct Lsea *prox;
};
struct Ld{
    int valor;
    struct Ld *ant;
    struct Ld *prox;
};
void inserir(int qual, int x, struct Lsea **ant, struct Lsea **ptr){
    struct Lsea *pt = malloc(sizeof(struct Lsea));
    pt->valor = x;
    switch (qual) {
        case 0:
            pt->prox = *ptr;
            *ptr = pt;
            return;
        case 1:
            pt->prox = *ptr;
            *ant = pt;
            return;
        case 2:
            pt->prox = NULL;
            *ant = pt;
        default:
            return;
    }
}
void inserirdupla(int x, struct Ld **ant, struct Ld **ptr,struct Ld **prox){
    struct Ld *pt = malloc(sizeof(struct Ld));
    pt->valor = x;
    pt->prox = *prox;
    pt->ant = *ant;
    *ptr = pt;
}

void Listasimplenc();
void Listaduplaenc();
void Fila();
void Pilha();

int main() {
    int resp;
    do {
        printf("\n Escolha a opção que deseja fazer:\n 1-Lista simplesmente encadeada\n 2-Lista circular simplesmente encadeada\n 3-Lista duplamente encadeada\n 4-Fila\n 5-Pilha\n\n");
        scanf(" %d", &resp);
        if((resp < 1) || (resp > 5)){
            printf("\n Opcao invalida!\n");
        }
    }while((resp < 1) || (resp > 5));
    switch (resp) {
        case 1:
            Listasimplenc();
            break;
        case 2:
            Listaduplaenc();
            break;
        case 3:
            Listaduplaenc();
            break;
        case 4:
            Fila();
            break;
        case 5:
            Pilha();
            break;
    }
    return 0;
}

void Listasimplenc(){
    char string[50];
    int valor, resp, contr, width, height, textwidth, textheight;
    struct Lsea *ant;
    struct Lsea *ptr;
    struct Lsea *prox;
    struct Lsea *ptlista = NULL;
    struct Lsea *pt = NULL;
    struct Lsea *desalocar = NULL;
    gfx_init(1280, 720, "Lista simplesmente encadeada");
    gfx_set_color(255, 255, 255);
    do {
        printf("\n Escolha a operação que deseja fazer:\n 1-Busca\n 2-Insercao\n 3-Remocao\n 4-Sair\n\n");
        scanf("%d", &resp);


        switch (resp) {
            case 1:
                printf("\n Informe o numero a ser buscado: \n");
                scanf("%d", &valor);
                contr = 0;
                pt = ptlista;
                while ((pt != NULL) && (contr == 0)){
                    if(pt->valor == valor){
                        contr = 1;
                        printf("\n O valor esta presente na lista.\n");
                    }
                    pt = pt->prox;
                }
                if(contr == 0){
                    printf("\n O valor nao esta presente na lista.\n");
                }
                break;
            case 2:
                printf("\n Informe o numero a ser inserido: \n");
                scanf("%d", &valor);
                contr = 0;
                if (ptlista == NULL) {
                    contr = 1;
                    inserir(0, valor, NULL, &ptlista);
                } else {
                    pt = ptlista;
                    if ((pt->valor > valor)) {
                        contr = 1;
                        inserir(0, valor, NULL, &ptlista);
                    } else {
                        if(pt->prox == NULL){
                            contr = 1;
                            inserir(2, valor, &ant, NULL);
                            pt->prox = ant;
                        }else {
                            while ((pt != NULL) && (contr == 0)) {
                                if (pt->prox != NULL) {
                                    prox = pt->prox;
                                }
                                if (pt->valor == valor) {
                                    contr = 1;
                                    printf(" Valor ja esta presente na lista.\n");
                                    break;
                                } else {
                                    if (pt->valor < valor) {
                                        if (prox->valor > valor) {
                                            inserir(1, valor, &ant, &prox);
                                            pt->prox = ant;
                                        } else {
                                            ptr = pt;
                                            pt = pt->prox;
                                        }
                                    } else {
                                        ptr = pt;
                                        pt = pt->prox;
                                    }
                                }
                            }
                        }
                        if(contr == 0){
                            inserir(2, valor, &ant, NULL);
                            ptr->prox = ant;
                        }
                    }
                }
                break;
            case 3:
                printf("\n Informe o numero a ser removido:\n");
                scanf("%d", &valor);
                contr = 0;
                ant = NULL;
                pt = ptlista;
                while ((pt != NULL) && (contr == 0)){
                    if(pt->valor == valor){
                        contr = 1;
                        ant->prox = pt->prox;
                        free(pt);
                    }else {
                        ant = pt;
                        pt = pt->prox;
                    }
                }
                if(contr == 0){
                    printf("\n O valor nao esta presente na lista.\n");
                }
        }
        gfx_clear();
        width = 0;
        height = 0;
    	pt = ptlista;
    	while (pt != NULL){
    	    if(pt->prox != NULL){
    	        sprintf(string, "%d -> ", pt->valor);
    	    }else{
    	        sprintf(string, "%d ", pt->valor);
    	    }
    	    gfx_get_text_size(string, &textwidth, &textheight);
    	    if(width+textwidth <= 1280){
    	        gfx_text(width, height, string);
    	        width += textwidth;
    	    }else{
    	        height += textheight;
    	        gfx_text(0, height, string);
    	        width = textwidth;
    	    }
            pt = pt->prox;
    	}    
        gfx_paint();

    }while(resp != 4);
    pt = ptlista;
    while (pt != NULL){
        desalocar = pt;
        pt = pt->prox;
        free(desalocar);
    }
    gfx_quit();
}

void Listaduplaenc(){
    char string[50];
    int valor, resp, contr, width, height, textwidth, textheight;
    struct Ld *nocabeca = malloc(sizeof(struct Ld));
    struct Ld *ant;
    struct Ld *prox;
    struct Ld *ptlista = nocabeca;
    struct Ld *pt = NULL;
    nocabeca->prox = NULL;
    nocabeca->ant = NULL;
    gfx_init(1280, 720, "Lista duplamente encadeada");
    gfx_set_color(255, 255, 255);
    do{
        printf("\n Escolha a operação que deseja fazer:\n 1-Busca\n 2-insercao\n 3-remocao\n 4-sair\n\n");
        scanf("%d", &resp);

        switch (resp) {
            case 1:
                printf("\n Informe o numero a ser buscado:\n");
                scanf("%d", &valor);
                contr = 0;
                pt = ptlista->prox;
                while ((pt != ptlista) && (contr == 0)){
                    if(pt->valor == valor){
                        contr = 1;
                        printf("\n O valor esta presente na lista.\n");
                    }
                    pt = pt->prox;
                }
                if(contr == 0){
                    printf("\n O valor nao esta presente na lista.\n");
                }
                break;
            case 2:
                printf("\n Informe o numero a ser inserido:\n");
                scanf("%d", &valor);
                if(ptlista->prox == NULL){
                    inserirdupla(valor, &ptlista, &pt, &ptlista);
                    ptlista->prox = pt;
                    ptlista->ant = pt;
                }else {
                    ant = ptlista->ant;
                    inserirdupla(valor, &ant, &pt, &ptlista);
                    ant->prox = pt;
                    ptlista->ant = pt;
                }
                break;
            case 3:
                printf("\n Informe o numero a ser removido:\n");
                scanf("%d", &valor);
                contr = 0;
                pt = ptlista->prox;
                while ((pt != ptlista) && (contr == 0)){
                    if(pt->valor == valor){
                        contr = 1;
                        ant = pt->ant;
                        prox = pt->prox;
                        ant->prox = prox;
                        prox->ant = ant;
                        printf("\n O valor removido foi = %d\n", pt->valor);
                        free(pt);
                    }
                    pt = pt->prox;
                }
                if(contr == 0){
                    printf("\n O valor nao esta presente na lista.\n");
                }
                break;
        }
        gfx_clear();
        width = 30;
        height = 0;
        pt = ptlista->prox;
        while ((pt != ptlista)){
            if(pt->prox != ptlista){
                sprintf(string, "%d <-> ", pt->valor);
            }else{
                sprintf(string, "%d ", pt->valor);
            }
            gfx_get_text_size(string, &textwidth, &textheight);
            if(width+textwidth <= 1280){
                gfx_text(width, height, string);
                width += textwidth;
            }else{
                height += textheight;
                gfx_text(10, height, string);
                width = textwidth + 10;
            }
            pt = pt->prox;
        }
        gfx_line(width, height+(textheight/2), width+10, height+5);
        gfx_line(width, height+(textheight/2), width+10, height+textheight-5);
        gfx_line(width, height+(textheight/2), width+30, height+(textheight/2));
        gfx_line(width+30, height+(textheight/2), width+30, height+textheight+10);
        gfx_line(width+30, height+textheight+10, 5, height+textheight+10);
        gfx_line(5, height+textheight+10, 5, textheight/2);
        gfx_line(5, textheight/2, 25, textheight/2);
        gfx_line(25, height+(textheight/2), 20, 5);
        gfx_line(25, height+(textheight/2), 20, textheight-5);
        gfx_paint();
    } while (resp != 4);
    pt = ptlista->prox;
    while ((pt != ptlista)){
        ant = pt;
        pt = pt->prox;
        free(ant);
    }
    free(ptlista);
    gfx_quit();

}

void Fila(){
    char string[50];
    int valor, resp, width, height, textwidth, textheight;
    struct Lsea *inicio = NULL;
    struct Lsea *fim = NULL;
    struct Lsea *pt = NULL;
    struct Lsea *desalocar = NULL;
    gfx_init(1280, 720, "Lista simplesmente encadeada");
    gfx_set_color(255, 255, 255);
    do{
        printf("\n Escolha a operação que deseja fazer:\n 1-insercao\n 2-remocao\n 3-sair\n\n");
        scanf("%d", &resp);
        switch (resp) {
            case 1:
                printf("\n Informe o numero a ser inserido:\n");
                scanf("%d", &valor);
                inserir(2, valor, &pt, NULL);
                if(fim != NULL){
                    fim->prox = pt;
                }else{
                    inicio = pt;
                }
                fim = pt;
                break;
            case 2:
                if(inicio != NULL){
                    pt = inicio;
                    inicio = inicio->prox;
                    if(inicio == NULL){
                        fim = NULL;
                    }
                    printf("\n O valor removido foi = %d\n", pt->valor);
                    free(pt);
                }else{
                    printf("\n A fila ja se encontra vazia!\n");
                }
                break;
        }
        gfx_clear();
        width = 0;
        height = 0;
        pt = inicio;
        sprintf(string, "inicio ");
        gfx_get_text_size(string, &textwidth, &textheight);
        gfx_text(width, height, string);
        width += textwidth;
        while (pt != NULL){
            if(pt->prox != NULL){
                sprintf(string, "%d -> ", pt->valor);
            }else{
                sprintf(string, "%d ", pt->valor);
            }
            gfx_get_text_size(string, &textwidth, &textheight);
            if(width+textwidth <= 1280){
                gfx_text(width, height, string);
                width += textwidth;
            }else{
                height += textheight;
                gfx_text(0, height, string);
                width = textwidth;
            }
            pt = pt->prox;
        }
        sprintf(string, " final");
        gfx_get_text_size(string, &textwidth, &textheight);
        gfx_text(width, height, string);
        width += textwidth;
        gfx_paint();
    } while (resp != 3);
    pt = inicio;
    while (pt != NULL){
        desalocar = pt;
        pt = pt->prox;
        free(desalocar);
    }
    gfx_quit();
}

void Pilha(){
    char string[50];
    int valor, resp, width, height, textwidth, textheight;
    struct Lsea *topo = NULL;
    struct Lsea *pt = NULL;
    struct Lsea *desalocar = NULL;
    gfx_init(1280, 720, "Pilha");
    gfx_set_color(255, 255, 255);
    do{
        printf("\n Escolha a operação que deseja fazer:\n 1-Insercao\n 2-Remocao\n 3-Sair\n\n");
        scanf("%d", &resp);
        switch (resp) {
            case 1:
                printf("\n Informe o numero que sera inserido:\n");
                scanf("%d", &valor);
                inserir(0, valor, NULL, &topo);
                break;
            case 2:
                if(topo != NULL){
                    pt = topo;
                    topo = topo->prox;
                    printf("\n O valor removido foi = %d\n", pt->valor);
                    free(pt);
                }else{
                    printf("\n A pilha ja se encontra vazia!\n");
                }
                break;
        }
        gfx_clear();
        width = 0;
        height = 0;
        pt = topo;
        sprintf(string, " topo ");
        gfx_get_text_size(string, &textwidth, &textheight);
        gfx_text(width, height, string);
        width += textwidth;
        while (pt != NULL){
            if(pt->prox != NULL){
                sprintf(string, "%d -> ", pt->valor);
            }else{
                sprintf(string, "%d ", pt->valor);
            }
            gfx_get_text_size(string, &textwidth, &textheight);
            if(width+textwidth <= 1280){
                gfx_text(width, height, string);
                width += textwidth;
            }else{
                height += textheight;
                gfx_text(0, height, string);
                width = textwidth;
            }
            pt = pt->prox;
        }
        gfx_paint();
    } while (resp != 3);
    pt = topo;
    while (pt != NULL){
        desalocar = pt;
        pt = pt->prox;
        free(desalocar);
    }
    gfx_quit();
}


