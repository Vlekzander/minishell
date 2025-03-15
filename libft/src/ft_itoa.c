/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:46:10 by apierret          #+#    #+#             */
/*   Updated: 2024/10/03 18:10:45 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"

static size_t	ft_numbersize(int n)
{
	size_t	size;

	if (n == -2147483648)
		return (11);
	size = 1;
	if (n < 0)
	{
		size++;
		n = -n;
	}
	while (n > 9)
	{
		size++;
		n /= 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	n_size;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	n_size = ft_numbersize(n);
	str = (char *) malloc(sizeof(char) * (n_size +1));
	if (str == NULL)
		return (NULL);
	str[n_size--] = '\0';
	if (n < 0)
	{
		n = -n;
		str[0] = '-';
	}
	else if (n == 0)
		str[0] = '0';
	while (n > 0)
	{
		str[n_size--] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}
