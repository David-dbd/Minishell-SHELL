/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 02:32:54 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/30 01:51:29 by migarrid         ###   ########.fr       */
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
		if (mode == CHILD)
			return (exit_error(data, NULL, EXIT_FAILURE));
		if (mode == FATHER)
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
		if (mode == CHILD)
			return (exit_error(data, NULL, EXIT_FAILURE));
		if (mode == FATHER)
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
		if (mode == CHILD)
			return (exit_error(data, NULL, EXIT_FAILURE));
		if (mode == FATHER)
			return (FAIL);
	}
	dup2(file_fd, fd_redir);
	close(file_fd);
	return (OK);
}

void	handle_redir_heredoc(int fd_heredoc)
{
	dup2(fd_heredoc, STDIN_FILENO);
	close(fd_heredoc);
}

int	apply_redirs(t_shell *data, t_node *node, int mode)
{
	t_redir	*curr;

	curr = node->redir;
	while(curr)
	{
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
				handle_redir_heredoc(curr->fd_heredoc);
		curr = curr->next;
	}
	return (SUCCESS);
}
