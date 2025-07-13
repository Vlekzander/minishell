/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 23:24:44 by apierret          #+#    #+#             */
/*   Updated: 2025/07/13 17:11:59 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "expand.h"
#include "utils.h"

static t_error	scan_next_var(t_var *var, char *str, int ign_quote, char quote)
{
	int		i;

	if (var == NULL || str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	i = 0;
	while (str[i] != '\0')
	{
		if (!ign_quote && is_quote(str[i]) && (quote == 0 || quote == str[i]))
			quote = toggle_quote(str[i], quote);
		if (((!ign_quote && quote != '\'') || ign_quote) && str[i] == '$')
		{
			var->id_index = i++;
			if (str[i] == '?')
				return (var->id_len = 2, error(ERR_NONE, NULL));
			while (ft_isalpha(str[i]) || str[i] == '_' || (i > var->id_index + 1
					&& ft_isalnum(str[i])))
				i++;
			if (i > var->id_index +1)
				return (var->id_len = i - var->id_index, error(ERR_NONE, NULL));
		}
		else
			i++;
	}
	return (var->id_index = -1, error(ERR_NONE, NULL));
}

static t_error	extract_next_var(t_var **var, char *str, int ign_quote,
		t_hash_table *env)
{
	t_var	*cvar;
	char	c;
	t_error	err;

	if (var == NULL || str == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	cvar = ft_calloc(1, sizeof(t_var));
	if (cvar == NULL)
		return (error(ERR_ALLOCATION, NULL));
	err = scan_next_var(cvar, str, ign_quote, 0);
	if (err.id != ERR_NONE)
		return (err);
	if (cvar->id_index == -1)
		return (free(cvar), *var = NULL, error(ERR_NONE, NULL));
	c = str[cvar->id_index + cvar->id_len];
	str[cvar->id_index + cvar->id_len] = '\0';
	err = get_var(&cvar->value, env, str + cvar->id_index +1);
	if (err.id != ERR_NONE)
		return (str[cvar->id_index + cvar->id_len] = c, free(cvar), err);
	str[cvar->id_index + cvar->id_len] = c;
	return (*var = cvar, error(ERR_NONE, NULL));
}

t_error	extract_vars(t_list **vars, char *base, int i_quote, t_hash_table *env)
{
	t_list	*lst;
	t_list	*node;
	t_var	*var;
	char	*str;
	t_error	err;

	if (vars == NULL || base == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	lst = NULL;
	str = base;
	err = extract_next_var(&var, str, i_quote, env);
	while (err.id == ERR_NONE && var != NULL)
	{
		var->id_index += str - base;
		str = base + var->id_index + var->id_len;
		node = ft_lstnew(var);
		if (node == NULL)
			return (ft_lstclear(&lst, free), error(ERR_ALLOCATION, NULL));
		ft_lstadd_back(&lst, node);
		err = extract_next_var(&var, str, i_quote, env);
		if (err.id != ERR_NONE)
			return (ft_lstclear(&lst, free), err);
	}
	return (*vars = lst, err);
}
