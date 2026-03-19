# 📦 Gerenciador de Estoque em C

Sistema de gerenciamento de estoque desenvolvido em C puro, construído do zero com foco em estruturas de dados dinâmicas e algoritmos clássicos.

---

## 🚀 Funcionalidades

| # | Funcionalidade |
|---|----------------|
| 1 | Cadastrar produto com validação de código duplicado |
| 2 | Listar todos os produtos em tabela formatada |
| 3 | Ordenar por nome (Bubble Sort) |
| 4 | Buscar por código (Busca Binária) |
| 5 | Remover produto com registro de data de saída |
| 6 | Atualizar nome, preço ou quantidade |
| 7 | Alertas de produtos vencidos e estoque baixo |
| 8 | Relatório: total de itens, valor do estoque, mais caro e mais barato |
| 9 | Salvar dados manualmente |
| 0 | Sair (salva automaticamente) |

---

## 🧠 Conceitos aplicados

**Estruturas de dados**
- `struct` com `struct` dentro (`Data` dentro de `Produto`)
- Lista encadeada simples com `struct Produto *proximo`

**Memória dinâmica**
- `malloc` — aloca cada produto individualmente na heap
- `calloc` — cria array temporário para ordenação
- `free` — libera memória ao remover produtos

**Ponteiros**
- Ponteiro para struct com acesso via `->`
- Ponteiro para ponteiro `**head` para modificar a lista dentro de funções

**Algoritmos**
- Bubble Sort para ordenar por nome
- Busca Binária para localizar por código — O(log n)

**Arquivo**
- `fopen` / `fprintf` / `fscanf` / `fclose` para salvar e carregar dados

**Validação**
- `sscanf` para garantir que o usuário só digita números onde necessário

---

## 🖥️ Demonstração

```
|======================|
|  GERENCIADOR ESTOQUE |
|======================|
| 1. Cadastrar produto |
| 2. Listar produtos   |
| 3. Ordenar por nome  |
| 4. Buscar por codigo |
| 5. Remover produto   |
| 6. Atualizar produto |
| 7. Alertas de estoque|
| 8. Relatorio         |
| 9. Salvar dados      |
| 0. Sair              |
|======================|

+--------+------------------------------+------------+------------+----------------+
| COD    | NOME                         | PRECO      | QTDE       | VALIDADE       |
+--------+------------------------------+------------+------------+----------------+
| 1      | BANANA                       | R$5.00     | 3          | 20/04/2026     |
| 2      | CHOCOLATE                    | R$15.00    | 7          | 18/03/2026     |
+--------+------------------------------+------------+------------+----------------+
```

---

## 🛠️ Como compilar e executar

**Pré-requisito:** GCC instalado.

```bash
# Compilar
gcc -o estoque estoque.c

# Executar
./estoque
```

Os dados são salvos automaticamente em `estoque.txt` ao sair do programa.

---

## 📁 Estrutura das funções

```
criarProduto()          → malloc + validação de duplicata
inserirProduto()        → insere na lista com **head
exibirProdutos()        → percorre lista com while
removerProduto()        → desencadeia nó + free
atualizarProduto()      → navega lista e edita campos
exibirOrdenadoPorNome() → calloc + bubble sort + exibição
buscaBinariaCodigo()    → busca binária em array ordenado
alertarEstoque()        → verifica vencimento e quantidade
relatorioEstoque()      → acumula totais em uma passagem
salvarEstoque()         → fprintf linha a linha
carregarEstoque()       → fscanf + inserirProduto
```

---

## 👨‍💻 Sobre

Projeto desenvolvido do zero como parte do aprendizado de programação em C, cobrindo desde structs básicas até gerenciamento manual de memória e persistência de dados.