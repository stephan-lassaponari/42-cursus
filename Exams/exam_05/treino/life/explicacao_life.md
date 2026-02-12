# Game of Life — Explicação da Lógica

## O que é o Game of Life?

O **Game of Life** (Jogo da Vida) é uma simulação criada pelo matemático John Conway.
É um "autômato celular": você tem um tabuleiro (grid) com células que podem estar
**vivas** (`0`) ou **mortas** (espaço ` `), e a cada "geração" (iteração) as células
nascem ou morrem seguindo regras simples.

---

## As 3 Regras do Game of Life

Para cada célula, contamos quantos **vizinhos vivos** ela tem (as 8 casas ao redor):

```
[NW] [N] [NE]
[W]  [X]  [E]     <- X é a célula atual
[SW] [S] [SE]
```

1. **Célula VIVA com 2 ou 3 vizinhos** → continua **viva** (sobrevive)
2. **Célula VIVA com menos de 2 ou mais de 3 vizinhos** → **morre** (solidão ou superpopulação)
3. **Célula MORTA com exatamente 3 vizinhos** → **nasce** (reprodução)

Só isso. Essas 3 regras geram comportamentos incrivelmente complexos.

---

## Como o Programa Funciona (passo a passo)

### 1. `main` — O ponto de entrada

```c
int main(int ac, char **av)
```

O programa recebe **3 argumentos**: `largura`, `altura` e `iterações`.

```
./life 5 5 3
        │ │ └─ quantas gerações simular
        │ └─── altura do tabuleiro
        └───── largura do tabuleiro
```

O fluxo é:
1. Cria o tabuleiro vazio
2. Lê os comandos de desenho do stdin (entrada padrão)
3. Executa N iterações do Game of Life
4. Imprime o resultado final
5. Libera a memória

---

### 2. `create_array` — Criando o tabuleiro

```c
char **create_array(int w, int h)
```

Cria uma **matriz 2D** (array de arrays) com altura `h` e largura `w`.
Cada célula começa com espaço `' '` (morta).

**Como funciona na memória:**
```
arr[0] → [' '][' '][' '][' '][' ']   ← primeira linha
arr[1] → [' '][' '][' '][' '][' ']   ← segunda linha
arr[2] → [' '][' '][' '][' '][' ']   ← terceira linha
...
```

`arr[y][x]` → acessa a célula na linha `y`, coluna `x`.

---

### 3. `draw_pattern` — Desenhando o padrão inicial

```c
void draw_pattern(char **arr, int w, int h)
```

Aqui é onde o programa lê os **comandos do stdin** para desenhar as células vivas
no tabuleiro. Funciona como uma **caneta** que anda pelo grid:

- A caneta começa na posição `(0, 0)` — canto superior esquerdo
- `drawing` começa em `0` (caneta levantada = não desenha)

**Comandos:**
| Tecla | Ação |
|-------|------|
| `w`   | Move a caneta para **cima** (y - 1) |
| `s`   | Move a caneta para **baixo** (y + 1) |
| `a`   | Move a caneta para **esquerda** (x - 1) |
| `d`   | Move a caneta para **direita** (x + 1) |
| `x`   | **Liga/desliga** o modo de desenho (toggle) |

Quando `drawing == 1`, a cada movimento a caneta marca a célula atual como viva (`'0'`).

**Exemplo:** `sdxddssaaww` em um grid 5x5:
```
Começa em (0,0), caneta levantada
s → move para (0,1)
d → move para (1,1)
x → LIGA caneta (agora desenha!)
d → move para (2,1), marca (2,1) como viva
d → move para (3,1), marca (3,1) como viva
s → move para (3,2), marca (3,2) como viva
s → move para (3,3), marca (3,3) como viva
a → move para (2,3), marca (2,3) como viva
a → move para (1,3), marca (1,3) como viva
w → move para (1,2), marca (1,2) como viva
w → move para (1,1), marca (1,1) como viva

Resultado:

 .00.     (. = espaço, 0 = vivo)
 0  0
 .00.

```

Isso forma um quadrado! (Os exemplos do subject usam `O` na saída, mas internamente usamos `0`.)

---

### 4. `count_n` — Contando vizinhos vivos

```c
int count_n(char **arr, int w, int h, int y, int x)
```

Para uma célula em `(y, x)`, esta função conta quantos dos seus **8 vizinhos** estão vivos.

Usa dois loops aninhados com `dy` e `dx` variando de `-1` a `+1`:
```
dy=-1, dx=-1  |  dy=-1, dx=0  |  dy=-1, dx=+1
dy= 0, dx=-1  |  (pula o 0,0) |  dy= 0, dx=+1
dy=+1, dx=-1  |  dy=+1, dx=0  |  dy=+1, dx=+1
```

- Pula `(0, 0)` porque é a própria célula
- Verifica se o vizinho está **dentro dos limites** do tabuleiro
- Se o vizinho for `'0'` (vivo), incrementa o contador

**Células fora do tabuleiro são consideradas mortas** (não contam).

---

### 5. `game_of_life` — A simulação de uma geração

```c
void game_of_life(char **arr, int w, int h)
```

Esta é a função principal da simulação. Para **cada célula** do tabuleiro:

1. Conta os vizinhos vivos com `count_n`
2. Aplica as regras:

```c
if (célula está VIVA) {
    if (vizinhos == 2 || vizinhos == 3)
        continua viva     // sobrevive
    else
        morre             // solidão ou superpopulação
}
else { // célula está MORTA
    if (vizinhos == 3)
        nasce             // reprodução
    else
        continua morta
}
```

**DETALHE IMPORTANTE:** A função cria um **novo tabuleiro** (`new_arr`) para calcular
a próxima geração. Isso é essencial porque todas as células devem ser avaliadas
**ao mesmo tempo**, com base no estado **atual**. Se modificássemos o tabuleiro original
durante o cálculo, as células calculadas depois seriam influenciadas pelas já modificadas.

Depois de calcular tudo, copia o novo tabuleiro de volta para o original e libera a cópia.

---

### 6. `print_array` — Imprimindo o resultado

```c
void print_array(char **arr, int w, int h)
```

Simplesmente percorre o tabuleiro e imprime cada célula com `putchar()`, adicionando
`\n` ao final de cada linha.

---

### 7. `free_array` — Liberando memória

```c
void free_array(char **arr, int h)
```

Libera cada linha (`arr[i]`) e depois o array de ponteiros (`arr`).
Sempre libere na ordem inversa da alocação: primeiro as linhas, depois o array principal.

---

## Fluxo Completo Resumido

```
./life 3 3 2  (com entrada "dxss")

1. Cria grid 3x3 vazio
2. Lê "dxss":
   d → move para (1,0)
   x → liga caneta
   s → move para (1,1), marca viva
   s → move para (1,2), marca viva
   (caneta já estava em (1,0) quando ligou, então (1,0) vira viva também)

   Grid inicial:        Geração 1:         Geração 2:
    . 0 .                . . .              . 0 .
    . 0 .                0 0 0              . 0 .
    . 0 .                . . .              . 0 .

3. Imprime o resultado da geração 2
```

Perceba que a linha vertical vira horizontal e depois volta a ser vertical.
Esse padrão é chamado de **"blinker"** — um dos padrões mais famosos do Game of Life!

---

## Dicas para Entender Melhor

- **Desenhe no papel!** Pegue um grid pequeno (3x3 ou 5x5), preencha algumas
  células e aplique as regras manualmente. Ajuda muito.
- O ponto mais difícil de entender costuma ser o `draw_pattern`. Trace os
  comandos um por um com papel e caneta.
- O `count_n` é um padrão comum: varrer vizinhos com delta (-1, 0, +1).
  Memorize esse pattern, ele aparece em muitos problemas.
- Sempre crie uma **cópia** do tabuleiro para calcular a próxima geração.
  Nunca modifique o original durante o cálculo.
