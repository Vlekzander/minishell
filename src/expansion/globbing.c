/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:20:14 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 16:29:15 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "expansion.h"
#include "utils.h"

char	*ft_strrstr(char *str, char *to_find)
{
	char	*last;
	char	*ptr;

	if (str == NULL || to_find == NULL)
		return (NULL);
	last = NULL;
	ptr = ft_strstr(str, to_find);
	while (ptr != NULL)
	{
		last = ptr;
		ptr++;
		ptr = ft_strstr(ptr, to_find);
	}
	return (last);
}

static int	check_infixes(char **str, t_pattern *pattern)
{
	t_list	*infixe;
	char	*ptr;

	if (str == NULL || pattern == NULL)
		return (-1);
	ptr = *str;
	if (pattern->infixes != NULL)
	{
		infixe = pattern->infixes;
		while (infixe != NULL && ptr != NULL)
		{
			ptr = ft_strstr(ptr, infixe->content);
			if (ptr != NULL)
				ptr += ft_strlen(infixe->content);
			infixe = infixe->next;
		}
		if (ptr == NULL)
			return (0);
		*str = ptr;
	}
	return (1);
}

static int	check_pattern(char *str, t_pattern *pattern)
{
	char	*ptr;
	int		ret;

	if (str == NULL || pattern == NULL)
		return (-1);
	ptr = str;
	if (pattern->prefix != NULL)
	{
		ptr = ft_strstr(ptr, pattern->prefix);
		if (ptr != str)
			return (0);
		ptr += ft_strlen(pattern->prefix);
	}
	ret = check_infixes(&ptr, pattern);
	if (ret != 1)
		return (ret);
	if (pattern->suffix != NULL)
	{
		ptr = ft_strrstr(ptr, pattern->suffix);
		if (ptr == NULL || ft_strlen(ptr) != ft_strlen(pattern->suffix))
			return (0);
	}
	return (1);
}

t_error	globbing(t_list **files, t_pattern *pattern, int include)
{
	t_list	*node;
	int		ret;

	if (files == NULL || pattern == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	node = *files;
	while (node != NULL)
	{
		ret = check_pattern(node->content, pattern);
		if (ret == -1)
			return (error(ERR_IMPLEMENTATION, NULL));
		if (ret == include)
			((char *) node->content)[0] = '\0';
		node = node->next;
	}
	lst_remove(files, str_empty, free);
	return (error(ERR_NONE, NULL));
}
