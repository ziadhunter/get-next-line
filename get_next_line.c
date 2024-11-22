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

#include "get_next_line.h"

void	make(int fd, t_list **the_line)
{
	t_list	*node;

	// t_list *current;
	while (check(*the_line))
	{
		node = new_node(fd);
		if (!node)
		{
			// current = *the_line;
			// while (current)
			// {
			// 	node = current->next;
			// 	free(current->word);
			// 	free(current);
			// 	current = node;
			// }
			// *the_line = NULL;
			return ;
		}
		append(the_line, node);
	}
}

int	size_lst(t_list *lst)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (lst)
	{
		i = 0;
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
		lst = lst->next;
	}
	return (len);
}

char	*squeezer(t_list *lst)
{
	int		i;
	int		len;
	char	*buffer;

	len = size_lst(lst);
	// printf("%d", len);
	buffer = malloc(len + 1 + 1);
	if (!buffer)
		return (NULL);
	len = 0;
	while (lst != NULL)
	{
		i = 0;
		while (lst->word[i])
		{
			if (lst->word[i] == '\n')
			{
				buffer[len++] = '\n';
				buffer[len++] = '\0';
				return (buffer);
			}
			// printf(" %d ",len);
			buffer[len++] = lst->word[i++];
		}
		lst = lst->next;
	}
	buffer[len++] = '\0';
	return (buffer);
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

char	*rest(t_list *lst)
{
	int		len;
	int		i;
	int		j;
	char	*remains;

	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	i = 0;
	while (lst->word[i] && lst->word[i] != '\n')
		i++;
	if (!lst->word[i] || lst->word[i + 1] == '\0')
		return (NULL);
	j = i + 1;
	while (lst->word[j])
		j++;
	len = size_lst(lst);
	remains = malloc(j + 1);
	if (!remains)
		return (NULL);
	j = 0;
	if (lst->word[i] == '\n')
	{
		i++;
		while (lst->word[i])
			remains[j++] = lst->word[i++];
		remains[j] = '\0';
	}
	return (remains);
}

char	*get_next_line(int fd)
{
	static t_list	*the_line = NULL;
	char			*the_bottom_line;
	char			*remains;
	char			test[1];

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, test, 0) < 0)
		return (NULL);
	make(fd, &the_line);
	if (!the_line)
		return (NULL);
	the_bottom_line = squeezer(the_line);
	remains = rest(the_line);
	clear(&the_line);
	if (remains)
	{
		the_line = malloc(sizeof(t_list));
		if (!the_line)
		{
			free(remains);
			return (NULL);
		}
		the_line->word = remains;
		the_line->next = NULL;
	}
	return (the_bottom_line);
}

// int main(void)
// {
//     int        fd;
//     char    *buffer;

//     fd = open("text.txt", O_RDONLY);
//     if (fd == -1)
//     {
//         printf("open failed\n");
//         return (0);
//     }
//     while ((buffer = get_next_line(fd)) != 0)
//     {
//         printf("%s", buffer);
//         free(buffer);
//     }
//     close(fd);
// }

// int main()
// {
// 	x();
// 	system("leaks a.out");
// }
