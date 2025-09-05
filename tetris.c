#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Definição da peça
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;
} Peca;

// Fila circular
typedef struct {
    Peca dados[TAM_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

// Pilha
typedef struct {
    Peca dados[TAM_PILHA];
    int topo;
} Pilha;

// Protótipos
Peca gerarPeca();
void inicializarFila(Fila* f);
void inicializarPilha(Pilha* p);
void enfileirar(Fila* f, Peca p);
Peca desenfileirar(Fila* f);
int empilhar(Pilha* p, Peca p);
Peca desempilhar(Pilha* p);
void exibirEstado(Fila f, Pilha p);
void trocarTopoComFrente(Fila* f, Pilha* p);
void trocarMultiplas(Fila* f, Pilha* p);

// Variável global para ID das peças
int proximoId = 0;

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    
    srand(time(NULL));
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    // Inicializa fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }
    
    do {
        printf("\n================ ESTADO ATUAL ================\n");
        exibirEstado(fila, pilha);
        printf("\n============== MENU DE AÇÕES =================\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("==============================================\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: // Jogar
                if (fila.total > 0) {
                    Peca jogada = desenfileirar(&fila);
                    printf("Peça [%c %d] jogada!\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca());
                } else {
                    printf("Fila está vazia!\n");
                }
                break;
            case 2: // Reservar
                if (pilha.topo < TAM_PILHA) {
                    if (fila.total > 0) {
                        Peca reservada = desenfileirar(&fila);
                        empilhar(&pilha, reservada);
                        printf("Peça [%c %d] reservada!\n", reservada.nome, reservada.id);
                        enfileirar(&fila, gerarPeca());
                    } else {
                        printf("Fila está vazia!\n");
                    }
                } else {
                    printf("Pilha cheia. Não é possível reservar.\n");
                }
                break;
            case 3: // Usar peça da pilha
                if (pilha.topo > 0) {
                    Peca usada = desempilhar(&pilha);
                    printf("Peça [%c %d] usada!\n", usada.nome, usada.id);
                } else {
                    printf("Pilha vazia.\n");
                }
                break;
            case 4: // Trocar peça da frente com topo da pilha
                trocarTopoComFrente(&fila, &pilha);
                break;
            case 5: // Troca múltipla
                trocarMultiplas(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}

// Função que gera uma nova peça com tipo aleatório
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = proximoId++;
    return nova;
}

// Inicializa fila
void inicializarFila(Fila* f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Inicializa pilha
void inicializarPilha(Pilha* p) {
    p->topo = 0;
}

// Enfileira peça na fila circular
void enfileirar(Fila* f, Peca p) {
    if (f->total < TAM_FILA) {
        f->dados[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_FILA;
        f->total++;
    }
}

// Remove peça da frente da fila
Peca desenfileirar(Fila* f) {
    Peca p = {0};
    if (f->total > 0) {
        p = f->dados[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_FILA;
        f->total--;
    }
    return p;
}

// Empilha peça
int empilhar(Pilha* p, Peca pe) {
    if (p->topo < TAM_PILHA) {
        p->dados[p->topo++] = pe;
        return 1;
    }
    return 0;
}

// Desempilha peça
Peca desempilhar(Pilha* p) {
    Peca pe = {0};
    if (p->topo > 0) {
        pe = p->dados[--p->topo];
    }
    return pe;
}

// Exibe estado atual das estruturas
void exibirEstado(Fila f, Pilha p) {
    printf("Fila de peças\t: ");
    int idx = f.inicio;
    for (int i = 0; i < f.total; i++) {
        printf("[%c %d] ", f.dados[idx].nome, f.dados[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
    printf("\n");
    
    printf("Pilha de reserva (Topo -> base): ");
    for (int i = p.topo - 1; i >= 0; i--) {
        printf("[%c %d] ", p.dados[i].nome, p.dados[i].id);
    }
    printf("\n");
}

// Troca peça da frente da fila com o topo da pilha
void trocarTopoComFrente(Fila* f, Pilha* p) {
    if (f->total > 0 && p->topo > 0) {
        int idx = f->inicio;
        Peca temp = f->dados[idx];
        f->dados[idx] = p->dados[p->topo - 1];
        p->dados[p->topo - 1] = temp;
        printf("Troca entre frente da fila e topo da pilha realizada.\n");
    } else {
        printf("Não é possível trocar (estrutura vazia).\n");
    }
}

// Troca múltipla entre 3 primeiros da fila e 3 da pilha
void trocarMultiplas(Fila* f, Pilha* p) {
    if (f->total >= 3 && p->topo >= 3) {
        for (int i = 0; i < 3; i++) {
            int idx = (f->inicio + i) % TAM_FILA;
            Peca temp = f->dados[idx];
            f->dados[idx] = p->dados[p->topo - 1 - i];
            p->dados[p->topo - 1 - i] = temp;
        }
        printf("Troca múltipla realizada.\n");
    } else {
        printf("Não é possível fazer troca múltipla (mínimo 3 peças em cada).\n");
    }
}