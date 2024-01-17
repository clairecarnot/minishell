
#include "../../include/env.h"

/*
 * min_len
 * Calculates and sends the mininum size between 2 chars*
 */

int	min_len(char *s1, char *s2)
{
	int	l1;
	int	l2;

	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	if (l1 > l2)
		return (l2);
	return (l1);
}

/*
 * Swaps 2 void*
 */

void	swap_lst(void **l1, void **l2)
{
	void	*tmp;

	tmp = *l1;
	*l1 = *l2;
	*l2 = tmp;
}

/*
 *lst_dup
 * Duplicates the given t_list
 */

t_list	*lst_dup(t_list *lst_tocpy)
{
	char	*newchar;
	t_list	*new;
	t_list	*tmp;
	t_list	*lst;

	new = NULL;
	tmp = NULL;
	if (!lst_tocpy)
		return (NULL);
	lst = lst_tocpy;
	while (lst)
	{
		newchar = ft_strdup(lst->content);
		if (!newchar)
			return (ft_lstfree(&new), NULL);
		tmp = ft_lstnew(newchar);
		if (!tmp)
			return (free(newchar), ft_lstfree(&new), NULL);
		ft_lstadd_back(&new, tmp);
		lst = lst->next;
	}
	return (new);
}

void	ft_printlst(t_list *lst)// temporaire
{
	t_list *tmp;

	tmp = lst;
	while (tmp)
	{
		printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}

void	init_exp_noenv(t_ms *ms)
{
	char	*content;
	char	*var;
	t_list	*new_lst;
	t_list	*tmp;
	t_list	*tmp2;

	tmp = ms->exp;
	tmp2 = ms->exp;
	var = getvar_env(ms, NULL,"OLDPWD");
	if (!var)// si on est dans le cas env -i
	{
		content = ft_strdup("OLDPWD");
		if (!content)// a verifier
			free_minishell(ms, 1);
		new_lst = ft_lstnew(content);
		if (!new_lst)// a verifier
		{
			free(content);
			free_minishell(ms, 1);
		}
		ft_lstadd_front(&ms->exp, new_lst);
	}
	else
		free(var);
}

/*
 * init_exp
 * Creates a copy of the original env list, but sorted by ascii
 */

int	init_exp(t_ms *ms)
{

	t_list	*lst;
	t_list	*suiv;

	ms->exp = lst_dup(ms->env);
	if (!ms->exp)
		return (1);
	lst = ms->exp;
	while (lst)
	{
		suiv = lst->next;
		while (suiv)
		{
			if (ft_strncmp(lst->content, suiv->content, \
			min_len(lst->content, suiv->content)) > 0)
				swap_lst(&lst->content, &suiv->content);
			suiv = suiv->next;
		}
		lst = lst->next;
	}
	add_qvar_lst(ms, ms->exp);
	init_exp_noenv(ms);
	// ft_printlst(ms->exp);
	return (0);
}
