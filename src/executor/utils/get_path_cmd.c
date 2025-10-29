/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 19:24:46 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/29 02:03:16 by migarrid         ###   ########.fr       */
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

static char	*search_in_path(t_shell *d, char *cmd, char **envp)
{
	char	**paths;
	char	*part_path;
	char	*path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (exit_error(d, ERR_MALLOC, EXIT_FAIL), NULL);
	i = -1;
	while (paths[++i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (!path)
			return (ft_free_str_array(paths), exit_error(d, MALLOC, 1), NULL);
		if (access(path, X_OK) == 0)
			return (ft_free_str_array(paths), path);
		free(path);
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
