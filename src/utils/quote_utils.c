/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:05:57 by apierret          #+#    #+#             */
/*   Updated: 2025/06/07 17:06:51 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

char	handle_quote(char c, char current)
{
	if (current == 0)
		return (c);
	if (current == c)
		return (0);
	return (current);
}
