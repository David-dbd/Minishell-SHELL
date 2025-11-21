/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_matches.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:42:56 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/21 16:44:46 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

static int	aux_alloc_mem(char ***dirs, int n_dirs)
{
	*dirs = ft_calloc(n_dirs + 1, sizeof(char *));
	if (!*dirs)
		return (ERROR);
	return (SUCCESS);
}

static int	extract_dir(char **dir_extract, char *word)
{
	*dir_extract = ft_strdup(word);
	if (!*dir_extract)
		return (ERROR);
	return (SUCCESS);
}

/*
	La lógica es sencilla, se lee el directorio y se si hay un match con el
	key to find. Para saber que comparar con que, se revisa el status/type
	del wildcard. También se revisa si el len del wildcard es mayor que el
	del dir/file/elemento. Entonces: *pdf -> no hara match con str:"pd"

	wildcard: *pdf
	elemento: minishell.pdf ft_strchr = "p";
	ft_strcmp: "pdf" vs "pdf"

	De este modo ya sabemos que debemos tomar el elemento en cuenta para la
	expansión.
	Cabe resaltar que ignoramos elemtos del sistema que tengan "." o ".."
*/

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

static int	through_dir(t_shell *data, t_wild *wd, DIR *dir, char **dirs)
{
	int				count;
	struct dirent	*entry;

	count = 0;
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (should_ignore_file(entry->d_name, wd))
			continue ;
		if (wd->type == ALL)
		{
			if (extract_dir(&dirs[count++], entry->d_name) == ERROR)
				return (ERROR);
		}
		else if (if_theres_match(entry->d_name, wd->key, wd->type)
			|| handle_complex_case(data, entry->d_name, wd->key, wd->type))
		{
			if (extract_dir(&dirs[count++], entry->d_name) == ERROR)
				return (ERROR);
		}
	}
	return (SUCCESS);
}

/*
	Función para encontrar elementos que coincidan en nombre con
	el key_to_find. Luego los copia en un char ** y los retorna para
	luego expandir el wildcard.

	ejemplo:

	echo *pfd;

	Dir: Minishell
		->Makefile
		->minishell
		->header
		->minishell.pdf -> Toma este!
*/

char	**find_matches(t_shell *data, t_wild *wildcard_info, int n_dirs)
{
	char	**dirs;
	DIR		*directory;

	directory = opendir(".");
	if (!directory)
	{
		perror("minishell: opendir: ");
		free (wildcard_info->key);
		return (NULL);
	}
	if (aux_alloc_mem(&dirs, n_dirs) == ERROR
		|| through_dir(data, wildcard_info, directory, dirs) == ERROR)
	{
		free (wildcard_info->key);
		ft_free_str_array(&dirs);
		closedir(directory);
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	}
	closedir(directory);
	return (dirs);
}
