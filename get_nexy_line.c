/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_nexy_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:50:27 by zfarouk           #+#    #+#             */
/*   Updated: 2024/11/20 18:01:24 by zfarouk          ###   ########.fr       */
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

void append(t_list **lst, t_list *node)
{
	t_list *current;
	
	if (*lst == NULL)
	{
		*lst = node;
		return;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = node;
}

t_list *new_node(int fd)
{
	t_list *lst;
	char *buffer;
	ssize_t i;

	lst = malloc(sizeof(t_list));
	if (!lst)
		return NULL;
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return NULL;
	i = read(fd, buffer, BUFFER_SIZE);
	// endoffile or -1 cus i can't read the file;
	if (i <= 0)
	{
		free(buffer);
		free(lst);
		return NULL;
	}
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

void make(int fd, t_list **the_line)
{
	t_list *current;
	t_list *node;

	current = *the_line;
	//gha galhali chat current li lta7
	if (current && current->word)
		current = current->next;
	while (check(current))
	{
		node = new_node(fd);
		if (!node)
			return;
		append(the_line, node);
		current = current->next;
	}
}


int size_lst(t_list *lst)
{
	int i;
	int len;

	i = 0;
	len = 0;
	while (lst)
	{
		while (lst->word[i])
		{
			if (lst->word[i] == '\n')
			{
				len++;
				return (len);
			}
			i++;
			len++;
		}
		i = 0;
		lst = lst->next;
	}
	return (len);
}


void squeezer(t_list *lst, char **the_bottom_line)
{
	int i;
	int len;

	len = size_lst(lst);
	*the_bottom_line = malloc(len + 1);
	if (!*the_bottom_line)
		return ;
	len = 0;
	while (lst != NULL)
	{
		i = 0;
		while (lst->word[i])
		{
			if (lst->word[i] == '\n')
			{
				*the_bottom_line[len++] = '\n';
				*the_bottom_line[len] = '\0';
				return;
			}
			*the_bottom_line[len++] = lst->word[i++];
		}
		lst = lst->next;
	}
}

void	clear(t_list **lst)
{
	t_list	*current;
	t_list	*the_next;

	if (!*lst || !lst)
		return ;
	current = *lst;
	while (current)
	{
		the_next = current->next;
		free(current->word);
		free(current);
		current = the_next;
	}
	*lst = NULL;
}

void rest(t_list *lst, char **remains)
{
	int len;
	int i;
	int j;

	while (lst->next)
		lst = lst->next;
	i = 0;
	while (lst->word[i] && lst->word[i] != '\n')
		i++;
	if (!lst->word[i]) {
        *remains = NULL;
        return;
    }
	len = size_lst(lst);
	*remains = malloc(len - i);
	if (!*remains)
        return;
	j = 0;
	if(lst->word[i] == '\n')
	{
		i++;
		while(lst->word[i])
			*remains[j++] = lst->word[i++];
		*remains[j] = '\0';
	}
}

char *get_next_line(int fd)
{
	static t_list *the_line = NULL;
	char *the_bottom_line;
	t_list *current;
	char *remains;

	if (fd < 0 || BUFFER_SIZE <= 0)
        return NULL;

	make(fd, &the_line);
	if (!the_line)
		return NULL;
	squeezer(the_line, &the_bottom_line);
	rest(the_line, &remains);
	clear(&the_line);
	if (remains) {
    	the_line = malloc(sizeof(t_list));
    	if (!the_line)
		{
			free(remains);
			return NULL;
		}
		the_line->word = remains;
    	the_line->next = NULL;
	}
	return the_bottom_line;
}


int    main(void)
{
    int        fd;
    char    *buffer;

    fd = open("text.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("open failed\n");
        return (0);
    }
    while ((buffer = get_next_line(fd)) != 0)
    {
        printf("%s", buffer);
        free(buffer);
    }
    close(fd);
}



























