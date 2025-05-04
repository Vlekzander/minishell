/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:55:20 by apierret          #+#    #+#             */
/*   Updated: 2025/05/04 15:57:39 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H
# include "data.h"
# include "error.h"

t_token	*pop_front(t_list **tk_lst);
t_token	*peek_front(t_list *tk_lst);
int		get_precedence(t_token_type type);
int		is_redirection(t_token *token);
t_error	nud(t_ast **ast, t_list **tk_lst, t_token *token, t_list **rds);
t_error	nud_word(t_ast **ast, t_token *token, t_list **redirs);
t_error	nud_subshell(t_ast **ast, t_list **tk_lst, t_list **redirs);
t_error	led(t_ast **ast, t_list **tk_lst, t_token *token);
t_error	led_word(t_ast **ast, t_token *token);
t_error	led_pipe(t_ast **ast, t_list **tk_lst);
t_error	led_logic(t_ast **ast, t_list **tk_lst, t_token_type tk_type);
t_error	parse_expression(t_ast **ast, t_list **tk_lst, int precedence);

#endif
