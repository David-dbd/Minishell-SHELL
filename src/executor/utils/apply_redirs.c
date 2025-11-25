/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 02:32:54 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/25 02:36:26 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	handle_redir_output(t_shell *data, char *filename, int fd_redir, int mode)
{
	int	file_fd;

	file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMS);
	if (file_fd < 0)
	{
		if (errno == ENOENT)
			ft_printf_fd(STDERR, ERR_FILE_NOT_FOUND, filename);
		if (errno == EACCES)
			ft_printf_fd(STDERR, ERR_PERM_DENIED, filename);
		if (errno == EISDIR)
			ft_printf_fd(STDERR, ERR_IS_DIR, filename);
		if (mode == CHILD)
			return (exit_error(data, NULL, EXIT_FAILURE));
		if (mode == FATHER || mode == SUBSHELL)
			return (FAIL);
	}
	dup2(file_fd, fd_redir);
	close(file_fd);
	return (OK);
}

int	handle_redir_append(t_shell *data, char *filename, int fd_redir, int mode)
{
	int	file_fd;

	file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, FILE_PERMS);
	if (file_fd < 0)
	{
		if (errno == ENOENT)
			ft_printf_fd(STDERR, ERR_FILE_NOT_FOUND, filename);
		if (errno == EACCES)
			ft_printf_fd(STDERR, ERR_PERM_DENIED, filename);
		if (errno == EISDIR)
			ft_printf_fd(STDERR, ERR_IS_DIR, filename);
		if (mode == CHILD)
			return (exit_error(data, NULL, EXIT_FAILURE));
		if (mode == FATHER || mode == SUBSHELL)
			return (FAIL);
	}
	dup2(file_fd, fd_redir);
	close(file_fd);
	return (OK);
}

int	handle_redir_input(t_shell *data, char *filename, int fd_redir, int mode)
{
	int	file_fd;

	file_fd = open(filename, O_RDONLY);
	if (file_fd < 0)
	{
		if (errno == ENOENT)
			ft_printf_fd(STDERR, ERR_FILE_NOT_FOUND, filename);
		if (errno == EACCES)
			ft_printf_fd(STDERR, ERR_PERM_DENIED, filename);
		if (errno == EISDIR)
			ft_printf_fd(STDERR, ERR_IS_DIR, filename);
		if (mode == CHILD)
			return (exit_error(data, NULL, EXIT_FAILURE));
		if (mode == FATHER || mode == SUBSHELL)
			return (FAIL);
	}
	dup2(file_fd, fd_redir);
	close(file_fd);
	return (OK);
}

int	handle_redir_heredoc(t_shell *data, t_redir *redir)
{
	char	*line;
	int		pipe_fd[2];
	t_list	*heredoc_line;

	if (pipe(pipe_fd) == ERROR)
		return (exit_error(data, ERR_PIPE, EXIT_FAILURE));
	heredoc_line = redir->heredoc_lines;
	while (heredoc_line)
	{
		line = ft_strdup((char *)heredoc_line->content);
		if (!line)
		{
			close(pipe_fd[1]);
			return (close(pipe_fd[1]), exit_error(data, ERR_MALLOC, 1));
		}
		if (redir->expand)
			expand_line_heredoc(data, &line);
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
		heredoc_line = heredoc_line->next;
	}
	close(pipe_fd[1]);
	redir->fd_heredoc = pipe_fd[0];
	dup2(redir->fd_heredoc, STDIN_FILENO);
	close(redir->fd_heredoc);
	return (SUCCESS);
}

int	apply_redirs(t_shell *data, t_node *node, int mode)
{
	t_redir	*curr;

	curr = node->redir;
	while (curr)
	{
		if ((check_ambiguous_redir(data, curr, mode)) == FAIL)
			return (FAILURE);
		if (curr->type == REDIR_INPUT)
			if (handle_redir_input(data, curr->filename, curr->fd_redir, mode))
				return (FAILURE);
		if (curr->type == REDIR_OUTPUT)
			if (handle_redir_output(data, curr->filename, curr->fd_redir, mode))
				return (FAILURE);
		if (curr->type == REDIR_APPEND)
			if (handle_redir_append(data, curr->filename, curr->fd_redir, mode))
				return (FAILURE);
		if (curr->type == REDIR_HEREDOC)
			handle_redir_heredoc(data, curr);
		curr = curr->next;
	}
	return (SUCCESS);
}
