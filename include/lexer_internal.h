/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:58:59 by apierret          #+#    #+#             */
/*   Updated: 2025/06/02 14:34:48 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H
# include "data.h"
# include "error.h"

t_error	extract_pattern(t_pattern **pattern, char *str);
t_error	extract_var(t_varpos **varpos, char *str);
t_error	add_token(t_list **list, t_token_type type, char *value);
t_error	process_input(t_list **tokens, char *input);
t_error	process_operator(t_list **tokens, char **input, char *buf);
t_error	process_separator(t_list **tokens, char *buf);
t_error	remove_empty_tokens(t_list **tk_list);
t_error	insert_tkword_sublist(t_list **node, t_list *words);
t_error	override_var(char **output, char *base, t_varpos *var, char **env);
char	handle_quote(char c, char current);
int		is_quote(char c);
int		is_operator(char *str);
int		is_separator(char c);
void	remove_str_quotes(char *str);

#endif
