/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:24:57 by apierret          #+#    #+#             */
/*   Updated: 2025/07/14 15:27:28 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expand.h"

t_error	expand_env(char **output, char *base, t_hash_table *env, int ign_quote)
{
	t_list	*vars;
	char	*str;
	char	*mask;
	t_error	err;

	if (output == NULL || base == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	mask = NULL;
	if (!ign_quote)
	{
		err = prepare_mask(&mask, base);
		if (err.id != ERR_NONE)
			return (err);
	}
	err = extract_vars(&vars, base, mask, env);
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
