#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>

typedef struct s_client
{
	int id;
	char msg[300000];
}	t_client;

t_client clients[1024];
char sbuff[400000], rbuff[300000];
int maxfd = 0, gid = 0;
fd_set wfd, rfd, curr;

void err(char *msg)
{
	if (msg)
		write(2, msg, strlen(msg));
	else
		write(2, "Fatal error", 11);
	write(2, "\n", 1);
	exit(1);
}

void send_all(int except)
{
	for(int fd = 0; fd <= maxfd; fd++)
		if(FD_ISSET(fd, &wfd) && fd != except)
			send(fd, sbuff, strlen(sbuff), 0);
}

int create_server(char *port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1) err(NULL);
	maxfd = fd;

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(port));

	if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		err(NULL);
	if(listen(fd, 100) == -1)
		err(NULL);

	FD_ZERO(&curr);
	FD_SET(fd, &curr);
	bzero(clients, sizeof(clients));
	return fd;
}

void add_client(int serverfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int fd = accept(serverfd, (struct sockaddr *)&addr, &len);
	if(fd == -1) return;

	if(fd > maxfd) maxfd = fd;
	clients[fd].id = gid++;
	FD_SET(fd, &curr);
	sprintf(sbuff, "server: client %d just arrived\n", clients[fd].id);
	send_all(fd);
}

void remove_client(int fd)
{
	sprintf(sbuff, "server: client %d just left\n", clients[fd].id);
	send_all(fd);
	FD_CLR(fd, &curr);
	close(fd);
	bzero(clients[fd].msg, strlen(clients[fd].msg));
}

void handle_msg(int fd)
{
	int ret = recv(fd, rbuff, sizeof(rbuff), 0);
	if(ret <= 0)
	{
		remove_client(fd);
		return;
	}

	for(int i = 0, j = strlen(clients[fd].msg); i < ret; i++, j++)
	{
		clients[fd].msg[j] = rbuff[i];
		if(clients[fd].msg[j] == '\n')
		{
			clients[fd].msg[j] = '\0';
			sprintf(sbuff, "client %d: %s\n", clients[fd].id, clients[fd].msg);
			send_all(fd);
			bzero(clients[fd].msg, strlen(clients[fd].msg));
			j = -1;
		}
	}
}

int main(int ac, char **av)
{
	if (ac != 2)
		err("Wrong number of arguments");

	int serverfd = create_server(av[1]);

	while(1)
	{
		wfd = rfd = curr;
		if(select(maxfd + 1, &rfd, &wfd, 0, 0) == -1)
			continue;

		for(int fd = 0; fd <= maxfd; fd++)
		{
			if(FD_ISSET(fd, &rfd))
			{
				if(fd == serverfd)
					add_client(serverfd);
				else
					handle_msg(fd);
				break;
			}
		}
	}
	return 0;
}
