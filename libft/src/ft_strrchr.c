/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 22:57:41 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 21:20:52 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	*ptr;
	char	ch;

	if (s == NULL)
		return (NULL);
	ptr = NULL;
	ch = (char) c;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == ch)
			ptr = (char *) &s[i];
		i++;
	}
	if (ch == '\0')
		ptr = (char *) &s[i];
	return (ptr);
}
