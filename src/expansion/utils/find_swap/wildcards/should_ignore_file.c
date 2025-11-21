/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   should_ignore_file.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:44:34 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/21 16:45:00 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

int	should_ignore_file(char *filename, t_wild *wildcard)
{
	if (ft_strcmp(filename, ".") == 0 || ft_strcmp(filename, "..") == 0)
		return (TRUE);
	if (wildcard->starts_with_dot)
	{
		if (filename[0] == '.')
			return (FALSE);
		else
			return (TRUE);
	}
	else
	{
		if (filename[0] == '.')
			return (TRUE);
		else
			return (FALSE);
	}
}
