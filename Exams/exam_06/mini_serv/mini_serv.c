#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

int count = 0, max_fd = 0;
int ids[70000];
char *msgs[70000];
fd_set rfd, wfd, afd;
char buf_read[1001], buf_write[42];

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void	fatal_error()
{
	write(2, "Fatal error\n", 12);
	exit(1);
}

void	notify(int author, char *str)
{
	for (int fd = 0; fd <= max_fd; fd++)
		if (FD_ISSET(fd, &wfd) && fd != author)
			send(fd, str, strlen(str), 0);
}

void	add_client(int fd)
{
	max_fd = fd > max_fd ? fd : max_fd;
	msgs[fd] = NULL;
	ids[fd] = count++;
	FD_SET(fd, &afd);
	sprintf(buf_write, "server: client %d just arrived\n", ids[fd]);
	notify(fd, buf_write);
}

void	remove_client(int fd)
{
	sprintf(buf_write, "server: client %d just left\n", ids[fd]);
	notify(fd, buf_write);
	FD_CLR(fd, &afd);
	free(msgs[fd]);
	close(fd);
}

void	send_msg(int fd)
{
	char *msg;

	while(extract_message(&(msgs[fd]), &msg))
	{
		sprintf(buf_write, "client %d: ", ids[fd]);
		notify(fd, buf_write);
		notify(fd, msg);
		free(msg);
	}
}

int	create_socket(void)
{
	max_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (max_fd < 0)
		fatal_error();
	FD_SET(max_fd, &afd);
	return (max_fd);
}

int main(int ac, char **av) {
	if (ac != 2)
	{
		write(2, "Wrong number of arguments\n", 26);
		exit(1);
	}
	int sockfd;
	struct sockaddr_in servaddr;

	FD_ZERO(&afd);
	sockfd = create_socket();
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1]));

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		fatal_error();
	if (listen(sockfd, 10) != 0)
		fatal_error();

	while(1)
	{
		rfd = wfd = afd;
		if (select(max_fd + 1, &rfd, &wfd, NULL, NULL) < 0)
			fatal_error();
		for (int fd = 0; fd <= max_fd; fd++)
		{
			if (!FD_ISSET(fd, &rfd))
				continue;
			if (fd == sockfd)
			{
				socklen_t len = sizeof(servaddr);
				int connfd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
				if (connfd >= 0)
				{
					add_client(connfd);
					break;
				}
			}
			else
			{
				int r_byte = recv(fd, buf_read, 1000, 0);
				if (r_byte <= 0)
				{
					remove_client(fd);
					break;
				}
				buf_read[r_byte] = 0;
				msgs[fd] = str_join(msgs[fd], buf_read);
				send_msg(fd);
			}
		}
	}

}
