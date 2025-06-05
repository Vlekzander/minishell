/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:52:49 by apierret          #+#    #+#             */
/*   Updated: 2025/06/05 20:39:27 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "test.h"
#include "test_utils.h"
#include "utils.h"

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
				.command = &(t_command)
				{
					.path = "pwd",
					.args = (char *[]){"pwd", NULL}
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
				.command = &(t_command)
				{
					.path = "/bin/ls",
					.args = (char *[]){"ls", NULL}
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
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "hello", NULL}
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
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "hello", "world", NULL}
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
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "hello", NULL}
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
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "world", NULL}
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
				.command = &(t_command)
				{
					.path = "cat",
					.args = (char *[]){"cat", NULL}
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
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
				.command = &(t_command)
				{
					.path = "cat",
					.args = (char *[]){"cat", NULL}
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
		"redir_out_inverted",
		&(t_parse_ast_case)
		{
			.input_tokens = {">", "out.txt", "echo", "hello", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "hello", NULL}
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
		"redir_append_inverted",
		&(t_parse_ast_case)
		{
			.input_tokens = {">>", "out.txt", "echo", "world", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "world", NULL}
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
		"redir_in_inverted",
		&(t_parse_ast_case)
		{
			.input_tokens = {"<", "in.txt", "cat", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "cat",
					.args = (char *[]){"cat", NULL}
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
					.next = NULL
				}
			}
		}
	},
	{
		"redir_heredoc_inverted",
		&(t_parse_ast_case)
		{
			.input_tokens = {"<<", "END", "cat", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "cat",
					.args = (char *[]){"cat", NULL}
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
			.input_tokens = {
				"echo", "hello", ">", "out1.txt", ">", "out2.txt", ">",
				"out3.txt", NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "hello", NULL}
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
								.type = REDIR_OUT, .out = "out3.txt",
								.append = 0
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
			.input_tokens = {
				"echo", "hello", "<", "in1.txt", "<", "in2.txt", "<", "in3.txt",
				NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "hello", NULL}
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
		"mult_redir_out_both_way",
		&(t_parse_ast_case)
		{
			.input_tokens = {
				">", "out1.txt", ">", "out2.txt", "echo", "hello", ">",
				"out3.txt", ">", "out4.txt", NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "hello", NULL}
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
								.type = REDIR_OUT, .out = "out3.txt",
								.append = 0
							},
							.next = &(t_list)
							{
								.content = &(t_redir){
									.type = REDIR_OUT, .out = "out4.txt",
									.append = 0
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
		"mult_redir_in_both_way",
		&(t_parse_ast_case)
		{
			.input_tokens = {
				"<", "in1.txt", "<", "in2.txt", "echo", "hello", "<", "in3.txt",
				"<", "in4.txt", NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "hello", NULL}
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
							.next = &(t_list)
							{
								.content = &(t_redir){
									.type = REDIR_IN, .out = "in4.txt",
									.append = 0
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
		"mult_redirs_both_way",
		&(t_parse_ast_case)
		{
			.input_tokens = {
				"<", "in1.txt", "<", "in2.txt", "echo", "hello", ">",
				"out1.txt", ">", "out2.txt", NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "echo",
					.args = (char *[]){"echo", "hello", NULL}
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
								.type = REDIR_OUT, .out = "out1.txt",
								.append = 0
							},
							.next = &(t_list)
							{
								.content = &(t_redir){
									.type = REDIR_OUT, .out = "out2.txt",
									.append = 0
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
		"single_pipe",
		&(t_parse_ast_case)
		{
			.input_tokens = {"ls", "|", "grep", ".c", NULL},
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
						.command = &(t_command)
						{
							.path = "ls",
							.args = (char *[]){"ls", NULL}
						},
						.redirs = NULL
					},
					.next = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "grep",
								.args = (char *[]){"grep", ".c", NULL}
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
			.input_tokens = {"ls", "|", "grep", ".c", "|", "wc", "-l", NULL},
			.expected_ast = &(t_ast){
				.type = NODE_PIPELINE,
				.exit_code = 0,
				.pipeline = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "ls",
							.args = (char *[]){"ls", NULL}
						},
						.redirs = NULL
					},
					.next = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "grep",
								.args = (char *[]){"grep", ".c", NULL}
							},
							.redirs = NULL
						},
						.next = &(t_list)
						{
							.content = &(t_ast)
							{
								.type = NODE_COMMAND,
								.exit_code = 0,
								.command = &(t_command)
								{
									.path = "wc",
									.args = (char *[]){"wc", "-l", NULL}
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
			.input_tokens = {
				"cat", "<", "in.txt", "|", "grep", "hello", "|", "wc", "-l",
				">", "out.txt", NULL
			},
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
						.command = &(t_command)
						{
							.path = "cat",
							.args = (char *[]){"cat", NULL}
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
							.command = &(t_command)
							{
								.path = "grep",
								.args = (char *[]){"grep", "hello", NULL}
							},
							.redirs = NULL
						},
						.next = &(t_list)
						{
							.content = &(t_ast)
							{
								.type = NODE_COMMAND,
								.exit_code = 0,
								.command = &(t_command)
								{
									.path = "wc",
									.args = (char *[]){"wc", "-l", NULL}
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
					.command = &(t_command)
					{
						.path = "make",
						.args = (char *[]){"make", NULL}
					},
					.redirs = NULL
				},
				.right = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "./program",
						.args = (char *[]){"program", NULL}
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
			.input_tokens = {"mkdir", "folder", "||", "echo", "fail", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_OR,
				.exit_code = 0,
				.left = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "mkdir",
						.args = (char *[]){"mkdir", "folder", NULL}
					},
					.redirs = NULL
				},
				.right = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "echo",
						.args = (char *[]){"echo", "fail", NULL}
					},
					.redirs = NULL
				}
			}
		}
	},
	{
		"subshell",
		&(t_parse_ast_case)
		{
			.input_tokens = {"(", "pwd", ")", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_SUBSHELL,
				.exit_code = 0,
				.child = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "pwd",
						.args = (char *[]){"pwd", NULL}
					},
					.redirs = NULL
				},
				.redirs = NULL
			}
		}
	},
	{
		"and_with_redir",
		&(t_parse_ast_case)
		{
			.input_tokens = {
				"echo", "success", ">", "out.txt", "&&", "cat", "<", "in.txt",
				NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_AND,
				.exit_code = 0,
				.left = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "echo",
						.args = (char *[]){"echo", "success", NULL}
					},
					.redirs = &(t_list)
					{
						.content = &(t_redir){
							.type = REDIR_OUT, .out = "out.txt", .append = 0
						},
						.next = NULL
					}
				},
				.right = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "cat",
						.args = (char *[]){"cat", NULL}
					},
					.redirs = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
						.next = NULL
					}
				}
			}
		}
	},
	{
		"or_with_pipe",
		&(t_parse_ast_case)
		{
			.input_tokens = {
				"echo", "hello", "|", "grep", "world", "||", "ls", "|", "grep",
				".h", NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_OR,
				.exit_code = 0,
				.left = &(t_ast)
				{
					.type = NODE_PIPELINE,
					.exit_code = 0,
					.pipeline = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "echo",
								.args = (char *[]){"echo", "hello", NULL}
							},
							.redirs = NULL
						},
						.next = &(t_list)
						{
							.content = &(t_ast)
							{
								.type = NODE_COMMAND,
								.exit_code = 0,
								.command = &(t_command)
								{
									.path = "grep",
									.args = (char *[]){"grep", "world", NULL}
								},
								.redirs = NULL
							},
							.next = NULL
						}
					}
				},
				.right = &(t_ast)
				{
					.type = NODE_PIPELINE,
					.exit_code = 0,
					.pipeline = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "ls",
								.args = (char *[]){"ls", NULL}
							},
							.redirs = NULL
						},
						.next = &(t_list)
						{
							.content = &(t_ast)
							{
								.type = NODE_COMMAND,
								.exit_code = 0,
								.command = &(t_command)
								{
									.path = "grep",
									.args = (char *[]){"grep", ".h", NULL}
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
		"subshell_with_pipe",
		&(t_parse_ast_case)
		{
			.input_tokens = {"(", "ls", "-la", ")", "|", "wc", "-l", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_PIPELINE,
				.exit_code = 0,
				.pipeline = &(t_list)
				{
					.content = &(t_ast)
					{
						.type = NODE_SUBSHELL,
						.exit_code = 0,
						.child = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "ls",
								.args = (char *[]){"ls", "-la", NULL}
							},
							.redirs = NULL
						},
						.redirs = NULL
					},
					.next = &(t_list)
					{
						.content = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "wc",
								.args = (char *[]){"wc", "-l", NULL}
							},
							.redirs = NULL
						},
						.next = NULL
					}
				},
			}
		}
	},
	{
		"subshell_and_redir",
		&(t_parse_ast_case)
		{
			.input_tokens = {
				"(", "echo", "hello", ")", ">", "out.txt", "&&", "echo", "done",
				NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_AND,
				.exit_code = 0,
				.left = &(t_ast)
				{
					.type = NODE_SUBSHELL,
					.exit_code = 0,
					.child = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "echo",
							.args = (char *[]){"echo", "hello", NULL}
						},
						.redirs = NULL
					},
					.redirs = &(t_list)
					{
						.content = &(t_redir){
							.type = REDIR_OUT, .out = "out.txt", .append = 0
						},
						.next = NULL
					}
				},
				.right = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "echo",
						.args = (char *[]){"echo", "done", NULL}
					},
					.redirs = NULL
				}
			}
		}
	},
	{
		"nested_logic",
		&(t_parse_ast_case)
		{
			.input_tokens = {
				"make", "&&", "(", "./run", "||", "echo", "fallback", ")", NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_AND,
				.exit_code = 0,
				.left = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "make",
						.args = (char *[]){"make", NULL}
					},
					.redirs = NULL
				},
				.right = &(t_ast){
					.type = NODE_SUBSHELL,
					.exit_code = 0,
					.child = &(t_ast)
					{
						.type = NODE_OR,
						.exit_code = 0,
						.left = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "./run",
								.args = (char *[]){"run", NULL}
							},
							.redirs = NULL
						},
						.right = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "echo",
								.args = (char *[]){"echo", "fallback", NULL}
							},
							.redirs = NULL
						}
					},
					.redirs = NULL
				}
			}
		}
	},
	{
		"nested_logic_two",
		&(t_parse_ast_case)
		{
			.input_tokens = {"(", "make", ")", "&&", "./run", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_AND,
				.exit_code = 0,
				.left = &(t_ast)
				{
					.type = NODE_SUBSHELL,
					.exit_code = 0,
					.child = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "make",
							.args = (char *[]){"make", NULL}
						},
						.redirs = NULL
					}
				},
				.right = &(t_ast)
				{
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "./run",
						.args = (char *[]){"run", NULL}
					},
					.redirs = NULL
				}
			}
		}
	},
	{
		"deep_nested_group",
		&(t_parse_ast_case)
		{
			.input_tokens = {"(", "(", "(", "echo", "ok", ")", ")", ")", NULL},
			.expected_ast = &(t_ast)
			{
				.type = NODE_SUBSHELL,
				.exit_code = 0,
				.child = &(t_ast)
				{
					.type = NODE_SUBSHELL,
					.exit_code = 0,
					.child = &(t_ast)
					{
						.type = NODE_SUBSHELL,
						.exit_code = 0,
						.child = &(t_ast)
						{
							.type = NODE_COMMAND,
							.exit_code = 0,
							.command = &(t_command)
							{
								.path = "echo",
								.args = (char *[]){"echo", "ok", NULL}
							},
							.redirs = NULL
						},
						.redirs = NULL
					},
					.redirs = NULL
				},
				.redirs = NULL
			}
		}
	},
	{
		"multi_op_complex",
		&(t_parse_ast_case)
		{
			.input_tokens = {
				"(", "cat", "file", "|", "grep", "error", ")", "&&", "echo",
				"found", "||", "echo", "none", NULL
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
						.type = NODE_SUBSHELL,
						.exit_code = 0,
						.child = &(t_ast)
						{
							.type = NODE_PIPELINE,
							.exit_code = 0,
							.pipeline = &(t_list)
							{
								.content = &(t_ast)
								{
									.type = NODE_COMMAND,
									.exit_code = 0,
									.command = &(t_command)
									{
										.path = "cat",
										.args = (char *[]){"cat", "file", NULL}
									},
									.redirs = NULL
								},
								.next = &(t_list)
								{
									.content = &(t_ast)
									{
										.type = NODE_COMMAND,
										.exit_code = 0,
										.command = &(t_command)
										{
											.path = "grep",
											.args = (char *[]){
												"grep", "error", NULL
											}
										},
										.redirs = NULL
									},
									.next = NULL
								}
							}
						},
						.redirs = NULL
					},
					.right = &(t_ast)
					{
						.type = NODE_COMMAND,
						.exit_code = 0,
						.command = &(t_command)
						{
							.path = "echo",
							.args = (char *[]){"echo", "found", NULL},
						},
						.redirs = NULL
					}
				},
				.right = &(t_ast){
					.type = NODE_COMMAND,
					.exit_code = 0,
					.command = &(t_command)
					{
						.path = "echo",
						.args = (char *[]){"echo", "none", NULL}
					},
					.redirs = NULL
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
			.input_tokens = {
				"cat", "<", "in1.txt", "hello", "<", "in2.txt", "<", "in3.txt",
				NULL
			},
			.expected_ast = &(t_ast)
			{
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command)
				{
					.path = "cat",
					.args = (char *[]){"cat", "hello", NULL}
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
	{ NULL }
};

static t_test_case parse_ast_error_cases[] =
{
	{ "err_pipe_start", &(t_parse_ast_case) { .input_tokens = {"|"}, .expected_error = ERR_SYNTAX } },
	{ "err_pipe_end", &(t_parse_ast_case) { .input_tokens = {"cmd", "|"}, .expected_error = ERR_SYNTAX } },
	{ "err_double_pipe", &(t_parse_ast_case) { .input_tokens = {"cmd", "|", "|", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_logic_start", &(t_parse_ast_case) { .input_tokens = {"&&"}, .expected_error = ERR_SYNTAX } },
	{ "err_logic_end", &(t_parse_ast_case) { .input_tokens = {"cmd", "||"}, .expected_error = ERR_SYNTAX } },
	{ "err_logic_double_and", &(t_parse_ast_case) { .input_tokens = {"cmd", "&&", "&&", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_logic_double_or", &(t_parse_ast_case) { .input_tokens = {"cmd", "||", "||", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_logic_mixed_pipe", &(t_parse_ast_case) { .input_tokens = {"cmd", "&&", "|", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_pipe_between_ands", &(t_parse_ast_case) { .input_tokens = {"cmd", "&&", "|", "&&", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_redir_without_target", &(t_parse_ast_case) { .input_tokens = {"cmd", ">"}, .expected_error = ERR_SYNTAX } },
	{ "err_redir_chain", &(t_parse_ast_case) { .input_tokens = {"cmd", ">", ">", "file"}, .expected_error = ERR_SYNTAX } },
	{ "err_redir_pipe_combo", &(t_parse_ast_case) { .input_tokens = {"cmd", ">", "|", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_paren_empty", &(t_parse_ast_case) { .input_tokens = {"(", ")"}, .expected_error = ERR_SYNTAX } },
	{ "err_open_paren_alone", &(t_parse_ast_case) { .input_tokens = {"("}, .expected_error = ERR_SYNTAX } },
	{ "err_close_paren_alone", &(t_parse_ast_case) { .input_tokens = {")"}, .expected_error = ERR_SYNTAX } },
	{ "err_unbalanced_parens", &(t_parse_ast_case) { .input_tokens = {"(", "cmd", "|", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_nested_unbalanced_parens", &(t_parse_ast_case) { .input_tokens = {"(", "(", "cmd", ")", "|"}, .expected_error = ERR_SYNTAX } },
	{ "err_bad_paren_combo", &(t_parse_ast_case) { .input_tokens = {"cmd", ")", "&&", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_mixed_logic_op", &(t_parse_ast_case) { .input_tokens = {"cmd", "&&", "||", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_consecutive_operators", &(t_parse_ast_case) { .input_tokens = {"cmd", "|", "&&", "|", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_pipe_and_or_mix", &(t_parse_ast_case) { .input_tokens = {"cmd", "|", "||", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ "err_and_or_chain", &(t_parse_ast_case) { .input_tokens = {"cmd", "&&", "||", "&&", "cmd"}, .expected_error = ERR_SYNTAX } },
	{ NULL }
};

static t_list	*tokens = NULL;
static t_ast	*expected = NULL;
static t_ast	*tested = NULL;

static int test_teardown(void **state) {
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
	error = parse_ast(&tested, tokens);
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
	error = parse_ast(&tested, tokens);
	equal = ast_equal(expected, tested);
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
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[25].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[26].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[27].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[28].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[29].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[30].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[31].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[32].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_basic_tests, NULL, test_teardown, parse_ast_cases[33].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[0].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[1].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[2].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[3].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[4].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[5].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[6].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[7].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[8].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[9].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[10].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[11].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[12].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[13].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[14].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[15].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[16].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[17].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[18].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[19].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[20].name),
		cmocka_unit_test_prestate_setup_teardown(parse_ast_error_tests, NULL, test_teardown, parse_ast_error_cases[21].name),
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
