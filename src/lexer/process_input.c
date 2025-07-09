/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:16:47 by apierret          #+#    #+#             */
/*   Updated: 2025/06/22 20:22:09 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "lexer.h"
#include "utils.h"

static t_error	handle_op_sep(t_list **tokens, char **input, char *buf)
{
	t_error	err;

	if (tokens == NULL || input == NULL || buf == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	if (is_operator(*input))
		err = process_operator(tokens, input, buf);
	else
	{
		err = process_separator(tokens, buf);
		(*input)++;
	}
	if (err.id != ERR_NONE)
		return (err);
	return (error(ERR_NONE, NULL));
}

static t_error	process_loop(t_list **tokens, char *input, char *buf)
{
	t_error	err;
	char	quote;
	size_t	i;

	if (tokens == NULL || input == NULL || buf == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	quote = 0;
	i = 0;
	while (*input != '\0')
	{
		if (quote == 0 && (is_operator(input) || is_separator(*input)))
		{
			i = 0;
			err = handle_op_sep(tokens, &input, buf);
			if (err.id != ERR_NONE)
				return (err);
			continue ;
		}
		if (is_quote(*input) && (quote == 0 || quote == *input))
			quote = toggle_quote(*input, quote);
		buf[i++] = *(input++);
	}
	if (quote != 0)
		return (error(ERR_SYNTAX, NULL));
	return (add_token(tokens, TK_WORD, buf));
}

t_error	process_input(t_list **tokens, char *input)
{
	t_error	err;
	char	*buf;

	if (tokens == NULL || input == NULL)
		return (error(ERR_IMPLEMENTATION, NULL));
	buf = ft_calloc(ft_strlen(input) +1, sizeof(char));
	if (buf == NULL)
		return (error(ERR_ALLOCATION, NULL));
	err = process_loop(tokens, input, buf);
	if (err.id != ERR_NONE)
		return (free(buf), ft_lstclear(tokens, (void *) free_token), err);
	return (free(buf), err);
}
