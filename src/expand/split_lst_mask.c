/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_lst_mask.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:26:24 by apierret          #+#    #+#             */
/*   Updated: 2025/07/15 00:42:02 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expand.h"
#include "lexer.h"
#include "utils.h"

static t_error	new_node(t_list **node, char *str)
{
	char	*content;
	t_list	*ptr;

	if (node == NULL || str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	content = ft_strdup(str);
	if (content == NULL)
		return (error(ERR_ALLOCATION, NULL));
	ptr = ft_lstnew(content);
	if (ptr == NULL)
		return (free(content), error(ERR_ALLOCATION, NULL));
	return (*node = ptr, error(ERR_NONE, NULL));
}

static t_error	add_str_to_list(t_list **lst, t_strbuilder *sb, int *was_quoted)
{
	t_list	*node;
	t_error	err;

	if (lst == NULL || sb == NULL || was_quoted == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (sb->length > 0 || *was_quoted)
	{
		err = new_node(&node, sb->buffer);
		if (err.id != ERR_NONE)
			return (err);
		ft_lstadd_back(lst, node);
	}
	return (sb->length = 0, *was_quoted = 0, error(ERR_NONE, NULL));
}

static void	manage_quotes(char str, char mask, char *quote, int *was_quoted)
{
	if (quote == NULL || was_quoted == NULL)
		return ;
	if (mask == 'Q')
	{
		*quote = toggle_quote(*quote, str);
		*was_quoted = 1;
	}
}

static t_error	prepare_vars(t_list **list, t_strbuilder **sb, int *was_quoted)
{
	if (list == NULL || sb == NULL || was_quoted == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*list = NULL;
	*sb = create_strbuilder(64);
	if (*sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	*was_quoted = 0;
	return (error(ERR_NONE, NULL));
}

t_error	split_list_mask(t_list **lst, char *str, char *mask, char quote)
{
	t_list			*list;
	t_strbuilder	*sb;
	int				i;
	int				was_quoted;
	t_error			err;

	if (lst == NULL || str == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_vars(&list, &sb, &was_quoted);
	if (err.id != ERR_NONE)
		return (err);
	i = 0;
	while (str[i] != '\0')
	{
		manage_quotes(str[i], mask[i], &quote, &was_quoted);
		if (quote == 0 && is_separator(str[i]))
			err = add_str_to_list(&list, sb, &was_quoted);
		else if (mask[i] != 'Q' && !strbuilder_append_char(sb, str[i]))
			err = error(ERR_ALLOCATION, NULL);
		if (err.id != ERR_NONE)
			return (free_strbuilder(sb), err);
		i++;
	}
	err = add_str_to_list(&list, sb, &was_quoted);
	return (free_strbuilder(sb), *lst = list, err);
}
