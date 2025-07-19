/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:22:41 by apierret          #+#    #+#             */
/*   Updated: 2025/07/19 12:20:33 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "data.h"
# include "error.h"
# include "hash.h"

t_error			parse_ast(t_ast **ast, t_list *tokens, t_hash_table *env);
t_token			*peek_front(t_list **tk_lst, int pop);
int				get_precedence(t_token_type type);
t_redir_type	get_redir_type(t_token_type type);
int				is_redirection(t_token *token);
t_error			nud(t_ast **ast, t_list **tk_lst, t_token *token, t_list **hds);
t_error			led(t_ast **ast, t_list **tk_lst, t_token *token, t_list **hds);
t_error			parse_expression(t_ast **ast, t_list **tk_lst, t_list **hds,
					int prec);
void			clear_redirs(t_list	**redirs);
int				redir_cond(t_ast *node, t_token *token, t_token_type redir);
t_error			redir_to_list(t_list **lst, t_redir *redir);
#endif
