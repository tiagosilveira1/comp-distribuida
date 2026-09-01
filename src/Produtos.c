//INTEGRANTES DO GRUPO
//Aluno: Tiago Silveira Lopes, RA: 10417600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//codigo global
int codigo = 1;
int quantidade_produtos = 0;

// Definição da struct
typedef struct {
    int codigo;
    char *nome;
    float preco;
    int quantidade;
} Produto;

//----------------------------------
//alocar_vetor_nome
//----------------------------------

//Essa função é uma estratégia utilizada para alocar dinamicamente um vetor de caracteres:
//O vetor dinâmico é realocado para cada interação, incrementando em um o seu tamanho e ao final do tamanho atual, recebe o próximo 
//caractere do vetor de entrada
//Ao final, é adicionado \0 ao vetor dinâmico em seu final para indicar final de string

void alocar_vetor_nome(char **nome) {
    char c;
    int tamanho = 0;

    *nome = NULL;

    //caracteres são alocados no laço, um a um
    while ((c = getchar()) != '\n') {
        char *temp = realloc(*nome, (tamanho + 1) * sizeof(char));

        if (temp == NULL) {
            printf("Erro ao alocar memoria.\n");
            free(*nome);
            *nome = NULL;
            return;
        }

        *nome = temp;
        (*nome)[tamanho] = c;
        tamanho++;
    }

    char *temp = realloc(*nome, (tamanho + 1) * sizeof(char));

    if (temp == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(*nome);
        *nome = NULL;
        return;
    }

    *nome = temp;
    (*nome)[tamanho] = '\0';
}

//----------------------------------
//1. ADICIONAR PRODUTO
//----------------------------------

//A lógica dessa função é que, ao obter os dados da struct informados pelo usuário, a função monta uma nova struct Produto e
//adiciona seu endereço no vetor de ponteiros de structs Produto, realocando seu tamanho para compor a nova struct.
//Especificamente ao atributo nome, ele é obtido através da função 'alocar_vetor_nome', na variável 'nome' com espaço de memória 
//reservado. Ao necessitar copiá-lo para o *nome da struct, o código dá outro malloc, faz strcpy da variável 'nome' para o *nome da
//struct e libera em seguida a memória de 'nome'.

void adicionar_produto(Produto **produtos) {
    float preco;
    int qtd;
    char *nome;

    //--------- informa todos os itens
    printf("--- Adicionar Produto ---\n");

    printf("Nome do produto: ");
    getchar();
    

    alocar_vetor_nome(&nome);
    printf("\n");

    printf("Preco do produto: ");
    scanf("%f", &preco);
    printf("\n");

    printf("Quantidade: ");
    scanf("%d", &qtd);
    printf("\n");


    //---------

    //--------- criar e alocar novo produto
    Produto *produto = realloc(*produtos, (quantidade_produtos + 1) * sizeof(Produto));

    //verificar se alocação foi bem sucedida
    if (produto == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(nome); //nome foi alocado previamente
        return;
    }
    //---------

    //--------- atributos do produto
    *produtos = produto;
    
    (*produtos)[quantidade_produtos].nome = malloc(strlen(nome) + 1);

    //verificar se alocação foi bem sucedida
    if ((*produtos)[quantidade_produtos].nome == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(nome);
        return;
    }

    strcpy((*produtos)[quantidade_produtos].nome, nome);
    (*produtos)[quantidade_produtos].preco = preco;
    (*produtos)[quantidade_produtos].quantidade = qtd;
    (*produtos)[quantidade_produtos].codigo = codigo;

    free(nome); //nome já foi copiado e com memória nova alocada para o produto, portanto deve ser liberado
    

    codigo++;
    quantidade_produtos++;
    //---------

    //impressão
    printf("Produto adicionado com codigo %d!\n", produto->codigo);
}

//----------------------------------
//2. LISTAR PRODUTO
//----------------------------------

//Essa função percorre o vetor dinâmico e da printf de suas informações em forma de tabela como solicitado no material do git.

void listar_produtos(Produto **produtos) {
    float preco_total = 0.0;

    if(*produtos == NULL) {
        printf("Nenhum produto na lista!\n");
        return;
    }

    printf("--- Lista de Produtos ---\n");
    printf("+--------+------------------+----------+------+---------------+\n");
    printf("| Codigo | Nome             | Preco    | Qtd  | Valor Estoque |\n");
    printf("+--------+------------------+----------+------+---------------+\n");

    for (int i = 0; i < quantidade_produtos; i++) {
        printf("| %6d | %-16s | %8.2f | %4d | %13.2f |\n",
           (*produtos)[i].codigo,
           (*produtos)[i].nome,
           (*produtos)[i].preco,
           (*produtos)[i].quantidade,
           (*produtos)[i].preco * (*produtos)[i].quantidade);
        preco_total += (*produtos)[i].preco * (*produtos)[i].quantidade;
    }
    printf("Valor total do estoque: %.2f",preco_total);
}

//----------------------------------
//3. BUSCAR PRODUTO
//----------------------------------

//Lógica similar à função de listagem: percorre o vetor dinâmico de produtos tentando encontrar o item com o código informado.

Produto* buscar_produto(Produto **produtos) {
    printf("--- Buscar Produto ---\n");

    int codigo;
    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    printf("\n");

    for (int i = 0; i < quantidade_produtos; i++) {
        if(codigo == (*produtos)[i].codigo) {
            return &(*produtos)[i];
        }
    }
    return NULL;
}

//Função utilizada para simplificar a main.

void buscar_produto_aux (Produto **produtos) {

    Produto* busca = buscar_produto(produtos);

            if(busca == NULL) {
                printf("Produto nao foi encontrado.\n");
            }

            else {
                printf("Produto com o codigo %d encontrado!\n",busca->codigo);

                printf("Nome: %s\n",busca->nome);
                printf("Preco: %.2f\n",busca->preco);
                printf("Quantidade: %d\n",busca->quantidade);
                printf("Codigo: %d\n",busca->codigo);
            }
}

//----------------------------------
//4. ATUALIZAR ESTOQUE
//----------------------------------

//Essa função é uma extensão da função de busca, sendo que ao encontrar o item de código informado o programa exige
//que o usuário informe a nova quantidade (atualizar seu estoque).

void atualizar_estoque(Produto **produtos) {
    printf("--- Atualizar Estoque ---\n");

    Produto* busca = buscar_produto(produtos);

    if(busca == NULL) {
        printf("Produto nao encontrado.");
        return;
    }

    printf("Nome: %s\n",busca->nome);
    printf("Quantidade atual: %d\n",busca->quantidade);

    int nova_qtd;
    printf("Nova quantidade: ");
    scanf("%d", &nova_qtd);
    printf("\n");

    busca->quantidade = nova_qtd;

    printf("Estoque atualizado com sucesso!\n");
}

//----------------------------------
//5. REMOVER PRODUTO
//----------------------------------

//Essa função realiza duas buscas no vetor de produtos para o código e índice do elemento (struct) a ser removido do vetor.
//O índice do nome do produto deve ter sua memória liberada, o vetor é reestruturado fazendo (*produtos)[i] = (*produtos)[i + 1]; e
//seu tamanho é realocado para a nova quantidade de produtos. Se a nova quantidade for 0, a memória do vetor dinâmico é liberada.  

void remover_produto(Produto **produtos) {
    printf("--- Remover Produto ---\n");

    Produto* busca = buscar_produto(produtos);

    if(busca == NULL) {
        printf("Produto não encontrado.");
        return;
    }

    int indice;

    //Pegar índice do produto
    for (int i = 0; i < quantidade_produtos; i++) {

        if ((*produtos)[i].codigo == busca->codigo) {
            indice = i;
            break;
        }
    }

    printf("Produto '%s' removido com sucesso!\n",(*produtos)[indice].nome);

    free((*produtos)[indice].nome);

    for (int i = indice; i < quantidade_produtos - 1; i++) {
        (*produtos)[i] = (*produtos)[i + 1];
    }

    quantidade_produtos--;

    if (quantidade_produtos > 0) {

        Produto *temp = realloc(*produtos, quantidade_produtos * sizeof(Produto));

        if (temp != NULL) {
            *produtos = temp;
        }

    } else {
        free(*produtos);
        *produtos = NULL;
    }
}

//Função utilizada para simplificar a main.

void liberar_memoria(Produto **produtos) {
    printf("Liberando memória...\n");

    for (int i = 0; i < quantidade_produtos; i++) {
        printf("Memoria do produto '%s' liberada.\n", (*produtos)[i].nome);
        free((*produtos)[i].nome);
    }

    printf("Vetor de produtos liberado.\n");
    free(*produtos);

    *produtos = NULL;
}

int main (void) {

    //variáveis utilizadas na main do programa
    int choose;
    Produto *produtos = NULL;
    
    while(true) {
        printf("========================================\n");
        printf("    SISTEMA DE CADASTRO DE PRODUTOS\n");
        printf("========================================\n");
        printf("1. Adicionar Produto\n");
        printf("2. Listar todos os Produtos\n");
        printf("3. Buscar Produtos por Codigo\n");
        printf("4. Atualizar estoque\n");
        printf("5. Remover Produto\n");
        printf("6. Sair do Programa\n");
        printf("----------------------------------------\n");
        printf("Escolha sua opcao: ");
        scanf("%d",&choose);
        printf("\n");

        if(choose == 1) {
            adicionar_produto(&produtos);
        }

        else if(choose == 2) {
            listar_produtos(&produtos);
        }

        else if(choose == 3) {
            buscar_produto_aux(&produtos);
        }

        else if(choose == 4) {
            atualizar_estoque(&produtos);
        }

        else if(choose == 5) {
            remover_produto(&produtos);
        }

        else if(choose == 6) {
            liberar_memoria(&produtos);   
            break;
        }

        else {
            printf("Opção invalida, escolha entre os numeros 1 e 6.\n");
        }
    }

    printf("Programa encerrado.\n");

    return 0;
}
