#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

	typedef struct no{
    int bollPessoa, hora, min; //no boleano 0 é pessoa física, e 1 é pessoa jurídica
    char nome[20];
    struct no *proxNo;
}tipoNo;

typedef struct listaGerenciada{
    tipoNo *inicio;
    tipoNo *fim;
    int quant;
}tipoLista;

void inicializa(tipoLista *lista){
    lista->fim = NULL;
    lista->inicio = NULL;
    lista->quant = 0;
}
    
int insereNaFila(tipoLista *listaEnc, int pessoa, char* nome, int hora, int min){
tipoNo *novoNo;
    novoNo = (tipoNo*)malloc(sizeof(tipoNo));
    if(novoNo==NULL)
        return 0;
    novoNo->bollPessoa= pessoa;
	strcpy (novoNo->nome, nome);
    novoNo->hora= hora;
    novoNo->min = min;
    novoNo->proxNo = NULL;
    if (listaEnc ->fim == NULL){
		listaEnc->inicio = novoNo;
		listaEnc->fim = novoNo;
	}else{
    listaEnc->fim->proxNo = novoNo;
    listaEnc->fim = novoNo;
}
   listaEnc->quant++;
    return 1;
}

int previsaoDaFila(tipoLista *listaEnc){
tipoNo *atual; 
if(listaEnc->inicio == NULL)
    return 0;
atual = listaEnc->inicio;
while(atual !=NULL){
    puts (atual->nome);
    atual = atual->proxNo;
    }
    return 1;
}

	int main(){
    tipoLista filaPF, filaPJ; 
    int op, op2, hr, mt;
    char nm[20];
    inicializa(&filaPF);
    inicializa(&filaPJ);
    do{
        printf("\n      Menu Fila do Banco");
        printf("\n1 - Adicionar pessoa na fila ");
        printf("\n2 - Previsao de Atendimento");
        printf("\n3 - ");
        printf("\n4 - ");
        printf("\n5 - ");
        printf("\n6 - ");
         printf("\n7 - ");
        printf("\n9 - ");
        printf("\n0- Encerrar o programa ");
        printf("\nDigite sua opcao:");
        scanf("%d",&op);
        switch(op){
			
        case 1:
			printf("\nqual fila que deseja inserir?");
			printf("\nPessoa fisica = 1");
			printf("\nPessoa juridica = 2\n");
            scanf("%d",&op2);
            printf("\nQual nome da pessoa?\n");
            getchar();
			fgets (nm, sizeof(nm), stdin);
            printf("\nEm qual tempo chegou");
            printf("\n(horas):(minutos)\n");
            scanf ("%d:%d", &hr, &mt);
				if (op2 ==1){
				insereNaFila (&filaPF, 0, nm, hr, mt); 
			}else if (op2 ==2){
				insereNaFila (&filaPJ, 1, nm, hr, mt); 
			}
           break;
            
            
        case 2:
			printf("\nqual fila que deseja analisar?");
			printf("\nPessoa fisica = 1");
			printf("\nPessoa juridica = 2\n");
			scanf ("%d", &op2);
			if (op2 ==1){
				printf("\nfila do caixa 1:\n");
				previsaoDaFila (&filaPF); 
			}else if (op2 ==2){
				printf("\nfila do caixa 2:\n");
				previsaoDaFila (&filaPJ); 
			}
             break;
        case 3:
             break;
        case 4: 
			 break;
        case 5: 
            break;
        case 6: 
            break;
        case 7: 
            break;
            
        case 9: 
            break;
        case 0: printf("\nEncerrando programa.");
            break;
        default: printf("\nOpção inválida!");
        }
     printf("\n\n\n\n");
    }while(op != 0);
    if (op == 0){
		return 0;
	}
}

