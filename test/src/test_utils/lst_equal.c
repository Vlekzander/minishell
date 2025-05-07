/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_equal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 19:44:01 by apierret          #+#    #+#             */
/*   Updated: 2025/03/24 23:28:37 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
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
