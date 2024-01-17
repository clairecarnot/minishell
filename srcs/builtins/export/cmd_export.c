#include "../../../include/builtins.h"
#include "../libft/libft.h"

int	add_variable(t_ms *ms, char *content)
{
	if (!has_equal(content) && !var_exists_exp(ms, content))
	{
		// dprintf(2, "add to exp\n");
		return (add_to_exp(ms, content) , 0);
	}
	else if (!has_equal(content) && var_exists_exp(ms, content))
		return (/*dprintf(2, "noting\n"),*/ 0);
	if (var_exists_exp(ms, content) && var_exists_env(ms, content) && find_plus(content))
	{
		// dprintf(2, "dup in env and exp\n");
		join_in_env(ms, content);
		join_in_exp(ms, content);
	}
	else if (var_exists_exp(ms, content) && !var_exists_env(ms, content) && find_plus(content))
	{
		// dprintf(2, "add in env  dup in exp\n");
		join_in_exp(ms, content);
		add_to_env(ms, content);
	}
	else if (var_exists_exp(ms, content) && var_exists_env(ms, content) && !find_plus(content))
	{
		// dprintf(2, "replace in env and exp\n");
		replace_in_env(ms, content);
		replace_in_exp(ms, content);//cpy_content
	}
	else if (var_exists_exp(ms, content) && !var_exists_env(ms, content) && !find_plus(content))
	{
		// dprintf(2, "add in env  replace in exp\n");
		add_to_env(ms, content);
		replace_in_exp(ms, content);//cpy_content
	}
	else//other cases when VAR doesn't exist before
	{
		// dprintf(2, "add in env and exp\n");
		add_to_env(ms, content);
		add_to_exp(ms, content);// precedemment cpy_content
	}
	return (0);

	// return (add_variable2(ms, content));// verifier que ca marche bien
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
		// replace_underscore_exp(ms, cmd);
		while (cmd->args[i])
		{
			// dprintf(2, "exp_arg->content: %s\n", (char*)exp_arg->content);
			if (!error_exp_spaces(cmd->args[i]) && !error_exp(cmd->args[i]))
			{
				if (add_variable(ms, cmd->args[i]))
				{
					// dprintf(2, "return after add variable\n");
					return (1);
				}
			}
			i++;
		}
	}
	return (0);
}
