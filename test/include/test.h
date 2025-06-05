/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:33:03 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 16:29:59 by apierret         ###   ########.fr       */
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
# include "error.h"
# include "libft.h"

typedef enum e_test_type
{
	TEST_TOKENIZE
}	t_test_type;

typedef struct s_tokenize_case
{
	char	*input;
	char	*expected_tokens[CASE_ARRAY_MAX];
}	t_tokenize_case;

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
