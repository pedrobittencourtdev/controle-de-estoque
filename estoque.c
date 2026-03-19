#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Struct para auxiliar nas datas

typedef struct{
    int dia;
    int mes;
    int ano;
}Data;

// struct principal dos produtos

typedef struct Produto{
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
    Data validade;     // <- struct dentro da struct
    Data dataEntrada;  // <- data que chegou no estoque
    Data dataSaida;    // <- datd que saiu do estoque
    struct Produto *proximo;
} Produto;

int lerInteiro(const char *prompt) {
    char buffer[20];
    int numero;
    int valido;

    do {
        printf("%s", prompt);
        fgets(buffer, 20, stdin);

        // tenta converter — se não conseguir, pede de novo
        valido = sscanf(buffer, "%d", &numero);

        if (valido != 1) {
            printf("  [!] Digite apenas numeros!\n");
        }

    } while (valido != 1);

    return numero;
}

int codigoExiste(Produto *head, int codigo) {
    Produto *atual = head;
    while (atual != NULL) {
        if (atual->codigo == codigo) return 1; // já existe!
        atual = atual->proximo;
    }
    return 0; // não existe
}

Produto* criarProduto(Produto *head){

    char buffer[10];
    Produto *novo = (Produto*) malloc(sizeof(Produto));

    if (novo== NULL){
        printf("\nErro ao alocar memória\n!");
        return NULL;
    }
    printf("\n|======================|\n");
    printf("|  CADASTRE O PRODUTO  |");
    printf("\n|======================|\n");
    printf("\n");
    novo->codigo = lerInteiro("Código: ");

    // verifica duplicata em loop até digitar um código válido
    while (codigoExiste(head, novo->codigo)) {
    printf("  [!] Codigo %d ja existe! Digite outro.\n", novo->codigo);
    novo->codigo = lerInteiro("Codigo: ");
    }
    

    printf("\n");

    printf("Nome: ");
    fgets(novo->nome, 30, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';

    printf("\n");

    printf("Preço: R$ ");
    fgets(buffer, 10, stdin);
    novo->preco =atof(buffer);

    printf("\n");

    
    novo->quantidade = lerInteiro("Quantidade: ");
    
    printf("\n");

    novo->validade.dia = lerInteiro("Dia de validade: ");
   
    novo->validade.mes = lerInteiro("Mês de validade: ");

    novo->validade.ano = lerInteiro("Ano de validade: ");

    printf("\n");

    novo->dataEntrada.dia = lerInteiro("Dia de entrada: ");
    
    novo->dataEntrada.mes = lerInteiro("Mês de entrada: ");
    
    novo->dataEntrada.ano = lerInteiro("Ano de entrada: ");
    
    // Produto ainda não saiu
    novo->dataSaida.dia = 0;
    novo->dataSaida.mes = 0;
    novo->dataSaida.ano = 0;
    

    novo->proximo = NULL; // inicializa o proximo como NULL
    //retorna o ponteiro
    return novo;
}

void inserirProduto(Produto **head, Produto *novo){
    novo->proximo = *head;
    *head = novo;
}

void exibirProdutos(Produto *head){

    Produto *atual = head;

    if (head == NULL) {
    printf("\n  [!] Nenhum produto cadastrado!\n");
    return;
}

    // cabeçalho
    printf("\n+--------+------------------------------+------------+------------+----------------+\n");
    printf("| %-6s | %-28s | %-10s | %-10s | %-14s |\n",
           "COD", "NOME", "PRECO", "QTDE", "VALIDADE");
    printf("+--------+------------------------------+------------+------------+----------------+\n");


    while(atual != NULL){
        printf("| %-6d | %-28s | R$%-8.2f | %-10d | %02d/%02d/%04d     |\n",
            atual->codigo,atual->nome,
            atual->preco,
            atual->quantidade,
            atual->validade.dia,
            atual->validade.mes,
            atual->validade.ano);
        atual=atual->proximo;
    }
}

void removerProduto(Produto **head, int codigo){

    Produto *atual =  *head;
    Produto *anterior = NULL;

    if (*head == NULL) {
    printf("\n  [!] Nenhum produto cadastrado!\n");
    return;
}

    // percorre a lista procurando o código
    while (atual != NULL && atual->codigo != codigo) {
        anterior = atual;
        atual    = atual->proximo;
    }

    // se não encontrou
    if (atual == NULL) {
        printf("\nProduto com codigo %d nao encontrado!\n", codigo);
        return;
    }

    // se é o primeiro (anterior == NULL)
    if (anterior == NULL) {
        *head = atual->proximo;
    } else {
        // pula o atual — conecta anterior ao próximo
        anterior->proximo = atual->proximo;
    }

    // pede a data de saída
    char buffer[10];
    atual->dataSaida.dia = lerInteiro("Dia de saida: ");
    atual->dataSaida.mes = lerInteiro("Mes de saida: ");
    atual->dataSaida.ano = lerInteiro("Ano de saida: ");

    // exibe o log antes de liberar
    printf("\n--- REGISTRO DE SAIDA ---\n");
    printf("Produto : %s\n", atual->nome);
    printf("Entrada : %02d/%02d/%04d\n",
        atual->dataEntrada.dia,
        atual->dataEntrada.mes,
        atual->dataEntrada.ano);
    printf("Saida   : %02d/%02d/%04d\n",
       atual->dataSaida.dia,
       atual->dataSaida.mes,
       atual->dataSaida.ano);
    printf("-------------------------\n");



    free(atual); // libera a memória!
    printf("\nProduto removido com sucesso!\n");

}

void exibirOrdenadoPorNome(Produto *head) {

    if (head == NULL) {
        printf("Nenhum produto cadastrado!\n");
        return;
    }

    // 1. conta quantos produtos tem
    int n = 0;
    Produto *atual = head;
    while (atual != NULL) {
        n++;
        atual = atual->proximo;
    }

    // 2. calloc — cria array de n ponteiros para Produto
    Produto **arr = calloc(n, sizeof(Produto*));
    if (arr == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    // 3. copia os ponteiros da lista para o array
    atual = head;
    for (int i = 0; i < n; i++) {
        arr[i] = atual;
        atual  = atual->proximo;
    }

    // 4. bubble sort por nome no array
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (strcmp(arr[j]->nome, arr[j+1]->nome) > 0) {
                Produto *temp = arr[j];
                arr[j]        = arr[j+1];
                arr[j+1]      = temp;
            }
        }
    }

    // 5. exibe o array ordenado
    printf("\n--- Produtos ordenados por nome ---\n");

    printf("\n+--------+------------------------------+------------+------------+----------------+\n");
    printf("| %-6s | %-28s | %-10s | %-10s | %-14s |\n",
           "COD", "NOME", "PRECO", "QTDE", "VALIDADE");
    printf("+--------+------------------------------+------------+------------+----------------+\n");

    for (int i = 0; i < n; i++) {
        printf("| %-6d | %-28s | R$%-8.2f | %-10d | %02d/%02d/%04d     |\n",
               arr[i]->codigo,
               arr[i]->nome,
               arr[i]->preco,
               arr[i]->quantidade,
               arr[i]->validade.dia,
               arr[i]->validade.mes,
               arr[i]->validade.ano);
    }
    printf("+--------+------------------------------+------------+------------+----------------+\n");

    // 6. libera o array temporário
    free(arr);
}

int buscaBinariaCodigo(Produto **arr, int n, int codigo) {
    int inicio = 0, fim = n - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        if (arr[meio]->codigo == codigo)   return meio;
        else if (arr[meio]->codigo < codigo) inicio = meio + 1;
        else                                  fim    = meio - 1;
    }
    return -1;
}

void atualizarProduto(Produto *head, int codigo) {

    Produto *atual = head;

    if (head == NULL) {
    printf("\n  [!] Nenhum produto cadastrado!\n");
    return;
}

    // percorre procurando o código
    while (atual != NULL && atual->codigo != codigo) {
        atual = atual->proximo;
    }

    // se não encontrou
    if (atual == NULL) {
        printf("Produto com codigo %d nao encontrado!\n", codigo);
        return;
    }

    // encontrou! exibe o que tem hoje
    printf("\nProduto encontrado: %s\n", atual->nome);
    printf("O que deseja atualizar?\n");
    printf("1. Nome\n");
    printf("2. Preco\n");
    printf("3. Quantidade\n");

    int opcao = lerInteiro("Escolha: ");

    switch(opcao) {
        case 1:
            printf("Novo nome: ");
            fgets(atual->nome, 50, stdin);
            atual->nome[strcspn(atual->nome, "\n")] = '\0';
            break;
        case 2: {
            char buffer[10];
            printf("Novo preco: ");
            fgets(buffer, 10, stdin);
            atual->preco = atof(buffer);
            break;
        }
        case 3:
            atual->quantidade = lerInteiro("Nova quantidade: ");
            break;
        default:
            printf("Opcao invalida!\n");
    }

    printf("Produto atualizado com sucesso!\n");
}

int estaVencido(Data validade, Data hoje) {
    if (validade.ano < hoje.ano) return 1;
    if (validade.ano == hoje.ano && validade.mes < hoje.mes) return 1;
    if (validade.ano == hoje.ano && validade.mes == hoje.mes
        && validade.dia < hoje.dia) return 1;
    return 0;
}

void alertarEstoque(Produto *head, int limiteEstoque) {

    if (head == NULL) {
        printf("\n  [!] Nenhum produto cadastrado!\n");
        return;
    }

    // pede a data de hoje para comparar
    printf("\n=== VERIFICACAO DE ALERTAS ===\n");
    Data hoje;
    hoje.dia = lerInteiro("Dia de hoje: ");
    hoje.mes = lerInteiro("Mes de hoje: ");
    hoje.ano = lerInteiro("Ano de hoje: ");

    int achouAlerta = 0;
    Produto *atual = head;

    while (atual != NULL) {

        // verifica se está vencido
        if (estaVencido(atual->validade, hoje)) {
            printf("\n  [VENCIDO]  Codigo: %d | %s | Validade: %02d/%02d/%04d\n",
                   atual->codigo, atual->nome,
                   atual->validade.dia,
                   atual->validade.mes,
                   atual->validade.ano);
            achouAlerta = 1;
        }

        // verifica estoque baixo
        if (atual->quantidade <= limiteEstoque) {
            printf("\n  [ESTOQUE BAIXO]  Codigo: %d | %s | Qtde: %d\n",
                   atual->codigo, atual->nome, atual->quantidade);
            achouAlerta = 1;
        }

        atual = atual->proximo;
    }

    if (achouAlerta == 0) {
        printf("\n  Nenhum alerta encontrado!\n");
    }
}

void relatorioEstoque(Produto *head) {

    if (head == NULL) {
        printf("\n  [!] Nenhum produto cadastrado!\n");
        return;
    }

    // variáveis para acumular os dados
    int totalProdutos  = 0;
    int totalItens     = 0;
    float valorTotal   = 0;
    int vencidos       = 0;
    int estoqueBaixo   = 0;

    // para achar o mais caro e mais barato
    Produto *maisCaro   = head;
    Produto *maisBarato = head;

    // pede a data de hoje para verificar vencidos
    Data hoje;
    hoje.dia = lerInteiro("Dia de hoje: ");
    hoje.mes = lerInteiro("Mes de hoje: ");
    hoje.ano = lerInteiro("Ano de hoje: ");

    Produto *atual = head;
    while (atual != NULL) {

        totalProdutos++;
        totalItens  += atual->quantidade;
        valorTotal  += atual->preco * atual->quantidade;

        if (atual->preco > maisCaro->preco)   maisCaro   = atual;
        if (atual->preco < maisBarato->preco) maisBarato = atual;
        if (estaVencido(atual->validade, hoje)) vencidos++;
        if (atual->quantidade <= 5)           estoqueBaixo++;

        atual = atual->proximo;
    }

    // exibe o relatório
    printf("\n========= RELATORIO DE ESTOQUE =========\n");
    printf("  Total de produtos  : %d\n",    totalProdutos);
    printf("  Total de itens     : %d\n",    totalItens);
    printf("  Valor total        : R$ %.2f\n", valorTotal);
    printf("  Produto mais caro  : %s - R$ %.2f\n",
           maisCaro->nome, maisCaro->preco);
    printf("  Produto mais barato: %s - R$ %.2f\n",
           maisBarato->nome, maisBarato->preco);
    printf("  Vencidos           : %d\n",    vencidos);
    printf("  Estoque baixo      : %d\n",    estoqueBaixo);
    printf("=========================================\n");
}

void salvarEstoque(Produto *head) {

    FILE *arquivo = fopen("estoque.txt", "w"); // w = sobrescreve tudo
    if (arquivo == NULL) {
        printf("  [!] Erro ao salvar arquivo!\n");
        return;
    }

    Produto *atual = head;
    int total = 0;

    while (atual != NULL) {
        fprintf(arquivo, "%d;%s;%.2f;%d;",
                atual->codigo, atual->nome,
                atual->preco, atual->quantidade);

        // datas: validade, entrada, saida
        fprintf(arquivo, "%d;%d;%d;",
                atual->validade.dia,
                atual->validade.mes,
                atual->validade.ano);

        fprintf(arquivo, "%d;%d;%d;",
                atual->dataEntrada.dia,
                atual->dataEntrada.mes,
                atual->dataEntrada.ano);

        fprintf(arquivo, "%d;%d;%d\n",
                atual->dataSaida.dia,
                atual->dataSaida.mes,
                atual->dataSaida.ano);

        total++;
        atual = atual->proximo;
    }

    fclose(arquivo); // fecha o arquivo!
    printf("  [OK] %d produto(s) salvo(s) em estoque.txt\n", total);
}

void carregarEstoque(Produto **head) {

    FILE *arquivo = fopen("estoque.txt", "r"); // r = leitura
    if (arquivo == NULL) {
        printf("  [!] Nenhum arquivo encontrado. Iniciando vazio.\n");
        return;
    }

    int total = 0;

    while (!feof(arquivo)) { // enquanto não chegar no fim do arquivo

        Produto *novo = (Produto*) malloc(sizeof(Produto));
        if (novo == NULL) break;

        // lê todos os campos na mesma ordem que salvou
        int lidos = fscanf(arquivo,
            "%d;%49[^;];%f;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d\n",
            &novo->codigo, novo->nome,
            &novo->preco, &novo->quantidade,
            &novo->validade.dia, &novo->validade.mes, &novo->validade.ano,
            &novo->dataEntrada.dia, &novo->dataEntrada.mes, &novo->dataEntrada.ano,
            &novo->dataSaida.dia, &novo->dataSaida.mes, &novo->dataSaida.ano);

        if (lidos == 13) { // leu todos os 13 campos corretamente
            novo->proximo = NULL;
            inserirProduto(head, novo);
            total++;
        } else {
            free(novo); // leitura falhou, descarta
        }
    }

    fclose(arquivo);
    printf("  [OK] %d produto(s) carregado(s)!\n", total);
}




int main() {
    Produto *head = NULL;

    // carrega ao iniciar!
    printf("Carregando dados...\n");
    carregarEstoque(&head);

    int opcao;
    char buffer[10];

    do {
        printf("\n|======================|\n");
        printf("|  GERENCIADOR ESTOQUE |\n");
        printf("|======================|\n");
        printf("| 1. Cadastrar produto |\n");
        printf("| 2. Listar produtos   |\n");
        printf("| 3. Ordenar por nome  |\n");
        printf("| 4. Buscar por codigo |\n");
        printf("| 5. Remover produto   |\n");
        printf("| 6. Atualizar produto |\n");
        printf("| 7. Alertas de estoque|\n");
        printf("| 8. Relatorio         |\n");
        printf("| 9. Salvar dados      |\n");
        printf("| 0. Sair              |\n");
        printf("|======================|\n");
        printf("Escolha: ");
        fgets(buffer, 10, stdin);
        opcao = atoi(buffer);

        switch(opcao) {
            case 1: {
                Produto *novo = criarProduto(head);
                if (novo != NULL)
                    inserirProduto(&head, novo);
                break;
            }
            case 2:
                exibirProdutos(head);
                break;
            case 3:
                exibirOrdenadoPorNome(head);
                break;
            case 4: {

                if (head == NULL) {
                printf("\n  [!] Nenhum produto cadastrado!\n");
                break;
                }
                // 1. conta
                int n = 0;
                Produto *atual = head;
                while (atual != NULL) { n++; atual = atual->proximo; }

                // 2. calloc
                Produto **arr = calloc(n, sizeof(Produto*));

                // 3. copia
                atual = head;
                for (int i = 0; i < n; i++) { arr[i] = atual; atual = atual->proximo; }

                // 4. ordena por código
                for (int i = 0; i < n - 1; i++)
                    for (int j = 0; j < n - 1 - i; j++)
                        if (arr[j]->codigo > arr[j+1]->codigo) {
                         Produto *temp = arr[j]; arr[j] = arr[j+1]; arr[j+1] = temp;
                        }

                // 5. pede o código
                int cod = lerInteiro("Digite o codigo: ");

                // 6. busca
                int pos = buscaBinariaCodigo(arr, n, cod);

                // 7. exibe
                if (pos != -1)
                printf("Encontrado: %s | R$%.2f | Qtde: %d\n",
               arr[pos]->nome, arr[pos]->preco, arr[pos]->quantidade);
                else
                printf("Codigo %d nao encontrado!\n", cod);

                // 8. libera
                free(arr);
            break;
            }
            case 5: {
                int cod = lerInteiro("Codigo para remover: ");
                removerProduto(&head, cod);
                break;
            }
            case 6:{
                int cod = lerInteiro("Codigo para atualizar: ");
                atualizarProduto(head, cod);
                break;
            }
            case 7:
                alertarEstoque(head, 5); // ← avisa se tiver 5 ou menos unidades
            break;
            case 8:
                relatorioEstoque(head);
            break;
            case 9:
                salvarEstoque(head);
            break;
            case 0:
                printf("Encerrando sistema...\n");
                salvarEstoque(head); // ← salva ao sair!
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}