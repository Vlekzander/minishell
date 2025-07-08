/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_char_order.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 21:52:26 by apierret          #+#    #+#             */
/*   Updated: 2025/07/08 21:55:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	export_char_order(char c)
{
	if (ft_isdigit(c))
		return (c - '0');
	if (ft_isalpha(c))
	{
		if (c <= 'Z')
			return (c - 'A' + 10);
		return (c - 'a' + 37);
	}
	if (c == '_')
		return (36);
	return (100 + c);
}
