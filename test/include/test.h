/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:33:03 by apierret          #+#    #+#             */
/*   Updated: 2025/05/26 11:14:41 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H
# include <setjmp.h>
# include <stdarg.h>
# include <stddef.h>
# include <cmocka.h>
# include "error.h"
# include "libft.h"
# define CASE_ARRAY_MAX 100
# define SUCCESS_MSG "\t[  \x1b[32mOK\x1b[0m  ] %s\n"
# define FAIL_MSG "\t[ \x1b[31mFAIL\x1b[0m ] %s\n"
# define CASE_NOT_FOUND_MSG "\t[ \x1b[31mFAIL\x1b[0m ] case \"%s\" not found\n"
# define GROUP_HEADER "[  START  ] %s\n"
# define GROUP_RESULT "[   END   ] %s - %d/%d tests passed\n"
# define GLOBAL_RESULt "\n[ \x1b[33mRESULTS\x1b[0m ] %d/%d tests passed\n"

typedef struct s_test_case {
	char	*name;
	union
	{
		struct
		{
			char	*input_tokenize;
			char	*expected_tokens_tokenize[CASE_ARRAY_MAX];
		};
		struct
		{
			t_list	*in_files_globbing;
			void	*patterns;
			t_list	*expected_out_files;
		};
		struct
		{
			char	*input_pattern;
			void	*expected_pattern;
		};
		struct
		{
			char	*input_tokens[CASE_ARRAY_MAX];
			union
			{
				void	*expected_ast;
				t_error	expected_error;
				struct
				{
					void	*extracted_pattern_expand[CASE_ARRAY_MAX];
					t_list	*filtered_files_expand[CASE_ARRAY_MAX];
					char	*expected_tokens_expand[CASE_ARRAY_MAX];
				};
			};
		};
		struct
		{
			char	*base_env[CASE_ARRAY_MAX];
			union
			{
				struct
				{
					char	*key_env;
					union
					{
						char	*value_env;
						char	*edited_env[CASE_ARRAY_MAX];
					};

				};
				char	*str_env;
			};

		};
	};
}	t_test_case;

typedef struct s_test_result
{
	int	total;
	int	failed;
	int successful;
} t_test_result;

t_test_result	execute_tests(void);

#endif
