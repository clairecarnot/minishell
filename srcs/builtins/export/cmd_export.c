#include "../../../include/builtins.h"
#include "../libft/libft.h"

void	add_variable2(t_ms *ms, t_cmd *cmd, char *ct)
{
	if (var_exists_exp(ms, ct) && var_exists_env(ms, ct) && find_p(ct))
	{
		join_in_env(ms, cmd, ct);
		join_in_exp(ms, cmd, ct);
	}
	else if (var_exists_exp(ms, ct) && !var_exists_env(ms, ct) && find_p(ct))
	{
		join_in_exp(ms, cmd, ct);
		add_to_env(ms, cmd, ct);
	}
	else if (var_exists_exp(ms, ct) && var_exists_env(ms, ct) && !find_p(ct))
	{
		replace_in_env(ms, cmd, ct);
		replace_in_exp(ms, cmd, ct);
	}
	else if (var_exists_exp(ms, ct) && !var_exists_env(ms, ct) && !find_p(ct))
	{
		add_to_env(ms, cmd, ct);
		replace_in_exp(ms, cmd, ct);
	}
	else
	{
		add_to_env(ms, cmd, ct);
		add_to_exp(ms, cmd, ct);
	}
}

void	add_variable(t_ms *ms, t_cmd *cmd, char *content)
{
	if (!has_equal(content) && !var_exists_exp(ms, content))
	{
		add_to_exp(ms, cmd, content);
		return ;
	}
	else if (!has_equal(content) && var_exists_exp(ms, content))
		return ;
	add_variable2(ms, cmd, content);
}

/*
print la liste t_list *exp en ecrivant export avant chaque ligne
*/
void	print_lst_exp(t_list *exp)
{
	t_list	*lst;

	lst = exp;
	while (lst)
	{
		printf("export %s\n", (char *)lst->content);
		lst = lst->next;
	}
}

// void	replace_underscore_exp(t_ms *ms, t_cmd *cmd)// a enlever
// {
// 	char	*new_content;
// 	t_list	*tmp_exp;

// 	tmp_exp = ms->exp;
// 	while (tmp_exp)
// 	{
// 		if (ft_strncmp("_=\"/", tmp_exp->content, 10) == 0)
// 		{
// 			free(tmp_exp->content);
// 			new_content = ft_strdup("_=/usr/bin/export");
// 			if (!tmp_exp->content)// a verifier
// 			{
// 				free_cmd(cmd);
// 				free_minishell(ms, 1);
// 			}
// 			free(tmp_exp->content);
// 			tmp_exp->content = new_content;
// 		}
// 		tmp_exp = tmp_exp->next;
// 	}
// }

/*
cmd export : si export n'est suivi de rien d'autre -> on print la list exp
sinon s'il n'y a pas d'erreur dans l'ecriture des variables, on les ajoute aux listes exp et env
*/
int	exec_export(t_ms *ms, t_cmd *cmd)
{
	int i;

	i = 1;
	if (!cmd->args[i])
		return (print_lst_exp(ms->exp), 0);
	else
	{
		while (cmd->args[i])
		{
			if (!error_exp_spaces(cmd->args[i]) && !error_exp(cmd->args[i]))
				add_variable(ms, cmd, cmd->args[i]);
			i++;
		}
	}
	return (0);
}
