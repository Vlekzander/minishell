/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 15:13:11 by apierret          #+#    #+#             */
/*   Updated: 2025/06/22 20:44:27 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>

#include "expand.h"

static t_error	add_var_node(t_list **vrefs, t_vref *var, char *str, char *base)
{
	t_list	*node_var;

	if (vrefs == NULL || var == NULL || str == NULL || base == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	node_var = ft_lstnew(var);
	if (node_var == NULL)
		return (ft_lstclear(vrefs, (void *) free_vref), free_vref(var),
			error(ERR_ALLOCATION, NULL));
	ft_lstadd_back(vrefs, node_var);
	var->index += (int)(str - base);
	return (error(ERR_NONE, NULL));
}

static	t_error	get_vars(t_list **vars_lst, char *base, int ignore_quote)
{
	char	*str;
	t_vref	*var;
	t_list	*vrefs;
	t_error	err;

	if (vars_lst == NULL || base == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	vrefs = NULL;
	str = base;
	while (1)
	{
		err = extract_var(&var, str, ignore_quote);
		if (err.code != ERR_NONE)
			return (free_vref(var), err);
		if (var->str == NULL)
			break ;
		add_var_node(&vrefs, var, str, base);
		str = base + var->index + ft_strlen(var->str);
	}
	return (*vars_lst = vrefs, free_vref(var), error(ERR_NONE, NULL));
}

t_error	expand_env(char **output, char *base, t_hash_table *env, int ign_quote)
{
	char	*str;
	t_list	*vars;
	t_error	err;

	if (base == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = get_vars(&vars, base, ign_quote);
	if (err.code != ERR_NONE)
		return (err);
	err = over_vars(&str, base, vars, env);
	if (err.code != ERR_NONE)
		return (ft_lstclear(&vars, (void *) free_vref), err);
	ft_lstclear(&vars, (void *) free_vref);
	return (*output = str, error(ERR_NONE, NULL));
}
