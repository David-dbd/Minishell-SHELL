/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorder_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:41:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 23:48:45 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

int	copy_token_members(t_token *tmp_index, t_token *src)
{
	tmp_index->type = src->type;
	tmp_index->id = src->id;
	tmp_index->hash = src->hash;
	tmp_index->double_quoted = src->double_quoted;
	tmp_index->single_quoted = src->single_quoted;
	if (is_alloc_type(src->type))
	{
		tmp_index->value = ft_strdup(src->value);
		if (!tmp_index->value)
			return (ERROR);
	}
	else
		tmp_index->value = src->value;
	return (SUCCESS);
}

/*
	FunciÃ³n para copiar en el nuevo arr tmp los tokens en el rango desde 0
	hasta el Ãºltimo indice (sin copiar el ultimo, es decir, hasta el penultimo)

	Ejemplo:
	prompt->tokens = a b c d e
	nuevo_arr = a b c d
*/

static int	copy_orig_tokens(t_shell *data, t_token *tmp, int ignore)
{
	int	i;
	int	tmp_i;

	i = 0;
	tmp_i = 0;
	while (i < ignore)
	{
		if (copy_token_members(&tmp[tmp_i], &data->prompt.tokens[i]) == ERROR)
		{
			free_tmp_tokens(tmp, tmp_i);
			return (ERROR);
		}
		tmp_i++;
		i++;
	}
	return (tmp_i);
}

/*
	FunciÃ³n para copiar los tokens que se crearon a partir del wildcard
	(que estan de Ãºltimos en prompt->tokens) desde donde estaba el Ãºltimo
	token anterior.

	Ejemplo:
	prompt->tokens = a b c d e
	new_tokens = f g h i
	old_arr = a b c d e f g h i
	se copian los new = a b c d f g h i
*/

static int	copy_new_tokens(t_shell *data, t_token *tmp, int *i, char **dirs)
{
	int	j;
	int	n_dirs;

	j = 0;
	n_dirs = ft_count_str_in_arr(dirs);
	while (dirs[j] != NULL)
	{
		tmp[*i].type = WORD;
		tmp[*i].double_quoted = FALSE;
		tmp[*i].single_quoted = FALSE;
		tmp[*i].id = *i;
		if (j == 0)
			tmp[*i].hash = data->prompt.tokens[*i].hash;
		else
			tmp[*i].hash = create_hash(data, n_dirs, data->prompt.n_tokens, *i);
		tmp[*i].value = ft_strdup(dirs[j]);
		if (!tmp[*i].value)
		{
			free_tmp_tokens(tmp, *i);
			return (ERROR);
		}
		(*i)++;
		j++;
	}
	return (SUCCESS);
}

/*
	FunciÃ³n para copiar el Ãºltimo token del old_arr al Ãºltimo index del
	new_arr despuÃ©s de haber traido los new_tokens.

	Ejemplo:
	prompt->tokens = a b c d e
	new_tokens = f g h i
	juntos = a b c d e f g h i
	se copian los new = a b c d f g h i e
*/

static int	copy_last_token(t_shell *data, t_token *tmp, int *i, int j)
{
	if (j < data->prompt.n_tokens)
	{
		while (j < data->prompt.n_tokens)
		{
			if (copy_token_members(&tmp[*i], &data->prompt.tokens[j]) == ERROR)
			{
				free_tmp_tokens(tmp, *i);
				return (ERROR);
			}
			(*i)++;
			j++;
		}
	}
	return (SUCCESS);
}

/*
	FunciÃ³n para reordenar los tokens de modo que los nuevos tokens que
	salieron a partir de la expansiÃ³n del wildcard (y que estan de ultimo
	en el arr) puedan cambiar posiciÃ³n con el token que se encuentra
	actualmente de Ãºltimo.

	La logica es copiar los tokens en un nuevo arr, teniendo en cuenta el orden.
	Luego se elimina el old_arr y se actualizan ptrs y otros datos
*/

int	reorder_tokens(t_shell *d, t_token *orig_token, int n_dirs, char **dirs)
{
	int		i;
	int		j;
	int		orig_size;
	int		wildcard;
	t_token	*tmp;

	orig_size = d->prompt.n_tokens;
	wildcard = orig_token->id;
	tmp = ft_calloc((d->prompt.n_alloc_tokens + n_dirs), sizeof(t_token)); // + n_dirs porque puede que al allocate x bites, puede que no alcance porque el wildcard son 50 items, de modo que se perderan algunos
	if (!tmp)
		return (ERROR);
	i = copy_orig_tokens(d, tmp, orig_token->id);
	if (i == ERROR)
		return (ERROR);
	if (copy_new_tokens(d, tmp, &i, dirs) == ERROR)
		return (ERROR);
	if (copy_last_token(d, tmp, &i, wildcard + 1) == ERROR)
		return (ERROR);
	j = d->prompt.n_alloc_tokens;
	free_tokens(&d->prompt);
	d->prompt.tokens = tmp;
	d->prompt.n_tokens = i;
	d->prompt.n_alloc_tokens = j + n_dirs;
	transform_tokens_logic(d, &d->prompt, d->prompt.tokens);
	return (SUCCESS);
}
