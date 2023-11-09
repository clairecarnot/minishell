/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 14:51:11 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/09 17:32:14 by ccarnot          ###   ########.fr       */
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

void	free_minishell(t_ms *minishell, int exit_status)
{
	// if (minishell->parser)
	// {	
	// 	printf("hello1\n");
		if (minishell->root)
			free_root_ast(minishell->root);
	// 	free(minishell->parser);
	// }
	if (minishell->lexer)
	{
		token_lst_free(&minishell->lexer->token_lst);
		free(minishell->lexer);
	}
	free(minishell->line);
	if (minishell)
		free(minishell);
	exit(exit_status);
}
