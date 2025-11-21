/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_matches.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:15:26 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/21 16:43:44 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

static int	if_theres_match(char *file, char *key, int wild_type)
{
	char	*found;
	int		file_len;
	int		key_len;

	file_len = ft_strlen(file);
	key_len = ft_strlen(key);
	if (key_len > file_len)
		return (FALSE);
	if (wild_type == BEGINING)
	{
		if (ft_strcmp(file + (file_len - key_len), key) == 0)
			return (TRUE);
	}
	else if (wild_type == END)
	{
		if (ft_strncmp(file, key, key_len) == 0)
			return (TRUE);
	}
	return (FALSE);
}

static int	go_through_dir(t_shell *data, t_wild *wildc, DIR *dir)
{
	int				count;
	struct dirent	*entry;

	count = 0;
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (should_ignore_file(entry->d_name, wildc))
			continue ;
		if (wildc->type == ALL)
			count++;
		if (handle_complex_case(data, entry->d_name, wildc->key, wildc->type))
			count++;
		if (if_theres_match(entry->d_name, wildc->key, wildc->type))
			count++;
	}
	return (count);
}

int	count_matches(t_shell *data, t_wild *wildcard)
{
	int		n_dirs;
	DIR		*directory;

	directory = opendir(".");
	if (!directory)
	{
		perror("minishell: opendir: ");
		free (wildcard->key);
		wildcard->key = NULL;
		return (0);
	}
	n_dirs = go_through_dir(data, wildcard, directory);
	closedir(directory);
	return (n_dirs);
}
