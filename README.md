# 🍔 Sistema de Gerenciamento de Lanchonete em C

Este é um projeto desenvolvido em linguagem C que simula o funcionamento de uma **lanchonete**, com recursos como:

- Cadastro e visualização de produtos
- Atendimento ao cliente
- Controle de fila de pedidos
- Pagamento via Pix, Cartão ou Dinheiro
- Geração de relatório de caixa diário
- Armazenamento de pedidos, cartões e produtos em arquivos `.DAT` ou `.TXT`

---

## 🧾 Funcionalidades

### ✅ Cadastro de Produtos
- Acesso protegido por senha (`123456`)
- Armazena os produtos em `PRODUTOS.DAT`
- Já inicia com um **cadastro padrão** de produtos, se o arquivo não existir

### 🤝 Atendimento ao Cliente
- Cliente pode escolher o nome de exibição
- Seleciona produtos e suas quantidades
- Efetua pagamento com:
  - 💸 Pix (com chave gerada aleatoriamente)
  - 💳 Cartão (com salvamento do número de forma segura)
  - 💵 Dinheiro
- Salva os pedidos no arquivo `PAGAMENTOS.DAT`

### ⏳ Controle de Fila
- Pedidos com mais de 5 itens vão para uma **fila secundária**
- Fila gerenciada por ordem de chegada (FIFO)
- Sistema notifica o atendente para liberar pedidos prontos

### 📦 Fechamento de Caixa
- Mostra:
  - Total de pedidos do dia
  - Valor total vendido
  - Número de clientes atendidos
- Permite gerar um **relatório diário** em `RELATORIO.txt`

---

## 📂 Organização dos Arquivos

- `PRODUTOS.DAT` → Produtos cadastrados
- `PAGAMENTOS.DAT` → Pedidos concluídos
- `CARTOES.DAT` → Cartões salvos (com máscara)
- `RELATORIO.txt` → Relatório diário do caixa

---

## 🔐 Segurança

- Acesso ao cadastro de produtos é **protegido por senha**
- Números de cartão são **mascarados** e não são exibidos totalmente
- Simulação de senha com `*` para ocultar caracteres no terminal

---

## 📌 Como Executar

1. Compile com um compilador C (ex: `gcc` ou compilador do Dev C++)
2. Execute o programa
3. O menu principal será exibido:

```text
1 - Cadastrar produtos (senha necessária)
2 - Atendimento ao cliente
3 - Controle de filas
4 - Fechar caixa
0 - Sair
