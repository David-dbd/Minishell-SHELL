#include "../../../../../inc/minishell.h"


/*
	Funcion para eliminar los tokens tmp que se han creado hasta el moemento.

	Si i = 5; y falla malloc, entonces se liberan los tmp[5] que si fueron
	allocated para que no hayan leaks
*/

static void	free_tmp_tokens(t_token *tmp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (tmp[i].value)
		{
			free(tmp[i].value);
			tmp[i].value = NULL;
		}
		i++;
	}
	free(tmp);
}

/*
	FunciÃ³n para copiar en el nuevo arr tmp los tokens en el rango desde 0
	hasta el Ãºltimo indice (sin copiar el ultimo, es decir, hasta el penultimo)

	Ejemplo:
	prompt->tokens = a b c d e
	nuevo_arr = a b c d
*/

static int	copy_orig_tokens(t_shell *data, t_token *tmp, int orig_size, int wildcard_idx)
{
	int	i;
	int	tmp_i;

	i = 0;
	tmp_i = 0;
	while (i < orig_size)
	{
		if (i != wildcard_idx)
		{
			tmp[tmp_i] = data->prompt.tokens[i];
			tmp[tmp_i].value = ft_strdup(data->prompt.tokens[i].value);
			if (!tmp[tmp_i].value)
			{
				free_tmp_tokens(tmp, tmp_i);
				exit_error(data, ERR_MALLOC, EXIT_FAILURE);
			}
			printf("%s\n", tmp[tmp_i].value);
			tmp_i++;
		}
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

static void	copy_new_tokens(t_shell *data, t_token *tmp, int *i, int orig_size)
{
	int	j;
	int	total_len;

	total_len = data->prompt.n_tokens;
	j = orig_size;
	while (j < total_len)// copio los nuevos tokens del wildcard
	{
		tmp[*i] = data->prompt.tokens[j];
		tmp[*i].value = ft_strdup(data->prompt.tokens[j].value);
		if (!tmp[*i].value)
		{
			free_tmp_tokens(tmp, *i);
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		}
		(*i)++;
		j++;
	}
}

/*
	FunciÃ³n para copiar el Ãºltimo token del old_arr al Ãºltimo index del new_arr
	despuÃ©s de haber traido los new_tokens.

	Ejemplo:
	prompt->tokens = a b c d e
	new_tokens = f g h i
	juntos = a b c d e f g h i
	se copian los new = a b c d f g h i e
*/

static void	copy_last_token(t_shell *d, t_token *or_token, t_token *tmp, int *i)
{
	tmp[*i] = *or_token;
	tmp[*i].value = ft_strdup(or_token->value);
	if (!tmp[*i].value)
	{
		free_tmp_tokens(tmp, *i);
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	}
	(*i)++;
}

/*
	FunciÃ³n para reordenar los tokens de modo que los nuevos tokens que
	salieron a partir de la expansiÃ³n del wildcard (y que estan de ultimo
	en el arr) puedan cambiar posiciÃ³n con el token que se encuentra actualmente
	de Ãºltimo.

	La logica es copiar los tokens en un nuevo arr, teniendo en cuenta el orden.
	Luego se elimina el old_arr y se actualizan ptrs y otros datos
*/

void	reorder_tokens(t_shell *data, t_token *orig_token, int orig_size, char **dirs)
{
	int		i;
	int		j;
	t_token	*tmp;

	tmp = ft_calloc(data->prompt.n_alloc_tokens, sizeof(t_token));
	if (!tmp)
	{
		ft_free_str_array(dirs);
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	}
	i = copy_orig_tokens(data, tmp, orig_size, orig_token->id);
	copy_new_tokens(data, tmp, &i, orig_size);
	j = data->prompt.n_alloc_tokens;
	while (data->prompt.n_tokens > 0)
		eliminate_token(&data->prompt, data->prompt.tokens, 0);
	data->prompt.tokens = tmp;
	data->prompt.n_tokens = i;
	data->prompt.n_alloc_tokens = j;
	j = 0;
	while (j < i)
	{
		data->prompt.tokens[j].id = j;
		j++;
	}
}

