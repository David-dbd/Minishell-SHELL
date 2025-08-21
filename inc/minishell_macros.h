/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_macros.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:23:16 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/21 21:56:26 by migarrid         ###   ########.fr       */
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

/* ************************************************************************** */
/*                               Global Flag                                  */
/* ************************************************************************** */
# define CONTINUE					0
# define CLEAN						1
# define RESET						-1
# define VIRGIN						0

/* ************************************************************************** */
/*                               Exit Codes                                   */
/* ************************************************************************** */
# define EXIT_SUCCESS				0
# define EXIT_FAILURE				1
# define EXIT_USE					2
# define EXIT_CMD_NOT_FOUND			127
# define EXIT_CMD_NOT_EXECUTABLE	126
# define EXIT_FATAL_SIGNAL			128
# define EXIT_CTRL_C				130
# define EXIT_CTRL_D				131

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
# define DEFAULT_PROMPT				"minishell$ "
# define DEFAULT_HOME

/* ************************************************************************** */
/*                               Parser utils                                 */
/* ************************************************************************** */
# define SPECIAL_CHARS				"''"

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
# define ERR_FORK		"minishell: fork: Resource temporarily unavailable\n"
# define ERR_PIPE		"minishell: pipe: Too many open files\n"
# define ERR_DUP		"minishell: dup2: Bad file descriptor\n"
# define ERR_SIGNAL		"minishell: signals state failed\n"
# define ERRNO			"Errno: %s\n"

//File Directory Errors
# define ERR_FILE_NOT_FOUND	"minishell: %s: No such file or directory\n"
# define ERR_PERM_DENIED	"minishell: %s: Permission denied\n"
# define ERR_IS_DIR			"minishell: %s: Is a directory\n"
# define ERR_NOT_DIR		"minishell: %s: Not a directory\n"

//Command Errors
# define ERR_CMD_NOT_FOUND	"minishell: %s: command not found\n"
# define ERR_CMD_NOT_EXEC	"minishell: %s: cannot execute binary file\n"
# define ERR_EXEC_FORMAT "minishell: %s: cannot execute binary file: Exec format error\n"

//Built-in Errors
# define ERR_CD_TOO_MANY	"minishell: cd: too many arguments\n"
# define ERR_CD_NO_HOME		"minishell: cd: HOME not set\n"
# define ERR_CD_OLDPWD		"minishell: cd: OLDPWD not set\n"
# define ERR_EXPORT_INVALID	"minishell: export: `%s': not a valid identifier\n"
# define ERR_UNSET_INVALID	"minishell: unset: `%s': not a valid identifier\n"
# define ERR_EXIT_NUMERIC	"minishell: exit: %s: numeric argument required\n"
# define ERR_EXIT_TOO_MANY	"minishell: exit: too many arguments\n"
# define ERR_PWD_ERROR		"minishell: pwd: error retrieving current directory\n"
# define ERR_ENV_NO_ARGS	"minishell: env: invalid option\n"

//Syntax Errors
# define ERR_SYNTAX			"minishell: syntax error near unexpected token `%s'\n"
# define ERR_COMPLETE		"minishell: syntax error near command completion\n"
# define ERR_SYNTAX_EOF		"minishell: syntax error: unexpected end of file\n"
# define ERR_SYNTAX_NEWLINE	"minishell: syntax error near unexpected token `newline'\n"
# define ERR_UNCLOSED_QUOTE	"minishell: unexpected EOF while looking for matching `%c'\n"
# define ERR_HEREDOC_EOF	"minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n"

//Variable Errors
# define ERR_VAR_NOT_SET	"minishell: %s: parameter null or not set\n"
# define ERR_BAD_SUBST		"minishell: %s: bad substitution\n"

//Redirection Errors
# define ERR_AMBIGUOUS_REDIR	"minishell: ambiguous redirect\n"
# define ERR_NO_INPUT_FILE	"minishell: No such file or directory\n"
# define ERR_REDIR_DENIED	"minishell: Permission denied\n"

/* ************************************************************************** */
/*                          Time messsage                                     */
/* ************************************************************************** */
# define MSG_TIME_START "\033[0;36m Session started at: %02d:%02d:%02d on %02d/%02d/%04d\033[0m\n\n"
# define MSG_TIME_END "\n\033[0;36m Session ended. Duration: %d minutes and %d seconds ⏳\033[0m\n\n"

/* ************************************************************************** */
/*                          Prompt & Success Messages                         */
/* ************************************************************************** */
# define MSG_WELCOME		    "\033[1m\033[1;32m────────── Welcome! ──────────\033[0m\n"
# define MSG_WELCOME_EARLY		"\033[1;32m ────────── You're up early! 🌅 ──────────\033[0m\n\n"
# define MSG_WELCOME_MORNING	"\033[1;32m ────────── Good morning, Commander! ☀️ ──────────\033[0m\n\n"
# define MSG_WELCOME_AFTER		"\033[1;32m ────────── Good afternoon, Explorer! 🌤️ ──────────\033[0m\n\n"
# define MSG_WELCOME_NIGHT		"\033[1;32m ────────── Good evening, Night Owl! 🌙 ──────────\033[0m\n\n"
# define MSG_WELCOME_MIDNIGHT	"\033[1;32m ────────── Burning the midnight oil? 🔥 ──────────\033[0m\n\n"
# define MSG_GOODBYE		    "\033[1m\033[1m\033[38;5;99m────────── Goodbye! ──────────\033[0m\n"
# define MINISHELL			    "\033[1;34mminishell>\033[0m "

/* ************************************************************************** */
/*                          Entry Message                                     */
/* ************************************************************************** */

#define MSG_TITLE_4 \
"\033[0;34m" \
"\n\n ███    ███ ██ ███    ██ ██ ███████ ██   ██ ███████ ██      ██      \n" \
" ████  ████ ██ ████   ██ ██ ██      ██   ██ ██      ██      ██      \n" \
" ██ ████ ██ ██ ██ ██  ██ ██ ███████ ███████ █████   ██      ██      \n" \
" ██  ██  ██ ██ ██  ██ ██ ██      ██ ██   ██ ██      ██      ██      \n" \
" ██      ██ ██ ██   ████ ██ ███████ ██   ██ ███████ ███████ ███████ \n" \
"\n\n" \
"\033[0m"

#define MSG_TITLE_5 \
"\033[0;34m" \
"\n::::    ::::  ::::::::::: ::::    ::: ::::::::::: ::::::::  :::    ::: :::::::::: :::        :::\n" \
"+:+:+: :+:+:+     :+:     :+:+:   :+:     :+:    :+:    :+: :+:    :+: :+:        :+:        :+:\n" \
"+:+ +:+:+ +:+     +:+     :+:+:+  +:+     +:+    +:+        +:+    +:+ +:+        +:+        :+:\n" \
"+#+  +:+  +#+     +#+     +#+ +:+ +#+     +#+    +#++:++#++ +#++:++#++ +#++:++#   +#+        +#+\n" \
"+#+       +#+     +#+     +#+  +#+#+#     +#+           +#+ +#+    +#+ +#+        +#+        +#+\n" \
"#+#       #+#     #+#     #+#   #+#+#     #+#    #+#    #+# #+#    #+# #+#        #+#        #+#\n" \
"###       ### ########### ###    #### ########### ########  ###    ### ########## ########## ##########\n\n" \
"\033[0m"


#endif
