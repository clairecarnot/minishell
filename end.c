/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 14:51:11 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/20 15:12:25 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/lexer.h"
#include "./include/parser.h"
#include "./include/struct.h"

void	ft_lstfree(t_list **lst)
{
	t_list	*ptr;
	t_list	*tmp;

	if (lst)
	{
		ptr = *lst;
		while (ptr)
		{
			tmp = ptr->next;
			if (ptr->content)
				free(ptr->content);
			free(ptr);
			ptr = tmp;
		}
		*lst = NULL;
	}
}
void	token_lst_free(t_token **lst)
{
	t_token	*ptr;
	t_token	*tmp;

	if (lst)
	{
		ptr = *lst;
		while (ptr)
		{
			tmp = ptr->next_token;
			if (ptr->type == T_WORD)
				free(ptr->value);
			free(ptr);
			ptr = tmp;
		}
		*lst = NULL;
	}
}

void	redirs_free(t_redirs **lst)
{
	t_redirs	*ptr;
	t_redirs	*tmp;

	if (lst)
	{
		ptr = *lst;
		while (ptr)
		{
			tmp = ptr->next_redir;
			if (ptr->filename)
				free(ptr->filename);
			free(ptr);
			ptr = tmp;
		}
		*lst = NULL;
	}
}
void	free_root_ast(t_ast *root)
{
	if (!root)
		return ;
	free_root_ast(root->left);
	free_root_ast(root->right);
	if (root->type == CMD)
		ft_lstfree(&root->args);
	if (root->redirs)
		redirs_free(&root->redirs);
	free(root);
}

void	free_minishell(t_ms *ms, int exit_status)
{
	if (ms->root)
		free_root_ast(ms->root);
	if (ms->lexer)
	{
		token_lst_free(&ms->lexer->token_lst);
		free(ms->lexer);
	}
	if (ms->env)
		ft_lstfree(&ms->env);
	if (ms->exp)
		ft_lstfree(&ms->exp);
	if (ms->wkdir)
		free(ms->wkdir);
	if (ms->old_wkdir)
		free(ms->old_wkdir);
	if (ms->line)
		free(ms->line);
	if (ms)
		free(ms);
	exit(exit_status);
}
