#include "../../../inc/minishell.h"

int which_builtin(t_shell *data, t_token *token, t_env *env)
{
	if (ft_strcmp(token->value, BUILTIN_ECHO) == 0)
		my_echo();
	else if (ft_strcmp(token->value, BUILTIN_PWD) == 0)
		my_pwd();
	else if (ft_strcmp(token->value, BUILTIN_CD) == 0)
		my_cd();
	else if (ft_strcmp(token->value, BUILTIN_EXPORT) == 0)
		my_export(data, token, env);
	else if (ft_strcmp(token->value, BUILTIN_ENV) == 0)
		my_env();
	else if (ft_strcmp(token->value, BUILTIN_UNSET) == 0)
		my_unset();
	else if (ft_strcmp(token->value, BUILTIN_EXIT) == 0)
		my_exit();
}
