/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mocks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:56:33 by apierret          #+#    #+#             */
/*   Updated: 2025/05/30 12:52:39 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"
#include "data.h"

__attribute__((weak))
t_error	__wrap_expand(t_list **tokens)
{
	(void) tokens;
	return (ERR_NONE);
}

__attribute__((weak))
t_error	__wrap_extract_pattern(t_pattern **pattern, char *str)
{
	(void) pattern;
	(void) str;
	return (ERR_NONE);
}

__attribute__((weak))
t_error	__wrap_globbing(t_list **out_files, t_list *in_files, t_pattern *pattern)
{
	(void) out_files;
	(void) in_files;
	(void) pattern;
	return (ERR_NONE);
}

__attribute__((weak))
t_error	__wrap_tokenize(t_list **tokens, char *input)
{
	(void) tokens;
	(void) input;
	return (ERR_NONE);
}

__attribute__((weak))
t_error	__wrap_parse_ast(t_ast **ast, t_list *tokens)
{
	(void) ast;
	(void) tokens;
	return (ERR_NONE);
}

__attribute__((weak))
t_error	__wrap_extract_var(char **var, char *str)
{
	(void) var;
	(void) str;
	return (ERR_NONE);
}
