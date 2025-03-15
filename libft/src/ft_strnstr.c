/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 23:28:58 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 21:23:28 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	needle_len;

	if (haystack == NULL || needle == NULL)
		return (NULL);
	if (needle[0] == '\0')
		return ((char *) haystack);
	needle_len = ft_strlen(needle);
	i = 0;
	while (i < len && haystack[i] != '\0')
	{
		if (haystack[i] == needle[0])
		{
			if (i + needle_len <= len
				&& ft_strncmp(haystack + i, needle, needle_len) == 0)
				return ((char *) haystack + i);
		}
		i++;
	}
	return (NULL);
}
