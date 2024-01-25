#include "../../../include/builtins.h"
#include "../libft/libft.h"

/*
Ajouter a la liste env, a la fin
*/
void add_to_env(t_ms *ms, t_cmd *cmd, char *content)
{
	char	*cpy_content;
	t_list	*new;

	cpy_content = ft_strdup_noplus(ms, cmd, content);// c'est verifie 2
	new = ft_lstnew(cpy_content);// c'est verifie 2
	if (!new)
	{
		free_cmd(cmd);
		prefree_minishell(ms, cpy_content);
	}
	ft_lstadd_back(&ms->env, new);
}

void	print_lst_env(t_list *env)
{
	t_list	*lst;

	lst = env;
	while (lst)
	{
		if (lst && lst->content)
			printf("%s\n", (char *)lst->content);
		lst = lst->next;
	}
}

void	replace_var_underscore_env(t_ms *ms, t_cmd *cmd)
{
	char	*new_content;
	t_list	*tmp;

	tmp = ms->env;
	while (tmp)
	{
		if (ft_strncmp("_=/usr/bin", tmp->content, 10) == 0)
		{
			new_content = ft_strdup("_=/usr/bin/env");
			if (!new_content)// c'est verifie 2
			{
				free_cmd(cmd);
				free_minishell(ms, 1);
			}
			free(tmp->content);
			tmp->content = new_content;
		}
		tmp = tmp->next;
	}
}

/*
La commande env liste les variables environnment 
*/
int	exec_env(t_ms *ms, t_cmd *cmd)
{
	replace_var_underscore_env(ms, cmd);
	if (!cmd->args[1])
		return (print_lst_env(ms->env), 0);
	else
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd("’: No such file or directory\n", 2);
		ms->exit_code = 127;
		return (0);
	}
}
