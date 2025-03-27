/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:24:49 by apierret          #+#    #+#             */
/*   Updated: 2025/03/27 18:27:46 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H

typedef enum e_token_type
{
	TK_WORD,
	TK_PIPE,
	TK_IN,
	TK_OUT,
	TK_APPEND,
	TK_HEREDOC,
	TK_AND,
	TK_OR,
	TK_P_OPEN,
	TK_P_CLOSE
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}	t_token;

t_token_type	get_token_type(char *token);
t_token			*create_token(t_token_type type, char *value);
void			free_token(t_token *token);

#endif
