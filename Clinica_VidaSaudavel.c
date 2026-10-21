#include <stdio.h>
#include <stdlib.h>
#include "lista_com_cabeca.h" // Lista de Pacientes Cadastrados
#include "lista_circular.h"   // Escala Rotativa de Médicos
#include "fila.h"             // Sala de Espera
#include "pilha.h"          // Histórico de Atendimentos

// Função para limpar o terminal e deixar o menu limpo
void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Função para pausar a tela até o usuário pressionar Enter
void pausar() {
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
    getchar();
}

void exibir_menu() {
    printf("=====================================================\n");
    printf("           CLÍNICA VIDASAUDÁVEL - SISTEMA            \n");
    printf("=====================================================\n");
    printf("1. Cadastrar Novo Paciente\n");
    printf("2. Encaminhar Paciente para Espera\n");
    printf("3. Atender Paciente\n");
    printf("4. Desfazer Último Atendimento\n");
    printf("5. Gerenciar Escala de Médicos\n");
    printf("6. Visualizar Fila de Espera Detalhada\n");
    printf("7. Visualizar Histórico de Atendimentos\n");
    printf("8. Listar Todos os Pacientes Cadastrados\n");
    printf("9. Exibir Status Geral da Clínica\n");
    printf("0. Sair\n");
    printf("-----------------------------------------------------\n");
    printf("Escolha uma opção: ");
}

int main() {
    // Inicialização das 4 estruturas
    No *cadastro = criar_lista();        // Lista com cabeça
    NoCircular *medicos = criar_lista_circular();         // Lista circular
    Fila *espera = fila_criar();         // Fila FIFO
    Pilha *historico = pilha_criar();   // Pilha LIFO

    int opcao, id,sub_opcao;

    // Cadastro inicial de médicos na escala
    adicionar_final_circular(&medicos, 101); // Médico ID 101
    adicionar_final_circular(&medicos, 102); // Médico ID 102
    adicionar_final_circular(&medicos, 103); // Médico ID 103

    do {
        limpar_tela();
        exibir_menu();
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
            case 1:
                printf("\n--- CADASTRO DE PACIENTE ---\n");
                printf("Digite o ID do paciente para cadastro: ");
                scanf("%d", &id);
                adicionar_final(cadastro, id);
                printf("Sucesso: Paciente %d cadastrado!\n", id);
                pausar();
                break;

            case 2:
                printf("\n--- ENCAMINHAR PARA ESPERA ---\n");
                printf("Digite o ID do paciente para entrar na espera: ");
                scanf("%d", &id);
                if (buscar_valor(cadastro, id)) {
                    fila_adicionar(&espera, id);
                    printf("Sucesso: Paciente %d entrou na Fila de Espera.\n", id);
                } else {
                    printf("Erro: Paciente %d não cadastrado no sistema!\n", id);
                }
                pausar();
                break;

            case 3:
                printf("\n--- ATENDIMENTO DE PACIENTE ---\n");
                if (fila_tamanho(espera) > 0 && tamanho_lista_circular(medicos) > 0) {
                    int p_id = fila_topo(espera);
                    fila_remover(&espera);

                    // Pega o ID do médico de plantão no momento (primeiro elemento da lista circular)
                    int m_id = medicos->prox->dado; 

                    // Avança a lista circular para o próximo médico do turno
                    medicos = medicos->prox; 

                    // Registra a consulta na pilha do histórico
                    pilha_adicionar(&historico, p_id);

                    printf("Sucesso: Paciente %d ATENDIDO pelo Médico %d!\n", p_id, m_id);
                } else {
                    printf("Aviso: Não há pacientes na fila ou médicos de plantão!\n");
                }
                pausar();
                break;

            case 4:
                printf("\n--- DESFAZER ATENDIMENTO ---\n");
                if (pilha_tamanho(historico) > 0) {
                    int p_id = pilha_topo(historico);
                    pilha_remover(&historico);
                    fila_adicionar(&espera, p_id);
                    printf("Sucesso: Atendimento do paciente %d desfeito. Retornou para a Fila.\n", p_id);
                } else {
                    printf("Aviso: Histórico vazio! Nada para desfazer.\n");
                }
                pausar();
                break;

            case 5:
                printf("\n--- GERENCIAR ESCALA DE MÉDICOS ---\n");
                printf("1. Adicionar Médico à Escala\n");
                printf("2. Remover Médico da Escala\n");
                printf("3. Visualizar Escala Atual\n");
                printf("Opção: ");
                scanf("%d", &sub_opcao);

                if (sub_opcao == 1) {
                    printf("ID do novo Médico: ");
                    scanf("%d", &id);
                    adicionar_final_circular(&medicos, id);
                    printf("Médico %d adicionado à escala!\n", id);
                } else if (sub_opcao == 2) {
                    printf("ID do Médico a remover: ");
                    scanf("%d", &id);
                    remover_valor_circular(&medicos, id);
                    printf("Médico %d removido da escala!\n", id);
                } else if (sub_opcao == 3) {
                    printf("\nEscala Circular Atual de Médicos:\n");
                    imprimir_lista_circular(medicos);
                } else {
                    printf("Opção inválida!\n");
                }
                pausar();
                break;

            case 6:
                printf("\n--- DETALHES DA FILA DE ESPERA ---\n");
                printf("Total na Fila: %d\n", fila_tamanho(espera));
                if (fila_tamanho(espera) > 0) {
                    printf("Fila de Espera (Início -> Fim):\n");
                    imprimir_lista(espera->L);
                } else {
                    printf("A Fila de Espera está vazia no momento.\n");
                }
                pausar();
                break;

            case 7:
                printf("\n--- HISTÓRICO DE ATENDIMENTOS ---\n");
                printf("Total de Atendimentos Realizados: %d\n", pilha_tamanho(historico));
                if (pilha_tamanho(historico) > 0) {
                    printf("Histórico (Início -> Fim):\n");
                    imprimir_lista(historico->L);
                } else {
                    printf("O Histórico de atendimentos está vazio.\n");
                }
                pausar();
                break;

            case 8:
                printf("\n--- PACIENTES CADASTRADOS ---\n");
                printf("Total de Pacientes Cadastrados: %d\n", tamanho_lista(cadastro));
                printf("Lista de Pacientes:\n");
                imprimir_lista(cadastro);
                pausar();
                break;

            case 9:
                printf("\n--- STATUS GERAL DA CLÍNICA ---\n");
                printf("Total de Cadastrados:       %d\n", tamanho_lista(cadastro));
                printf("Médicos na Escala:         %d\n", tamanho_lista_circular(medicos));
                printf("Aguardando na Fila:        %d\n", fila_tamanho(espera));
                printf("Atendimentos no Histórico: %d\n", pilha_tamanho(historico));
                pausar();
                break;

            case 0:
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
                pausar();
        }
    } while (opcao != 0);

    // Liberação final de memória
    destruir_lista(&cadastro);
    destruir_lista_circular(&medicos);
    fila_destruir(&espera);
    pilha_destruir(&historico);

    printf("Memória totalmente liberada.\n");
    return 0;
}