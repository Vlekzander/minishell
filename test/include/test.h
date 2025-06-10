/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:33:03 by apierret          #+#    #+#             */
/*   Updated: 2025/06/10 11:59:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H
# define CASE_ARRAY_MAX 100
# define SUCCESS_MSG "\t[  \x1b[32mOK\x1b[0m  ] %s\n"
# define FAIL_MSG "\t[ \x1b[31mFAIL\x1b[0m ] %s - %s\n"
# define CASE_NOT_FOUND_MSG "\t[ \x1b[31mFAIL\x1b[0m ] case \"%s\" not found\n"
# define GROUP_HEADER "[  START  ] %s\n"
# define GROUP_RESULT "[   END   ] %s - %d/%d tests passed\n"
# define GLOBAL_RESULt "\n[ \x1b[33mRESULTS\x1b[0m ] %d/%d tests passed\n"
# include <setjmp.h>
# include <stdarg.h>
# include <stddef.h>
# include <cmocka.h>
# include "data.h"
# include "error.h"
# include "libft.h"

typedef struct s_file
{
	char	name[CASE_ARRAY_MAX +1];
	int		perm;
	int		is_folder;
}	t_file;

typedef struct s_env_case
{
	char	*base_env[CASE_ARRAY_MAX];
	char	*edited_env[CASE_ARRAY_MAX];
	union
	{
		struct
		{
			char	*key_env;
			char	*value_env;
		};
		char	*str_env;
	};
}	t_env_case;

typedef struct s_expand_case
{
	char		*input_tokens[CASE_ARRAY_MAX];
	t_vref		*extracted_vrefs[CASE_ARRAY_MAX];
	t_pattern	*extracted_patterns[CASE_ARRAY_MAX];
	t_list		*filtered_files[CASE_ARRAY_MAX];
	char		*expected_tokens[CASE_ARRAY_MAX];
}	t_expand_case;

typedef struct s_extract_pattern_case
{
	char		*input;
	t_pattern	*expected_pattern;
}	t_extract_pattern_case;

typedef struct s_extract_var_case
{
	char	*input;
	t_vref	*expected_vref;
}	t_extract_var_case;

typedef struct s_globbing_case
{
	t_list		*input_files;
	t_pattern	*pattern;
	t_list		*expected_out_files;
}	t_globbing_case;

typedef struct s_prompt_redirs_case
{
	char	*eof;
	char	*lines[CASE_ARRAY_MAX];
	char	*outputs[CASE_ARRAY_MAX];
}	t_prompt_redirs_case;

typedef struct s_open_file_case
{
	t_file			file;
	t_redir_type	type;
	int				append;
	int				create;
	t_error			expected_error;
}	t_open_file_case;

typedef struct s_parse_ast_case
{
	char	*input_tokens[CASE_ARRAY_MAX];
	union
	{
		t_ast	*expected_ast;
		t_error	expected_error;
	};
}	t_parse_ast_case;

typedef struct s_strbuilder_case
{
	char	*strs[CASE_ARRAY_MAX];
	char	*str;
}	t_strbuilder_case;

typedef struct s_tokenize_case
{
	char	*input;
	char	*expected_tokens[CASE_ARRAY_MAX];
}	t_tokenize_case;

typedef struct s_test_case
{
	char		*name;
	void		*data;
}	t_test_case;

typedef struct s_test_result
{
	int	total;
	int	failed;
	int successful;
} t_test_result;

t_test_result	execute_tests(void);

#endif
