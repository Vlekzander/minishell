/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:22:48 by apierret          #+#    #+#             */
/*   Updated: 2025/07/01 17:44:14 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "data.h"
# include "error.h"
# include "libft.h"

t_error	tokenize(t_list **tokens, char *input);

t_error	add_token(t_list **list, t_token_type type, char *value);
t_error	process_input(t_list **tokens, char *input);
t_error	process_operator(t_list **tokens, char **input, char *buf);
t_error	process_separator(t_list **tokens, char *buf);
t_error	remove_empty_tokens(t_list **tk_list);
int		is_operator(char *str);
int		is_separator(char c);
void	remove_str_quotes(char *str);

#endif
