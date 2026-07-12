# MINI_SERV — CHEATSHEET (A4 ÷ 4)

---

## PARTE 1 — Includes, Structs, Globais, err(), send_all()

```c
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>

typedef struct s_client { int id; char msg[300000]; } t_client;

t_client clients[1024];         // clientes indexados pelo fd
char sbuff[400000], rbuff[300000]; // send buffer / recv buffer
int maxfd = 0, gid = 0;         // maior fd ativo / id global crescente
fd_set wfd, rfd, curr;          // write/read/set atual dos fds

void err(char *msg) {
    write(2, msg ? msg : "Fatal error", msg ? strlen(msg) : 11);
    write(2, "\n", 1); exit(1);
}

void send_all(int except) {     // envia sbuff p/ todos EXCETO 'except'
    for(int fd = 0; fd <= maxfd; fd++)
        if(FD_ISSET(fd, &wfd) && fd != except)
            send(fd, sbuff, strlen(sbuff), 0);
}
```
**Notas:**
- `sbuff` = o que vai ser enviado; `rbuff` = o que foi recebido
- `curr` = conjunto permanente de fds; `rfd`/`wfd` = cópias temporárias p/ select
- `send_all` usa `wfd` (quem está pronto p/ escrita) — vem do select

---

## PARTE 2 — create_server()

```c
int create_server(char *port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1) err(NULL);
    maxfd = fd;

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    addr.sin_port        = htons(atoi(port));

    if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) err(NULL);
    if(listen(fd, 100) == -1) err(NULL);

    FD_ZERO(&curr);
    FD_SET(fd, &curr);          // adiciona serverfd ao conjunto
    bzero(clients, sizeof(clients));
    return fd;
}
```
**Notas:**
- Fluxo obrigatório: `socket → bind → listen`
- `htonl` / `htons` = host-to-network (byte order) para endereço e porta
- `INADDR_LOOPBACK` = loopback (127.0.0.1); usar `htonl` nele
- Só `curr` é inicializado aqui; `rfd`/`wfd` são copiados no loop do main

---

## PARTE 3 — add_client(), remove_client()

```c
void add_client(int serverfd) {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int fd = accept(serverfd, (struct sockaddr *)&addr, &len);
    if(fd == -1) return;

    if(fd > maxfd) maxfd = fd;  // atualiza maxfd se necessário
    clients[fd].id = gid++;     // atribui id único
    FD_SET(fd, &curr);          // adiciona ao conjunto permanente
    sprintf(sbuff, "server: client %d just arrived\n", clients[fd].id);
    send_all(fd);               // avisa todos (menos o que chegou)
}

void remove_client(int fd) {
    sprintf(sbuff, "server: client %d just left\n", clients[fd].id);
    send_all(fd);               // avisa antes de remover
    FD_CLR(fd, &curr);          // remove do conjunto permanente
    close(fd);
    bzero(clients[fd].msg, strlen(clients[fd].msg)); // limpa buffer
}
```
**Notas:**
- `accept` retorna um novo fd para o cliente (diferente do serverfd)
- Sempre avisar ANTES de remover (send_all usa curr, então fd ainda está nele)
- `bzero(clients[fd].msg, ...)` limpa o buffer parcial de mensagem

---

## PARTE 4 — handle_msg(), main()

```c
void handle_msg(int fd) {
    int ret = recv(fd, rbuff, sizeof(rbuff), 0);
    if(ret <= 0) { remove_client(fd); return; }

    for(int i = 0, j = strlen(clients[fd].msg); i < ret; i++, j++) {
        clients[fd].msg[j] = rbuff[i];
        if(clients[fd].msg[j] == '\n') {
            clients[fd].msg[j] = '\0';  // termina a string
            sprintf(sbuff, "client %d: %s\n", clients[fd].id, clients[fd].msg);
            send_all(fd);
            bzero(clients[fd].msg, strlen(clients[fd].msg));
            j = -1;  // reset (vai virar 0 no i++,j++)
        }
    }
}

int main(int ac, char **av) {
    if(ac != 2) err("Wrong number of arguments");
    int serverfd = create_server(av[1]);

    while(1) {
        wfd = rfd = curr;       // copia curr p/ rfd e wfd a cada iteração
        if(select(maxfd + 1, &rfd, &wfd, 0, 0) == -1) continue;

        for(int fd = 0; fd <= maxfd; fd++) {
            if(FD_ISSET(fd, &rfd)) {    // fd pronto p/ leitura?
                if(fd == serverfd) add_client(serverfd);
                else               handle_msg(fd);
                break;              // processa 1 por vez (evita invalidar set)
            }
        }
    }
}
```
**Notas:**
- `recv <= 0` = cliente desconectou (ou erro) → remove
- `j = strlen(clients[fd].msg)` continua de onde parou (msg pode vir em partes)
- `j = -1` porque o `j++` do for vai executar logo após, fazendo j=0
- `break` após processar: curr pode ter mudado (add/remove), recomeçar o loop
- `select` modifica `rfd`/`wfd` — por isso copiar `curr` todo loop
