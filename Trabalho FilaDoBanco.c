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


// Remove o primeiro nó da fila e retorna-o
tipoNo* removeDaFila(tipoLista *listaEnc) {
    if (listaEnc->inicio == NULL) {
        return NULL; // Fila vazia
    }
    tipoNo *noRemovido = listaEnc->inicio;
    listaEnc->inicio = listaEnc->inicio->proxNo;
    if (listaEnc->inicio == NULL) { // Se a fila ficou vazia após a remoção
        listaEnc->fim = NULL;
    }
    listaEnc->quant--;
    return noRemovido; // Retorna o nó removido para que a memória possa ser liberada
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

int previsaoClienteEspecifico(tipoLista *listaNaoPreferencial, tipoLista *listaPreferencial, char *nomeCliente) {
    tipoNo *atualNP = listaNaoPreferencial->inicio;
    tipoNo *atualP = listaPreferencial->inicio;
    int clientesAntes = 0;
    int atendidosP = 0;

	// Percorre as duas filas alternando o atendimento
    while (atualNP != NULL || atualP != NULL) {
        tipoNo *selecionado = NULL;

		// Atende preferencial até 3 vezes seguidas ou enquanto houver
        if ((atendidosP < 3 && atualP != NULL) || atualNP == NULL) {
            // Atendimento preferencial
            if (atualP != NULL) {
                selecionado = atualP;
                atualP = atualP->proxNo;
                atendidosP++;
            }
        } else {
            // Atendimento não preferencial
            if (atualNP != NULL) {
                selecionado = atualNP;
                atualNP = atualNP->proxNo;
                atendidosP = 0;
            }
        }

		// Verifica se o cliente atual é o buscado
        if (selecionado != NULL) {
            if (strcmp(selecionado->nome, nomeCliente) == 0) {
                printf("\nO cliente %s está na posição %d da fila.\n", selecionado->nome, clientesAntes+1);
                return 1;
            }
            clientesAntes++;
        }
    }
	// Caso o cliente não seja encontrado
    printf("\nCliente não encontrado na fila.\n");
    return 0;
}

void atenderCliente(tipoLista *listaNaoPreferencial, tipoLista *listaPreferencial, int caixa) {
    tipoNo *clienteAtendido = NULL;
    static int clientesPAtendidosSequencia = 0; // Mantém o contador de preferenciais atendidos em sequência
	// Static references
    if (listaNaoPreferencial->inicio == NULL && listaPreferencial->inicio == NULL) {
        printf("\nAmbas as filas (Caixa %d) estao vazias. Nenhum cliente para atender.\n", caixa);
        return;
    }

    // Lógica para determinar qual cliente atender (3 preferenciais, depois 1 não preferencial)
    if ((clientesPAtendidosSequencia < 3 && listaPreferencial->inicio != NULL) || listaNaoPreferencial->inicio == NULL) {
        // Atende cliente preferencial
        if (listaPreferencial->inicio != NULL) {
            clienteAtendido = removeDaFila(listaPreferencial);
            clientesPAtendidosSequencia++;
        }
    } else {
        // Atende cliente não preferencial
        if (listaNaoPreferencial->inicio != NULL) {
            clienteAtendido = removeDaFila(listaNaoPreferencial);
            clientesPAtendidosSequencia = 0; // Reinicia o contador
        }
    }

    if (clienteAtendido != NULL) {
        printf("\nChamando o proximo cliente para o CAIXA %d:\n", caixa);
        imprimeSenha(clienteAtendido);
        free(clienteAtendido); // Libera a memória do nó do cliente atendido
    } else {
        printf("\nNao ha clientes para atender no Caixa %d no momento, seguindo as regras de prioridade.\n", caixa);
    }
}

int main() {
    tipoLista filaPF, filaPJ, filaPPF, filaPPJ; // Declara as quatro filas (PF, PJ) x (Preferencial, Não Preferencial)
    int opcao, opcao2, opcao3, boolFilaUnificada = 0, hora, minuto;
    char nome[40]; // Aumentado para 40 para corresponder à struct tipoNo
    
    inicializa(&filaPF);
    inicializa(&filaPJ);
    inicializa(&filaPPF);
    inicializa(&filaPPJ);

    do {
        printf("\n\n   --- Menu Fila do Banco ---\n");
        printf("1 - Adicionar pessoa na fila\n");
        printf("2 - Previsao de Atendimento (Por tipo de fila)\n");
        printf("3 - Unificar Filas (PF e PJ em uma unica)\n");
        printf("4 - Previsao de atendimento por nome\n");
        printf("5 - Atender proximo cliente\n");
        printf("0 - Encerrar o programa\n");
        printf("----------------------------------\n");
        printf("Digite sua opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Adicionar pessoa na fila
                if (!boolFilaUnificada) {
                    printf("\nQual fila deseja inserir?\n1 - Pessoa Fisica\n2 - Pessoa Juridica\nDigite sua opcao: ");
                    scanf("%d", &opcao2);
                } else {
                    printf("\nAs filas estao unificadas. O cliente sera adicionado a fila geral.\n");
                    opcao2 = 1; // Força para a fila unificada (PF é a que contém a unificada)
                }
                printf("\nA pessoa tem prioridade?\n0 - Nao\n1 - Sim\nDigite sua opcao: ");
                scanf("%d", &opcao3);
                printf("\nQual o nome da pessoa? (Max 39 caracteres)\n");
                getchar(); // Consome o newline pendente
                fgets(nome, sizeof(nome), stdin);
                
                printf("\nEm qual horario chegou (HH:MM)?\n");
                scanf("%d:%d", &hora, &minuto);

                if (opcao2 == 1 || boolFilaUnificada) { // Escolhe a fila para colocar a pessoa
                    if (opcao3) { // Preferencial
                        insereNaFila(&filaPPF, 0, opcao3, nome, hora, minuto);
                    } else { // Não Preferencial
                        insereNaFila(&filaPF, 0, opcao3, nome, hora, minuto);
                    }
                } else if (opcao2 == 2) { // Pessoa Jurídica
                    if (opcao3) { // Preferencial
                        insereNaFila(&filaPPJ, 1, opcao3, nome, hora, minuto);
                    } else { // Não Preferencial
                        insereNaFila(&filaPJ, 1, opcao3, nome, hora, minuto);
                    }
                }
                printf("\nCliente adicionado com sucesso!\n");
                break;

            case 2: // Previsao de Atendimento (Por tipo de fila)
                printf("\nQual fila deseja analisar a previsao?\n1 - Pessoa Fisica (Caixa 1)\n2 - Pessoa Juridica (Caixa 2)\nDigite sua opcao: ");
                scanf("%d", &opcao2);
                if (opcao2 == 1) {
                    printf("\n--- Previsao Fila Caixa 1 (Pessoa Fisica) ---\n");
                    previsaoDaFila(&filaPF, &filaPPF);
                } else if (opcao2 == 2) {
                    printf("\n--- Previsao Fila Caixa 2 (Pessoa Juridica) ---\n");
                    previsaoDaFila(&filaPJ, &filaPPJ);
                } else {
                    printf("\nOpcao de fila invalida!\n");
                }
                break;

            case 3: // Unificar Filas
                if (boolFilaUnificada) {
                    printf("\nFilas ja unificadas!\n");
                    break;
                }
                printf("\nUnificando filas Pessoa Fisica e Pessoa Juridica...\n");
                unificarFilas(&filaPF, &filaPJ); // Unifica as não preferenciais
                unificarFilas(&filaPPF, &filaPPJ); // Unifica as preferenciais
                boolFilaUnificada++;
                printf("\nFilas unificadas com sucesso!\n");
                break;

            case 4: // Previsao de atendimento por nome
                printf("\nDigite o nome do cliente para buscar a previsao: (Max 39 caracteres)\n");
                getchar(); // Consome o newline pendente
                fgets(nome, sizeof(nome), stdin);

                printf("\n--- Buscando cliente na Fila Caixa 1 (Pessoa Fisica) ---\n");
                previsaoClienteEspecifico(&filaPF, &filaPPF, nome);

                printf("\n--- Buscando cliente na Fila Caixa 2 (Pessoa Juridica) ---\n");
                previsaoClienteEspecifico(&filaPJ, &filaPPJ, nome);
                break;

            case 5: // Atender proximo cliente
                printf("\nDe qual tipo de fila deseja atender o cliente?\n1 - Pessoa Fisica (Caixa 1)\n2 - Pessoa Juridica (Caixa 2)\nDigite sua opcao: ");
                scanf("%d", &opcao2);
                if (opcao2 == 1) {
                    atenderCliente(&filaPF, &filaPPF, 1); // Atende cliente para o Caixa 1 (PF)
                } else if (opcao2 == 2) {
                    atenderCliente(&filaPJ, &filaPPJ, 2); // Atende cliente para o Caixa 2 (PJ)
                } else {
                    printf("\nOpcao de fila invalida para atendimento!\n");
                }
                break;
            
            case 0: // Encerrar o programa
                printf("\nEncerrando programa. Ate mais!\n");
                break;

            default: // Opção inválida
                printf("\nOpcao invalida! Por favor, digite uma opcao valida do menu.\n");
                // getchar();
                break;
        }
    } while (opcao != 0);

    // Liberação de memória 
    tipoNo *current, *next;
    current = filaPF.inicio;
    while(current != NULL){
        next = current->proxNo;
        free(current);
        current = next;
    }
    current = filaPJ.inicio;
    while(current != NULL){
        next = current->proxNo;
        free(current);
        current = next;
    }
    current = filaPPF.inicio;
    while(current != NULL){
        next = current->proxNo;
        free(current);
        current = next;
    }
    current = filaPPJ.inicio;
    while(current != NULL){
        next = current->proxNo;
        free(current);
        current = next;
    }

    return 0;
}
