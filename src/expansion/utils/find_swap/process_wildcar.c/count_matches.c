/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_matches.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:15:26 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/31 13:00:11 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

static int	go_through_dir(DIR *directory, char *key_to_find)
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
		ptr = ft_strchr(entry->d_name, key_to_find[0]); //buscar el el index 0 del wildcard en el nombre del archvio/directorio/ejecutable
		//PERO FALTA CONFIRMAR QUE ESTE AL FINAL PORQUE AHORA MISMO ESTO PUEDE ENCONTRARSE UNA COINCIDENCIA PERO ESTA AL INICO. PERO BUENO, LUEGO EL STRCMP LO REVISA
		if (ptr && ft_strcmp(key_to_find, ptr) == 0) //Se confirma que el wilrdcar completo este en el dir
			i++;
	}
	return (i);
}

int	count_matches(t_shell *data, char *key_to_find)
{
	int		n_dirs;
	DIR 	*directory;
	struct 	dirent *entry;

	directory = opendir(".");
	if (!directory)
	{
		perror("opendir: ");
		free (key_to_find);
		return (0);
	}
	n_dirs = go_through_dir(directory, key_to_find);
	closedir(directory);
	return (n_dirs);
}
