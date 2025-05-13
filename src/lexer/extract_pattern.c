/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_pattern.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:11:50 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 11:33:08 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "error.h"
#include "lexer.h"
#include "lexer_internal.h"

static t_error	add_prefix(t_glob_pattern *pattern, char *buffer)
{
	if (pattern == NULL || buffer == NULL)
		return (ERR_IMPLEMENTATION);
	pattern->prefix = ft_strdup(buffer);
	if (pattern->prefix == NULL)
		return (ERR_ALLOCATION);
	return (ERR_NONE);
}

static t_error	add_infix(t_glob_pattern *pattern, char *buffer)
{
	t_list	*node;
	char	*str;

	if (pattern == NULL || buffer == NULL)
		return (ERR_IMPLEMENTATION);
	str = ft_strdup(buffer);
	if (str == NULL)
		return (ERR_ALLOCATION);
	node = ft_lstnew(str);
	if (node == NULL)
		return (ERR_ALLOCATION);
	ft_lstadd_back(&pattern->infixes, node);
	return (ERR_NONE);
}

static t_error	add_suffix(t_glob_pattern *pattern, char *buffer)
{
	if (pattern == NULL || buffer == NULL)
		return (ERR_IMPLEMENTATION);
	pattern->suffix = ft_strdup(buffer);
	if (pattern->suffix == NULL)
		return (ERR_ALLOCATION);
	return (ERR_NONE);
}

t_error	extract_pattern(t_glob_pattern **pattern, char *str)
{
	t_glob_pattern	*g_pattern;
	char			*buf;
	char			quote;
	size_t			i;
	int				seen_star;
	t_error			error;

	if (pattern == NULL || str == NULL)
		return (ERR_IMPLEMENTATION);
	buf = ft_calloc(ft_strlen(str), sizeof(char));
	if (buf == NULL)
		return (*pattern = NULL, ERR_ALLOCATION);
	g_pattern = NULL;
	error = ERR_NONE;
	seen_star = 0;
	quote = 0;
	i = 0;
	while (*str != '\0')
	{
		if (is_quote(*str) && (quote == 0 || quote == *str))
			quote = handle_quote(*str, quote);
		else if (quote == 0 && *str == '*')
		{
			if (g_pattern == NULL)
			{
				g_pattern = create_glob_pattern();
				if (g_pattern == NULL)
					return (*pattern = NULL, free(buf), ERR_ALLOCATION);
			}
			buf[i] = '\0';
			i = 0;
			if (ft_strlen(buf) > 0)
			{
				if (!seen_star)
					error = add_prefix(g_pattern, buf);
				else
					error = add_infix(g_pattern, buf);
				if (error != ERR_NONE)
					return (*pattern = NULL, free_glob_pattern(g_pattern), free(buf), error);
			}
			seen_star = 1;
		}
		else
			buf[i++] = *str;
		str++;
	}
	buf[i] = '\0';
	if (ft_strlen(buf) > 0 && g_pattern != NULL)
		error = add_suffix(g_pattern, buf);
	if (error != ERR_NONE)
		return (*pattern = NULL, free_glob_pattern(g_pattern), free(buf), error);
	return (free(buf), *pattern = g_pattern, ERR_NONE);
}
