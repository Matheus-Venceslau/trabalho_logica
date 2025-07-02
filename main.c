/* BIBLIOTECAS */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// Tipo de dado para o produto
typedef struct
{
	int codProd;
	char nomeProd[20 + 1];
	float precoProd;

} PRODUTOS;

typedef struct // Fiz essa struct para controlar a fila de pedidos com as formas de pagamento
{
	int fila[4 + 1];
	int forma_pagamento[4 + 1];
} FILA;

// Tipo de dado para os itens do pedido
typedef struct
{
	int codProd;
	char nomeProd[20 + 1];
	float precoProd;
	int quantidade;
	float totalItem;

} ITENS_PEDIDO;

// Tipo de dado para o pedido em si
typedef struct
{
	int codPedido;
	char nomeCliente[20 + 1];
	int quantidade_pedidos;
	ITENS_PEDIDO itensPedido[100 + 1];
	float totalPedido;
	int forma_pagamento;

} PEDIDO;

// struct somente para salvar o cartão
typedef struct
{
	int codPedido;
	char numeroCartao[16 + 1];
} CARTOES;

// Variaveis globais:
FILA fila;
int inicio_fila = 0;
int fim_fila = 0;

int quantidade_pedidos_dia = 0;
float valor_vendido = 0;
int quantidade_clientes = 0;
char senha[] = "123456";

// Protótipos de funções
void fechar_caixa();
void cadastrar_produtos();
int contador_produtos(char nome_arquivo[20 + 1]);
void menu();
void atendimento_cliente();
char *gerador_chave();
void salva_cartao(int codPedido);
void adicionar_fila(int codigo, int forma_pagamento);
void liberar_fila();
void cadastro_padrao();
void verificacao_cadastro_padrao();

void cadastro_padrao()
{
	FILE *arquivo;
	int i;
	PRODUTOS produtos[] = {
		{1, "Big Mickey Extra", 15},
		{2, "Pão Mickey", 2},
		{3, "Big Fritas", 10},
		{4, "Coca-cola 500ml", 5},
		{5, "Agua 500ml", 3.5}};

	arquivo = fopen("PRODUTOS.DAT", "w");

	for (i = 0; i < 5; i++)
	{
		fwrite(&produtos[i], sizeof(produtos[i]), 1, arquivo);
	}

	fclose(arquivo);
}

void verificacao_cadastro_padrao()
{
	FILE *arquivo;

	arquivo = fopen("PRODUTOS.DAT", "r");

	if (arquivo == NULL)
	{ // Verificação se o arquivo ja existe
		system("cls");
		printf("Bem-vindo! O arquivo PRODUTOS.DAT já esta pronto para o cadastro padrao! Pressione qualquer tecla \n");
		getch();
		cadastro_padrao();
	}

	fclose(arquivo);
	menu();
}

void fechar_caixa()
{
	if (fim_fila != 0)
	{
		printf("\n\nAinda existem pedidos na fila, por favor atenda-os antes de fechar o caixa!\n");
		getch();
		menu();
	}

	time_t t;
	struct tm *data_atual;
	char data_formatada[11]; // "DD/MM/AAAA" + '\0'

	char fechar_caixa = 'n';
	FILE *arquivo;

	system("cls");
	printf("A quantidade de pedidos feitos foi: %d", quantidade_pedidos_dia);
	getch();
	printf("\nO valor total vendido foi: %.2f", valor_vendido);
	getch();
	printf("\nForam atendidos: %d", quantidade_clientes);
	getch();

	printf("\n\nDeseja gerar o relatório do dia? [s/n]-> ");
	fechar_caixa = getch();
	if (fechar_caixa == 's')
	{
		arquivo = fopen("RELATORIO.txt", "w");
		// Pega o tempo atual
		time(&t);
		// Converte para struct tm local
		data_atual = localtime(&t);
		// Formata a data no estilo "DD/MM/AAAA"
		strftime(data_formatada, sizeof(data_formatada), "%d/%m/%Y", data_atual);

		fprintf(arquivo, "\n=========================== RELATORIO DIA: %s ===========================", data_formatada);
		fprintf(arquivo, "\nA quantidade de pedidos feitos foi: %d", quantidade_pedidos_dia);
		fprintf(arquivo, "\nO valor total vendido foi: %.2f", valor_vendido);
		fprintf(arquivo, "\nForam atendidos: %d", quantidade_clientes);
		fclose(arquivo);
		exit(0);
	}
	else
	{
		exit(0);
	}
}

void leve(int formaPagamento)
{
	switch (formaPagamento)
	{
	case 1:
		system("color F1");
		printf("\n\n========== PEDIDO PAGO COM PIX ========== \n");
		getch();
		break;

	case 2:
		system("color F5");
		printf("\n\n========== PEDIDO PAGO COM CARTÃO DE CRÉDITO/DÉBITO ========== \n");
		getch();
		break;

	case 3:
		system("color F2");
		printf("\n\n========== PEDIDO PAGO COM DINHEIRO ==========\n");
		getch();
		break;

	default:
		printf("\n\nERRO no pagamento!\n");
		getch();
		atendimento_cliente();
	}
}

int verificacao_senha()
{ // Fun��o responsavel pela verifica��o da senha
	char senha_digitada[6 + 1];

	system("cls");

	printf("Digite a senha de acesso: ");
	int i;
	for (i = 0; i < 6; i++)
	{
		fflush(stdin);
		senha_digitada[i] = getch();
		printf("*");
	}
	senha_digitada[i] = '\0';

	if (strcmp(senha, senha_digitada) == 0)
	{
		return 1;
	}
	else
	{
		printf("%s", senha_digitada);
		return 0;
	}
}

void cadastrar_produtos()
{ // Fun��o respons�vel pelo cadastro de novos produtos
	// Declaracao de variaveis
	FILE *arquivo;
	arquivo = fopen("PRODUTOS.DAT", "a"); // Abrindo o arquivo no modo de alteração ou criação do arquivo
	PRODUTOS produto_cadastro;
	char grava_produto;
	int quantidade_produtos;

	system("cls");
	printf("================== Cadastro de produtos ==================\n");

	printf("\nDigite o nome do produto: ");
	fflush(stdin);
	gets(produto_cadastro.nomeProd);

	printf("\nDigite o preço do produto: ");
	fflush(stdin);
	scanf("%f", &produto_cadastro.precoProd);

	system("cls");
	printf("================== Cadastro de produtos ==================\n");

	do
	{ // Verifica o que o usuario deseja fazer com os dados limitando suas respostas
		printf("\nDeseja salvar o produto em PRODUTOS.DAT? [s/n]\n-> ");
		grava_produto = getch();

		if (grava_produto == 'S')
		{
			grava_produto = 's';
		}

		if (grava_produto == 'N')
		{
			grava_produto = 'n';
		}

	} while (grava_produto != 's' && grava_produto != 'n');

	if (grava_produto == 's')
	{
		quantidade_produtos = contador_produtos("PRODUTOS.DAT");
		produto_cadastro.codProd = quantidade_produtos + 1;

		if (arquivo == NULL)
		{
			printf("\nERRO AO ABRIR O PRODUTOS.DAT");
			getch();
			menu();
		}

		fwrite(&produto_cadastro, sizeof(produto_cadastro), 1, arquivo);
	}
	fclose(arquivo);

	menu();
}

int contador_produtos(char nome_arquivo[20 + 1])
{
	int contador = 0;
	PRODUTOS produto;
	FILE *arquivo;
	arquivo = fopen(nome_arquivo, "r");

	if (arquivo == NULL)
	{
		printf("\nArquivo n�o encontrado ou erro ao abrir.\n");
		return 0;
	}

	while (!feof(arquivo))
	{
		fread(&produto, sizeof(produto), 1, arquivo);
		if (!feof(arquivo))
		{
			contador = contador + 1;
		}
	}
	fclose(arquivo);

	return contador;
}

int contador_pedidos(char nome_arquivo[20 + 1])
{
	int contador_pedidos = 0;
	FILE *arquivo;
	char linha[256];

	arquivo = fopen(nome_arquivo, "r");
	rewind(arquivo);
	if (arquivo == NULL)
	{
		printf("\nArquivo n�o encontrado ou erro ao abrir\n");
		return -1;
	}

	while (fgets(linha, sizeof(linha), arquivo))
	{
		if (strstr(linha, "================================"))
		{
			contador_pedidos++;
		}
	}

	fclose(arquivo);
	return contador_pedidos;
}

int pagamento_pedido(int codPedido)
{
	int pagamento;

	do
	{
		system("cls");
		printf("================================================\n");
		printf("======= Como deseja efetuar o pagamento? =======\n");
		printf("================================================\n");
		printf("=------ 1 - Pix -------------------------------=\n");
		printf("=------ 2 - Cartão Debito/credito -------------=\n");
		printf("=------ 3 - Dinheiro --------------------------=\n");
		printf("=------ 0 - Cancelar pedido -------------------=\n");
		printf("================================================\n");

		printf("\n-> ");
		fflush(stdin);
		scanf("%i", &pagamento);
	} while (pagamento > 3 || pagamento < 0);

	if (pagamento == 0)
	{
		atendimento_cliente();
	}
	else if (pagamento == 1)
	{
		system("cls");
		printf("\n\n================= Chave PIX gerada: %s =================\n", gerador_chave());
		printf("\n\nPor favor, pague o valor do pedido e depois aperte qualquer tecla para continuar!\n");
		getch();
		system("cls");
		printf("============== Voc� pagou! ==============\n\n");
		printf("Para prosseguir aperte qualquer tecla! ");
		getch();

		return 1;
	}
	else if (pagamento == 2)
	{
		salva_cartao(codPedido);
		system("cls");
		printf("============== Voc� pagou! ==============\n\n");
		printf("Para prosseguir aperte qualquer tecla! ");
		getch();
		return 2;
	}
	else if (pagamento == 3)
	{
		system("cls");
		printf("============== Voc� pagou! ==============\n\n");
		printf("Para prosseguir aperte qualquer tecla! ");
		getch();
		return 3;
	}
	else
	{
		printf("opcao invalida!");
	}
}

void salva_cartao(int codPedido)
{
	FILE *arquivo_cartao;
	CARTOES cartao;
	char cartao_correto = 'n';
	char cartao_formatado[19 + 1]; // Ex: "1234 **** **** 5678"

	cartao.codPedido = codPedido;
	arquivo_cartao = fopen("CARTOES.DAT", "a");

	if (arquivo_cartao == NULL)
	{
		printf("Erro ao abrir o arquivo CARTOES.DAT!\n");
		getch();
		return;
	}

	do
	{
		system("cls");
		printf("Digite o número do cartão (16 dígitos): ");
		fflush(stdin);
		fgets(cartao.numeroCartao, sizeof(cartao.numeroCartao), stdin);

		// Remove o \n se existir
		size_t len = strlen(cartao.numeroCartao);
		if (cartao.numeroCartao[len - 1] == '\n')
			cartao.numeroCartao[len - 1] = '\0';

		if (strlen(cartao.numeroCartao) == 16)
		{
			printf("\nO número digitado está correto? [s/n]\n-> ");
			fflush(stdin);
			cartao_correto = getche();
		}

	} while (cartao_correto != 's');

	// Formata: mostra 4 primeiros e 4 últimos
	sprintf(cartao_formatado, "%.4s **** **** %.4s",
			cartao.numeroCartao,
			cartao.numeroCartao + 12);

	fprintf(arquivo_cartao,
			"-----------------------------\nCódigo do pedido: %d\nNúmero do cartão: %s\n\n",
			cartao.codPedido,
			cartao_formatado);

	fclose(arquivo_cartao);

}

char *gerador_chave()
{ // Fun��o para gerar a chave pix :)
	int i = 0;
	static char chave[35 + 1];
	int numero;
	char aleatorio[71 + 1] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	for (i = 0; i < 35; i++)
	{
		numero = rand() % 72;
		chave[i] = aleatorio[numero];
	}

	return chave;
}

void atendimento_cliente()
{
	FILE *arquivo, *salva_pedido;
	PRODUTOS produto;
	PEDIDO pedido;
	ITENS_PEDIDO itens_pedido;

	system("color 07");
	int produto_escolhido = 0;

	int quantidade_itens_pedidos = 0;

	int contador = 0; // Variavel para "imitar" um for

	char novo_item = 's';

	int repete = 1;
	do
	{
		system("cls");
		printf("===================== Bem-vindo ao atendimento ao cliente! =====================\n");

		printf("\nComo voc� deseja ser chamado?\n-> ");
		fflush(stdin);
		gets(pedido.nomeCliente);

		if (strlen(pedido.nomeCliente) < 3)
		{
			repete = 1;
		}
		else
		{
			repete = 0;
		}
	} while (strcmp(pedido.nomeCliente, "") == 0 || repete == 1);

	do
	{
		arquivo = fopen("PRODUTOS.DAT", "r");

		while (!feof(arquivo))
		{ // Mostra os produtos cadastrados
			fread(&produto, sizeof(produto), 1, arquivo);
			if (!feof(arquivo))
			{
				printf("\n%d -> %s ..... R$ %.2f", produto.codProd, produto.nomeProd, produto.precoProd); // Ex: 0 -> produto a ...... R$ 15,00
			}
		}

		printf("\nDigite o n�mero do produto escolhido [0 -> Voltar]: ");
		fflush(stdin);
		scanf("%i", &produto_escolhido);

		if (produto_escolhido > contador_produtos("PRODUTOS.DAT"))
		{
			printf("\n\nProduto inv�lido!");
			getch();
			atendimento_cliente();
		}

		if (produto_escolhido == 0)
		{
			menu();
		}

		contador = contador + 1;

		arquivo = fopen("PRODUTOS.DAT", "r");
		while (!feof(arquivo))
		{ // Mostra os produtos cadastrados
			fread(&produto, sizeof(produto), 1, arquivo);
			if (!feof(arquivo))
			{

				if (produto.codProd == produto_escolhido)
				{
					itens_pedido.codProd = produto.codProd;
					strcpy(itens_pedido.nomeProd, produto.nomeProd);
					itens_pedido.precoProd = produto.precoProd;
				}
			}
		}

		if (itens_pedido.nomeProd == NULL)
		{
			printf("\n\nERRO - Produto n�o encontrado\n");
			getch();
			atendimento_cliente();
		}

		do
		{
			printf("\nQual � a quantidade desejada? [0 -> Voltar]: ");
			fflush(stdin);
			scanf("%d", &itens_pedido.quantidade);

		} while (itens_pedido.quantidade < 0);

		if (itens_pedido.quantidade == 0)
		{
			menu();
		}

		itens_pedido.totalItem = itens_pedido.precoProd * itens_pedido.quantidade;

		pedido.itensPedido[contador - 1] = itens_pedido;

		system("cls");

		printf("============= Pedido adicionado ao carrinho! :) =============\n");

		// Saida:
		do
		{
			printf("\nDeseja pedir outro item? [s/n] \n-> ");
			fflush(stdin);
			novo_item = getche();
			if (novo_item == 'N')
			{
				novo_item = 'n';
			}
		} while (novo_item != 'n' && novo_item != 's');

		fclose(arquivo);
		quantidade_itens_pedidos = quantidade_itens_pedidos + 1; // Contador de itens pedidos

	} while (novo_item != 'n');
	quantidade_pedidos_dia = quantidade_pedidos_dia + quantidade_itens_pedidos; // Fechar caixa

	int quantidade_pedidos = contador_pedidos("PAGAMENTOS.DAT");

	if (quantidade_pedidos == -1)
	{
		menu();
	}

	pedido.codPedido = quantidade_pedidos + 1;

	// Modulo pagamento

	pedido.forma_pagamento = pagamento_pedido(pedido.codPedido);

	// Depois de pago, salva o pedido
	salva_pedido = fopen("PAGAMENTOS.DAT", "a");

	pedido.quantidade_pedidos = quantidade_itens_pedidos;

	fprintf(salva_pedido, "\n================================\nC�digo do pedido: %d \n\nNome do cliente: %s", pedido.codPedido, pedido.nomeCliente);
	pedido.totalPedido = 0;
	int i = 0;
	for (i = 0; i < pedido.quantidade_pedidos; i++)
	{
		fprintf(salva_pedido,
				"\nItem %d:\nNome do item: %s\nQuantidade pedida: %d\nValor unit�rio: %.2f\nTotal do item: %.2f\n\n",
				i + 1,
				pedido.itensPedido[i].nomeProd,
				pedido.itensPedido[i].quantidade,
				pedido.itensPedido[i].precoProd,
				pedido.itensPedido[i].totalItem);
		pedido.totalPedido = pedido.totalPedido + pedido.itensPedido[i].totalItem;
	}
	fprintf(salva_pedido, "O total do pedido �: %.2f\n\n", pedido.totalPedido);

	valor_vendido = valor_vendido + pedido.totalPedido;
	quantidade_clientes = quantidade_clientes + 1;

	switch (pedido.forma_pagamento)
	{
	case 1:
		fprintf(salva_pedido, "Pagamento efetuado com: PIX\n");
		break;

	case 2:
		fprintf(salva_pedido, "Pagamento efetuado com: CARTÃO\n");
		break;

	case 3:
		fprintf(salva_pedido, "Pagamento efetuado com: DINHEIRO\n");
	}

	fclose(arquivo);
	fclose(salva_pedido);

	if (pedido.quantidade_pedidos > 5 || pedido.itensPedido->quantidade > 5)
	{
		adicionar_fila(pedido.codPedido, pedido.forma_pagamento);
	}
	else
	{
		leve(pedido.forma_pagamento);
	}

	atendimento_cliente();
}

void liberar_fila()
{
	int i;

	for (i = inicio_fila; i < fim_fila; i++)
	{
		fila.fila[i] = fila.fila[i + 1];
		fila.forma_pagamento[i] = fila.forma_pagamento[i + 1];
	}

	fim_fila = fim_fila - 1;
}

void controle_de_filas()
{
	char onde_ir = '0';
	int i;
	int forma_pagamento;
	char liberar_pedido;
	system("cls");

	// Verificação se não tem ninguem esperando
	if (fim_fila == 0 && inicio_fila == 0)
	{
		printf("Ainda não tem ninguém na fila!");
		printf("\nPara onde voce deseja ir? [1-> menu / 2-> atendimento]\n-> ");
		fflush(stdin);
		onde_ir = getch();

		if (onde_ir == '2')
		{
			atendimento_cliente();
		}
		else
		{
			menu();
		}
	}

	for (i = inicio_fila; i < fim_fila; i++)
	{
		printf("Codigo do pedido: %d\n", fila.fila[i]);
	}

	printf("\n\nO pedido %d já esta pronto? [s/n]\n-> ", fila.fila[inicio_fila]);
	liberar_pedido = getch();

	if (liberar_pedido == 's')
	{
		forma_pagamento = fila.forma_pagamento[inicio_fila];
		liberar_fila();
		leve(forma_pagamento);
	}
	menu();
}

void adicionar_fila(int codigo, int forma_pagamento)
{
	system("cls");
	printf("Como seu pedido é muito grande, por favor se direcine a fila secundaria!\n\n");
	getch();

	fila.fila[fim_fila] = codigo;					  // Adiciona o cliente no numero do final da fila, o codigo vai ser usado para puxar as informações do pedido
	fila.forma_pagamento[fim_fila] = forma_pagamento; // Adiciona a forma de pagamento do cliente no final da fila
	fim_fila = fim_fila + 1;						  // corrige o novo final da fila

	atendimento_cliente(); // retorna para um novo atendimento
}

void menu()
{ // Fun��o respons�vel por mostrar o menu, analisar a op��o escolhida e executar a fun��o correspondente
	int escolha = 0;
	int autenticacao; // Variavel para definir o nivel de permissao do usuario

	system("color 07");
	do
	{
		system("cls");

		printf("==========================================\n");
		printf("===========        MENU        ===========\n");
		printf("==========================================\n");
		printf("= -------- 1 - Cadastrar produtos ------ =\n");
		printf("= -------- 2 - Atendimeto ao cliente --- =\n");
		printf("= -------- 3 - Controle de filas ------- =\n");
		printf("= -------- 4 - Fechar caixa ------------ =\n");
		printf("= -------- 0 - Sair  ------------------- =\n");
		printf("==========================================\n");

		printf("\n\n O que deseja fazer? [0 - 4]\n -> ");
		fflush(stdin);
		scanf("%d", &escolha);

	} while (escolha > 4 || escolha < 0); // Limita dentro das op��es oferecidas

	if (escolha == 1)
	{
		autenticacao = verificacao_senha();

		if (autenticacao == 0)
		{ // Verifica��o se o usuario tem a permiss�o necess�ria para cadastrar produtos
			printf("\nERRO: Acesso negado\n");

			getch();
			menu();
		}
		else
		{
			cadastrar_produtos();
		}
	}

	switch (escolha)
	{
	case 0:
		exit(0);
		break;
	case 2:
		atendimento_cliente();
		break;
	case 3:
		controle_de_filas();
		break;
	case 4:
		fechar_caixa();
	}
}

int main()
{
	system("color 07");
	srand(time(NULL));
	setlocale(LC_ALL, "");
	verificacao_cadastro_padrao();

	return 0;
}
