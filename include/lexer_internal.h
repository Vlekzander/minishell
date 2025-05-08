/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:58:59 by apierret          #+#    #+#             */
/*   Updated: 2025/05/08 19:18:18 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H
# include "data.h"
# include "error.h"

t_error	add_token(t_list **list, t_token_type type, char *value);
t_error	process_input(t_list **tokens, char *input);
t_error	process_operator(t_list **tokens, char **input, char *buf);
t_error	process_separator(t_list **tokens, char *buf);
char	handle_quote(char c, char current);
void	remove_str_quotes(char *str);
int		is_quote(char c);
int		is_operator(char *str);
int		is_separator(char c);

#endif
