/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_pattern.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:11:50 by apierret          #+#    #+#             */
/*   Updated: 2025/06/22 20:18:27 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "error.h"
#include "utils.h"

static t_error	add_prefix_suffix(t_pattern *pattern, char *buf, int is_prefix)
{
	char	**str_ptr;

	if (pattern == NULL || buf == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	str_ptr = &pattern->suffix;
	if (is_prefix)
		str_ptr = &pattern->prefix;
	*str_ptr = ft_strdup(buf);
	if (*str_ptr == NULL)
		return (error(ERR_ALLOCATION, NULL));
	return (error(ERR_NONE, NULL));
}

static t_error	add_infix(t_pattern *pattern, char *buf)
{
	t_list	*node;
	char	*str;

	if (pattern == NULL || buf == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	str = ft_strdup(buf);
	if (str == NULL)
		return (error(ERR_ALLOCATION, NULL));
	node = ft_lstnew(str);
	if (node == NULL)
		return (error(ERR_ALLOCATION, NULL));
	ft_lstadd_back(&pattern->infixes, node);
	return (error(ERR_NONE, NULL));
}

static t_error	process_star(t_pattern **pattern, char *buf, int *star_seen)
{
	t_error	err;

	if (pattern == NULL || buf == NULL || star_seen == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = error(ERR_NONE, NULL);
	if (*pattern == NULL)
	{
		*pattern = create_pattern();
		if (*pattern == NULL)
			return (error(ERR_ALLOCATION, NULL));
	}
	if (ft_strlen(buf) > 0)
	{
		if (!*star_seen)
			err = add_prefix_suffix(*pattern, buf, 1);
		else
			err = add_infix(*pattern, buf);
	}
	*star_seen = 1;
	return (err);
}

static t_error	parse_pattern(t_pattern **pattern, char *str, char *buf,
	size_t *i)
{
	t_error	err;
	char	quote;
	int		star_seen;

	if (pattern == NULL || str == NULL || buf == NULL || i == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	star_seen = 0;
	quote = 0;
	while (*str != '\0')
	{
		if (is_quote(*str) && (quote == 0 || quote == *str))
			quote = handle_quote(*str, quote);
		else if (quote == 0 && *str == '*')
		{
			buf[*i] = '\0';
			*i = 0;
			err = process_star(pattern, buf, &star_seen);
			if (err.code != ERR_NONE)
				return (err);
		}
		else
			buf[(*i)++] = *str;
		str++;
	}
	return (error(ERR_NONE, NULL));
}

t_error	extract_pattern(t_pattern **pattern, char *str)
{
	char			*buf;
	size_t			i;
	t_error			err;

	if (pattern == NULL || str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	*pattern = NULL;
	buf = ft_calloc(ft_strlen(str) +1, sizeof(char));
	if (buf == NULL)
		return (error(ERR_ALLOCATION, NULL));
	i = 0;
	err = parse_pattern(pattern, str, buf, &i);
	if (err.code != ERR_NONE)
		return (free_pattern(*pattern), *pattern = NULL, free(buf), err);
	buf[i] = '\0';
	if (ft_strlen(buf) > 0 && *pattern != NULL)
		err = add_prefix_suffix(*pattern, buf, 0);
	if (err.code != ERR_NONE)
		return (free_pattern(*pattern), *pattern = NULL, free(buf), err);
	return (free(buf), error(ERR_NONE, NULL));
}
