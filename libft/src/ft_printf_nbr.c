/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_nbr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:22:36 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 23:05:26 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

int	ft_printf_char(char c);
int	ft_printf_str(char *s);

int	ft_printf_nbr(int n)
{
	int	len;

	len = 0;
	if (n == -2147483648)
		len += ft_printf_str("-2147483648");
	else
	{
		if (n < 0)
		{
			len += ft_printf_char('-');
			if (len == -1)
				return (-1);
			n = -n;
		}
		if (n < 10)
			len += ft_printf_char('0' + n);
		else
		{
			len += ft_printf_nbr(n / 10);
			if (len == -1)
				return (-1);
			len += ft_printf_char('0' + (n % 10));
		}
	}
	return (len);
}

int	ft_printf_nbrbase(unsigned long n, char *base)
{
	int		len;
	size_t	base_size;

	len = 0;
	if (base == NULL)
		return (len);
	base_size = ft_strlen(base);
	if (n < base_size)
		len += ft_printf_char(base[n]);
	else
	{
		len += ft_printf_nbrbase(n / base_size, base);
		if (len == -1)
			return (-1);
		len += ft_printf_char(base[n % base_size]);
	}
	return (len);
}

int	ft_printf_hex(unsigned long n, int is_upper)
{
	if (is_upper)
		return (ft_printf_nbrbase(n, "0123456789ABCDEF"));
	return (ft_printf_nbrbase(n, "0123456789abcdef"));
}
