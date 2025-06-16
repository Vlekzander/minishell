/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:52:49 by apierret          #+#    #+#             */
/*   Updated: 2025/06/16 15:22:01 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "test.h"
#include "test_utils.h"

static t_test_case parse_ast_cases[] =
{
	{
		"single_command",
		&(t_parse_ast_case)
		{
			.input_tokens = {"pwd", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "pwd",
					.next = NULL
				},
				.redirs = NULL
			}
		}
	},
	{
		"single_command_with_path",
		&(t_parse_ast_case)
		{
			.input_tokens = {"/bin/ls", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "/bin/ls",
					.next = NULL
				},
				.redirs = NULL
			}
		}
	},
	{
		"one_arg",
		&(t_parse_ast_case)
		{
			.input_tokens = {"echo", "hello", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "echo",
					.next = &(t_list)
					{
						.content = "hello",
						.next = NULL
					},
				},
				.redirs = NULL
			}
		}
	},
	{
		"multiple_args",
		&(t_parse_ast_case)
		{
			.input_tokens = {"echo", "hello", "world", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "echo",
					.next = &(t_list)
					{
						.content = "hello",
						.next = &(t_list)
						{
							.content = "world",
							.next = NULL
						},
					},
				},
				.redirs = NULL
			}
		}
	},
	{
		"redir_out",
		&(t_parse_ast_case)
		{
			.input_tokens = {"echo", "hello", ">", "out.txt", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "echo",
					.next = &(t_list)
					{
						.content = "hello",
						.next = NULL
					},
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_OUT, .out = "out.txt", .append = 0
					},
					.next = NULL
				}
			}
		}
	},
	{
		"redir_append",
		&(t_parse_ast_case)
		{
			.input_tokens = {"echo", "world", ">>", "out.txt", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "echo",
					.next = &(t_list)
					{
						.content = "world",
						.next = NULL
					},
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_OUT, .out = "out.txt", .append = 1
					},
					.next = NULL
				}
			}
		}
	},
	{
		"redir_in",
		&(t_parse_ast_case)
		{
			.input_tokens = {"cat", "<", "in.txt", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "cat",
					.next = NULL
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_IN, .in = "in.txt"
					},
					.next = NULL
				}
			}
		}
	},
	{
		"redir_heredoc",
		&(t_parse_ast_case)
		{
			.input_tokens = {"cat", "<<", "END", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "cat",
					.next = NULL
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_HEREDOC, .heredoc = "END"
					},
					.next = NULL
				}
			}
		}
	},
	{
		"redir_out_reverse",
		&(t_parse_ast_case)
		{
			.input_tokens = {">", "out.txt", "echo", "hello", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "echo",
					.next = &(t_list)
					{
						.content = "hello",
						.next = NULL
					},
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_OUT, .out = "out.txt", .append = 0
					},
					.next = NULL
				}
			}
		}
	},
	{
		"redir_append_reverse",
		&(t_parse_ast_case)
		{
			.input_tokens = {">>", "out.txt", "echo", "world", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "echo",
					.next = &(t_list)
					{
						.content = "world",
						.next = NULL
					},
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_OUT, .out = "out.txt", .append = 1
					},
					.next = NULL
				}
			}
		}
	},
	{
		"redir_in_reverse",
		&(t_parse_ast_case)
		{
			.input_tokens = {"<", "in.txt", "cat", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "cat",
					.next = NULL
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_IN, .in = "in.txt"
					},
					.next = NULL
				}
			}
		}
	},
	{
		"redir_heredoc_reverse",
		&(t_parse_ast_case)
		{
			.input_tokens = {"<<", "END", "cat", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "cat",
					.next = NULL
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_HEREDOC, .heredoc = "END"
					},
					.next = NULL
				}
			}
		}
	},
	{
		"mult_redir_out",
		&(t_parse_ast_case)
		{
			.input_tokens = { "echo", "hello", ">", "out1.txt", ">", "out2.txt", ">>", "out3.txt", NULL },
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "echo",
					.next = &(t_list)
					{
						.content = "hello",
						.next = NULL
					}
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_OUT, .out = "out1.txt", .append = 0
					},
					.next = &(t_list)
					{
						.content = &(t_redir){
							.type = REDIR_OUT, .out = "out2.txt", .append = 0
						},
						.next = &(t_list)
						{
							.content = &(t_redir){
								.type = REDIR_OUT, .out = "out3.txt", .append = 1
							},
							.next = NULL
						}
					}
				}
			}
		}
	},
	{
		"mult_redir_in",
		&(t_parse_ast_case)
		{
			.input_tokens = { "cat", "<", "in1.txt", "<", "in2.txt", "<", "in3.txt", "<<", "END", NULL },
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "cat",
					.next = NULL
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_IN, .in = "in1.txt"
					},
					.next = &(t_list)
					{
						.content = &(t_redir){
							.type = REDIR_IN, .in = "in2.txt"
						},
						.next = &(t_list)
						{
							.content = &(t_redir){
								.type = REDIR_IN, .in = "in3.txt"
							},
							.next = &(t_list)
							{
								.content = &(t_redir){
									.type = REDIR_HEREDOC, .heredoc = "END"
								},
								.next = NULL
							}
						}
					}
				}
			}
		}
	},
	{
		"mult_redir_both_way",
		&(t_parse_ast_case)
		{
			.input_tokens = { "<", "in1.txt", ">", "out1.txt", "cmd", "<", "in2.txt", ">", "out2.txt", NULL },
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "cmd",
					.next = NULL
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_IN, .in = "in1.txt"
					},
					.next = &(t_list)
					{
						.content = &(t_redir){
							.type = REDIR_OUT, .out = "out1.txt", .append = 0
						},
						.next = &(t_list)
						{
							.content = &(t_redir){
								.type = REDIR_IN, .in = "in2.txt"
							},
							.next = &(t_list)
							{
								.content = &(t_redir){
									.type = REDIR_OUT, .out = "out2.txt", .append = 0
								},
								.next = NULL
							}
						}
					}
				}
			}
		}
	},
	{
		"redir_out_alone",
		&(t_parse_ast_case)
		{
			.input_tokens = {">", "out.txt", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_REDIR,
				.exit_code = 0,
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_OUT, .out = "out.txt", .append = 0
					},
					.next = NULL
				}
			}
		}
	},
	{
		"redir_in_alone",
		&(t_parse_ast_case)
		{
			.input_tokens = {"<", "in.txt", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_REDIR,
				.exit_code = 0,
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
					.next = NULL
				}
			}
		}
	},
	{
		"cmd_arg_between_redirs",
		&(t_parse_ast_case)
		{
			.input_tokens = { "cat", "<", "in1.txt", "hello", "<", "in2.txt", "<", "in3.txt", NULL },
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command_args = &(t_list)
				{
					.content = "cat",
					.next = &(t_list)
					{
						.content = "hello",
						.next = NULL
					},
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){
						.type = REDIR_IN, .out = "in1.txt", .append = 0
					},
					.next = &(t_list)
					{
						.content = &(t_redir){
							.type = REDIR_IN, .out = "in2.txt", .append = 0
						},
						.next = &(t_list)
						{
							.content = &(t_redir){
								.type = REDIR_IN, .out = "in3.txt", .append = 0
							},
							.next = NULL
						}
					}
				}
			}
		}
	},
	{
		"single_pipe",
		&(t_parse_ast_case)
		{
			.input_tokens = { "ls", "|", "grep", ".c", NULL },
			.expected_ast = &(t_ast)
			{
				.type = NODE_PIPELINE,
				.exit_code = 0,
				.pipeline = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command_args = &(t_list)
						{
							.content = "ls",
							.next = NULL
						},
						.redirs = NULL
					},
					.next = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command_args = &(t_list)
							{
								.content = "grep",
								.next = &(t_list)
								{
									.content = ".c",
									.next = NULL
								},
							},
							.redirs = NULL
						},
						.next = NULL
					}
				}
			}
		}
	},
	{
		"multiple_pipes",
		&(t_parse_ast_case)
		{
			.input_tokens = { "ls", "|", "grep", ".c", "|", "wc", "-l", NULL },
			.expected_ast = &(t_ast)
			{
				.type = NODE_PIPELINE,
				.exit_code = 0,
				.pipeline = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command_args = &(t_list)
						{
							.content = "ls",
							.next = NULL
						},
						.redirs = NULL
					},
					.next = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command_args = &(t_list)
							{
								.content = "grep",
								.next = &(t_list)
								{
									.content = ".c",
									.next = NULL
								},
							},
							.redirs = NULL
						},
						.next = &(t_list)
						{
							.content = &(t_ast)
							{
								.type = NODE_COMMAND,
								.exit_code = 0,
								.command_args = &(t_list)
								{
									.content = "wc",
									.next = &(t_list)
									{
										.content = "-l",
										.next = NULL
									},
								},
								.redirs = NULL
							},
							.next = NULL
						}
					}
				}
			}
		}
	},
	{
		"pipes_and_redir",
		&(t_parse_ast_case)
		{
			.input_tokens = { "cat", "<", "in.txt", "|", "grep", "hello", "|", "wc", "-l", ">", "out.txt", NULL },
			.expected_ast = &(t_ast)
			{
				.type = NODE_PIPELINE,
				.exit_code = 0,
				.pipeline = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command_args = &(t_list)
						{
							.content = "cat",
							.next = NULL
						},
						.redirs = &(t_list)
						{
							.content = &(t_redir){
								.type = REDIR_IN, .in = "in.txt"
							},
							.next = NULL
						}
					},
					.next = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command_args = &(t_list)
							{
								.content = "grep",
								.next = &(t_list)
								{
									.content = "hello",
									.next = NULL
								},
							},
							.redirs = NULL
						},
						.next = &(t_list)
						{
							.content = &(t_ast)
							{
								.type = NODE_COMMAND,
								.exit_code = 0,
								.command_args = &(t_list)
								{
									.content = "wc",
									.next = &(t_list)
									{
										.content = "-l",
										.next = NULL
									},
								},
								.redirs = &(t_list)
								{
									.content = &(t_redir){
										.type = REDIR_OUT, .out = "out.txt",
										.append = 0
									},
									.next = NULL
								}
							},
							.next = NULL
						}
					}
				}
			}
		}
	},
	{
		"command_and",
		&(t_parse_ast_case)
		{
			.input_tokens = {"make", "&&", "./program", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_AND,
				.exit_code = 0,
				.left = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command_args = &(t_list)
					{
						.content = "make",
						.next = NULL
					},
					.redirs = NULL
				},
				.right = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command_args = &(t_list)
					{
						.content = "./program",
						.next = NULL
					},
					.redirs = NULL
				}
			}
		}
	},
	{
		"command_or",
		&(t_parse_ast_case)
		{
			.input_tokens = {"mkdir", "folder", "||", "echo", "mkdir", "failed", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_OR,
				.exit_code = 0,
				.left = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command_args = &(t_list)
					{
						.content = "mkdir",
						.next = &(t_list)
					{
						.content = "folder",
						.next = NULL
					},
					},
					.redirs = NULL
				},
				.right = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command_args = &(t_list)
					{
						.content = "echo",
						.next = &(t_list)
						{
							.content = "mkdir",
							.next = &(t_list)
							{
								.content = "failed",
								.next = NULL
							},
						},
					},
					.redirs = NULL
				}
			}
		}
	},
	{
		"group_single_command",
		&(t_parse_ast_case)
		{
			.input_tokens = {"(", "pwd", ")", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_GROUP,
				.exit_code = 0,
				.group = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command_args = &(t_list)
					{
						.content = "pwd",
						.next = NULL
					},
					.redirs = NULL
				}
			}
		}
	},
	{
		"deep_group",
		&(t_parse_ast_case)
		{
			.input_tokens = {"(", "(", "(", "pwd", ")", ")", ")", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_GROUP,
				.exit_code = 0,
				.group = &(t_ast)
				{
					.type = NODE_GROUP,
					.exit_code = 0,
					.group = &(t_ast)
					{
						.type = NODE_GROUP,
						.exit_code = 0,
						.group = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command_args = &(t_list)
							{
								.content = "pwd",
								.next = NULL
							},
							.redirs = NULL
						}
					}
				}
			}
		}
	},
	{
		"multi_op_complex",
		&(t_parse_ast_case)
		{
			.input_tokens = { "(", "cat", "file", "|", "grep", "error", ")", "&&", "echo", "found", "||", "echo", "none", NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_OR,
				.exit_code = 0,
				.left = &(t_ast)
				{
					.type = NODE_AND,
					.exit_code = 0,
					.left = &(t_ast)
					{
						.type = NODE_GROUP,
						.exit_code = 0,
						.group = &(t_ast)
						{
							.type = NODE_PIPELINE,
							.exit_code = 0,
							.pipeline = &(t_list)
							{
								.content = &(t_ast)
								{
									.type = NODE_COMMAND,
									.exit_code = 0,
									.command_args = &(t_list)
									{
										.content = "cat",
										.next = &(t_list)
										{
											.content = "file",
											.next = NULL
										},
									},
									.redirs = NULL
								},
								.next = &(t_list)
								{
									.content = &(t_ast)
									{
										.type = NODE_COMMAND,
										.exit_code = 0,
										.command_args = &(t_list)
										{
											.content = "grep",
											.next = &(t_list)
											{
												.content = "error",
												.next = NULL
											},
										},
										.redirs = NULL
									},
									.next = NULL
								}
							}
						}
					},
					.right = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command_args = &(t_list)
						{
							.content = "echo",
							.next = &(t_list)
							{
								.content = "found",
								.next = NULL
							},
						},
						.redirs = NULL
					}
				},
				.right = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command_args = &(t_list)
					{
						.content = "echo",
						.next = &(t_list)
						{
							.content = "none",
							.next = NULL
						},
					},
					.redirs = NULL
				}
			}
		}
	},
	{ NULL }
};

static t_test_case parse_ast_error_cases[] =
{
	{ "err_pipe_start", &(t_parse_ast_case) { .input_tokens = {"|", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_pipe_end", &(t_parse_ast_case) { .input_tokens = {"cmd", "|", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_double_pipe", &(t_parse_ast_case) { .input_tokens = {"cmd", "|", "|", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_logic_start", &(t_parse_ast_case) { .input_tokens = {"&&", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_logic_end", &(t_parse_ast_case) { .input_tokens = {"cmd", "||", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_logic_double_and", &(t_parse_ast_case) { .input_tokens = {"cmd", "&&", "&&", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_logic_double_or", &(t_parse_ast_case) { .input_tokens = {"cmd", "||", "||", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_logic_mixed_pipe", &(t_parse_ast_case) { .input_tokens = {"cmd", "&&", "|", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_pipe_between_ands", &(t_parse_ast_case) { .input_tokens = {"cmd", "&&", "|", "&&", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_redir_without_target", &(t_parse_ast_case) { .input_tokens = {"cmd", ">", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_redir_chain", &(t_parse_ast_case) { .input_tokens = {"cmd", ">", ">", "file", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_redir_pipe_combo", &(t_parse_ast_case) { .input_tokens = {"cmd", ">", "|", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_paren_empty", &(t_parse_ast_case) { .input_tokens = {"(", ")", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_open_paren_alone", &(t_parse_ast_case) { .input_tokens = {"(", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_close_paren_alone", &(t_parse_ast_case) { .input_tokens = {")", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_unbalanced_parens", &(t_parse_ast_case) { .input_tokens = {"(", "cmd", "|", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_nested_unbalanced_parens", &(t_parse_ast_case) { .input_tokens = {"(", "(", "cmd", ")", "|", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_bad_paren_combo", &(t_parse_ast_case) { .input_tokens = {"cmd", ")", "&&", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_mixed_logic_op", &(t_parse_ast_case) { .input_tokens = {"cmd", "&&", "||", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_consecutive_operators", &(t_parse_ast_case) { .input_tokens = {"cmd", "|", "&&", "|", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_pipe_and_or_mix", &(t_parse_ast_case) { .input_tokens = {"cmd", "|", "||", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_and_or_chain", &(t_parse_ast_case) { .input_tokens = {"cmd", "&&", "||", "&&", "cmd", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_redir_group", &(t_parse_ast_case) { .input_tokens = {"(", "pwd", ")", ">", "out.txt", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_redir_group_reverse", &(t_parse_ast_case) { .input_tokens = {">", "out.txt", "(", "pwd", ")", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_pipeline_group_left", &(t_parse_ast_case) { .input_tokens = {"(", "pwd", ")", "|", "cat", NULL}, .expected_error = ERR_SYNTAX } },
	{ "err_pipeline_group_right", &(t_parse_ast_case) { .input_tokens = {"pwd", "|", "(", "cat", ")", NULL}, .expected_error = ERR_SYNTAX } },
	{ NULL }
};

static t_list	*tokens;
static t_ast	*expected;
static t_ast	*tested;
static char		*env[] = { NULL };

t_error	__wrap_prompt_redirs(t_list *redirs)
{
	(void) redirs;
	return (ERR_NONE);
}

static int	test_setup(void **state)
{
	(void) state;
	tokens = NULL;
	expected = NULL;
	tested = NULL;
	return (0);
}

static int	test_teardown(void **state) {
	(void) state;
	free_ast(tested);
	ft_lstclear(&tokens, (void *) free_token);
	return (0);
}

static void	parse_ast_basic_tests(void **case_name)
{
	t_test_case			*tc;
	t_parse_ast_case	*data;
	t_error				error;
	int					equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(parse_ast_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	expected = data->expected_ast;
	tokens = create_token_list(data->input_tokens);
	if (tokens == NULL && data->input_tokens[0] != NULL)
		return(printf(FAIL_MSG, (char *) *case_name, "create_token_list failed"), assert_true(0));
	error = parse_ast(&tested, tokens, env);
	equal = ast_equal(expected, tested);
	if (error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

static void	parse_ast_error_tests(void **case_name)
{
	t_test_case			*tc;
	t_parse_ast_case	*data;
	t_error				error;
	int					equal;

	if (case_name == NULL)
		return(printf("Implementation error.\n"), assert_true(0));
	tc = find_case(parse_ast_error_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	data = tc->data;
	expected = NULL;
	tokens = create_token_list(data->input_tokens);
	if (tokens == NULL && data->input_tokens[0] != NULL)
		return(printf(FAIL_MSG, (char *) *case_name, "create_token_list failed"), assert_true(0));
	error = parse_ast(&tested, tokens, env);
	equal = tested == NULL;
	if (error != data->expected_error)
		return(printf(FAIL_MSG, (char *) *case_name, "error"), assert_true(0));
	if (!equal)
		return(printf(FAIL_MSG, (char *) *case_name, "not equal"), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[4].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[5].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[6].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[7].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[8].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[9].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[10].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[11].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[12].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[13].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[14].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[15].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[16].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[17].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[18].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[19].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[20].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[21].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[22].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[23].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[24].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[4].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[5].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[6].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[7].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[8].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[9].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[10].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[11].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[12].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[13].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[14].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[15].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[16].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[17].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[18].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[19].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[20].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[21].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[22].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[23].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[24].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, test_setup, test_teardown, parse_ast_error_cases[25].name),
	};
	char	name[] = "parser/parse_ast";
	t_test_result	result;

	printf(GROUP_HEADER, name);
	result.total = sizeof(test_cases)/sizeof(struct CMUnitTest);
	result.failed = cmocka_run_group_tests_name(name, test_cases, NULL, NULL);
	result.successful = result.total - result.failed;
	printf(GROUP_RESULT, name, result.successful, result.total);
	return (result);
}
