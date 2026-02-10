# Resumo & Guia de Estudo — BSQ e Life (Exam 05)

---

## 1. LIFE — Game of Life

### O que é?
Simulação do **Jogo da Vida de Conway** num grid 2D.

### Protótipo
```
./life <width> <height> <iterations>
```

### Funções permitidas
`atoi, read, putchar, malloc, calloc, realloc, free`

### Como funciona

#### 1) Desenho do padrão inicial (stdin)
Um "cursor" começa no canto superior esquerdo `(0,0)`. Comandos:
| Tecla | Ação |
|-------|------|
| `w` | move cursor para cima |
| `a` | move cursor para esquerda |
| `s` | move cursor para baixo |
| `d` | move cursor para direita |
| `x` | alterna desenho ON/OFF (toggle) |

- Quando o desenho está **ON**, cada posição por onde o cursor passa vira `'0'` (viva).
- Células mortas = `' '` (espaço).

#### 2) Regras do Game of Life (aplicadas `iterations` vezes)
| Condição | Resultado |
|----------|-----------|
| Célula **viva** com **2 ou 3** vizinhos | **sobrevive** |
| Célula **morta** com **exatamente 3** vizinhos | **nasce** |
| Qualquer outro caso | **morre** / continua morta |

- 8 vizinhos (horizontal, vertical, diagonal)
- Células fora do grid = **mortas**
- Todas as transições acontecem **simultaneamente** (usar grid auxiliar)

### Estrutura do código (funções-chave)

```
main(ac, av)
├── create_array(width, height)     → aloca grid de chars inicializado com ' '
├── draw_pattern(arr, width, height)→ lê stdin char a char e desenha
├── game_of_life(arr, w, h)         → 1 iteração (repete N vezes)
│   ├── create_array (grid auxiliar)
│   ├── count_neighbors(arr, w, h, y, x)
│   └── copia new → arr, free new
├── print_array(arr, width, height) → imprime com putchar + '\n'
└── free_array(arr, height)
```

### Pontos de atenção no exame
- **`read(0, &c, 1)`** para ler stdin char a char (não pode usar scanf/getline)
- **Toggle do x**: variável `drawing` alterna entre 0 e 1
- **Bounds check** no movimento: cursor não sai do grid
- **Grid auxiliar** obrigatório na simulação (senão corrompe a contagem de vizinhos)
- **`putchar`** para output (não printf)

### Exemplo mental
```
echo 'dxss' | ./life 3 3 0    →  Desenha coluna vertical no meio
 0
 0
 0

echo 'dxss' | ./life 3 3 1    →  Uma iteração: vira linha horizontal

000

```

---

## 2. BSQ — Biggest Square

### O que é?
Encontrar o **maior quadrado** sem obstáculos num mapa 2D e preenchê-lo.

### Protótipo
```
./bsq [arquivo1] [arquivo2] ...
```
Sem argumentos → lê de stdin.

### Funções permitidas
`malloc, calloc, realloc, free, fopen, fclose, getline, fscanf, fputs, fprintf, printf, stdout, stdin`

### Formato do mapa

**Primeira linha** (separado por espaços):
```
<num_linhas> <char_vazio> <char_obstaculo> <char_cheio>
```
Exemplo: `9 . o x` → 9 linhas, `.` = vazio, `o` = obstáculo, `x` = cheio

**Resto**: grid feito de chars vazio e obstáculo.

### Algoritmo — Programação Dinâmica (DP)

Esta é a parte mais importante. Usa-se uma **tabela DP** do mesmo tamanho do grid:

```
dp[i][j] = tamanho do maior quadrado com canto inferior-direito em (i,j)
```

**Regras de preenchimento:**
```c
if (grid[i][j] == obstáculo)
    dp[i][j] = 0;
else if (i == 0 || j == 0)          // primeira linha ou coluna
    dp[i][j] = 1;
else
    dp[i][j] = min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1;
```

**Após preencher a tabela**, encontra-se a célula com o **maior valor** (mais ao topo, depois mais à esquerda em caso de empate).

**Preencher o quadrado:**
O quadrado vai de `(row - size + 1, col - size + 1)` até `(row, col)`.

### Estrutura do código (funções-chave)

```
main(ac, av)
├── process_file(FILE *fp)
│   ├── parse_first_line(fp, &map)  → fscanf para ler header
│   ├── read_map(fp, &map)          → getline linha a linha + validação
│   ├── create_dp_table(&map)       → aloca int** do tamanho do grid
│   ├── find_biggest_square(&map)   → preenche DP + retorna t_square
│   ├── fill_square(&map, square)   → substitui chars vazios por full
│   ├── print_map(&map)             → printf linha a linha
│   └── free_map(&map)
└── (loop para múltiplos arquivos, ou stdin se ac==1)
```

### Structs usadas
```c
typedef struct s_map {
    char **grid;
    int  **dp;
    int  rows, cols;
    char empty, obstacle, full;
} t_map;

typedef struct s_square {
    int size, row, col;    // canto inferior-direito
} t_square;
```

### Validação do mapa (erros comuns)
- Todas as linhas devem ter o **mesmo comprimento**
- Pelo menos 1 linha com pelo menos 1 célula
- Cada linha termina com `\n`
- Só chars `empty` e `obstacle` no grid
- Os 3 chars (empty, obstacle, full) devem ser **diferentes entre si**
- Mapa inválido → `"Error: invalid map\n"`

### Pontos de atenção no exame
- **`fscanf`** para o header: `fscanf(fp, "%d%c%c%c\n", ...)`
  - Note: chars separados por espaço no arquivo, mas `%c` captura o espaço! Use `" %c"` ou ajuste.
- **`getline`** para ler cada linha do mapa
- **Desempate**: o quadrado mais ao **topo** e mais à **esquerda** ganha (basta usar `>` e não `>=` no tracking)
- **`min3`**: função auxiliar para o mínimo de 3 valores
- **Múltiplos arquivos**: separar saída com `\n` entre cada resultado
- **Memory management**: liberar tudo (grid + dp)

---

## 3. Checklist de Estudo / Treino

### Life
- [ ] Consegue escrever `create_array` e `free_array` de memória?
- [ ] Lembra da lógica do toggle (`x`) e movimentação (`wasd`)?
- [ ] Sabe implementar `count_neighbors` com bounds check?
- [ ] Lembra que precisa de grid auxiliar para cada iteração?
- [ ] Sabe que output é com `putchar`, não printf?

### BSQ
- [ ] Consegue parsear a primeira linha com `fscanf`?
- [ ] Sabe ler o mapa com `getline` e validar?
- [ ] Lembra da fórmula DP: `min(cima, esquerda, diagonal) + 1`?
- [ ] Sabe calcular o canto superior-esquerdo do quadrado resultado?
- [ ] Lembra de tratar stdin quando não há argumentos?
- [ ] Sabe tratar erros de mapa inválido?

---

## 4. Dicas Gerais para o Exame

1. **Comece pelas structs** — defina as estruturas de dados primeiro
2. **Divida em funções pequenas** — parse, process, print, free
3. **Teste com os exemplos do subject** antes de submeter
4. **Memory leaks** — sempre free tudo (use uma função `free_*` dedicada)
5. **Não esqueça edge cases**: grid 1x1, sem obstáculos, tudo obstáculo
6. **Life**: o cursor começa em (0,0) com desenho OFF
7. **BSQ**: primeira linha/coluna do DP é sempre 0 ou 1

---

*Boa sorte no exam! 🍀*
