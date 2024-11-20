/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_nexy_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:50:27 by zfarouk           #+#    #+#             */
/*   Updated: 2024/11/20 00:19:18 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>

typedef struct s_list
{
	char		*word;
	struct s_list	*next;
}					t_list;

#define BUFFER_SIZE 10

void append(t_list *lst, t_list *node)
{
	t_list *current;
	
	if (lst == NULL)
	{
		lst = node;
		return;
	}
	current = lst;
	while (current->next)
		current = current->next;
	current->next = node;
}

t_list *new_node(fd)
{
	t_list *lst;
	char *buffer;
	ssize_t i;

	buffer = malloc(BUFFER_SIZE + 1);
	i = read(fd, buffer, BUFFER_SIZE);
	buffer[i] = '\0';
	lst->word = buffer;
	lst->next = NULL;
	return (lst);
}

int check(t_list *node)
{
	 int i;

	 i = 0;
	 if (node->word == NULL)
		 return (1);
	 while (node->word[i])
	 {
		 if (node->word[i] == '\n')
			 return 0;
		 i++;
	 }
	 return (1);
}

t_list *make(int fd, t_list *the_line)
{
	t_list *current;
	t_list *node;

	current = the_line;
	if (current->word)
		current = current->next;
	while (check(current))
	{
		node = new_node(fd);
		append(the_line, node);
		current = current->next;
	}
	return (current);
}

int print(t_list *lst)
{
	t_list *current;
	int i;

	i = 0;
	while (current != NULL)
	{
		i = 0;
		while (current->word[i])
		{
			if (current->word[i] == '\n')
			{
				write(1, &current->word[i], 1);
				return (i);
			}
			write(1, &current->next[i], 1);
			i++;
		}
		current = current->next;
	}
	return (i);
}

void clear(t_list *lst)
{
	t_list *current;
	
	current = lst;
	while(lst)
	{
		current = lst->next;
		free(lst->word);
		free(lst);
		lst = current;
	}
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*p;

	if (!s)
		return (NULL);
	p = malloc((len + 1) * sizeof(char));
	if (!p)
		return (NULL);
	i = 0;
	while (i < len)
	{
		p[i] = s[start + i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

void get_next_line(int fd)
{
	static t_list *the_line;
	char *rest;
	t_list *current;
	int index;

	current = make(fd, the_line);
	index = print(the_line);
	clear(the_line);
	if (index <= BUFFER_SIZE)
	{
		rest = ft_substr(current->word, index + 1, BUFFER_SIZE - index - 1);
		the_line->word = rest;
		the_line->next = NULL;
	}

}
int main()
{
	int i = open("text", O_RDWR);
	get_next_line(i);
}



























