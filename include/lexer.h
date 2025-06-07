/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:22:48 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 18:26:22 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "data.h"
# include "error.h"
# include "libft.h"

t_error	tokenize(t_list **tokens, char **env, char *input);
t_error	expand_tokens(t_list **tk_list, char **env);

t_error	add_token(t_list **list, t_token_type type, char *value);
t_error	process_input(t_list **tokens, char *input);
t_error	process_operator(t_list **tokens, char **input, char *buf);
t_error	process_separator(t_list **tokens, char *buf);
t_error	remove_empty_tokens(t_list **tk_list);
t_error	insert_tkword_sublist(t_list **node, t_list *words);
int		is_operator(char *str);
int		is_separator(char c);
void	remove_str_quotes(char *str);

#endif
