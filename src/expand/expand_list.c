/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 13:54:11 by apierret          #+#    #+#             */
/*   Updated: 2025/07/13 17:22:17 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expand.h"
#include "utils.h"

static t_error	prepare_mask(char **mask, char *str)
{
	int		i;
	char	quote;
	char	*str_mask;

	if (mask == NULL || str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	str_mask = ft_strdup(str);
	if (str_mask == NULL)
		return (error(ERR_ALLOCATION, NULL));
	quote = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (is_quote(str[i]) && (quote == 0 || quote == str[i]))
		{
			quote = toggle_quote(str[i], quote);
			str_mask[i] = 'Q';
		}
		else
			str_mask[i] = ' ';
		i++;
	}
	return (*mask = str_mask, error(ERR_NONE, NULL));
}

static t_error	proc_expand_env(char **output, t_hash_table *env, char **mask)
{
	t_list	*vars;
	char	*str;
	t_error	err;

	if (output == NULL || env == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = extract_vars(&vars, *output, 0, env);
	if (err.id != ERR_NONE)
		return (err);
	if (vars == NULL)
		return (error(ERR_NONE, NULL));
	err = substitute_vars(&str, *output, vars, 0);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&vars, free), err);
	free(*output);
	*output = str;
	err = substitute_vars(&str, *mask, vars, 1);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&vars, free), err);
	free(*mask);
	*mask = str;
	return (ft_lstclear(&vars, free), error(ERR_NONE, NULL));
}

static t_error	proc_expand_wildcard(char **output, char *mask)
{
	char	*base;
	char	*str;
	t_error	err;

	if (output == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	base = *output;
	err = expand_wildcard(&str, base, mask);
	if (err.id != ERR_NONE)
		return (err);
	if (str == base)
		return (error(ERR_NONE, NULL));
	*output = str;
	return (free(base), error(ERR_NONE, NULL));
}

static t_error	remove_quotes(char *str, char *mask)
{
	int	i;
	int	len;

	if (str == NULL || mask == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	i = 0;
	while (mask[i] != '\0')
	{
		if (mask[i] == 'Q')
		{
			len = ft_strlen(mask + i + 1);
			ft_memmove(str + i, str + i + 1, len + 1);
			ft_memmove(mask + i, mask + i + 1, len + 1);
		}
		else
			i++;
	}
	return (error(ERR_NONE, NULL));
}

t_error	expand_list(t_list **lst, t_hash_table *env)
{
	t_list	*node;
	char	*mask;
	t_list	*next;
	t_error	err;

	if (lst == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	node = *lst;
	while (node != NULL)
	{
		next = node->next;
		err = prepare_mask(&mask, node->content);
		if (err.id != ERR_NONE)
			return (err);
		err = proc_expand_env((char **) &node->content, env, &mask);
		if (err.id != ERR_NONE)
			return (err);
		err = proc_expand_wildcard((char **) &node->content, mask);
		if (err.id != ERR_NONE)
			return (err);
		err = remove_quotes(node->content, mask);
		if (err.id != ERR_NONE)
			return (err);
		free(mask);
		node = next;
	}
	lst_remove(lst, str_empty, free);
	if (*lst == NULL)
		return (error(ERR_CMD_EMPTY, NULL));
	return (error(ERR_NONE, NULL));
}
