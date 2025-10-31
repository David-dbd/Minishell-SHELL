/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 19:24:46 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 16:43:00 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	special_cases_path_dir(t_shell *data, char *cmd)
{
	if (ft_strcmp(cmd, ".") == 0)
		return ((void)exit_error(data, ERR_IS_DIR, 126, cmd));
	if (ft_strcmp(cmd, "..") == 0)
		return ((void)exit_error(data, ERR_CMD_NOT_FOUND, 127, cmd));
	if (ft_strcmp(cmd, "") == 0)
		return ((void)exit_error(data, ERR_CMD_NOT_FOUND, 127, cmd));
}

char *build_and_check_path(t_shell *data, const char *dir, const char *cmd)
{
	char *path;
	char *tmp;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!path)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	if (access(path, X_OK) == 0)
		return (path);
	free(path);
	return NULL;
}

static char	*search_in_path(t_shell *data, char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (build_and_check_path(data, ".", cmd));
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	i = -1;
	while (paths[++i])
	{
		path = build_and_check_path(data, paths[i], cmd);
		if (path)
		{
			ft_free_str_array(paths);
			return (path);
		}
	}
	ft_free_str_array(paths);
	return (NULL);
}

char	*get_path(t_shell *data, char *cmd, char **envp)
{
	char		*path;
	struct stat	st;

	special_cases_path_dir(data, cmd);
	if (ft_strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == -1)
			return (exit_error(data, ERR_FILE_NOT_FOUND, 127, cmd), NULL);
		if (S_ISDIR(st.st_mode))
			return (exit_error(data, ERR_IS_DIR, 126, cmd), NULL);
		if (access(cmd, X_OK) != 0)
			return (exit_error(data, ERR_PERM_DENIED, 126, cmd), NULL);
		path = ft_strdup(cmd);
		if (!path)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		return (path);
	}
	path = search_in_path(data, cmd, envp);
	if (!path)
		return (exit_error(data, ERR_CMD_NOT_FOUND, 127, cmd), NULL);
	return (path);
}
