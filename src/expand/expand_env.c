/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:24:57 by apierret          #+#    #+#             */
/*   Updated: 2025/07/14 17:35:09 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expand.h"

t_error	check_mask(char **mask, int *fmask, char *str)
{
	t_error	err;

	if (mask == NULL || fmask == NULL)
		return (error(ERR_ALLOCATION, NULL));
	*fmask = 0;
	if (*mask == NULL)
	{
		*fmask = 1;
		err = prepare_mask(mask, str, 1);
		if (err.id != ERR_NONE)
			return (err);
	}
	return (error(ERR_NONE, NULL));
}

void	free_mask(int fmask, char *mask)
{
	if (!fmask || mask == NULL)
		return ;
	free(mask);
}

t_error	expand_env(char **output, char *base, t_hash_table *env, char *mask)
{
	t_list	*vars;
	char	*str;
	int		fmask;
	t_error	err;

	if (output == NULL || base == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	check_mask(&mask, &fmask, base);
	err = extract_vars(&vars, base, mask, env);
	if (err.id != ERR_NONE)
		return (free_mask(fmask, mask), err);
	free_mask(fmask, mask);
	if (vars == NULL)
		return (*output = base, error(ERR_NONE, NULL));
	err = substitute_vars(&str, base, vars, 0);
	if (err.id != ERR_NONE)
		return (ft_lstclear(&vars, free), err);
	*output = str;
	return (ft_lstclear(&vars, free), error(ERR_NONE, NULL));
}
