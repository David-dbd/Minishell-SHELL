/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_macros.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:23:16 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/11 06:44:54 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_MACROS_H
# define MINISHELL_MACROS_H

/* ************************************************************************** */
/*                              File Descriptors                             */
/* ************************************************************************** */
# define STDIN						0
# define STDOUT						1
# define STDERR						2

/* ************************************************************************** */
/*                              Boolean Values                                */
/* ************************************************************************** */
# define TRUE						1
# define FALSE						0
# define SUCCESS					1
# define FAILURE					0

/* ************************************************************************** */
/*                              Return Values                                 */
/* ************************************************************************** */
# define ERROR						-1
# define OK							0
# define YES						1
# define NO							0
# define NOT_FOUND					0
# define SYNTAX_ERROR				0
# define IGNORE						2
# define REPLACED					2
# define VOID						0
# define RECIVED_SIGNAL				1
# define FAIL						1

/* ************************************************************************** */
/*                               Global Flag                                  */
/* ************************************************************************** */
# define CONTINUE					0
# define CLEAN						1
# define RESET						-1
# define VIRGIN						0
# define FORDWARD					1
# define BACKWARDS					-1
# define INITIAL_PHASE				1
# define FINAL_PHASE				2
# define FATHER						1
# define CHILD						2
# define LEFT						2
# define RIGHT						1

/* ************************************************************************** */
/*                               Exit Codes                                   */
/* ************************************************************************** */
# define EXIT_SUCCESS				0
# define EXIT_FAILURE				1
# define EXIT_FAIL					1
# define EXIT_USE					2
# define EXIT_CMD_NOT_FOUND			127
# define EXIT_CMD_NOT_EXEC			126
# define EXIT_FATAL_SIGNAL			128
# define EXIT_CTRL_C				130

/* ************************************************************************** */
/*                              File Operations                               */
/* ************************************************************************** */
# define READ_MODE					0
# define WRITE_MODE					1
# define FILE_PERMS					0644
# define DIR_PERMS					0755

/* ************************************************************************** */
/*                               Buffer Sizes                                 */
/* ************************************************************************** */
# define BUFFER_SIZE				1024
# define INIT_TOKENS				32
# define PATH_MAX_SIZE				4096
# define CMD_MAX_LEN				1024
# define ARG_MAX_COUNT				1024
# define ENV_VAR_MAX				1024
# define PROMPT_MAX					256

/* ************************************************************************** */
/*                              Built-in Commands                             */
/* ************************************************************************** */
# define BUILTIN_ECHO				"echo"
# define BUILTIN_CD					"cd"
# define BUILTIN_PWD				"pwd"
# define BUILTIN_EXPORT				"export"
# define BUILTIN_UNSET				"unset"
# define BUILTIN_ENV				"env"
# define BUILTIN_EXIT				"exit"

/* ************************************************************************** */
/*                              Default Values                                */
/* ************************************************************************** */
# define DEFAULT_PATH				"/usr/bin:/bin:/usr/sbin:/sbin"
# define DEFAULT_PROMPT				"\033[1;34mminishell\033[1;34m> \033[0m"
# define DEFAULT_HISTORY			"./ext/history_minishell.txt"
# define DEFAULT_HOME

/* ************************************************************************** */
/*                                   ARGS                                     */
/* ************************************************************************** */
# define NO_FILE					0
# define ILIMITED					-1

/* ************************************************************************** */
/*                              Signal Handling                               */
/* ************************************************************************** */
# define SIG_INTERACTIVE			1
# define SIG_CHILD					2
# define SIG_HEREDOC				3
# define SIG_IGNORE					4

/* ************************************************************************** */
/*                              Error Messages                                */
/* ************************************************************************** */
//General Errors
# define ERR_MALLOC		"minishell: memory allocation failed\n"
# define MALLOC			"minishell: memory allocation failed\n"
# define ERR_FORK		"minishell: fork: Resource temporarily unavailable\n"
# define ERR_PIPE		"minishell: pipe: Too many open files\n"
# define ERR_DUP		"minishell: dup2: Bad file descriptor\n"
# define ERR_SIGNAL		"minishell: signals state failed\n"
# define ERR_MAX_TOKENS "minishell: error: maximum number of tokens exceeded\n"
# define ERRNO			"Errno: %s\n"

//File Directory Errors
# define ERR_FILE_NOT_FOUND	"minishell: %s: No such file or directory\n"
# define ERR_PERM_DENIED	"minishell: %s: Permission denied\n"
# define ERR_IS_DIR			"minishell: %s: Is a directory\n"
# define ERR_NOT_DIR		"minishell: %s: Not a directory\n"

//Command Errors
# define ERR_CMD_NOT_FOUND	"minishell: %s: command not found\n"
# define ERR_CMD_NOT_EXEC	"minishell: %s: cannot execute binary file\n"
# define ERR_EXEC			"minishell: %s: cannot execute binary file: \
Exec format error\n"

//Built-in Errors
# define ERR_CD_TOO_MANY	"minishell: cd: too many arguments\n"
# define ERR_CD_NO_HOME		"minishell: cd: HOME not set\n"
# define ERR_CD_OLDPWD		"minishell: cd: OLDPWD not set\n"
# define ERR_EXPORT			"minishell: export: `%s': not a valid identifier\n"
# define ERR_UNSET			"minishell: unset: `%s': not a valid identifier\n"
# define ERR_EXIT_NUMERIC	"minishell: exit: %s: numeric argument required\n"
# define ERR_EXIT_TOO_MANY	"minishell: exit: too many arguments\n"
# define ERR_ENV		"minishell: env: invalid option\n"
# define ERR_PWD		"minishell: pwd: error retrieving current directory\n"

//Syntax Errors
# define ERR_SYNTAX		"minishell: syntax error near unexpected token `%s'\n"
# define ERR_COMPLETE	"minishell: syntax error near command completion\n"
# define ERR_SYNTAX_EOF		"minishell: syntax error: unexpected end of file\n"
# define ERR_HEREDOC_EOF	"minishell: warning: here-document at line %d \
delimited by end-of-file (wanted `%s')\n"
# define ERR_DOUBLE_PARENT	"minishell: ((: %s: syntax error in expression \
(error token is \"%s\")\n"

//Variable Errors
# define ERR_VAR_NOT_SET	"minishell: %s: parameter null or not set\n"
# define ERR_BAD_SUBST		"minishell: %s: bad substitution\n"
# define ERR_SHLVL_HIGH		"\033[1m\033[1;31mminishell: warning: shell level \
(INT_MAX) too high, resetting to 1\033[0m\n"
# define ERR_SHLVL_LOW		"\033[1m\033[1;31mminishell: warning: shell level \
is negative resetting to 1\033[0m\n"

//Redirection Errors
# define ERR_AMBIGUOUS_REDIR	"minishell: ambiguous redirect\n"
# define ERR_NO_INPUT_FILE		"minishell: No such file or directory\n"
# define ERR_REDIR_DENIED		"minishell: Permission denied\n"

/* ************************************************************************** */
/*                          Time messsage                                     */
/* ************************************************************************** */
# define MSG_TIME_START "\
\033[0;36m Your session started \
at: %02d:%02d:%02d on %02d/%02d/%04d\033[0m\n\n"
# define MSG_TIME_END "\
\n\033[0;36m Your session ended. \
Duration: %d minutes and %d seconds ⏳\033[0m\n\n"

/* ************************************************************************** */
/*                          Prompt & Success Messages                         */
/* ************************************************************************** */
# define MSG_WELCOME			"\
\033[1m\033[1;32m────────── Welcome %s! ──────────\033[0m\n"
# define MSG_WELCOME_EARLY		"\
\033[1;32m ────────────── You're up early, %s! 🌅 ──────────\033[0m\n\n"
# define MSG_WELCOME_MORNING	"\
\033[1;32m ───────────── Good morning, %s! ☀️ ──────────\033[0m\n\n"
# define MSG_WELCOME_AFTER		"\
\033[1;32m ───────────── Good afternoon, %s! 🌤️ ──────────\033[0m\n\n"
# define MSG_WELCOME_NIGHT		"\
\033[1;32m ───────────── Good evening, %s! 🌙 ─────────────\033[0m\n\n"
# define MSG_WELCOME_MIDNIGHT	"\
\033[1;32m ────────── %s, Burning the midnight oil? 🔥 ──────────\033[0m\n\n"
# define MSG_GOODBYE			"\
\033[1m\033[1m\033[38;5;99m────────────── Goodbye %s! ──────────────\033[0m\n"
# define MSG_GOODBYE_V2			"\
\033[1m\033[1m\033[38;5;99m────────────── Goodbye! ──────────────\033[0m\n"
# define MINISHELL				"\
\033[1;34mminishell>\033[0m "

/* ************************************************************************** */
/*                          Error User_name Messages                          */
/* ************************************************************************** */
# define MSG_INVALID_NAME "\
\033[1m\033[1;31mInvalid loggin: It must be alphanumeric, \
please try again \033[0m\n\n"

/* ************************************************************************** */
/*                          Entry Message                                     */
/* ************************************************************************** */
# define TITLE_COLOR "\033[0;34m"
# define T1 "███    ███ ██ ███    ██ ██ ███████ ██   ██ ███████ ██      ██     "
# define T2 "████  ████ ██ ████   ██ ██ ██      ██   ██ ██      ██      ██     "
# define T3 "██ ████ ██ ██ ██ ██  ██ ██ ███████ ███████ █████   ██      ██     "
# define T4 "██  ██  ██ ██ ██  ██ ██ ██      ██ ██   ██ ██      ██      ██     "
# define T5 "██      ██ ██ ██   ████ ██ ███████ ██   ██ ███████ ███████ ███████"
# define RESET_COLOR "\033[0m"

#endif
