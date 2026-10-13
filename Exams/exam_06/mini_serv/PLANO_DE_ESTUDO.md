# Plano de memorização — `mini_serv`

**Período:** terça-feira, 13/10, a quarta-feira, 21/10
**Meta:** conseguir escrever de memória a versão de `mini_serv.c` que já foi entregue, compilar e fazer os testes básicos. O plano é de memorização e reprodução; não exige estudar a teoria do código.

## Método para cada bloco

Use sempre o mesmo ciclo:

1. Leia o bloco no arquivo original por 2 minutos.
2. Feche ou cubra o original e escreva o bloco do zero, no papel ou num arquivo de treino.
3. Compare com o original e anote somente o que esqueceu ou escreveu diferente.
4. Corrija esses pontos e escreva o bloco novamente sem olhar.
5. No começo da sessão seguinte, tente reproduzir os blocos anteriores antes de consultar o original.

**Não edite nem substitua o `mini_serv.c` original durante os treinos.** Use papel ou um arquivo separado. Tente escrever cada bloco corretamente três vezes antes de avançar.

## Blocos do arquivo

Memorize nesta ordem, usando os nomes das funções como pistas:

1. Includes, `t_client`, variáveis globais e `err`.
2. `send_all` e `create_server`.
3. `add_client` e `remove_client`.
4. `handle_msg`.
5. `main`.

Não tente decorar o arquivo inteiro de uma vez. A cada dia, some o bloco novo aos anteriores.

## Rotina diária sugerida

Reserve cerca de **1h30 por dia**:

- **10 min:** escreva de memória o que estudou no dia anterior.
- **25 min:** treine o bloco novo ou o ponto que mais errou.
- **5 min:** pausa.
- **25 min:** reproduza todos os blocos já estudados, em sequência e sem consultar.
- **15 min:** compare com o original, corrija os erros e repita os trechos errados.
- **10 min:** diga em voz alta a ordem dos blocos e os nomes das funções, sem explicar a teoria.

Nos dias de simulado, use mais tempo se precisar e faça uma pausa entre as tentativas.

## Calendário

### Terça, 13/10 — Preparação e primeiro bloco

- Separe o original para consulta e prepare papel ou um arquivo de treino em branco.
- No WSL, vá até a pasta e compile o original:
  ```bash
  cd /mnt/c/Users/steph/Documents/mini_serv
  gcc -Wall -Wextra -Werror mini_serv.c -o mini_serv
  ```
- Treine o **bloco 1** até escrevê-lo corretamente três vezes.
- Faça uma tentativa final do bloco 1 sem olhar e encerre a sessão.

### Quarta, 14/10 — `send_all` e `create_server`

- Comece reproduzindo o bloco 1 sem consultar.
- Estude e treine o **bloco 2** pelo ciclo de memorização.
- Escreva os blocos 1 e 2 juntos, na ordem, duas vezes.
- Se esquecer algo, anote o trecho exato e repita esse trecho antes de encerrar.

### Quinta, 15/10 — Entrada e saída de clientes

- Reproduza os blocos 1 e 2 sem olhar.
- Estude e treine o **bloco 3**: `add_client` e `remove_client`.
- Escreva os blocos 1 a 3 em sequência.
- Termine com uma tentativa de memória e compare só no final.

### Sexta, 16/10 — `handle_msg`

- Reproduza os blocos 1 a 3 sem consultar.
- Estude e treine o **bloco 4**.
- Escreva os blocos 1 a 4 juntos.
- Repita separadamente as linhas que confundiu ou esqueceu.

### Sábado, 17/10 — `main` e primeira reprodução completa

- Faça uma revisão curta dos blocos 1 a 4.
- Estude e treine o **bloco 5**, `main`.
- Escreva o arquivo inteiro de memória, sem cronômetro.
- Compare com o original e marque os erros por bloco; não recomece o arquivo inteiro por causa de um erro localizado.

### Domingo, 18/10 — Primeiro simulado completo

- Sem consultar o original, escreva o arquivo inteiro do zero.
- Depois compare e conte os erros: nomes, ordem, argumentos e linhas esquecidas.
- Treine apenas os blocos com erros e faça uma segunda tentativa completa.
- Se ainda estiver difícil, use o resto da sessão para repetir os blocos mais fracos, não para reler tudo passivamente.

### Segunda, 19/10 — Simulado com tempo

- Faça uma reprodução completa usando um cronômetro. Se souber o tempo do exame, use esse tempo; se não souber, use **75 minutos**.
- Compile a reprodução no WSL:
  ```bash
  gcc -Wall -Wextra -Werror mini_serv_treino.c -o mini_serv_treino
  ```
- Corrija os erros e faça mais uma tentativa cronometrada, concentrando-se nos trechos que falharam.

### Terça, 20/10 — Fixação final

- Faça duas reproduções completas, com uma pausa entre elas.
- Na primeira, pode consultar o original somente depois de terminar.
- Na segunda, não consulte nada e compile ao final.
- Revise apenas a sua lista de erros. Não tente mudar ou “melhorar” a versão que já foi entregue.

### Quarta, 21/10 — Dia do exame

- Faça um aquecimento leve de **20 a 30 minutos**: escreva de memória a ordem dos blocos e os trechos que costumam escapar.
- Se houver tempo, reproduza uma vez as funções mais difíceis; não faça uma maratona nem tente aprender conteúdo novo.
- No exame, escreva os blocos na ordem treinada e reserve os minutos finais para comparar nomes, chaves, parênteses e ponto e vírgula.

## Teste rápido no WSL

Faça este teste em casa depois de compilar. Use três terminais WSL:

1. No primeiro, inicie o servidor:
   ```bash
   ./mini_serv 4242
   ```
2. No segundo e no terceiro, conecte dois clientes:
   ```bash
   nc 127.0.0.1 4242
   ```
3. Escreva `oi` no segundo cliente e pressione Enter. Confira se o outro cliente recebe a mensagem.
4. Feche um cliente e confira se o outro recebe o aviso de saída.

Também teste a execução sem argumentos:

```bash
./mini_serv
```

Ela deve mostrar `Wrong number of arguments`. Não compile `main.c` junto com `mini_serv.c`; para este exercício, pratique o arquivo `mini_serv.c`.

Se o comando `nc` não existir no WSL, instale-o uma vez com:

```bash
sudo apt update
sudo apt install netcat-openbsd
```

## Controle de progresso

- [ ] 13/10 — bloco 1 escrito de memória três vezes
- [ ] 14/10 — blocos 1 e 2 escritos juntos
- [ ] 15/10 — blocos 1 a 3 escritos juntos
- [ ] 16/10 — blocos 1 a 4 escritos juntos
- [ ] 17/10 — primeira reprodução integral
- [ ] 18/10 — simulado integral sem consulta
- [ ] 19/10 — simulado integral cronometrado e compilado
- [ ] 20/10 — duas reproduções integrais
- [ ] 21/10 — aquecimento leve antes do exame
