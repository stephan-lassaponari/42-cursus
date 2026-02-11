# Life - Resumo para Papel

## Estrutura geral
```
args: ./life width height iterations
funções permitidas: atoi, read, putchar, malloc, calloc, realloc, free
includes: stdlib.h, unistd.h, stdio.h
```

## Regras do Game of Life
- Viva + 2 ou 3 vizinhos → **sobrevive**
- Morta + exatamente 3 vizinhos → **nasce**
- Qualquer outro caso → **morre/continua morta**

---

## 1. main(ac, av)
```
se ac != 4 → return 1
w = atoi(av[1]), h = atoi(av[2]), it = atoi(av[3])
arr = create_array(w, h)
draw_pattern(arr, w, h)
loop i < it: game_of_life(arr, w, h)
print_array(arr, w, h)
free_array(arr, h)
```

## 2. create_array(w, h) → char**
```
arr = malloc(char* × h)
para cada linha i:
  arr[i] = malloc(w)
  preenche com ' '
return arr
```

## 3. free_array(arr, h)
```
para cada i: free(arr[i])
free(arr)
```

## 4. print_array(arr, w, h)
```
para cada i (linha):
  para cada j (col): putchar(arr[i][j])
  putchar('\n')
```

## 5. draw_pattern(arr, w, h)
```
x=0, y=0, drawing=0
enquanto read(0, &c, 1) > 0:
  'x' → drawing = !drawing
  'w' → y > 0 ? y--
  's' → y < h-1 ? y++
  'a' → x > 0 ? x--
  'd' → x < w-1 ? x++
  se drawing E posição válida → arr[y][x] = '0'
```

## 6. count_neighbors(arr, w, h, y, x) → int
```
count = 0
dy de -1 a 1, dx de -1 a 1:
  pula se dy==0 E dx==0
  ny = y+dy, nx = x+dx
  se ny e nx dentro dos limites E arr[ny][nx] == '0':
    count++
return count
```

## 7. game_of_life(arr, w, h)
```
new = create_array(w, h)
para cada célula [i][j]:
  n = count_neighbors(arr, w, h, i, j)
  se arr[i][j] == '0':       (viva)
    n==2 ou n==3 → new='0', senão new=' '
  senão:                      (morta)
    n==3 → new='0', senão new=' '
copia new → arr
free_array(new, h)
```

---

## Fluxo resumido
```
main → cria grid → lê comandos (draw) → aplica N iterações → imprime → libera
```

## Dicas de escrita
- '0' = célula viva, ' ' = célula morta
- Vizinhos: 8 direções (dy,dx de -1 a 1, menos 0,0)
- Fora do grid = morto
- draw: x liga/desliga caneta, wasd movimenta
