/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_matches.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:42:56 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/31 13:00:01 by davdiaz-         ###   ########.fr       */
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
	La lógica es sencilla, se lee el directorio y se revisa que el primer
	caracter del key_to_find (wildcard: *pdf -> "p") este en el elemento
	que estamos analizando. Luego, se comparan ambos. La clave es que
	si el file es: minishell.pdf, claramente no se compara desde el inicio,
	sino desde el char que ft_strchr nos dio, de modo que:

	wildcard: *pdf
	elemento: minishell.pdf ft_strchr = "p";
	ft_strcmp: "pdf" vs "pdf"

	De este modo ya sabemos que debemos tomar el elemento en cuenta para la
	expansión
*/

static int	go_through_dir(DIR *directory, char **dirs, char *key_to_find)
{
	char	*ptr;
	int		i;
	struct 	dirent *entry;

	ptr = NULL;
	i = 0;
	while (1)
	{
		entry = readdir(directory);
		if (!entry)
			break ;
		if ((ft_strcmp(entry->d_name, ".") == 0)
			|| (ft_strcmp(entry->d_name, "..") == 0)) //ignorar ignorar entradas especiales "." y ".."
			continue ;
		ptr = ft_strchr(entry->d_name, key_to_find[0]); //buscar el el wildcard en el nombre del archvio/directorio/ejecutable
		if (ptr && ft_strcmp(key_to_find, ptr) == 0)
		{
			if (!extract_dir(&dirs[i], entry->d_name))
				return (ERROR);
			i++;
		}
	}
	return (SUCCESS);
}

/*
	Función para encontrar elementos que su final del nombre coincide con
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

char	**find_matches(t_shell *data, char *key_to_find, int n_dirs)
{
	char	**dirs;
	char	*ptr;
	int		i;
	DIR 	*directory;

	i = 0;
	directory = opendir(".");
	if (!directory)
	{
		perror("opendir: ");
		free (key_to_find);
		return (NULL);
	}
	if (aux_alloc_mem(&dirs, n_dirs) == ERROR
		|| go_through_dir(directory, dirs, key_to_find) == ERROR)
	{
		free (key_to_find);
		closedir(directory);
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	}
	closedir(directory);
	return (dirs);
}
