#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Definição da peça
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;
} peca;

// Fila circular
typedef struct {
    peca dados[TAM_FILA];
    int inicio;
    int fim;
    int total;
} fila;

// Pilha
typedef struct {
    peca dados[TAM_PILHA];
    int topo;
} pilha;

// Protótipos
peca gerarpeca();
void inicializarfila(fila* f);
void inicializarpilha(pilha* p);
void enfileirar(fila* f, peca p);
peca desenfileirar(fila* f);
int empilhar(pilha* p, peca p);
peca desempilhar(pilha* p);
void exibirEstado(fila f, pilha p);
void trocarTopoComFrente(fila* f, pilha* p);
void trocarMultiplas(fila* f, pilha* p);

// Variável global para ID das peças
int proximoId = 0;

int main() {
    fila f;
    pilha p;
    int opcao;

    srand(time(NULL));

    inicializarfila(&f);
    inicializarpilha(&p);

    // Inicializa fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&f, gerarpeca());
    }

    do {
        printf("\n================ ESTADO ATUAL ================\n");
        exibirEstado(f, p);
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
                if (f.total > 0) {
                    peca jogada = desenfileirar(&f);
                    printf("Peça [%c %d] jogada!\n", jogada.nome, jogada.id);
                    enfileirar(&f, gerarpeca());
                } else {
                    printf("Fila está vazia!\n");
                }
                break;

            case 2: // Reservar
                if (f.total > 0) {
                    peca reservada = desenfileirar(&f);
                    if (empilhar(&p, reservada)) {
                        printf("Peça [%c %d] reservada!\n", reservada.nome, reservada.id);
                        enfileirar(&f, gerarpeca());
                    } else {
                        printf("Pilha cheia. Não é possível reservar.\n");
                        // Pode-se opcionalmente devolver à fila, mas omitido aqui
                    }
                } else {
                    printf("Fila está vazia!\n");
                }
                break;

            case 3: // Usar peça da pilha
                if (p.topo > 0) {
                    peca usada = desempilhar(&p);
                    printf("Peça [%c %d] usada!\n", usada.nome, usada.id);
                } else {
                    printf("Pilha vazia.\n");
                }
                break;

            case 4: // Trocar frente da fila com topo da pilha
                trocarTopoComFrente(&f, &p);
                break;

            case 5: // Troca múltipla
                trocarMultiplas(&f, &p);
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

// Gera nova peça com tipo aleatório
peca gerarpeca() {
    peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = proximoId++;
    return nova;
}

// Inicializa fila
void inicializarfila(fila* f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Inicializa pilha
void inicializarpilha(pilha* p) {
    p->topo = 0;
}

// Enfileira peça
void enfileirar(fila* f, peca p) {
    if (f->total < TAM_FILA) {
        f->dados[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_FILA;
        f->total++;
    }
}

// Desenfileira peça
peca desenfileirar(fila* f) {
    peca p = {0};
    if (f->total > 0) {
        p = f->dados[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_FILA;
        f->total--;
    }
    return p;
}

// Empilha peça
int empilhar(pilha* p, peca pe) {
    if (p->topo < TAM_PILHA) {
        p->dados[p->topo++] = pe;
        return 1;
    }
    return 0;
}

// Desempilha peça
peca desempilhar(pilha* p) {
    peca pe = {0};
    if (p->topo > 0) {
        pe = p->dados[--p->topo];
    }
    return pe;
}

// Exibe estado atual das estruturas
void exibirEstado(fila f, pilha p) {
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

// Troca topo da pilha com frente da fila
void trocarTopoComFrente(fila* f, pilha* p) {
    if (f->total > 0 && p->topo > 0) {
        int idx = f->inicio;
        peca temp = f->dados[idx];
        f->dados[idx] = p->dados[p->topo - 1];
        p->dados[p->topo - 1] = temp;
        printf("Troca entre frente da fila e topo da pilha realizada.\n");
    } else {
        printf("Não é possível trocar (estrutura vazia).\n");
    }
}

// Troca múltipla entre fila e pilha
void trocarMultiplas(fila* f, pilha* p) {
    if (f->total >= 3 && p->topo >= 3) {
        for (int i = 0; i < 3; i++) {
            int idx = (f->inicio + i) % TAM_FILA;
            peca temp = f->dados[idx];
            f->dados[idx] = p->dados[p->topo - 1 - i];
            p->dados[p->topo - 1 - i] = temp;
        }
        printf("Troca múltipla realizada.\n");
    } else {
        printf("Não é possível fazer troca múltipla (mínimo 3 peças em cada).\n");
    }
}