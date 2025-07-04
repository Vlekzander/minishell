/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir_target.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 14:48:17 by apierret          #+#    #+#             */
/*   Updated: 2025/07/04 16:09:04 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "env.h"
#include "expand.h"
#include "lexer.h"
#include "utils.h"

static t_error	check_var_content(t_vref *vref, t_hash_table *env)
{
	char	*value;
	t_error	err;

	if (vref == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = get_var(&value, env, vref->str +1);
	if (err.id != ERR_NONE)
		return (err);
	if (ft_strchr(value, ' '))
		return (error(ERR_AMB_REDIR, NULL));
	return (error(ERR_NONE, NULL));
}

static t_error	val_var(char *ptr, t_vref **vref, char quote, t_hash_table *env)
{
	t_error	err;

	if (ptr == NULL || vref == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (*vref == NULL)
	{
		err = extract_var(vref, ptr, 0);
		if (err.id != ERR_NONE)
			return (err);
	}
	else
		(*vref)->index--;
	if ((*vref)->str != NULL && (*vref)->index == 0)
	{
		if (quote == 0)
		{
			err = check_var_content(*vref, env);
			if (err.id != ERR_NONE)
				return (err);
		}
		free_vref(*vref);
		*vref = NULL;
	}
	return (error(ERR_NONE, NULL));
}

static t_error	validate_vars(char *str, t_hash_table *env)
{
	char	*ptr;
	t_vref	*vref;
	char	quote;
	t_error	err;

	if (str == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	vref = NULL;
	quote = 0;
	ptr = str;
	while (*ptr != '\0')
	{
		if (is_quote(*ptr) && (quote == 0 || quote == *ptr))
			quote = handle_quote(*ptr, quote);
		err = val_var(ptr, &vref, quote, env);
		if (err.id == ERR_AMB_REDIR)
			return (free_vref(vref), error(ERR_AMB_REDIR, str));
		if (err.id != ERR_NONE)
			return (free_vref(vref), err);
		ptr++;
	}
	return (free_vref(vref), error(ERR_NONE, NULL));
}

t_error	expand_redir_str(char **file, t_hash_table *env)
{
	char	*base;
	char	*str;
	t_error	err;

	if (file == NULL || env == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	err = validate_vars(*file, env);
	if (err.id != ERR_NONE)
		return (err);
	base = ft_strdup(*file);
	if (base == NULL)
		return (error(ERR_ALLOCATION, NULL));
	err = expand_env(&str, base, env, 0);
	if (err.id != ERR_NONE)
		return (free(base), err);
	if (base == str)
		return (free(base), error(ERR_NONE, NULL));
	free(base);
	base = str;
	err = expand_wildcard(&str, base);
	if (err.id != ERR_NONE)
		return (free(base), err);
	if (base != str)
		free(base);
	return (free(*file), *file = str, error(ERR_NONE, NULL));
}

t_error	expand_redir_target(t_redir *redir, t_hash_table *env)
{
	char	**file;
	t_error	err;

	if (redir == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (redir->type == REDIR_HEREDOC || redir->type == REDIR_PIPE)
		return (error(ERR_NONE, NULL));
	file = &redir->in;
	if (redir->type == REDIR_OUT)
		file = &redir->out;
	err = expand_redir_str(file, env);
	if (err.id != ERR_NONE)
		return (err);
	return (error(ERR_NONE, NULL));
}
