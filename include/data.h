/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:24:49 by apierret          #+#    #+#             */
/*   Updated: 2025/06/16 16:39:58 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_H
# define DATA_H
# include "libft.h"

typedef enum e_token_type
{
	TK_NONE,
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
	NODE_GROUP,
	NODE_REDIR
}	t_node_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	union
	{
		char	*in;
		struct
		{
			char	*out;
			int		append;
		};
		struct
		{
			char	*heredoc;
			int		fd;
		};
	};
}	t_redir;

typedef struct s_ast
{
	t_node_type	type;
	int			exit_code;
	union
	{
		t_list			*pipeline;
		struct s_ast	*group;
		struct
		{
			t_list	*redirs;
			t_list	*command_args;
		};
		struct
		{
			struct s_ast	*right;
			struct s_ast	*left;
		};
	};
}	t_ast;

typedef struct s_pattern
{
	char	*prefix;
	char	*suffix;
	t_list	*infixes;
}	t_pattern;

typedef struct s_vref
{
	char	*str;
	int		index;
}	t_vref;

typedef struct s_strbuilder
{
	char	*buffer;
	size_t	size;
	size_t	length;
}	t_strbuilder;

t_token_type	get_token_type(char *token);
t_token			*create_token(t_token_type type, char *value);
t_ast			*create_ast(t_node_type type);
t_redir			*create_redir(t_redir_type type, char *value, int append);
t_pattern		*create_pattern(void);
t_vref			*create_vref(void);
t_strbuilder	*create_strbuilder(size_t size);
void			free_token(t_token *token);
void			free_ast(t_ast *ast);
void			free_redir(t_redir *redir);
void			free_pattern(t_pattern *pattern);
void			free_vref(t_vref *vref);
void			free_strbuilder(t_strbuilder *sb);
int				strbuilder_append(t_strbuilder *sb, char *str);

#endif
