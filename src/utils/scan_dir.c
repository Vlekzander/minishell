/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:46:16 by apierret          #+#    #+#             */
/*   Updated: 2025/08/04 19:50:00 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include "utils.h"

static int	ft_strncmp_case(char *s1, char *s2, size_t n)
{
	size_t	i;
	char	a;
	char	b;

	if (n == 0)
		return (0);
	if (s1 == NULL || s2 == NULL)
		return (INT_MAX);
	a = 0;
	b = 0;
	i = 0;
	while (i < n -1 && s1[i] != '\0' && s2[i] != '\0')
	{
		a = (char) ft_tolower(s1[i]);
		b = (char) ft_tolower(s2[i]);
		if (a != b)
			break ;
		i++;
	}
	return ((unsigned char) a - (unsigned char) b);
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
	lst_sort(*content, ft_strncmp_case);
	if (closedir(dir) == -1)
		return (error(ERR_ERRNO, path));
	return (err);
}
