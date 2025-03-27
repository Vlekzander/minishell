/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:23:36 by apierret          #+#    #+#             */
/*   Updated: 2025/03/27 22:58:25 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "data.h"
#include "lexer.h"

void	add_token(t_list **list, t_token_type type, char *value);
char	handle_quote(char c, char current);
void	process_operator(t_list **tokens, char **input, char *buf);
void	process_separator(t_list **tokens, char *buf);

static int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

static int	is_operator(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '<' && str[1] == '<')
		return (1);
	if (str[0] == '>' && str[1] == '>')
		return (1);
	if (str[0] == '|' && str[1] == '|')
		return (1);
	if (str[0] == '&' && str[1] == '&')
		return (1);
	if (str[0] == '<')
		return (1);
	if (str[0] == '>')
		return (1);
	if (str[0] == '|')
		return (1);
	if (str[0] == '(')
		return (1);
	if (str[0] == ')')
		return (1);
	return (0);
}

static int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static void	process_input(t_list **tokens, char *input, char *buf)
{
	char	quote;
	size_t	i;

	quote = 0;
	i = 0;
	while (*input != '\0')
	{
		if (is_quote(*input) && (quote == 0 || quote == *input))
			quote = handle_quote(*input, quote);
		else if (quote == 0 && is_operator(input))
		{
			process_operator(tokens, &input, buf);
			i = 0;
			continue ;
		}
		else if (quote == 0 && is_separator(*input))
		{
			process_separator(tokens, buf);
			i = 0;
		}
		else
			buf[i++] = *input;
		input++;
	}
	add_token(tokens, TK_WORD, buf);
}

t_error	tokenize(t_list **tokens, char *input)
{
	char	*buf;

	if (tokens == NULL || input == NULL)
		return (ERR_IMPLEMENTATION);
	buf = ft_calloc(ft_strlen(input) +1, sizeof(char));
	if (buf == NULL)
		return (ERR_ALLOCATION);
	process_input(tokens, input, buf);
	return (free(buf), ERR_NONE);
}
