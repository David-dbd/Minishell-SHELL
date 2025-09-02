/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_structs.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:51:54 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/02 19:28:43 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_STRUCTS_H
# define MINISHELL_STRUCTS_H

# include "minishell.h"

typedef struct s_var	t_var;
typedef struct s_env	t_env;
typedef struct s_token	t_token;
typedef struct s_token	t_node;

typedef enum e_type
{
	START,
	WORD,
	GROUP,
	PIPE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	COMMAND,
	SCAPE,
	BUILT_IN,
	SEMICOLON,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	CMD_SUBSTITUTION,
	PAREN_OPEN,
	PAREN_CLOSE,
	WILDCAR,
	EXPANSION,
	BACKGROUND,
	AND,
	OR,
	EXPAN_VALUE,
	EXPAN_CMD,
	NO_SPACE,
	LOCAL,
	ENV,
}	t_type;

struct s_token
{
	int			id;
	t_type		type;
	char		*value;
	bool		single_quoted;
	bool		double_quoted;
	bool		expand;
	t_env		*env;
};

typedef struct s_prompt
{
	int			n_parentesis;
	int			n_single_quotes;
	int			n_double_quotes;
	int			n_escape;
	int			n_pipes;
	int			n_cmds;
	int			n_or;
	int			n_and;
	int			n_tokens;
	int			n_alloc_tokens;
	int			n_redir_input;
	int			n_redir_output;
	char		*prompt;
	t_token		*tokens;
}	t_prompt;

struct s_var
{
	char	*key;
	char	*value;
	t_type	type;
	t_var	*next;
};

typedef struct s_env
{
	char	**og_env;
	t_var	*vars;
	int		size;
}	t_env;

typedef struct s_exec
{
	pid_t		*pids;
	int			n_pids;
	int			n_pipes;
	int			**pipes;
	int			original_stdin;
	int			original_stdout;
	t_env		*env;
}	t_exec;

struct s_node
{
	int			id;
	t_type		type;
	t_token		token;
	t_node		*left;
	t_node		*right;
	pid_t		pid;
	int			exit_code;
	bool		executed;
};

typedef struct s_shell
{
	t_prompt	prompt;
	t_node		*ast_root;
	t_exec		executor;
	t_env		env;
	time_t		session_start;
	int			last_exit_code;
}	t_shell;

#endif
