/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_pattern.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:11:50 by apierret          #+#    #+#             */
/*   Updated: 2025/07/31 12:01:28 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "utils.h"
#include "expansion.h"

static t_error	add_pattern_part(t_pattern *pattern, t_strbuilder *sb,
									char **ptr)
{
	t_list	*node;
	char	*str;

	if (pattern == NULL || sb == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	sb->length = 0;
	str = ft_strdup(sb->buffer);
	if (str == NULL)
		return (error(ERR_ALLOCATION, NULL));
	if (ptr != NULL)
		return (*ptr = str, error(ERR_NONE, NULL));
	node = ft_lstnew(str);
	if (node == NULL)
		return (error(ERR_ALLOCATION, NULL));
	ft_lstadd_back(&pattern->infixes, node);
	return (error(ERR_NONE, NULL));
}

static t_error	process_star(t_pattern **pattern, t_strbuilder *sb,
	int *star_seen)
{
	t_error	err;

	if (pattern == NULL || sb == NULL || star_seen == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = error(ERR_NONE, NULL);
	if (*pattern == NULL)
	{
		*pattern = create_pattern();
		if (*pattern == NULL)
			return (error(ERR_ALLOCATION, NULL));
	}
	if (sb->length > 0)
	{
		if (!*star_seen)
			err = add_pattern_part(*pattern, sb, &(*pattern)->prefix);
		else
			err = add_pattern_part(*pattern, sb, NULL);
	}
	*star_seen = 1;
	return (err);
}

static t_error	parse_pattern(t_pattern **pattern, char *str, char *mask,
	t_strbuilder *sb)
{
	int		star_seen;
	int		i;
	t_error	err;

	if (pattern == NULL || str == NULL || mask == NULL || sb == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = error(ERR_NONE, NULL);
	star_seen = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (mask[i] == ' ' && str[i] == '*')
			err = process_star(pattern, sb, &star_seen);
		else if (mask[i] != 'Q' && !strbuilder_append_char(sb, str[i]))
			err = error(ERR_ALLOCATION, NULL);
		if (err.id != ERR_NONE)
			return (err);
		i++;
	}
	return (error(ERR_NONE, NULL));
}

t_error	extract_pattern(t_pattern **pattern, char *str, char *mask)
{
	t_strbuilder	*sb;
	t_error			err;

	if (pattern == NULL || mask == NULL || str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*pattern = NULL;
	sb = create_strbuilder(ft_strlen(str) +1);
	if (sb == NULL)
		return (error(ERR_ALLOCATION, NULL));
	err = parse_pattern(pattern, str, mask, sb);
	if (err.id != ERR_NONE)
		return (free_pattern(*pattern), *pattern = NULL, free_strbuilder(sb),
			err);
	if (sb->length > 0 && *pattern != NULL)
		err = add_pattern_part(*pattern, sb, &(*pattern)->suffix);
	if (err.id != ERR_NONE)
		return (free_pattern(*pattern), *pattern = NULL, free_strbuilder(sb),
			err);
	return (free_strbuilder(sb), error(ERR_NONE, NULL));
}
