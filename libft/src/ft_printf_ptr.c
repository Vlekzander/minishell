/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_ptr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:22:43 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 22:13:55 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

int	ft_printf_str(char *s);
int	ft_printf_hex(unsigned long n, int is_upper);

int	ft_printf_ptr(void *ptr)
{
	int	len;

	len = 0;
	len += ft_printf_str("0x");
	if (len == -1)
		return (-1);
	len += ft_printf_hex((unsigned long) ptr, 0);
	return (len);
}
