/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:20:14 by apierret          #+#    #+#             */
/*   Updated: 2025/06/22 23:21:52 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "expand.h"
#include "utils.h"

static void	check_pattern(t_list *node, t_pattern *pattern)
{
	t_list	*infixe;
	char	*ptr;

	if (node == NULL || pattern == NULL)
		return ;
	ptr = node->content;
	if (pattern->prefix != NULL)
		ptr = ft_strstr(ptr, pattern->prefix);
	if ((pattern->prefix != NULL && ptr != node->content)
		|| (pattern->prefix == NULL && ptr[0] == '.'))
		((char *) node->content)[0] = '\0';
	if (pattern->infixes != NULL && ptr != NULL)
	{
		infixe = pattern->infixes;
		while (infixe != NULL && ptr != NULL)
		{
			ptr = ft_strstr(ptr, infixe->content);
			infixe = infixe->next;
		}
	}
	if (pattern->suffix != NULL && ptr != NULL)
		ptr = ft_strstr(ptr, pattern->suffix);
	if (ptr == NULL || (pattern->suffix != NULL
			&& ft_strlen(ptr) != ft_strlen(pattern->suffix)))
		((char *) node->content)[0] = '\0';
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

static void	remove_node(t_list **lst, t_list **prev, t_list **node)
{
	t_list	*temp;

	temp = *node;
	if (*prev == NULL)
	{
		*lst = (*node)->next;
		*node = *lst;
	}
	else
	{
		(*prev)->next = (*node)->next;
		*node = (*prev)->next;
	}
	ft_lstdelone(temp, free);
}

static void	lst_remove_null(t_list **lst)
{
	t_list	*prev;
	t_list	*node;

	if (lst == NULL)
		return ;
	prev = NULL;
	node = *lst;
	while (node != NULL)
	{
		if (ft_strlen(node->content) == 0)
		{
			remove_node(lst, &prev, &node);
			continue ;
		}
		prev = node;
		node = node->next;
	}
}

t_error	globbing(t_list **out_files, t_list *in_files, t_pattern *pattern)
{
	t_list	*node;
	t_error	err;

	if (out_files == NULL || pattern == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*out_files = NULL;
	err = lst_dup(out_files, in_files);
	if (err.code != ERR_NONE)
		return (err);
	node = *out_files;
	while (node != NULL)
	{
		check_pattern(node, pattern);
		node = node->next;
	}
	lst_remove_null(out_files);
	return (error(ERR_NONE, NULL));
}
