/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:58:59 by apierret          #+#    #+#             */
/*   Updated: 2025/05/08 15:58:52 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H
# include "data.h"

void	add_token(t_list **list, t_token_type type, char *value);
char	handle_quote(char c, char current);
void	process_operator(t_list **tokens, char **input, char *buf);
void	process_separator(t_list **tokens, char *buf);
void	remove_str_quotes(char *str);
int		is_quote(char c);
int		is_operator(char *str);
int		is_separator(char c);

#endif
