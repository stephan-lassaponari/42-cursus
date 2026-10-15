# Plano de estudo — `mini_serv`

**Período:** quinta-feira, 15/10, a quarta-feira, 21/10

**Estratégia para o exame:** copiar o `main.c` fornecido e usá-lo como esqueleto, em vez de tentar escrever o servidor inteiro do zero.

**Meta:** saber o que preservar, o que substituir e como testar a solução dentro do tempo do exame.

## O que fazer com o `main.c` fornecido

O `main.c` é uma base, não uma solução pronta. Copie-o para o ficheiro de entrega e adapte-o ao subject:

- Preserve a criação do socket, a configuração de `sockaddr_in`, o `bind` e o `listen`.
- Preserve o endereço `127.0.0.1`, mas substitua a porta fixa por `htons(atoi(av[1]))`.
- Adicione a validação `ac == 2` e escreva exatamente `Wrong number of arguments\n` em `stderr` quando falhar.
- Substitua mensagens de diagnóstico como `printf` e saídas com status incorreto por `write`/`Fatal error\n` e status 1, conforme o caso.
- Não mantenha o único `accept` bloqueante do exemplo. Inicialize os `fd_set` e use `select` num ciclo para aceitar clientes e ler dados de todos eles.
- Ao aceitar um cliente, atribua o próximo ID, registe o descritor e avise os clientes existentes.
- Ao receber dados, acumule o texto até às quebras de linha, prefixe cada linha com `client %d: ` e envie aos outros clientes.
- Ao receber `recv <= 0`, avise os restantes clientes, remova o descritor do conjunto e feche-o.

O subject avisa que o ficheiro de exemplo pode usar funções proibidas ou conter código que não deve ir para a entrega. Antes de copiar cada trecho, confira-o contra a lista de funções permitidas.

## Ordem para montar a solução no exame

Treine sempre esta sequência, com o `main.c` aberto como referência inicial:

1. Copie o exemplo para `mini_serv.c`; confira nome do ficheiro e includes necessários.
2. Ajuste `main` primeiro: argumentos, criação do socket, endereço loopback, porta por argumento, `bind` e `listen`.
3. Prepare o estado global: IDs, buffers, clientes e conjuntos de descritores; faça `FD_ZERO` antes do primeiro `FD_SET`.
4. Complete as funções de apoio para erro fatal, notificação, entrada/saída de clientes e envio de mensagens.
5. Troque o `accept` único por `select`: socket do servidor pronto significa novo cliente; outro descritor pronto significa `recv`.
6. Compile com `-Wall -Wextra -Werror` e corrija primeiro erros de compilação; depois valide as mensagens e desconexões.

**Ponto-chave:** não tente adaptar o exemplo linha a linha sem entender o fluxo. O maior salto é trocar “aceitar um cliente” por “monitorizar servidor e clientes com `select`”.

## Rotina diária

Reserve cerca de **1h30**:

- **10 min:** reproduza de memória o fluxo geral e os nomes das funções.
- **25 min:** treine a transformação do `main.c` do dia.
- **5 min:** pausa.
- **25 min:** escreva a solução por blocos, começando pelo esqueleto do exemplo.
- **15 min:** compare com a solução de referência e registe apenas os erros.
- **10 min:** repita os trechos errados sem consultar.

Nos simulados, use o tempo real do exame, se o souber. Não memorize mensagens ou código de diagnóstico do exemplo.

## Calendário

### Quinta, 15/10 — Ler o esqueleto

- **Funções:** `main`, `create_socket`, `fatal_error`.
- Leia o subject e marque os requisitos, as funções permitidas e as mensagens exatas.
- Leia o `main.c` e classifique cada trecho como **manter**, **alterar** ou **remover**.
- Treine copiar o `main.c` e fazer as alterações de argumentos, porta, endereço e erros.
- Explique em voz alta por que o `accept` único não atende ao exercício.

### Sexta, 16/10 — Clientes e mensagens

- **Funções:** `add_client`, `remove_client`, `notify`, `send_msg`, `extract_message`, `str_join`.
- Reproduza as alterações do dia anterior sem consultar apontamentos.
- Treine a atribuição sequencial de IDs e as mensagens de chegada e saída.
- Reproduza o processamento de mensagens completas e fragmentadas em várias leituras.
- Confirme que cada linha recebida ganha o prefixo correto e não volta ao remetente.

### Sábado, 17/10 — `select`

- **Funções e chamadas:** `main`, `create_socket`, `add_client`, `remove_client`, `send_msg`; integre `select`, `accept` e `recv` no ciclo principal.
- Desenhe de memória o ciclo: copiar o conjunto ativo, chamar `select`, percorrer descritores e tratar servidor ou cliente.
- Escreva o ciclo completo a partir do `main.c`.
- Treine remoção do cliente: notificar, `FD_CLR`, `close` e libertar o buffer.
- Compile e corrija erros sem consultar a solução até ter uma primeira tentativa funcional.

### Domingo, 18/10 — Primeiro simulado

- **Funções:** reproduza todas — `fatal_error`, `notify`, `add_client`, `remove_client`, `send_msg`, `create_socket`, `extract_message`, `str_join` e `main`.
- Comece de uma cópia limpa do `main.c`, como fará no exame.
- Complete o ficheiro sem consultar a solução de referência.
- Compile com `gcc -Wall -Wextra -Werror`.
- Compare e anote erros de ordem, argumentos, IDs, mensagens e descritores.

### Segunda, 19/10 — Simulado cronometrado

- **Funções:** todas as funções do programa, com atenção especial à integração em `main`.
- Repita o exercício usando o tempo real do exame ou **75 minutos** como referência.
- Faça os testes básicos com dois clientes: chegada, envio de várias linhas e saída.
- Corrija apenas o que falhou e repita esses trechos.

### Terça, 20/10 — Fixação

- **Funções:** todas, em duas reproduções completas iniciadas pelo esqueleto do `main.c`.
- Faça duas reproduções a partir do `main.c`, com uma pausa entre elas.
- Na segunda, use cronómetro e não consulte apontamentos.
- Reserve o fim para compilar e executar os testes básicos.

### Quarta, 21/10 — Dia do exame

- **Revisão:** `main`, `create_socket`, `fatal_error`, `notify`, `add_client`, `remove_client`, `send_msg`, `extract_message` e `str_join`.
- Faça um aquecimento leve de **20 a 30 minutos**: requisitos, mensagens exatas e ciclo `select`.
- No exame, leia o subject antes de copiar o exemplo: confirme funções permitidas e detalhes pedidos.
- Copie o `main.c`, aplique as alterações na sequência treinada e compile.
- Use o tempo restante para verificar argumentos, erros fatais, IDs, prefixos, múltiplas linhas e desconexões.

## Testes rápidos em Linux/WSL

Compile apenas o ficheiro da solução:

```bash
gcc -Wall -Wextra -Werror mini_serv.c -o mini_serv
```

Abra três terminais:

1. Inicie o servidor: `./mini_serv 4242`
2. Em cada um dos outros terminais, ligue um cliente: `nc 127.0.0.1 4242`
3. Verifique o aviso de chegada, envie várias linhas e confirme que o outro cliente recebe cada linha com o prefixo.
4. Feche um cliente e confirme o aviso de saída no que permaneceu ligado.
5. Execute `./mini_serv` sem argumentos e confirme `Wrong number of arguments`.

Não compile `main.c` junto com `mini_serv.c`: o `main.c` é o esqueleto fornecido para copiar e adaptar, não uma unidade adicional do programa final.

## Controle de progresso

- [ ] 15/10 — classificar o `main.c` em manter, alterar e remover
- [ ] 16/10 — reproduzir IDs e processamento de mensagens
- [ ] 17/10 — montar e compilar o ciclo `select`
- [ ] 18/10 — primeiro simulado completo
- [ ] 19/10 — simulado cronometrado e testes básicos
- [ ] 20/10 — duas reproduções a partir do esqueleto
- [ ] 21/10 — aquecimento leve e execução seguindo o subject
