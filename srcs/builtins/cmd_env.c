#include "../../include/builtins.h"
#include "../libft/libft.h"

/*
Ajouter a la liste env, a la fin
*/
void add_to_env(t_ms *ms, char *content)
{
	char	*cpy_content;
	t_list	*new;

	cpy_content = ft_strdup_noplus(content);//ajout protec
	
	new = ft_lstnew(cpy_content);//ajouter protec
	ft_lstadd_back(&ms->env, new);
}

void	print_lst_env(t_list *env)
{
	t_list	*lst;

	lst = env;
	while (lst)
	{
		printf("%s\n", (char *)lst->content);
		lst = lst->next;
	}
}

/*
La commande env liste les variables environnment 
*/
int	exec_env(t_ms *ms)
{
	t_list	*exp_arg;

	exp_arg = ms->root->args;
	if (ft_strncmp(exp_arg->content, "env", 3) == 0)
	{
		print_lst_env(ms->env);
	}
	return (0);
}
