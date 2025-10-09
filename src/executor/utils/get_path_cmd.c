/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 19:24:46 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/08 23:14:19 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

char	*get_path(t_shell *d, char *cmd, char **envp)
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
	return (exit_error(d, ERR_CMD_NOT_FOUND, EXIT_CMD_NOT_FOUND, cmd), NULL);
}
