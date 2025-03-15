/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 13:57:29 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 21:29:23 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	str_len;

	if (s == NULL)
		return (NULL);
	str_len = ft_strlen(s);
	if (str_len < start)
	{
		str_len = 0;
		start = 0;
	}
	else
		str_len -= start;
	if (str_len > len)
		str_len = len;
	str = (char *) ft_calloc(str_len +1, sizeof(char));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s + start, str_len +1);
	return (str);
}
