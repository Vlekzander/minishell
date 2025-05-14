/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_equal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:58:12 by apierret          #+#    #+#             */
/*   Updated: 2025/05/13 19:01:13 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_utils.h"

int	lst_equal(t_list *a, t_list *b, int (*equ)(void *, void *))
{
	while (a != NULL && b != NULL)
	{
		if (!equ(a->content, b->content))
			return (0);
		a = a->next;
		b = b->next;
	}
	return (a == b);
}
