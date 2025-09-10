#include "../../../inc/minishell.h"

void which_builtin(t_shell *data, t_token *tokens, t_token *token)
{
	if (ft_strcmp(token->value, BUILTIN_ECHO) == 0)
		my_echo(tokens);
	else if (ft_strcmp(token->value, BUILTIN_PWD) == 0)
		my_pwd(data);
	// else if (ft_strcmp(token->value, BUILTIN_CD) == 0)
	// 	// my_cd();
	else if (ft_strcmp(token->value, BUILTIN_EXPORT) == 0)
		my_export(data, token, &data->env);
	else if (ft_strcmp(token->value, BUILTIN_ENV) == 0)
		my_env(data->env.vars);
	// else if (ft_strcmp(token->value, BUILTIN_UNSET) == 0)
	// 	// my_unset();
	// else if (ft_strcmp(token->value, BUILTIN_EXIT) == 0)
	// 	// my_exit();
}
