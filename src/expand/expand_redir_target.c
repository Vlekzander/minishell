/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir_target.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:22:39 by apierret          #+#    #+#             */
/*   Updated: 2025/07/16 11:50:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expand.h"
#include "lexer.h"
#include "utils.h"

static t_error	check_expanded_target(char *str)
{
	int		i;
	char	quote;

	if (str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	quote = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (is_quote(str[i]) && (quote == 0 || quote == str[i]))
			quote = toggle_quote(str[i], quote);
		if (quote == 0 && is_blank(str[i]))
			return (error(ERR_AMB_REDIR, NULL));
		i++;
	}
	return (error(ERR_NONE, NULL));
}

static t_error	expand_redir_str(char **output, char *base, t_hash_table *env)
{
	char	*str;
	char	*mask;
	t_error	err;

	if (output == NULL || base == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = prepare_mask(&mask, base, 0);
	if (err.id != ERR_NONE)
		return (err);
	err = expand_env(&str, base, env, mask);
	if (err.id != ERR_NONE)
		return (free(mask), err);
	free(mask);
	if (base != str)
		base = str;
	err = expand_wildcard(&str, base, NULL);
	if (err.id != ERR_NONE)
		return (free(str), err);
	remove_str_quotes(str);
	if (base == str)
		return (*output = str, error(ERR_NONE, NULL));
	err = check_expanded_target(str);
	if (err.id != ERR_NONE)
		return (free(str), err);
	return (*output = str, error(ERR_NONE, NULL));
}

t_error	expand_redir_target(t_redir *redir, t_hash_table *env)
{
	char	**file;
	char	*base;
	char	*expanded;
	t_error	err;

	if (redir == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (redir->type == REDIR_HEREDOC || redir->type == REDIR_PIPE)
		return (error(ERR_NONE, NULL));
	file = &redir->in;
	if (redir->type == REDIR_OUT)
		file = &redir->out;
	base = ft_strdup(*file);
	if (base == NULL)
		return (error(ERR_ALLOCATION, NULL));
	err = expand_redir_str(&expanded, base, env);
	if (err.id != ERR_NONE)
	{
		if (err.id == ERR_AMB_REDIR)
			return (free(base), error(ERR_AMB_REDIR, *file));
		return (free(base), err);
	}
	if (expanded != base)
		free(base);
	return (free(*file), *file = expanded, err);
}
