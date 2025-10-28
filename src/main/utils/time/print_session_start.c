/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_session_start.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:13:28 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/28 17:12:11 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Imprime el título del minishell y muestra la hora exacta en que se inicia la
	sesión. La hora se formatea en color cian e incluye hora, minutos, segundos,
	día, mes y año. También obtiene el loggin del usuario para hacer el output
	de bienevenida mucho mas personalizado, aceptando solamente loggins valido.
	Luego llama a la función print_time_of_day para mostrar un
	mensaje personalizado según el momento del día en que se inicia la sesión.
*/

void	print_minishell_title(void)
{
	printf(TITLE_COLOR "\n\n");
	printf(T1 "\n");
	printf(T2 "\n");
	printf(T3 "\n");
	printf(T4 "\n");
	printf(T5 "\n");
	printf("\n\n" RESET_COLOR);
}

static void	check_user_name_syntax(char *name, int *validation)
{
	int	i;

	i = 0;
	while (name[i] != '\0')
	{
		if (!ft_isprint(name[i]) || ft_isspace(name[i]))
		{
			*validation = FAILURE;
			return ;
		}
		i++;
	}
	*validation = SUCCESS;
}

static char	*get_user_name(char **name, int *validation)
{
	*name = ic_readline("\033[1m\033[1;32mIntroduce your login: \033[0m");
	printf("\n\n");
	if (!*name)
	{
		*validation = FAILURE;
		return (NULL);
	}
	if (!**name)
	{
		*validation = FAILURE;
		return (*name);
	}
	check_user_name_syntax(*name, validation);
	return (*name);
}

static char	*is_valid_user_name(void)
{
	char	*name;
	int		validation;

	name = NULL;
	while (1)
	{
		if (!get_user_name(&name, &validation))
			return (NULL);
		if (validation == SUCCESS)
			break ;
		free(name);
	}
	return (name);
}

char *find_user(t_shell *data, t_var *vars)
{
	char	*name;
	t_var 	*var;

	var = vars;
	while(var)
	{
		if (ft_strcmp(var->key, "USER") == 0)
		{
			name = ft_strdup(var->value);
			name = ft_capitalize(name);
			return(name);
		}
		var = var->next;
	}
	name = is_valid_user_name();
	name = ft_capitalize(name);
	if (!name)
		exit_succes(data, MSG_GOODBYE_V2, EXIT_SUCCESS);
	return (name);
}

void	print_session_start(t_shell *data, time_t start, char **user_name)
{
	char		*name;
	struct tm	*local;

	local = localtime(&start);
	print_minishell_title();
	*user_name = find_user(data, data->env.vars);
	print_time_of_day(start, data->extras.user_name);
	printf(MSG_TIME_START,
		local->tm_hour, local->tm_min, local->tm_sec,
		local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
}

