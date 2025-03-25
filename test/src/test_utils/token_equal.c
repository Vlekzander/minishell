/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_equal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:25:43 by apierret          #+#    #+#             */
/*   Updated: 2025/03/25 11:10:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "test_utils.h"

int	token_equal(void *a, void *b)
{
	t_token	*tk_a;
	t_token	*tk_b;
	size_t	len;
	size_t	len_b;

	tk_a = (t_token *) a;
	tk_b = (t_token *) b;
	if (tk_a == NULL || tk_b == NULL || tk_a->type != tk_b->type)
		return (0);
	if (tk_a->value == NULL && tk_b->value == NULL)
		return (1);
	if (tk_a->value == NULL || tk_b->value == NULL)
		return (0);
	len = ft_strlen(tk_a->value);
	len_b = ft_strlen(tk_b->value);
	if (len_b > len)
		len = len_b;
	return (ft_strncmp(tk_a->value, tk_b->value, len) == 0);
}
