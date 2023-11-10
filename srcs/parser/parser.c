/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/10 17:33:55 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

t_ast	*new_node(t_node_type type)
{
	t_ast	*new_ast;

	new_ast = ft_calloc(1, sizeof(t_ast));
	if (!new_ast)
		return (NULL);
	new_ast->type = type;
	new_ast->right = NULL;
	new_ast->left = NULL;
	new_ast->args = NULL;
	new_ast->subsh = 0;
	new_ast->redirs = NULL;
	return (new_ast);
}

t_ast	*handle_par(t_ms *ms)
{
	t_ast	*new_ast;
	t_ast	*tmp_tree;

	new_ast = NULL;
	eat_token(ms, T_LPAR);
	while (ms->cur_tok && ms->cur_tok->type != T_RPAR)
	{
		if (ms->cur_tok->type == T_WORD || ms->cur_tok->type >= T_LPAR)
			tmp_tree = handle_cmd(ms);
		else if (ms->cur_tok->type == T_PIPE)
			tmp_tree = handle_pipe(ms);
		else if (ms->cur_tok->type == T_AND_IF || ms->cur_tok->type == T_OR_IF)
			tmp_tree = handle_op(ms);
		if (!tmp_tree)
			return (NULL);
		if (!new_ast)
			new_ast = tmp_tree;
		else
			add_subtree(tmp_tree, &new_ast);
	}
	eat_token(ms, T_RPAR);
	new_ast->subsh = 1;
	return (new_ast);
}

int	is_redir(int type)
{
	if (type == T_LESS || type == T_GREAT
		|| type == T_DLESS || type == T_DGREAT)
		return (1);
	return (0);
}

t_redirs	*handle_red(t_ms *ms, t_ast *new_ast)
{
	t_redirs	*new_redir;

	if (ms->cur_tok->next_token->type != T_WORD)
		return (NULL);
	new_redir = redirs_new(ms->cur_tok->next_token, ms->cur_tok->type);
	if (!new_redir)
		return (NULL);
	redirs_add_back(&new_ast->redirs, new_redir);
	eat_token(ms, ms->cur_tok->type);
	eat_token(ms, ms->cur_tok->type);
	return (new_ast->redirs);
}

t_ast	*handle_cmd(t_ms *ms)
{
	t_ast	*new_ast;
	char	*cmd;
	t_list	*new_arg;

	if (ms->cur_tok->type == T_LPAR)
		return (handle_par(ms));
	new_ast = new_node(token_to_node(ms->cur_tok->type));
	if (!new_ast)
		return (NULL);
	while (ms->cur_tok && ms->cur_tok->type != T_PIPE
		&& ms->cur_tok->type != T_AND_IF && ms->cur_tok->type != T_OR_IF
		&& ms->cur_tok->type != T_EOF && ms->cur_tok->type != T_RPAR)
	{
		if (is_redir(ms->cur_tok->type))
		{
			if (!handle_red(ms, new_ast))
				return (free_root_ast(new_ast), NULL);
		}
		else
		{
			cmd = ft_strdup(ms->cur_tok->value);
			if (!cmd)
				return (free_root_ast(new_ast), NULL);
			new_arg = ft_lstnew(cmd);
			if (!new_arg)
				return (free(cmd), free_root_ast(new_ast), NULL);
			ft_lstadd_back(&new_ast->args, new_arg);
			eat_token(ms, T_WORD);
		}
	}
	return (new_ast);
}

t_ast	*handle_op(t_ms *ms)
{
	t_ast	*new_ast;
	t_ast	*tmp_tree;

	tmp_tree = NULL;
	new_ast = new_node(token_to_node(ms->cur_tok->type));
	if (!new_ast)
		return (NULL);
	eat_token(ms, ms->cur_tok->type);
	if (ms->cur_tok && ms->cur_tok->type != T_EOF
		&& (ms->cur_tok->type == T_WORD
			|| ms->cur_tok->type >= T_LPAR))
	{
		new_ast->right = handle_cmd(ms);
		if (!new_ast->right)
			return (free_root_ast(new_ast), NULL);
	}
	return (new_ast);
}

t_ast	*handle_pipe(t_ms *ms)
{
	t_ast	*new_ast;
	t_ast	*tmp_tree;

	tmp_tree = NULL;
	new_ast = new_node(token_to_node(T_PIPE));
	if (!new_ast)
		return (NULL);
	eat_token(ms, T_PIPE);
	while (ms->cur_tok && ms->cur_tok->type != T_PIPE
		&& ms->cur_tok->type != T_RPAR
		&& ms->cur_tok->type != T_EOF)
	{
		if (ms->cur_tok->type == T_WORD || ms->cur_tok->type >= T_LPAR)
			tmp_tree = handle_cmd(ms);
		else if (ms->cur_tok->type >= T_PIPE && ms->cur_tok->type <= T_OR_IF)
			tmp_tree = handle_op(ms);
		if (!tmp_tree)
			return (free_root_ast(new_ast), NULL);
		add_subtree(tmp_tree, &new_ast->right);
	}
	return (new_ast);
}

void	add_subtree(t_ast *node, t_ast	**root)
{
	if (!node)
		return ;
	if (!*root)
		*root = node;
	else
	{
		node->left = *root;
		*root = node;
	}
}

void	parse(t_ms *ms)
{
	t_ast	*tmp_tree;

	tmp_tree = NULL;
	while (ms->cur_tok && ms->cur_tok->type != T_EOF)
	{
		if (ms->cur_tok->type == T_WORD || ms->cur_tok->type >= T_LPAR)
			tmp_tree = handle_cmd(ms);
		else if (ms->cur_tok->type == T_PIPE)
			tmp_tree = handle_pipe(ms);
		else if (ms->cur_tok->type == T_AND_IF || ms->cur_tok->type == T_OR_IF)
			tmp_tree = handle_op(ms);
		if (!tmp_tree)
			free_minishell(ms, 1);
		add_subtree(tmp_tree, &ms->root);
	}
}
