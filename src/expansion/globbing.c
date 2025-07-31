/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:20:14 by apierret          #+#    #+#             */
/*   Updated: 2025/07/30 16:04:40 by apierret         ###   ########.fr       */
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
	if (pattern->prefix == NULL && str[0] == '.')
		return (0);
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

static t_error	lst_dup(t_list **dest, t_list *base)
{
	t_list	*node;
	char	*str;

	if (dest == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	while (base != NULL)
	{
		str = ft_strdup(base->content);
		if (str == NULL)
			return (ft_lstclear(dest, free), error(ERR_ALLOCATION, NULL));
		node = ft_lstnew(str);
		if (node == NULL)
			return (ft_lstclear(dest, free), error(ERR_ALLOCATION, NULL));
		ft_lstadd_back(dest, node);
		base = base->next;
	}
	return (error(ERR_NONE, NULL));
}

t_error	globbing(t_list **out_files, t_list *in_files, t_pattern *pattern)
{
	t_list	*lst;
	t_list	*node;
	t_error	err;
	int		ret;

	if (out_files == NULL || pattern == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	lst = NULL;
	err = lst_dup(&lst, in_files);
	if (err.id != ERR_NONE)
		return (err);
	node = lst;
	while (node != NULL)
	{
		ret = check_pattern(node->content, pattern);
		if (ret == -1)
			return (ft_lstclear(&lst, free), error(ERR_IMPLEMENTATION, NULL));
		if (ret == 0)
			((char *) node->content)[0] = '\0';
		node = node->next;
	}
	lst_remove(&lst, str_empty, free);
	*out_files = lst;
	return (error(ERR_NONE, NULL));
}
