/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:43:12 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/16 17:32:48 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/export.h"
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

// check that first character of the variable name is a letter (upper or lower case)
// check that there's not a special character in the entire name (before the egal)
int	check_error_exp(char *content)
{
	//the message error = bash: export: `8hey=hey': not a valid identifier
	
	return (0);
}

// print la liste t_list *exp en ecrivant export avant chaque ligne
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

// la fonction export recupere les t_list *env et t_list *exp
int	exec_export(t_ms *ms)
{
	t_list	*exp_arg;
	
	exp_arg = ms->root->args->content;
	if (ft_strncmp(exp_arg, "export", 6) == 0)//si content est bien la cmd export
	{
		if (!exp_arg->next)//si export n'a pas d'arg qui le succede, alors print la lst exp
			print_lst_exp(ms->exp);
		else
		{
			exp_arg = exp_arg->next;
			while (exp_arg)
			{
				if (!check_error_exp(exp_arg->content))//s'il n'y a pas d'erreur on va ajouter la variable
					add_variable(ms, exp_arg);
				exp_arg = exp_arg->next;//on passe a l'arg suivant
			}
		}
	}
	return (0);
}
