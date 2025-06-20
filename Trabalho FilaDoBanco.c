#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct no{
    int boolPessoa, boolPrioridade, hora, minuto; //no boleano 0 é pessoa física, e 1 é pessoa jurídica 
    char nome[40];
    struct no *proxNo;
}tipoNo;

typedef struct listaGerenciada{
    tipoNo *inicio;
    tipoNo *fim;
    int quant;
}tipoLista;

void inicializa(tipoLista *listaEnc){
    listaEnc->fim = NULL;
    listaEnc->inicio = NULL;
    listaEnc->quant = 0;
}
void imprimeSenha(tipoNo* cliente){ //gera e mostra senha baseada nos dados do cliente "cliente"
	printf("\n");
	if(cliente->boolPrioridade){
		printf("PRIO");
	}
	printf("%d%d", cliente->hora, cliente->minuto);
	if(!(cliente->boolPessoa))
		printf("PF");
	else
		printf("PJ");
	printf("%s", cliente->nome);
}


int insereNaFila(tipoLista *listaEnc, int pessoa, int prioridade, char* nome, int hora, int min){
	tipoNo *novoNo;
	novoNo = (tipoNo*)malloc(sizeof(tipoNo));
	if(novoNo==NULL)
			return 0;
			
	novoNo->boolPessoa= pessoa;
	strcpy (novoNo->nome, nome);
	novoNo->hora= hora;
	novoNo->minuto = min;
	novoNo->boolPrioridade = prioridade;
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

void unificarFilas(tipoLista* listaUm, tipoLista* listaDois){
	tipoLista listaNova;
	inicializa(&listaNova);
	tipoNo* proxNo, *atualUm, *atualDois;
	atualUm = listaUm->inicio;
	atualDois = listaDois->inicio;
	while(atualUm != NULL || atualDois != NULL){
		if (atualUm == NULL){
			proxNo = atualDois;
			atualDois = atualDois->proxNo;
		} 
		else if (atualDois == NULL){
			proxNo = atualUm;
			atualUm = atualUm->proxNo;
		}
		else if (atualUm->hora*60 + atualUm->minuto < atualDois->hora*60 + atualDois->minuto){
			proxNo = atualUm;
			atualUm = atualUm->proxNo;
		} else {
			proxNo = atualDois;
			atualDois = atualDois->proxNo;
		}
		insereNaFila(&listaNova, proxNo->boolPessoa, proxNo->boolPrioridade, proxNo->nome, proxNo->hora, proxNo->minuto);
	}
	*listaUm = listaNova;
}

int previsaoDaFila(tipoLista *listaNaoPreferencial, tipoLista *listaPreferencial){
	tipoNo *atual; 
	tipoNo *atualPref;
	if(listaNaoPreferencial->inicio == NULL && listaPreferencial->inicio == NULL)
		return 0;
		
	atualPref = listaPreferencial->inicio;
	atual = listaNaoPreferencial->inicio;
	int clientesPAtendidos = 0;
	while(atual != NULL || atualPref != NULL){
		if((atualPref == NULL || clientesPAtendidos == 3) && atual != NULL){ //Mostrar nao preferencial
			imprimeSenha(atual);
			clientesPAtendidos = 0;
			atual = atual->proxNo;
		} else { 															//Mostrar preferencial
			imprimeSenha(atualPref);
			clientesPAtendidos++;
			atualPref = atualPref->proxNo;
		}
	}
	return 1;
}

int main(){
	tipoLista filaPF, filaPJ, filaPPF, filaPPJ; //Declara as quatro filas (PF, PJ) x (Preferencial, Não Preferencial) 
    int opcao, opcao2, opcao3, boolFilaUnificada = 0, hora, minuto;
    char nome[20];
    inicializa(&filaPF);
    inicializa(&filaPJ);
    inicializa(&filaPPF);
    inicializa(&filaPPJ);
    do{
        printf("\n      Menu Fila do Banco");
        printf("\n1 - Adicionar pessoa na fila ");
        printf("\n2 - Previsao de Atendimento");
        printf("\n3 - Unificar Filas");
        printf("\n4 - ");
        printf("\n5 - ");
        printf("\n6 - ");
        printf("\n7 - ");
        printf("\n9 - ");
        printf("\n0 - Encerrar o programa ");
        printf("\nDigite sua opcao:");
        scanf("%d",&opcao);
        switch(opcao){
		
        case 1:
			if (!boolFilaUnificada){ //se a fila não estiver unificada, nao pergunte qual o tipo de fila que será inserido
				printf("\nQual fila que deseja inserir?\nPessoa fisica = 1\nPessoa juridica = 2\n");
				scanf("%d",&opcao2);
			}
            printf("\nA pessoa tem prioridade?\n0 - Nao\n1 - Sim\n");
            scanf("%d",&opcao3);
            printf("\nQual nome da pessoa?\n");
            getchar();
			fgets (nome, sizeof(nome), stdin);
            printf("\nEm qual tempo chegou");
            printf("\n(horas):(minutos)\n");
            scanf ("%d:%d", &hora, &minuto);
            
			if (opcao2 == 1 || boolFilaUnificada){ //Escolhe a fila para botar a pessoa
				if (opcao3){
					insereNaFila (&filaPPF, 0, opcao3, nome, hora, minuto);
				}else{
					insereNaFila (&filaPF, 0, opcao3, nome, hora, minuto);
				}
			}else if (opcao2 == 2){
				if (opcao3){
					insereNaFila (&filaPPJ, 1, opcao3, nome, hora, minuto);
				}
				else{
					insereNaFila (&filaPJ, 1, opcao3, nome, hora, minuto);
				}
			}
           break;
            
            
        case 2:
			printf("\nQual fila que deseja analisar?");
			printf("\nPessoa fisica = 1");
			printf("\nPessoa juridica = 2\n");
			scanf ("%d", &opcao2);
			if (opcao2 ==1){
				printf("\nFila do caixa 1:\n");
				previsaoDaFila (&filaPF, &filaPPF);
			}else if (opcao2 ==2){
				printf("\nFila do caixa 2:\n");
				previsaoDaFila (&filaPJ, &filaPPJ);
			}
             break;
        case 3:
			 if(boolFilaUnificada){
				 printf("\nFilas ja unificadas!\n");
				 break;
			 }
			 unificarFilas(&filaPF, &filaPJ);
			 unificarFilas(&filaPPF, &filaPPJ);
			 boolFilaUnificada++;
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
				 getchar();
        }
     printf("\n\n\n\n");
    }while(opcao != 0);
    if (opcao == 0){
		return 0;
	}
}

