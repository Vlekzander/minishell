/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:20:26 by apierret          #+#    #+#             */
/*   Updated: 2025/05/12 12:28:25 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_UTILS_H
# define TEST_UTILS_H
# include "data.h"
# include "libft.h"

int		ast_equal(t_ast *a, t_ast *b);
int		lst_equal(t_list *a, t_list *b, int (*cmp)(void *, void *));
int		str_array_equal(char **a, char **b);
int		str_equal(char *a, char *b);
int		token_equal(t_token *a, t_token *b);
int		glob_pattern_equal(t_pattern *a, t_pattern *b);
t_list	*create_token_list(char **strings);

#endif
