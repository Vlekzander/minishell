/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 12:41:24 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 13:45:49 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expansion.h"

static t_error	append_value(t_strbuilder *sb, char *value, char context)
{
	if (sb == NULL || value == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (context)
	{
		if (!strbuilder_append_mchar(sb, context, ft_strlen(value)))
			return (error(ERR_ALLOCATION, NULL));
		return (error(ERR_NONE, NULL));
	}
	if (!strbuilder_append(sb, value))
		return (error(ERR_ALLOCATION, NULL));
	return (error(ERR_NONE, NULL));
}

static char	get_context(int mask, char c)
{
	if (mask)
		return (c);
	return (0);
}

static t_error	loop_vars(t_strbuilder *sb, char *base, t_list *vars, int mask)
{
	t_list	*node;
	t_var	*var;
	int		base_index;
	t_error	err;

	if (sb == NULL || base == NULL || vars == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	base_index = 0;
	node = vars;
	while (node != NULL)
	{
		var = node->content;
		base[var->id_index] = '\0';
		if (!strbuilder_append(sb, base + base_index))
			return (error(ERR_ALLOCATION, NULL));
		err = append_value(sb, var->value,
				get_context(mask, base[var->id_index +1]));
		if (err.id != ERR_NONE)
			return (err);
		base_index = var->id_index + var->id_len;
		node = node->next;
	}
	if (!strbuilder_append(sb, base + base_index))
		return (error(ERR_ALLOCATION, NULL));
	return (error(ERR_NONE, NULL));
}

t_error	substitute_vars(char **output, char *base, t_list *vars, int is_mask)
{
	t_strbuilder	*sb;
	t_error			err;

	if (output == NULL || base == NULL || vars == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	sb = create_strbuilder(64);
	if (sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	err = loop_vars(sb, base, vars, is_mask);
	if (err.id != ERR_NONE)
		return (free_strbuilder(sb), err);
	*output = ft_strdup(sb->buffer);
	if (*output == NULL)
		return (free_strbuilder(sb), error(ERR_ALLOCATION, NULL));
	return (free_strbuilder(sb), error(ERR_NONE, NULL));
}
