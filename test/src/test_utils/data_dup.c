/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_dup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:42:00 by apierret          #+#    #+#             */
/*   Updated: 2025/06/02 14:54:59 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "test_utils.h"

t_pattern	*pattern_dup(t_pattern *src)
{
	t_pattern	*pattern;

	if (src == NULL)
		return (NULL);
	pattern = ft_calloc(1, sizeof(t_pattern));
	if (pattern == NULL)
		return (NULL);
	pattern->prefix = ft_strdup(src->prefix);
	if (src->prefix != NULL && pattern->prefix == NULL)
		return (free_pattern(pattern), NULL);
	pattern->suffix = ft_strdup(src->suffix);
	if (src->suffix != NULL && pattern->suffix == NULL)
		return (free_pattern(pattern), NULL);
	pattern->infixes = lst_dup(src->infixes, (void *) ft_strdup, free);
	if (src->infixes != NULL && pattern->infixes == NULL)
		return (free_pattern(pattern), NULL);
	return (pattern);
}

t_varpos	*varpos_dup(t_varpos *src)
{
	t_varpos	*varpos;
	if (src == NULL)
		return (NULL);
	varpos = ft_calloc(1, sizeof(t_varpos));
	if (varpos == NULL)
		return (NULL);
	varpos->index = src->index;
	varpos->str = ft_strdup(src->str);
	if (src->str != NULL && varpos->str == NULL)
		return (free_varpos(varpos), NULL);
	return (varpos);
}
