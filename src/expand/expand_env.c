/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:24:57 by apierret          #+#    #+#             */
/*   Updated: 2025/07/13 18:35:22 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expand.h"

t_error	expand_env(char **output, char *base, t_hash_table *env, int ign_quote)
{
	t_list	*vars;
	char	*str;
	t_error	err;

	if (output == NULL || base == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = extract_vars(&vars, base, ign_quote, env);
	if (err.id != ERR_NONE)
		return (err);
	if (vars == NULL)
		return (*output = base, error(ERR_NONE, NULL));
	err = substitute_vars(&str, base, vars, 0);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&vars, free), err);
	*output = str;
	return (ft_lstclear(&vars, free), error(ERR_NONE, NULL));
}
