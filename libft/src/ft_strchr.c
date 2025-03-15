/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 22:42:31 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 21:20:49 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	ch;

	if (s == NULL)
		return (NULL);
	ch = (char) c;
	i = 0;
	while (s[i] != '\0' && s[i] != ch)
		i++;
	if (s[i] == ch)
		return ((char *) &s[i]);
	return (NULL);
}
