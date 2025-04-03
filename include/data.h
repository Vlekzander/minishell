/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:24:49 by apierret          #+#    #+#             */
/*   Updated: 2025/04/03 15:04:32 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H
# include "libft.h"

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

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPELINE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef struct s_redirect
{
	char	*in;
	char	*out;
	char	*heredoc;
	int		append;
}	t_redir;

typedef struct s_command
{
	char		*path;
	char		**args;
	t_redir		redir;
}	t_command;

typedef struct s_ast
{
	t_node_type	type;
	int			exit_code;
	union
	{
		t_command		*command;
		t_list			*pipeline;
		struct s_ast	*child;
		struct
		{
			struct s_ast	*left;
			struct s_ast	*right;
		};
	};
}	t_ast;

t_token_type	get_token_type(char *token);
t_token			*create_token(t_token_type type, char *value);
void			free_token(t_token *token);
void			free_ast(t_ast *ast);
void			free_command(t_command *command);

#endif
