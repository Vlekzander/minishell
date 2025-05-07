/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:27:54 by apierret          #+#    #+#             */
/*   Updated: 2025/03/27 18:32:54 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data.h"
#include "libft.h"

t_token_type	get_token_type(char *token)
{
	if (ft_strncmp(token, "|", 2) == 0)
		return (TK_PIPE);
	if (ft_strncmp(token, "<", 2) == 0)
		return (TK_IN);
	if (ft_strncmp(token, ">", 2) == 0)
		return (TK_OUT);
	if (ft_strncmp(token, ">>", 3) == 0)
		return (TK_APPEND);
	if (ft_strncmp(token, "<<", 3) == 0)
		return (TK_HEREDOC);
	if (ft_strncmp(token, "&&", 3) == 0)
		return (TK_AND);
	if (ft_strncmp(token, "||", 3) == 0)
		return (TK_OR);
	if (ft_strncmp(token, "(", 2) == 0)
		return (TK_P_OPEN);
	if (ft_strncmp(token, ")", 2) == 0)
		return (TK_P_CLOSE);
	return (TK_WORD);
}
