/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:20:37 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 22:14:02 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

int	ft_printf_str(char *s)
{
	size_t	len;

	if (s == NULL)
		return (ft_printf_str("(null)"));
	len = ft_strlen(s);
	return (write(1, s, len));
}
