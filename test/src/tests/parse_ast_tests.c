/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_tests.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apierret <apierret@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:04:02 by apierret          #+#    #+#             */
/*   Updated: 2025/06/04 14:32:04 by apierret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "parser.h"
#include "test.h"
#include "test_utils.h"

static t_test_case parse_ast_cases[] = {
	{
		.name = "single_command",
		.input_tokens = { "pwd", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "pwd",
				.args = (char*[]){"pwd", NULL}
			},
			.redirs = NULL
		}
	},
	{
		.name = "single_command_with_path",
		.input_tokens = { "/bin/ls", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "/bin/ls",
				.args = (char*[]){"ls", NULL}
			},
			.redirs = NULL
		}
	},
	{
		.name = "one_arg",
		.input_tokens = { "echo", "hello", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = NULL
		}
	},
	{
		.name = "multiple_args",
		.input_tokens = { "echo", "hello", "world", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", "world", NULL}
			},
			.redirs = NULL
		}
	},
	{
		.name = "redir_out",
		.input_tokens = { "echo", "hello", ">", "out.txt", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_append",
		.input_tokens = { "echo", "world", ">>", "out.txt", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "world", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 1},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_in",
		.input_tokens = { "cat", "<", "in.txt", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "cat",
				.args = (char*[]){"cat", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_heredoc",
		.input_tokens = { "cat", "<<", "END", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "cat",
				.args = (char*[]){"cat", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_HEREDOC, .heredoc = "END"},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_out_inverted",
		.input_tokens = { ">", "out.txt", "echo", "hello", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_append_inverted",
		.input_tokens = { ">>", "out.txt", "echo", "world", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "world", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 1},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_in_inverted",
		.input_tokens = { "<", "in.txt", "cat", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "cat",
				.args = (char*[]){"cat", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_heredoc_inverted",
		.input_tokens = { "<<", "END", "cat", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "cat",
				.args = (char*[]){"cat", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_HEREDOC, .heredoc = "END"},
				.next = NULL
			}
		}
	},
	{
		.name = "mult_redir_out",
		.input_tokens = { "echo", "hello", ">", "out1.txt", ">", "out2.txt", ">", "out3.txt", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out1.txt", .append = 0},
				.next = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out2.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_OUT, .out = "out3.txt", .append = 0},
						.next = NULL
					}
				}
			}
		}
	},
	{
		.name = "mult_redir_in",
		.input_tokens = { "echo", "hello", "<", "in1.txt", "<", "in2.txt", "<", "in3.txt", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_IN, .out = "in1.txt", .append = 0},
				.next = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .out = "in2.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_IN, .out = "in3.txt", .append = 0},
						.next = NULL
					}
				}
			}
		}
	},
	{
		.name = "mult_redir_out_both_way",
		.input_tokens = { ">", "out1.txt", ">", "out2.txt", "echo", "hello", ">", "out3.txt", ">", "out4.txt", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out1.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_OUT, .out = "out2.txt", .append = 0},
						.next = &(t_list)
						{
							.content = &(t_redir){.type = REDIR_OUT, .out = "out3.txt", .append = 0},
							.next = &(t_list)
							{
								.content = &(t_redir){.type = REDIR_OUT, .out = "out4.txt", .append = 0},
								.next = NULL
							}
						}
					}
				}
		}
	},
	{
		.name = "mult_redir_in_both_way",
		.input_tokens = { "<", "in1.txt", "<", "in2.txt", "echo", "hello", "<", "in3.txt", "<", "in4.txt", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .out = "in1.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_IN, .out = "in2.txt", .append = 0},
						.next = &(t_list)
						{
							.content = &(t_redir){.type = REDIR_IN, .out = "in3.txt", .append = 0},
							.next = &(t_list)
							{
								.content = &(t_redir){.type = REDIR_IN, .out = "in4.txt", .append = 0},
								.next = NULL
							}
						}
					}
				}
		}
	},
	{
		.name = "mult_redirs_both_way",
		.input_tokens = { "<", "in1.txt", "<", "in2.txt", "echo", "hello", ">", "out1.txt", ">", "out2.txt", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "echo",
				.args = (char*[]){"echo", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_IN, .out = "in1.txt", .append = 0},
				.next = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .out = "in2.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_OUT, .out = "out1.txt", .append = 0},
						.next = &(t_list)
						{
							.content = &(t_redir){.type = REDIR_OUT, .out = "out2.txt", .append = 0},
							.next = NULL
						}
					}
				}
			}
		}
	},
	{
		.name = "single_pipe",
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
					.command = &(t_command)
					{
						.path = "ls",
						.args = (char*[]){"ls", NULL}
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
							.args = (char*[]){"grep", ".c", NULL}
						},
						.redirs = NULL
					},
					.next = NULL
				}
			}
		}
	},
	{
		.name = "multiple_pipes",
		.input_tokens = { "ls", "|", "grep", ".c", "|", "wc", "-l", NULL },
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
						.args = (char*[]){"ls", NULL}
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
							.args = (char*[]){"grep", ".c", NULL}
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
								.args = (char*[]){"wc", "-l", NULL}
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
		.name = "pipes_and_redir",
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
					.command = &(t_command)
					{
						.path = "cat",
						.args = (char*[]){"cat", NULL}
					},
					.redirs = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_IN, .in = "in.txt"},
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
							.args = (char*[]){"grep", "hello", NULL}
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
								.args = (char*[]){"wc", "-l", NULL}
							},
							.redirs = &(t_list)
							{
								.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
								.next = NULL
							}
						},
						.next = NULL
					}
				}
			}
		}
	},
	{
		.name = "command_and",
		.input_tokens = { "make", "&&", "./program", NULL },
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
					.args = (char*[]){"make", NULL}
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
					.args = (char*[]){"program", NULL}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "command_or",
		.input_tokens = { "mkdir", "folder", "||", "echo", "fail", NULL },
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
					.args = (char*[]){"mkdir", "folder", NULL}
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
					.args = (char*[]){"echo", "fail", NULL}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "subshell",
		.input_tokens = { "(", "pwd", ")", NULL },
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
					.args = (char*[]){"pwd", NULL}
				},
				.redirs = NULL
			},
			.redirs = NULL
		}
	},
	{
		.name = "and_with_redir",
		.input_tokens = { "echo", "success", ">", "out.txt", "&&", "cat", "<", "in.txt", NULL },
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
					.args = (char*[]){"echo", "success", NULL}
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
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
					.args = (char*[]){"cat", NULL}
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
		.name = "or_with_pipe",
		.input_tokens = { "echo", "hello", "|", "grep", "world", "||", "ls", "|", "grep", ".h", NULL },
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
							.args = (char*[]){"echo", "hello", NULL}
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
								.args = (char*[]){"grep", "world", NULL}
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
							.args = (char*[]){"ls", NULL}
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
								.args = (char*[]){"grep", ".h", NULL}
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
		.name = "subshell_with_pipe",
		.input_tokens = { "(", "ls", "-la", ")", "|", "wc", "-l", NULL },
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
							.args = (char*[]){"ls", "-la", NULL}
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
							.args = (char*[]){"wc", "-l", NULL}
						},
						.redirs = NULL
					},
					.next = NULL
				}
			},
		}
	},
	{
		.name = "subshell_and_redir",
		.input_tokens = { "(", "echo", "hello", ")", ">", "out.txt", "&&", "echo", "done", NULL },
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
						.args = (char*[]){"echo", "hello", NULL}
					},
					.redirs = NULL
				},
				.redirs = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
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
					.args = (char*[]){"echo", "done", NULL}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "nested_logic",
		.input_tokens = { "make", "&&", "(", "./run", "||", "echo", "fallback", ")", NULL },
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
					.args = (char*[]){"make", NULL}
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
							.args = (char*[]){"run", NULL}
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
							.args = (char*[]){"echo", "fallback", NULL}
						},
						.redirs = NULL
					}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "nested_logic_two",
		.input_tokens = { "(", "make", ")", "&&", "./run", NULL },
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
						.args = (char*[]){"make", NULL}
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
					.args = (char*[]){"run", NULL}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "deep_nested_group",
		.input_tokens = { "(", "(", "(", "echo", "ok", ")", ")", ")", NULL },
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
							.args = (char*[]){"echo", "ok", NULL}
						},
						.redirs = NULL
					},
					.redirs = NULL
				},
				.redirs = NULL
			},
			.redirs = NULL
		}
	},
	{
		.name = "multi_op_complex",
		.input_tokens = { "(", "cat", "file", "|", "grep", "error", ")", "&&", "echo", "found", "||", "echo", "none", NULL },
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
									.args = (char*[]){"cat", "file", NULL}
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
										.args = (char*[]){"grep", "error", NULL}
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
						.args = (char*[]){"echo", "found", NULL},
					},
					.redirs = NULL
				}
			},
			.right = &(t_ast){
				.type = NODE_COMMAND,
				.exit_code = 0,
				.command = &(t_command){
					.path = "echo",
					.args = (char*[]){"echo", "none", NULL}
				},
				.redirs = NULL
			}
		}
	},
	{
		.name = "redir_out_alone",
		.input_tokens = { ">", "out.txt", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_REDIR,
			.exit_code = 0,
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_OUT, .out = "out.txt", .append = 0},
				.next = NULL
			}
		}
	},
	{
		.name = "redir_in_alone",
		.input_tokens = { "<", "in.txt", NULL },
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
	},
	{
		.name = "cmd_arg_between_redirs",
		.input_tokens = { "cat", "<", "in1.txt", "hello", "<", "in2.txt", "<", "in3.txt", NULL },
		.expected_ast = &(t_ast)
		{
			.type = NODE_COMMAND,
			.exit_code = 0,
			.command = &(t_command)
			{
				.path = "cat",
				.args = (char*[]){"cat", "hello", NULL}
			},
			.redirs = &(t_list)
			{
				.content = &(t_redir){.type = REDIR_IN, .out = "in1.txt", .append = 0},
				.next = &(t_list)
				{
					.content = &(t_redir){.type = REDIR_IN, .out = "in2.txt", .append = 0},
					.next = &(t_list)
					{
						.content = &(t_redir){.type = REDIR_IN, .out = "in3.txt", .append = 0},
						.next = NULL
					}
				}
			}
		}
	},
	{ NULL }
};

static t_test_case	parse_ast_error_cases[] = {
	{ .name = "pipe_start", .input_tokens = {"|"}, .expected_error = ERR_SYNTAX },
	{ .name = "pipe_end", .input_tokens = {"cmd", "|"}, .expected_error = ERR_SYNTAX },
	{ .name = "double_pipe", .input_tokens = {"cmd", "|", "|", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "logic_start", .input_tokens = {"&&"}, .expected_error = ERR_SYNTAX },
	{ .name = "logic_end", .input_tokens = {"cmd", "||"}, .expected_error = ERR_SYNTAX },
	{ .name = "logic_double_and", .input_tokens = {"cmd", "&&", "&&", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "logic_double_or", .input_tokens = {"cmd", "||", "||", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "logic_mixed_pipe", .input_tokens = {"cmd", "&&", "|", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "pipe_between_ands", .input_tokens = {"cmd", "&&", "|", "&&", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "redir_without_target", .input_tokens = {"cmd", ">"}, .expected_error = ERR_SYNTAX },
	{ .name = "redir_chain", .input_tokens = {"cmd", ">", ">", "file"}, .expected_error = ERR_SYNTAX },
	{ .name = "redir_pipe_combo", .input_tokens = {"cmd", ">", "|", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "paren_empty", .input_tokens = {"(", ")"}, .expected_error = ERR_SYNTAX },
	{ .name = "open_paren_alone", .input_tokens = {"("}, .expected_error = ERR_SYNTAX },
	{ .name = "close_paren_alone", .input_tokens = {")"}, .expected_error = ERR_SYNTAX },
	{ .name = "unbalanced_parens", .input_tokens = {"(", "cmd", "|", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "nested_unbalanced_parens", .input_tokens = {"(", "(", "cmd", ")", "|"}, .expected_error = ERR_SYNTAX },
	{ .name = "bad_paren_combo", .input_tokens = {"cmd", ")", "&&", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "mixed_logic_op", .input_tokens = {"cmd", "&&", "||", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "consecutive_operators", .input_tokens = {"cmd", "|", "&&", "|", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "pipe_and_or_mix", .input_tokens = {"cmd", "|", "||", "cmd"}, .expected_error = ERR_SYNTAX },
	{ .name = "and_or_chain", .input_tokens = {"cmd", "&&", "||", "&&", "cmd"}, .expected_error = ERR_SYNTAX },
	{ NULL }
};

static void	parse_ast_basic_tests(void **case_name)
{
	t_test_case	*tc;
	t_list		*tokens;
	t_ast		*tested;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return (printf("Implementation error.\n"), assert_true(0));
	tokens = NULL;
	tested = NULL;
	tc = find_case(parse_ast_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	tokens = create_token_list(tc->input_tokens);
	error = parse_ast(&tested, tokens);
	equal = ast_equal(tc->expected_ast, tested);
	free_ast(tested);
	ft_lstclear(&tokens, (void *) free_token);
	if (!equal || error != ERR_NONE)
		return(printf(FAIL_MSG, (char *) *case_name), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

static void	parse_ast_error_tests(void **case_name)
{
	t_test_case	*tc;
	t_list		*tokens;
	t_ast		*tested;
	t_error		error;
	int			equal;

	if (case_name == NULL)
		return (printf("Implementation error.\n"), assert_true(0));
	tokens = NULL;
	tested = NULL;
	tc = find_case(parse_ast_error_cases, *case_name);
	if (tc == NULL)
		return (printf(CASE_NOT_FOUND_MSG, (char *) *case_name), assert_true(0));
	tokens = create_token_list(tc->input_tokens);
	error = parse_ast(&tested, tokens);
	equal = tested == NULL;;
	free_ast(tested);
	ft_lstclear(&tokens, (void *) free_token);
	if (!equal || error != tc->expected_error)
		return(printf(FAIL_MSG, (char *) *case_name), assert_true(0));
	return (printf(SUCCESS_MSG, (char *) *case_name), assert_true(1));
}

t_test_result	execute_tests(void)
{
	const struct CMUnitTest test_cases[] = {
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[0].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[1].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[2].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[3].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[4].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[5].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[6].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[7].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[8].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[9].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[10].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[11].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[12].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[13].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[14].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[15].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[16].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[17].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[18].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[19].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[20].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[21].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[22].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[23].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[24].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[25].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[26].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[27].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[28].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[29].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[30].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[31].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[32].name),
		cmocka_unit_test_prestate(parse_ast_basic_tests, parse_ast_cases[33].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[0].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[1].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[2].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[3].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[4].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[5].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[6].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[7].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[8].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[9].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[10].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[11].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[12].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[13].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[14].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[15].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[16].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[17].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[18].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[19].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[20].name),
		cmocka_unit_test_prestate(parse_ast_error_tests, parse_ast_error_cases[21].name),
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