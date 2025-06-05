/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:48:19 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 20:33:58 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_UTILS_H
# define TEST_UTILS_H
# include "data.h"
# include "libft.h"
# include "test.h"

t_test_case	*find_case(t_test_case *cases, char *name);
t_list		*create_token_list(char **strings);
int			ast_equal(t_ast *a, t_ast *b);
int			lst_equal(t_list *a, t_list *b, int (*equ)(void *, void *));
int			pattern_equal(t_pattern *a, t_pattern *b);
int			str_array_equal(char **a, char **b);
int			str_equal(char *a, char *b);
int			token_equal(t_token *a, t_token *b);
t_list		*lst_dup(t_list *src, void *(*dup)(void *), void (*del)(void *));
t_pattern	*pattern_dup(t_pattern *src);
t_vref		*varpos_dup(t_vref *src);
void		print_token_list(t_list *tokens);

#endif
