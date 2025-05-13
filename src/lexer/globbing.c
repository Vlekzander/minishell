/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:41:52 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 15:10:54 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "lexer.h"
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
		node->content = NULL;
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
	if (ptr == NULL)
		node->content = NULL;
}

static t_error	lst_copy(t_list **dest, t_list *base)
{
	t_list	*node;

	if (dest == NULL)
		return (ERR_IMPLEMENTATION);
	while (base != NULL)
	{
		node = ft_lstnew(base->content);
		if (node == NULL)
			return (ft_lstclear(dest, NULL), ERR_ALLOCATION);
		ft_lstadd_back(dest, node);
		base = base->next;
	}
	return (ERR_NONE);
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
	ft_lstdelone(temp, NULL);
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
		if (node->content == NULL)
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
	t_error	error;

	if (out_files == NULL || pattern == NULL)
		return (ERR_IMPLEMENTATION);
	*out_files = NULL;
	error = lst_copy(out_files, in_files);
	if (error != ERR_NONE)
		return (error);
	node = *out_files;
	while (node != NULL)
	{
		check_pattern(node, pattern);
		node = node->next;
	}
	lst_remove_null(out_files);
	return (ERR_NONE);
}
