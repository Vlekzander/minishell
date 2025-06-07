/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 15:57:43 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:22:58 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	is_operator(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '<' && str[1] == '<')
		return (1);
	if (str[0] == '>' && str[1] == '>')
		return (1);
	if (str[0] == '|' && str[1] == '|')
		return (1);
	if (str[0] == '&' && str[1] == '&')
		return (1);
	if (str[0] == '<')
		return (1);
	if (str[0] == '>')
		return (1);
	if (str[0] == '|')
		return (1);
	if (str[0] == '(')
		return (1);
	if (str[0] == ')')
		return (1);
	return (0);
}

int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static t_error	create_tkword_sublist(t_list **sublist, t_list *words)
{
	t_list	*lst;
	t_list	*node;
	t_token	*token;

	if (sublist == NULL || words == NULL)
		return (ERR_IMPLEMENTATION);
	lst = NULL;
	*sublist = NULL;
	while (words != NULL)
	{
		token = create_token(TK_WORD, words->content);
		if (token == NULL)
			return (ft_lstclear(&lst, (void *) free_token), ERR_ALLOCATION);
		node = ft_lstnew(token);
		if (node == NULL)
			return (ft_lstclear(&lst, (void *) free_token), free_token(token),
				ERR_ALLOCATION);
		ft_lstadd_back(&lst, node);
		words = words->next;
	}
	return (*sublist = lst, ERR_NONE);
}

t_error	insert_tkword_sublist(t_list **node, t_list *words)
{
	t_list	*sublist;
	t_error	error;

	if (node == NULL || words == NULL)
		return (ERR_IMPLEMENTATION);
	error = create_tkword_sublist(&sublist, words);
	if (error != ERR_NONE)
		return (error);
	free_token((*node)->content);
	ft_lstadd_back(&sublist, (*node)->next);
	(*node)->content = sublist->content;
	(*node)->next = sublist->next;
	ft_lstdelone(sublist, NULL);
	return (ERR_NONE);
}
