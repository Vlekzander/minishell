/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:46:16 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 13:10:12 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdlib.h>
#include "utils.h"

static void	lst_swap_content(t_list *a, t_list *b)
{
	void	*tmp;

	if (a == NULL || b == NULL)
		return ;
	tmp = a->content;
	a->content = b->content;
	b->content = tmp;
}

static void	sort_ascii_list(t_list *lst)
{
	t_list	*a;
	t_list	*b;

	if (lst == NULL)
		return ;
	a = lst;
	while (a != NULL)
	{
		b = a->next;
		while (b)
		{
			if (ft_strncmp(a->content, b->content, 255) > 0)
				lst_swap_content(a, b);
			b = b->next;
		}
		a = a->next;
	}
}

t_error	loop_entries(t_list **content, DIR *dir)
{
	struct dirent	*entry;
	t_list			*node;
	char			*str;

	if (content == NULL || dir == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (ft_strncmp(entry->d_name, ".", 2) == 0
			|| ft_strncmp(entry->d_name, "..", 3) == 0)
		{
			entry = readdir(dir);
			continue ;
		}
		str = ft_strdup(entry->d_name);
		if (str == NULL)
			return (error(ERR_ALLOCATION, NULL));
		node = ft_lstnew(str);
		if (node == NULL)
			return (free(str), error(ERR_ALLOCATION, NULL));
		ft_lstadd_back(content, node);
		entry = readdir(dir);
	}
	return (error(ERR_NONE, NULL));
}

t_error	scan_dir(t_list **content, char *path)
{
	DIR		*dir;
	t_error	err;

	if (content == NULL || path == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*content = NULL;
	dir = opendir(path);
	if (dir == NULL)
		return (error(ERR_ERRNO, path));
	err = loop_entries(content, dir);
	if (err.id != ERR_NONE)
	{
		ft_lstclear(content, free);
		*content = NULL;
	}
	sort_ascii_list(*content);
	if (closedir(dir) == -1)
		return (error(ERR_ERRNO, path));
	return (err);
}
