/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zfarouk <zfarouk@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:54:58 by zfarouk           #+#    #+#             */
/*   Updated: 2024/11/28 14:55:09 by zfarouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	size_lst(t_list *lst)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (lst)
	{
		i = 0;
		while (lst->word && lst->word[i])
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
	buffer = malloc(len + 1);
	if (!buffer)
		return (NULL);
	len = 0;
	while (lst != NULL)
	{
		i = 0;
		while (lst->word && lst->word[i])
		{
			if (lst->word[i] == '\n')
			{
				buffer[len++] = '\n';
				buffer[len++] = '\0';
				return (buffer);
			}
			buffer[len++] = lst->word[i++];
		}
		lst = lst->next;
	}
	return (buffer[len++] = '\0', buffer);
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
	j = 0;
	len = i + 1;
	while (lst->word[++i])
		j++;
	remains = malloc(j + 1);
	if (!remains)
		return (NULL);
	j = 0;
	while (lst->word[len])
		remains[j++] = lst->word[len++];
	return (remains[j] = '\0', remains);
}

char	*get_next_line(int fd)
{
	static t_list	*the_line[FD_SETSIZE];
	char			*the_bottom_line;
	char			*remains;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > FD_SETSIZE)
		return (NULL);
	make(fd, &the_line[fd]);
	if (!the_line[fd])
		return (NULL);
	the_bottom_line = squeezer(the_line[fd]);
	remains = rest(the_line[fd]);
	clear(&the_line[fd]);
	if (remains)
	{
		the_line[fd] = malloc(sizeof(t_list));
		if (!the_line[fd])
		{
			free(remains);
			return (NULL);
		}
		the_line[fd]->word = remains;
		the_line[fd]->next = NULL;
	}
	return (the_bottom_line);
}
