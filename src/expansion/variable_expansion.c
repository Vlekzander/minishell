/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:51:30 by apierret          #+#    #+#             */
/*   Updated: 2025/07/30 16:00:24 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expansion.h"

static void	free_mask(char *msk, char **mask)
{
	if (mask == NULL || *mask == NULL)
		free(msk);
}

t_error	process_substitution(char **str, char **mask, t_list *vars)
{
	char	*temp;
	t_error	err;

	if (str == NULL || *str == NULL || vars == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = substitute_vars(&temp, *str, vars, 0);
	if (err.id != ERR_NONE)
		return (err);
	free(*str);
	*str = temp;
	if (mask != NULL && *mask != NULL)
	{
		err = substitute_vars(&temp, *mask, vars, 1);
		if (err.id != ERR_NONE)
			return (err);
		free(*mask);
		*mask = temp;
	}
	return (error(ERR_NONE, NULL));
}

t_error	variable_expansion(char **str, char **mask, t_hash_table *env)
{
	char	*msk;
	t_list	*vars;
	t_error	err;

	if (str == NULL || *str == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (mask != NULL && *mask != NULL)
		msk = *mask;
	else
	{
		err = create_mask(&msk, *str);
		if (err.id != ERR_NONE)
			return (err);
	}
	err = extract_vars(&vars, *str, msk, env);
	if (err.id != ERR_NONE)
		return (free_mask(msk, mask), err);
	if (vars == NULL)
		return (free_mask(msk, mask), error(ERR_NONE, NULL));
	free_mask(msk, mask);
	err = process_substitution(str, mask, vars);
	if (err.id != ERR_NONE)
		return (free_mask(msk, mask), ft_lstclear(&vars, free), err);
	return (ft_lstclear(&vars, free), error(ERR_NONE, NULL));
}
