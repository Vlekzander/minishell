/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:26:51 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:28:39 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static	t_error	extract_vars(t_list **vars_lst, char *base, int ignore_quote)
{
	char	*str;
	t_vref	*var;
	t_list	*vrefs;
	t_list	*node_var;
	t_error	error;

	if (vars_lst == NULL || base == NULL)
		return (ERR_IMPLEMENTATION);
	vrefs = NULL;
	str = base;
	while (1)
	{
		error = extract_var(&var, str, ignore_quote);
		if (error != ERR_NONE)
			return (free_vref(var), error);
		if (var->str == NULL)
			break ;
		node_var = ft_lstnew(var);
		if (node_var == NULL)
			return (ft_lstclear(&vrefs, (void *) free_vref), free_vref(var),
				ERR_ALLOCATION);
		ft_lstadd_back(&vrefs, node_var);
		str = base + var->index + ft_strlen(var->str);
	}
	return (*vars_lst = vrefs, free_vref(var), ERR_NONE);
}

t_error	expand_env(char **output, char *base, char **env, int ignore_quote)
{
	char	*str;
	t_list	*vars;
	t_error	error;

	if (base == NULL || env == NULL)
		return (ERR_IMPLEMENTATION);
	error = extract_vars(&vars, base, ignore_quote);
	if (error != ERR_NONE)
		return (error);
	error = override_vars(&str, base, vars, env);
	if (error != ERR_NONE)
		return (ft_lstclear(&vars, (void *) free_vref), error);
	return (*output = str, ft_lstclear(&vars, (void *) free_vref), ERR_NONE);
}
