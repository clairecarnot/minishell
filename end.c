/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 14:51:11 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/30 15:01:28 by ccarnot          ###   ########.fr       */
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
//	dprintf(1, "seg1\n");
//	dprintf(2, "root exists2\n");
	if (!root)
		return ;
	// dprintf(1, "seg2\n");
	if (root->left)
		free_root_ast(root->left);
	// dprintf(1, "seg3\n");
	if (root->right)
		free_root_ast(root->right);
	// dprintf(1, "seg4\n");
	if (root->type == CMD)
		ft_lstfree(&root->args);
	// dprintf(1, "seg5\n");
//	root->args = NULL;
	if (root->redirs)
		redirs_free(&root->redirs);
	// dprintf(1, "seg6\n");
//	root->redirs = NULL;
	free(root);
	// dprintf(1, "seg7\n");
	root = NULL;
}
/*
peut-etre a changer: dans les conditions if exit_status != 0 
(en cas d'erreur free certaines choses si non erreur(= passage
a la ligne suivante) ne pas les free)
*/

void	free_minishell(t_ms *ms, int exit_status)
{
	if (ms->root)
	{
		free_root_ast(ms->root);
		ms->root = NULL;
	}
	if (ms->lexer)
	{
		token_lst_free(&ms->lexer->token_lst);
		free(ms->lexer);
		ms->lexer = NULL;
	}
	if (ms->env && exit_status != 0)
		ft_lstfree(&ms->env);
	if (ms->exp && exit_status != 0)
		ft_lstfree(&ms->exp);
	if (ms->wkdir && exit_status != 0)
		free(ms->wkdir);
	if (ms->old_wkdir && exit_status != 0)
		free(ms->old_wkdir);
	if (ms->line)
	{
		free(ms->line);
		ms->line = NULL;
	}
	if (ms && exit_status != 0)
		free(ms);
	if (exit_status != 0)
		exit(exit_status);
}
