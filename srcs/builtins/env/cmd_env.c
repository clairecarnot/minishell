#include "../../../include/builtins.h"
#include "../libft/libft.h"

/*
Ajouter a la liste env, a la fin
*/
void add_to_env(t_ms *ms, char *content)
{
	char	*cpy_content;
	t_list	*new;

	cpy_content = ft_strdup_noplus(ms, content);// a reverifier
	new = ft_lstnew(cpy_content);// c'est protege
	if (!new)
		prefree_minishell(ms, cpy_content);
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

/*
La commande env liste les variables environnment 
*/
int	exec_env(t_ms *ms, t_cmd *cmd)
{
	if (!cmd->args[1])
		return (print_lst_env(ms->env), 0);
	else
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd("’: No such file or directory\n", 2);
		// ms->exit_code = 127;
		return (0);
	}
}
