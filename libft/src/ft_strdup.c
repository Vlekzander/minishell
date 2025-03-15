/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:58:08 by apierret          #+#    #+#             */
/*   Updated: 2024/11/15 21:27:18 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*str;
	size_t	len;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1) +1;
	str = (char *) malloc(sizeof(char) * len);
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s1, len);
	return (str);
}
