/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_pattern.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:11:50 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 13:08:40 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "error.h"
#include "lexer.h"
#include "lexer_internal.h"

static t_error	add_prefix_suffix(t_pattern *pattern, char *buf, int is_prefix)
{
	char	**str_ptr;

	if (pattern == NULL || buf == NULL)
		return (ERR_IMPLEMENTATION);
	str_ptr = &pattern->suffix;
	if (is_prefix)
		str_ptr = &pattern->prefix;
	*str_ptr = ft_strdup(buf);
	if (*str_ptr == NULL)
		return (ERR_ALLOCATION);
	return (ERR_NONE);
}

static t_error	add_infix(t_pattern *pattern, char *buf)
{
	t_list	*node;
	char	*str;

	if (pattern == NULL || buf == NULL)
		return (ERR_IMPLEMENTATION);
	str = ft_strdup(buf);
	if (str == NULL)
		return (ERR_ALLOCATION);
	node = ft_lstnew(str);
	if (node == NULL)
		return (ERR_ALLOCATION);
	ft_lstadd_back(&pattern->infixes, node);
	return (ERR_NONE);
}

static t_error	process_star(t_pattern **pattern, char *buf, int *star_seen)
{
	t_error	error;

	if (pattern == NULL || buf == NULL || star_seen == NULL)
		return (ERR_IMPLEMENTATION);
	error = ERR_NONE;
	if (*pattern == NULL)
	{
		*pattern = create_pattern();
		if (*pattern == NULL)
			return (ERR_ALLOCATION);
	}
	if (ft_strlen(buf) > 0)
	{
		if (!*star_seen)
			error = add_prefix_suffix(*pattern, buf, 1);
		else
			error = add_infix(*pattern, buf);
	}
	*star_seen = 1;
	return (error);
}

static t_error	parse_pattern(t_pattern **pattern, char *str, char *buf,
	size_t *i)
{
	t_error	error;
	char	quote;
	int		star_seen;

	if (pattern == NULL || str == NULL || buf == NULL || i == NULL)
		return (ERR_IMPLEMENTATION);
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
			error = process_star(pattern, buf, &star_seen);
			if (error != ERR_NONE)
				return (error);
		}
		else
			buf[(*i)++] = *str;
		str++;
	}
	return (ERR_NONE);
}

t_error	extract_pattern(t_pattern **pattern, char *str)
{
	char			*buf;
	size_t			i;
	t_error			error;

	if (pattern == NULL || str == NULL)
		return (ERR_IMPLEMENTATION);
	*pattern = NULL;
	buf = ft_calloc(ft_strlen(str) +1, sizeof(char));
	if (buf == NULL)
		return (ERR_ALLOCATION);
	i = 0;
	error = parse_pattern(pattern, str, buf, &i);
	if (error != ERR_NONE)
		return (free_pattern(*pattern), *pattern = NULL, free(buf), error);
	buf[i] = '\0';
	if (ft_strlen(buf) > 0 && *pattern != NULL)
		error = add_prefix_suffix(*pattern, buf, 0);
	if (error != ERR_NONE)
		return (free_pattern(*pattern), *pattern = NULL, free(buf), error);
	return (free(buf), ERR_NONE);
}
