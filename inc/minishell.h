/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 22:31:39 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/30 02:05:42 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
/* ************************************************************************** */
/*                               Includes                                     */
/* ************************************************************************** */
# include "../lib/isocline/include/isocline.h"
# include "../lib/libft_plus/libft_plus.h"
# include "minishell_structs.h"
# include "minishell_macros.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <strings.h>
# include <termios.h>
# include <dirent.h>
# include <string.h>
# include <signal.h>
# include <errno.h>
# include <term.h>
# include <time.h>

/* ************************************************************************** */
/*                                 Globales                                   */
/* ************************************************************************** */
extern volatile sig_atomic_t	g_signal[2];
extern const char				*g_type_names[];

/* ************************************************************************** */
/*                             Initialization                                 */
/* ************************************************************************** */
void	init_minishell(t_shell *data, int argc, char **argv, char **envp);
void	init_data(t_shell *data, char **envp);
void	init_arg(t_shell *data, int ac, char **av);
void	init_exec(t_exec *exec, t_env *env);
void	init_enviroment(t_shell *data, t_env *env, char **envp);
void	init_ic_readline(void);

/* ************************************************************************** */
/*                               Tokenizer                                    */
/* ************************************************************************** */
char	*receive_input(char **input, t_shell *data);
int		tokenizer(t_shell *data, t_prompt *prompt, char *input);
void	get_tokens(t_shell *data, t_prompt *prompt, char *input);
int		add_token(t_shell *data, t_prompt *prompt, char *value, int type);

/* ************************************************************************** */
/*                               Expansion                                    */
/* ************************************************************************** */
int		expansion(t_shell *data, t_token *token, int phase);
int		asignation(t_shell *data, t_token *token, int type);

/* ************************************************************************** */
/*                                  AST                                       */
/* ************************************************************************** */
void	ast_builder(t_shell *data, t_token *tokens, int n_tokens);
t_node	*create_node(t_shell *data, t_token *token, t_type type);
t_node	*create_true_node(t_shell *data, t_type type);
t_node	*parse_sequence(t_shell *data, t_token *tokens, int *i, int n_tokens);
t_node	*parse_and_or(t_shell *data, t_token *tokens, int *i, int n_tokens);
t_node	*parse_pipes(t_shell *data, t_token *tokens, int *i, int n_tokens);
t_node	*parse_subshell(t_shell *data, t_token *tokens, int *i, int n_tokens);
t_node	*parse_assignations(t_shell *d, t_token *tokens, int *i, int n_tokens);
t_node	*parse_cmd(t_shell *data, t_token *tokens, int *i, int n_tokens);

/* ************************************************************************** */
/*                                Executor                                    */
/* ************************************************************************** */
void	executor_recursive(t_shell *data, t_node *root, t_exec *exec, int mode);
void	exec_semicolon(t_shell *data, t_node *node, t_exec *exec, int mode);
void	exec_and(t_shell *data, t_node *node, t_exec *exec, int mode);
void	exec_or(t_shell *data, t_node *node, t_exec *exec, int mode);
void	exec_pipe(t_shell *data, t_node *node, t_exec *exec, int mode);
void	exec_subshell(t_shell *data, t_node *node, t_exec *exec, int mode);
void	exec_builtin(t_shell *data, t_node *node, int mode);
void	exec_command(t_shell *data, t_node *node, t_exec *exec, int mode);

/* ************************************************************************** */
/*                                buil_in                                     */
/* ************************************************************************** */
int		my_pwd(void);
int		my_echo(char **args);
int		my_env(t_var *vars, char **args);
int		my_cd(t_shell *data, char **args);
void	my_exit(t_shell *data, char **args);
int		my_alias(t_shell *data, t_cmd *cmd, char **args);
int		my_unset(t_shell *data, t_env *env, char **args);
int		my_cd(t_shell *data, char **args);
int		my_unalias(t_cmd	*cmd, char **args);
int		my_export(t_shell *data, t_token *tokens, t_env *env, t_node *node);

/* ************************************************************************** */
/*                                Signals                                     */
/* ************************************************************************** */
void	setup_signals_interactive(void);
void	setup_signals_child(void);
void	setup_signals_heredoc(void);
void	handle_sigint_interative(int sig);
void	handle_sigint_heredoc(int sig);
int		check_signal_node_heredoc(t_node *node, t_redir *redir);
int		check_signals(t_shell *data, t_redir *redir, char *line, int *pipe_fd);
void	setup_signals_cmd_correction(void);
int		check_signal_cmd_correction(t_shell *data);

/* ************************************************************************** */
/*                                 Clean                                      */
/* ************************************************************************** */
void	clean_all(t_shell *data);
void	clean_ast(t_node *node);
void	clean_node(t_node **node);
void	clean_prompt(t_prompt *prompt);
void	clean_token(t_token **token);
void	clean_tokens(t_prompt **prompt);
void	clean_redirs(t_redir **lst);
void	clean_env(t_env *env, t_var *vars);
void	clean_extras(t_extras *extras);
void	clean_cycle(t_shell *data, t_prompt *prompt, t_node **ast_root);
void	my_clean_unset(t_shell *data, t_env *env, t_token *tokens, int *index);
void	clean_temp_variables(t_shell *d, t_env *e, t_token *t, t_node *node);
void	restore_fd(t_exec *exec);
void	clean_fd(t_exec *exec);

/* ************************************************************************** */
/*                                 Exits                                      */
/* ************************************************************************** */
int		exit_error(t_shell *data, const char *error, int exit_code, ...);
int		exit_succes(t_shell *data, char *msg, int exit_code);
int		syntax_error(t_shell *data, const char *error, int exit_code, ...);

/* ************************************************************************** */
/*                                 utils                                      */
/* ************************************************************************** */
// INIT
void	allocate_tokens(t_shell *data, t_prompt *prompt, char *input);

// GET TOKENS
void	is_cmd(t_shell *data, t_token *token, char *str);
void	is_word(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_dolar(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_single_quote(t_shell *d, t_prompt *prompt, const char *s, int *i);
void	is_double_quote(t_shell *d, t_prompt *prompt, const char *s, int *i);
void	is_wildcar(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_scape(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_redir(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_heredoc(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_semicolon(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_cmdsubs(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_pipe(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_or(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_and(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_parenten(t_shell *data, t_prompt *prompt, const char *str, int *i);
void	is_not_token(const char *str, int *i);
void	is_hash(const char *str, int *i);
int		is_symbol(int c);
int		is_primitive_cmd_type(int type);
int		is_arg_type(int type);
int		is_quote_type(int type);
int		is_redir_type(int type);
int		is_delimiter_type(int type);
int		is_alloc_type(int type);
int		is_between_quotes_type(int type);
int		is_simplify_type(int type);
int		is_cmd_builtin_type(int type);
int		is_asignation_type(int type);
int		is_real_assignation_type(int type);
int		is_redir_output_type(int type);
int		is_redir_input_type(int type);
int		is_invalid_char(int c);
int		is_built_in(t_token *token, char *str);

//VALID TOKENS
void	is_it_quoted(t_prompt *prompt, t_token *tokens);
int		check_double_balance(t_shell *d, t_prompt *p, t_token *t);
int		check_single_balance(t_shell *d, t_prompt *p, t_token *t);
int		check_semicolon(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_background(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_cmd_syntax(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_open_parent(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_redir_input(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_close_parent(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_redir_output(t_shell *d, t_prompt *p, t_token *t, int i);
int		check_pipe(t_shell *data, t_prompt *prompt, t_token *tokens, int i);
int		check_double_parent(t_shell *data, t_token *tokens, t_prompt *prompt);
int		check_or_and(t_shell *data, t_prompt *prompt, t_token *tokens, int i);
int		check_parent_balance(t_shell *data, t_prompt *prompt, t_token *tokens);
int		check_cmd_externs(t_shell *d, t_prompt *prompt, t_token *tokens, int i);

//SIMPLIFY TOKENS
int		find_range_end(t_token *tokens, int no_space_position);
int		find_range_start(t_token *tokens, int no_space_position);
void	remove_quotes_tokens(t_prompt *prompt, t_token *tokens);
void	simplify_tokens(t_shell *data, t_prompt *prompt, t_token *tokens);
void	reorganize_tokens(t_prompt *p, t_token *tokens, int *range, char *res);

//TRANSFORM TOKENS
void	transform_word_to_file(t_prompt *prompt, t_token *tokens);
void	transform_asig_to_asig_plus(t_prompt *pro, t_token *tokens);
void	transform_cmd_to_word(t_shell *data, t_token *tokens, int phase);
void	transform_invalid_asig_to_word(t_prompt *prompt, t_token *tokens);
void	transform_word_to_asignation(t_shell *data, t_token *tokens, int phase);
void	transform_tokens_logic(t_shell *data, t_prompt *promp, t_token *tokens);
void	transform_word_to_file(t_prompt *prompt, t_token *tokens);
void	transform_command_built_lowercase(t_prompt *prompt, t_token *tokens);
void	transform_asig_to_temp(t_prompt *prompt, t_token *tokens);
void	transform_cmd_to_built_in(t_prompt *prompt, t_token *tokens);

//AST
int		get_heredoc(t_shell *data, t_redir *redir, char *delimiter);
void	expand_alias(t_shell *data, t_token *tokens, int i);
int		get_background(t_token *tokens, int n_tokens, int *i);
int		*get_arg_types(t_shell *data, t_node *node, int i, int j);
char	**get_args_for_binary(t_shell *data, t_token *token, int *i);
char	**get_temp_asignations(t_shell *data, t_token *tokens, int i);
t_redir	*get_redirs(t_shell *data, t_token *tokens, int *i, int mode);


//EXECUTOR
char	*get_path(t_shell *data, char *cmd, char **envp);
void	which_builtin(t_shell *data, t_token *token, t_node *node);
int		apply_properties(t_shell *data, t_node *node, int mode);
int		apply_redirs(t_shell *data, t_node *node, int mode);
void	apply_temp_asig(t_shell *da, t_token *tokens, t_node *node);

//EXPANSION
int		copy_key(char *buffer, char **key_to_find);
int		find_key_in_lst(t_shell *d, t_token *t, char **key_to_f);
int		extract_key(t_shell *d, t_token *t, char **key_to_f, int phase);
int		expand_empty_str(t_shell *data, t_token *token, char **key_to_find);
int		copy_value(t_shell *d, char **t_val, char *key_value, char *key_to_f);

//ASIGNATION
int		check_asignation_syntax(t_token *token, int type);
void	eliminate_temp_asig(t_prompt *prompt, t_token *tokens);
int		send_tokens_for_asig(t_shell *data, t_token *tokens, int phase);
int		is_it_asig(t_shell *data, t_token *token, int type);
int		verify_if_already_set(t_shell *data, char *key, char **value, int t);
int		check_externs_syntax(t_shell *d, t_token *tkens, t_token *token, int t);

//ENV
void	add_var(t_shell *data, char *key, char *value, int type);
void	add_var_and_envp(t_shell *data, char *key, char *value, int type);
void	update_shlvl(t_var *vars);
void	delete_var(t_shell *data, t_env *env, char *key);
void	*lstlast_var(void *data, char type);
void	path_null_no_env(t_shell *data, char **path);
char	**make_envp(t_shell *data, t_env *env, t_var *vars);
void	update_envp(t_shell *data);
void	update_var(t_shell *data, char *new_value, char *key_to_find);
char	*get_var_value(t_var *vars, const char *key);

//HIGHLIGHTHER
void	highlighter(ic_highlight_env_t *henv, const char *input, void *args);
int		double_string_hl(ic_highlight_env_t *henv, const char *s, size_t *i);
int		single_string_hl(ic_highlight_env_t *henv, const char *s, size_t *i);
int		and_hl(ic_highlight_env_t *henv, const char *s, size_t *i);
int		or_hl(ic_highlight_env_t *henv, const char *s, size_t *i);
int		pipe_hl(ic_highlight_env_t *henv, const char *s, size_t *i);
int		pipe_hl(ic_highlight_env_t *henv, const char *s, size_t *i);
int		redir_hl(ic_highlight_env_t *env, const char *s, size_t *i);
int		separator_hl(ic_highlight_env_t *env, const char *s, size_t *i);
int		paren_hl(ic_highlight_env_t *henv, const char *s, size_t *i);
int		number_hl(ic_highlight_env_t *henv, const char *s, size_t *i);
int		background_hl(ic_highlight_env_t *henv, const char *s, size_t *i);

//DEBUG
void	print_ast(t_node *root);
void	print_tokens_debug(t_prompt *prompt);

//UTILS
char	*cleanner_slash_quotes_d(t_shell *data, char *word, int len, int *flag);
char	*clean_slash_expan_d(t_shell *data, char *word, int len, char slash);
void	clean_quote_until_slash_d(char *word, char *clean_word, char quote);
void	void_tokens_at_the_end(t_token *tokens, int n_alloc, int n_tokens);
void	eliminate_token(t_prompt *prompt, t_token *tokens, int index);
int		cmd_correction(t_shell *data, t_token *tokens, int n_tokens);
void	safe_index_plus(int *i, int n_tokens);
void	normalize_token_to_lower(char *str);

//BUILD_IN
int		find_cmd(t_shell *data, t_cmd *cmd, char *to_find, char *alias);
int		check_arg_syntax(char *arg, const char *built_in_err);

/* ************************************************************************** */
/*                                  extras                                    */
/* ************************************************************************** */
void	print_session_end(time_t start);
void	print_time_of_day(time_t start, char *user_name);
void	print_session_start(t_shell *data, time_t start, char **user_name);
void	add_node_rule(t_shell *data, char *value, char *alias, int state);

#endif
