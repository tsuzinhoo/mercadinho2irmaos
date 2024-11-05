#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50

typedef struct Fruta {
    int codigo, quantidade;
    char nome[MAX_NAME];
    float preco;
    struct Fruta *proxima;
} Fruta;

Fruta* cadastrarFruta(Fruta *lista);
void listarFrutas(Fruta *lista);
Fruta* buscarFruta(Fruta *lista, int codigo);
void alterarFruta(Fruta *lista);
Fruta* excluirFruta(Fruta *lista);
void venderFruta(Fruta *lista);
void registrarVenda(int codigo, char *nome, int quantidadeVendida, float preco);

int main() {
    Fruta *lista = NULL;
    int opcao;
    do {
        printf("\n1. Cadastrar Fruta\n2. Listar Frutas\n3. Buscar Fruta\n4. Alterar Fruta\n5. Excluir Fruta\n6. Vender Fruta\n7. Sair\nEscolha: ");
        scanf("%d", &opcao);
        if (opcao == 1) lista = cadastrarFruta(lista);
        else if (opcao == 2) listarFrutas(lista);
        else if (opcao == 3) {
            int codigo;
            printf("Digite o codigo da fruta: ");
            scanf("%d", &codigo);
            Fruta *f = buscarFruta(lista, codigo);
            if (f) printf("Fruta: Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n", f->codigo, f->nome, f->quantidade, f->preco);
            else printf("Fruta nao encontrada.\n");
        } else if (opcao == 4) alterarFruta(lista);
        else if (opcao == 5) lista = excluirFruta(lista);
        else if (opcao == 6) venderFruta(lista);
    } while (opcao != 7);

    while (lista) {
        Fruta *aux = lista;
        lista = lista->proxima;
        free(aux);
    }
    return 0;
}

Fruta* cadastrarFruta(Fruta *lista) {
    Fruta *nova = (Fruta*)malloc(sizeof(Fruta));
    if (!nova) { printf("Erro de memoria!\n"); return lista; }
    printf("Digite o codigo: "); scanf("%d", &nova->codigo);
    if (buscarFruta(lista, nova->codigo)) { printf("Codigo ja existe!\n"); free(nova); return lista; }
    printf("Digite o nome: "); scanf("%s", nova->nome);
    printf("Digite a quantidade: "); scanf("%d", &nova->quantidade);
    printf("Digite o preco: "); scanf("%f", &nova->preco);
    nova->proxima = lista;
    printf("Fruta cadastrada com sucesso!\n");
    return nova;
}

void listarFrutas(Fruta *lista) {
    if (!lista) { printf("Nenhuma fruta cadastrada.\n"); return; }
    for (Fruta *aux = lista; aux; aux = aux->proxima)
        printf("Codigo: %d, Nome: %s, Quantidade: %d, Preco: %.2f\n", aux->codigo, aux->nome, aux->quantidade, aux->preco);
}

Fruta* buscarFruta(Fruta *lista, int codigo) {
    for (Fruta *aux = lista; aux; aux = aux->proxima)
        if (aux->codigo == codigo) return aux;
    return NULL;
}

void alterarFruta(Fruta *lista) {
    int codigo;
    printf("Digite o codigo: "); scanf("%d", &codigo);
    Fruta *f = buscarFruta(lista, codigo);
    if (!f) { printf("Fruta nao encontrada.\n"); return; }
    printf("Digite o novo nome: "); scanf("%s", f->nome);
    printf("Digite a nova quantidade: "); scanf("%d", &f->quantidade);
    printf("Digite o novo preco: "); scanf("%f", &f->preco);
    printf("Fruta alterada com sucesso!\n");
}

Fruta* excluirFruta(Fruta *lista) {
    int codigo;
    printf("Digite o codigo: "); scanf("%d", &codigo);
    Fruta *anterior = NULL, *atual = lista;
    while (atual && atual->codigo != codigo) { anterior = atual; atual = atual->proxima; }
    if (!atual) { printf("Fruta nao encontrada.\n"); return lista; }
    if (atual->quantidade > 0) { printf("Nao pode excluir com estoque.\n"); return lista; }
    if (!anterior) lista = atual->proxima;
    else anterior->proxima = atual->proxima;
    free(atual);
    printf("Fruta excluida com sucesso!\n");
    return lista;
}

void venderFruta(Fruta *lista) {
    int codigo, quantidadeVendida;
    printf("Digite o codigo: "); scanf("%d", &codigo);
    Fruta *f = buscarFruta(lista, codigo);
    if (!f) { printf("Fruta nao encontrada.\n"); return; }
    printf("Digite a quantidade: "); scanf("%d", &quantidadeVendida);
    if (quantidadeVendida > f->quantidade) { printf("Quantidade insuficiente.\n"); return; }
    f->quantidade -= quantidadeVendida;
    registrarVenda(f->codigo, f->nome, quantidadeVendida, f->preco);
    printf("Venda realizada com sucesso!\n");
}

void registrarVenda(int codigo, char *nome, int quantidadeVendida, float preco) {
    FILE *file = fopen("vendas.txt", "a");
    if (!file) { printf("Erro ao abrir arquivo.\n"); return; }
    fprintf(file, "Codigo: %d, Nome: %s, Quantidade Vendida: %d, Preco: %.2f, Total: %.2f\n", codigo, nome, quantidadeVendida, preco, quantidadeVendida * preco);
    fclose(file);
    printf("Venda registrada no arquivo vendas.txt.\n");
}
