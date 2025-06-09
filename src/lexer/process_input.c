/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:16:47 by apierret          #+#    #+#             */
/*   Updated: 2025/06/09 23:53:02 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "lexer.h"
#include "utils.h"

static t_error	handle_op_sep(t_list **tokens, char **input, char *buf)
{
	t_error	error;

	if (tokens == NULL || input == NULL || buf == NULL)
		return (ERR_IMPLEMENTATION);
	if (is_operator(*input))
		error = process_operator(tokens, input, buf);
	else
	{
		error = process_separator(tokens, buf);
		(*input)++;
	}
	if (error != ERR_NONE)
		return (error);
	return (ERR_NONE);
}

static t_error	process_loop(t_list **tokens, char *input, char *buf)
{
	t_error	error;
	char	quote;
	size_t	i;

	if (tokens == NULL || input == NULL || buf == NULL)
		return (ERR_IMPLEMENTATION);
	quote = 0;
	i = 0;
	while (*input != '\0')
	{
		if (quote == 0 && (is_operator(input) || is_separator(*input)))
		{
			i = 0;
			error = handle_op_sep(tokens, &input, buf);
			if (error != ERR_NONE)
				return (error);
			continue ;
		}
		if (is_quote(*input) && (quote == 0 || quote == *input))
			quote = handle_quote(*input, quote);
		buf[i++] = *(input++);
	}
	if (quote != 0)
		return (ERR_SYNTAX);
	return (add_token(tokens, TK_WORD, buf));
}

t_error	process_input(t_list **tokens, char *input)
{
	t_error	error;
	char	*buf;

	if (tokens == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	buf = ft_calloc(ft_strlen(input) +1, sizeof(char));
	if (buf == NULL)
		return (ERR_ALLOCATION);
	error = process_loop(tokens, input, buf);
	if (error != ERR_NONE)
		return (free(buf), ft_lstclear(tokens, (void *) free_token), error);
	return (free(buf), error);
}
