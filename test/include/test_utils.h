/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:20:26 by apierret          #+#    #+#             */
/*   Updated: 2025/04/03 13:36:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_UTILS_H
# define TEST_UTILS_H
# include "data.h"
# include "libft.h"

int		ast_equal(t_ast *a, t_ast *b);
int		lst_equal(t_list *a, t_list *b, int (*cmp)(void *, void *));
int		token_equal(void *a, void *b);
t_list	*create_token_list(char **strings);

#endif
