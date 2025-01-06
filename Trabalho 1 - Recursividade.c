#include "gfx.h"
#include <stdio.h> 
#include <unistd.h>

//Parte do quadrado
void quadradorec
	(
	int x,		//posicionamento do quadrado
 	int y,		//posicionamento do quadrado
  	int r,		//controle da recursão
        int loc		//ajuda nos pontos onde a figura se encontra
	)
{

    //Quadrado sem preenchimento
    gfx_set_color(255, 255, 255);
    gfx_rectangle(x - (loc/2), y - (loc/2), x + (loc/2), y + (loc/2));  
    //Quadrado com preenchimento para as linhas não se sobreporem
    gfx_set_color(0,0,0);
    gfx_filled_rectangle(x - (loc / 2) + 1, y - (loc/2) + 1, x + (loc/2) - 1, y + (loc/2) - 1);

	//quadrados laterais
    if(r > 0)	//criacao de quadrados laterais para niveis acima de zero
    {
        quadradorec(x, y + (loc/2) + (loc/4), r-1, loc/2);	//quadrado de baixo
        quadradorec(x, y - (loc/2) - (loc/4), r-1, loc/2);	//quadrado de cima
        quadradorec(x - (loc / 2) - (loc/4), y, r-1, loc/2);	//quadrado da esquerda
        quadradorec(x + (loc / 2) + (loc/4), y, r-1, loc/2);	//quadrado da direita
        	
        gfx_filled_rectangle(x - (loc / 2) + 1, y - (loc/2) + 1, x + (loc/2) - 1, y + (loc/2) - 1);
    }
}

//recursão do quadrado
void quadrado()
{
    int r;
    int proc = 0;		//avanço ou regresso do procedimento
    //Verificacao
    do{
        printf("Entre com o nivel de recursao desejado: ");
        scanf("%d", &r);

        if(r > 10 || r < 0){
            proc = -1;
            printf("O nivel deve ser entre 0 e 10.\n");
        		   }
        else{
            proc = 0;
            //Primeira chamada de recursao para a construcao de quadrados
            quadradorec(300, 300, r, 200);
            }    
      } 
    while (proc != 0);
    gfx_paint();
    sleep(3);
    gfx_clear();
} 

//Desenha o círculo principal
void recurcircles
 	(
	int x,		//localizacao
	int y,		//localizacao
	int largura,	//dimensao
	int altura,	//dimensao
	int r,		//recursao
        int loc		//localizacao
        )
{
    //Principal
    gfx_set_color(255, 255, 255);
    gfx_ellipse
   	(
    	x,		//localizacao
    	y,       	//localizacao
    	largura,	//dimensao
        altura   	//dimensao
 	);
    if(r > 0)
    {
        //Right-Bottom
        recurcircles(x + loc, y + loc, largura/2, altura/2, r-1, loc/2);
        //Right-Top
        recurcircles(x + loc, y - loc, largura/2, altura/2, r-1, loc/2);
        //Left-Top
        recurcircles(x - loc, y - loc, largura/2, altura/2, r-1, loc/2);
        //Left-Bottom
        recurcircles(x - loc, y + loc, largura/2, altura/2, r-1, loc/2);
    }
}

//Método para círculo
void circles()
{ 
    int r;			//nivel de recursao
    int proc = 0;		//avanço ou regresso do procedimento
    do
    {
        printf("Entre com o nivel de recursao desejado: ");
        scanf("%d", &r);

        if(r < 0 || r > 10)
        {
            proc = -1;
            printf("O nivel deve ser entre 0 e 10.\n");
        }
        else
        {
            proc = 0;
            //Primeira chamada de recursao para a construcao de circulos
            recurcircles(300, 300, 250, 250, r, 86);
        }    
    } while (proc != 0);
     //Pintura e limpeza para gerar próximas figuras sem corromper o terminal
    gfx_paint();
    sleep(3);
    gfx_clear();
}

void menu()
{
    int escolha;
    do{
        printf("\nEscolha a figura a ser desenhada.\n 1 - Circulos\n 2 - Quadrados\n\n" );
        scanf("%d", &escolha);
        switch (escolha)
        {
        case 1:
            circles();
            gfx_clear();
            break;
            
        case 2:
            quadrado();
            gfx_clear();
            break;

        default:
            printf("\nA opcao escolhida nao eh valida, tente novamente.\n");
            gfx_paint();
            sleep(1);
            gfx_clear();
            break;
        }
       }
    while(escolha != 0);
}

int main()
{
	gfx_init(600, 600, "Figuras recursivas");
    
    menu();
	gfx_quit();

	return 0;
}

