/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:50:29 by apierret          #+#    #+#             */
/*   Updated: 2025/05/14 12:15:52 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_error	expand(t_list **tokens)
{
	t_list		*node;
	t_token		*token;
	t_pattern	*pattern;
	t_error		error;

	if (tokens == NULL)
		return (ERR_IMPLEMENTATION);
	node = *tokens;
	while (node != NULL)
	{
		token = node->content;
		if (token == NULL)
			return (ERR_IMPLEMENTATION);
		if (token->type == TK_WORD)
		{
			error = extract_pattern(&pattern, token->value);
			if (error != ERR_NONE)
				return (error);
			if (pattern != NULL)
			{
				//1. Récuperer la liste des fichiers
				//2. Appliquer le filtre
				//3. Créer les tokens pour chaque fichier
				//4. Remplacer le token actuel par les nouveaux
			}
			free_pattern(pattern);
		}
		node = node->next;
	}
	return (ERR_NONE);
}
