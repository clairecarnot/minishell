#include "../../../include/builtins.h"
#include "../libft/libft.h"

// It saves at the end of the chained list, the new variable
// export variables in the env list

// It works with several arguments, do it one by one 

// Export without argument print the env in ASCII order and will print…

// Careful with error in the name variable : the first character has to 
// be a letter (no number or special character) and no special character in the entire name

// If several variables and an error in one, there will be an error message but 
// the others variables are still gonna be add to env and/or exp.
// Attention aux erreurs de nom de variables, si la variable commence par un chiffre,
// un ^, un = ou un -, contient un + ou un ^, une erreur est renvoyee. 
// Peut etre qu'il existe d'autres exceptions...

// La syntaxe "export a" ajoute la variable a dans export mais PAS dans env
// La syntaxe "export a=" ajoute la variable a dans export (a="") et dans env (a)
// La syntaxe "export a=b" ajoute la variable a dans export et dans env de maniere classique

// La syntaxe "export a+=b" permet de join l'ancienne variable a la nouvelle (sinon ca serait trop facile).

// La variable d'environnement "_" est un peu particuliere, lorsqu'on export _=a et qu'on fait env la variable 
// n'as pas change, en realite elle a change car lorsqu'on export *=a et qu'on fait echo $* la nouvelle 
// variable s'affiche bien, a voir si vous voulez traiter ce cas ou non.


/*
Cherche si une variable du meme nom existe deja
Si oui la remplacer
*/
int	var_exists_exp(t_ms *ms, char *content)
{
	int		i;
	int		size;
	t_list	*exp_tmp;

	i = 0;
	size = ft_strlen_equal(content);
	// dprintf(2, "size = %d\n", size);
	exp_tmp = ms->exp;
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, content, size/* + 1*/) == 0)//variable deja existante trouvee
			return (1);
		exp_tmp = exp_tmp->next;
	}
	return (0);
}

int	var_exists_env(t_ms *ms, char *content)
{
	int		i;
	int		size;
	t_list	*env_tmp;

	i = 0;
	size = ft_strlen_equal(content);
	// dprintf(2, "size = %d\n", size);
	env_tmp = ms->env;
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, content, size/* + 1*/) == 0)//variable deja existante trouvee
			return (1);
		env_tmp = env_tmp->next;
	}
	return (0);
}

/*
S'il n'y a pas de '=' apres le nom de la variable et que la variable n'existe pas deja -> ajouter a exp list
Si la variable existe deja --> la remplacer dans env et dans exp
Sinon --> l'ajouter a env et a exp
*/
int	add_variable(t_ms *ms, char *content)
{
	int		i;
	char	*cpy_content;

	i = 0;
	if (!has_equal(content) && !var_exists_exp(ms, content))
		return (add_to_exp(ms, content) , 0);
	cpy_content = add_qvar(content);
	if (!cpy_content)
		return (1);//error a verifier
	if (var_exists_exp(ms, content) && var_exists_env(ms, content) && find_plus(content))//case like VAR+=hey VAR exist already
	{
		dprintf(2, "dup in env and exp\n");
		dup_in_env(ms, content);
		dup_in_exp(ms, content);
	}
	else if (var_exists_exp(ms, content) && !var_exists_env(ms, content) && find_plus(content))
	{
		dprintf(2, "add in env  dup in exp\n");
		add_to_env(ms, content);
		dup_in_exp(ms, content);
	}
	else if (var_exists_exp(ms, content) && var_exists_env(ms, content) && !find_plus(content))//case like VAR=hey VAR exist already
	{
		dprintf(2, "replace in env and exp\n");
		replace_in_env(ms, content);
		replace_in_exp(ms, cpy_content);
	}
	else if (var_exists_exp(ms, content) && !var_exists_env(ms, content) && !find_plus(content))//case like VAR=hey VAR exist already
	{
		dprintf(2, "add in env  replace in exp\n");
		add_to_env(ms, content);
		replace_in_exp(ms, cpy_content);
	}
	else//other cases when VAR doesn't exist before
	{
		dprintf(2, "add in env and exp\n");
		add_to_env(ms, content);
		add_to_exp(ms, cpy_content);
	}
	return (0);
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

/*
cmd export : si export n'est suivi de rien d'autre -> on print la list exp
sinon s'il n'y a pas d'erreur dans l'ecriture des variables, on les ajoute aux listes exp et env
*/
int	exec_export(t_ms *ms)
{
	int		i;
	t_list	*exp_arg;
	
	i = 0;
	exp_arg = ms->root->args;
	if (ft_strncmp(exp_arg->content, "export", 6) == 0)
	{
		if (!exp_arg->next)
			return (print_lst_exp(ms->exp), 0);
		else
		{
			exp_arg = exp_arg->next;
			while (exp_arg)
			{
				if (!error_exp_spaces(exp_arg->content) && !error_exp(exp_arg->content))
				{
					if (add_variable(ms, exp_arg->content))
						return (1);
				}
				exp_arg = exp_arg->next;
				i++;
			}
		}
	}
	return (0);
}

