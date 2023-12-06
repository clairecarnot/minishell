/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:43:12 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/06 17:08:06 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"
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
Ajouter a la liste exp, dans l'ordre ascii
*/
void	add_to_exp(t_ms *ms, char *content)
{
	t_list	*exp_tmp;
	t_list	*exp_tmp2;
	t_list	*new;

	exp_tmp = ms->exp;
	exp_tmp2 = ms->exp;
	new = ft_lstnew(content);
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, content, ft_strlen(content)) >= 0)
		{
			new->next = exp_tmp2->next;
			exp_tmp2->next = new;
			return ;
		}
		exp_tmp2 = exp_tmp;
		exp_tmp = exp_tmp->next;
	}
	new->next = exp_tmp2->next;
	exp_tmp2->next = new;
}

int	ft_strlen_equal(char *content)
{
	int	i;

	i = 0;
	while (content[i] != '=')
		i++;
	return (i);
}

/*
Cherche si une variable du meme nom existe deja
Si oui la remplacer
*/
int	var_exists(t_ms *ms, char *content)
{
	int		i;
	int		size;
	t_list	*env_tmp;

	i = 0;
	size = ft_strlen_equal(content);
	env_tmp = ms->env;
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, content, size + 1) == 0)//variable deja existante trouvee
			return (1);
		env_tmp = env_tmp->next;
	}
	return (0);
}

void	replace_in_exp(t_ms *ms, char *content)
{
	t_list	*exp_tmp;
	t_list	*exp_tmp2;
	t_list	*new;
	
	exp_tmp = ms->exp;
	exp_tmp2 = ms->exp;
	new = ft_lstnew(content);
	while (exp_tmp)
	{
		// printf("hello1\n");
		if (ft_strncmp(exp_tmp->content, content, ft_strlen_equal(content)) == 0)
		{
			dprintf(1, "hey1\n");
			new->next = exp_tmp->next;
			free(exp_tmp->content);
			free(exp_tmp);
			exp_tmp2->next = new;
			return ;
		}
		exp_tmp2 = exp_tmp;
		exp_tmp = exp_tmp->next;
	}
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
	if (!has_equal(content) && !var_exists(ms, content))
		return (add_to_exp(ms, content) , 0);
	cpy_content = add_qvar(content);
	if (!cpy_content)
		return (1);//error a verifier	
	if (var_exists(ms, content))
	{
		// replace_in_env(ms, content);
		replace_in_exp(ms, cpy_content);
	}
	else
	{
		// printf("content = %s\n", content);
		// printf("cpy_content = %s\n", cpy_content);
		ms->env = add_to_env(ms, content);
		add_to_exp(ms, cpy_content);
	}
	return (0);
}

// print la liste t_list *exp en ecrivant export avant chaque ligne
void	print_lst_exp(t_list *exp)
{
	t_list	*lst;

	lst = exp;
	// printf("print export ---->\n");
	while (lst)
	{
		printf("export %s\n", (char *)lst->content);
		lst = lst->next;
	}
}

// la fonction export recupere les t_list *env et t_list *exp
int	exec_export(t_ms *ms)
{
	int		i;
	t_list	*exp_arg;
	
	i = 0;
	exp_arg = ms->root->args;
	if (ft_strncmp(exp_arg->content, "export", 6) == 0)//si content est bien la cmd export
	{
		if (!exp_arg->next)//si export n'a pas d'arg qui le succede, alors print la lst exp
			return (print_lst_exp(ms->exp), 0);
		else
		{
			exp_arg = exp_arg->next;
			while (exp_arg)
			{
				if (!error_exp_spaces(exp_arg->content) && !error_exp(exp_arg->content))//s'il n'y a pas d'erreur on va ajouter la variable
				{
					// printf("add_variable\n");
					if (add_variable(ms, exp_arg->content))
						return (1);
				}
				exp_arg = exp_arg->next;//on passe a l'arg suivant
				i++;
			}
		}
	}
	return (0);
}

