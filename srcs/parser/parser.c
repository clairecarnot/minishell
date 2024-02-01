/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:40:48 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:40:49 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

t_ast	*factor(t_ms *ms)
{
	t_ast	*node;

	if (!ms->cur_tok || (ms->cur_tok->type != T_WORD
			&& !is_redirect(ms, ms->cur_tok)))
	{
		if (!ms->cur_tok || ms->cur_tok->type == T_EOF)
			ms->cur_tok->type = T_NEWLINE;
		return (NULL);
	}
	node = new_node(ms, CMD);
	while (node && ms->cur_tok && ms->cur_tok->type != T_EOF
		&& (ms->cur_tok->type == T_WORD || is_redirect(ms, ms->cur_tok)))
	{
		if (ms->cur_tok && ms->cur_tok->type == T_WORD)
		{
			if (!add_cmd_args(ms, node))
				return (free_root_ast(node), NULL);
		}
		while (is_redirect(ms, ms->cur_tok))
		{
			if (!handle_red(ms, node))
				return (free_root_ast(node), NULL);
		}
	}
	return (node);
}

t_ast	*handle_par(t_ms *ms)
{
	t_ast	*node;

	node = NULL;
	eat_token(ms, T_LPAR);
	node = expr(ms);
	if (!node)
		return (NULL);
	eat_token(ms, T_RPAR);
	return (node);
}

t_ast	*term(t_ms *ms)
{
	t_ast	*node;
	t_ast	*middle_node;

	node = NULL;
	if (ms->cur_tok && ms->cur_tok->type == T_LPAR)
		node = handle_par(ms);
	else if (ms->cur_tok)
	{
		node = factor(ms);
		while (node && ms->cur_tok && ms->cur_tok->type == T_PIPE)
		{
			middle_node = new_node(ms, PIPE);
			if (!middle_node)
				return (free_root_ast(node), NULL);
			eat_token(ms, T_PIPE);
			middle_node->right = factor(ms);
			if (!middle_node->right)
				return (free(middle_node), free_root_ast(node), NULL);
			middle_node->right->parent = middle_node;
			middle_node->left = node;
			node->parent = middle_node;
			node = middle_node;
		}
	}
	return (node);
}

t_ast	*expr(t_ms *ms)
{
	t_ast	*node;
	t_ast	*middle_node;

	node = NULL;
	middle_node = NULL;
	node = term(ms);
	if (!node)
		return (NULL);
	while (ms->cur_tok->type == T_AND_IF || ms->cur_tok->type == T_OR_IF)
	{
		middle_node = new_node(ms, token_to_node(ms->cur_tok->type));
		if (!middle_node)
			return (free_root_ast(node), NULL);
		eat_token(ms, ms->cur_tok->type);
		middle_node->right = term(ms);
		if (!middle_node->right)
			return (free(middle_node), free_root_ast(node), NULL);
		middle_node->right->parent = middle_node;
		middle_node->left = node;
		node->parent = middle_node;
		node = middle_node;
	}
	return (node);
}

int	parse(t_ms *ms)
{
	if (!ms->cur_tok || ms->cur_tok->type == T_EOF)
		return (1);
	ms->root = expr(ms);
	if (ms->cur_tok->type != T_EOF && ms->exit_code != 255)
	{
		if (ms->cur_tok->type == T_NEWLINE)
			ft_putstr_fd("minishell: syntax error near \
				unexpected token `newline'\n", 2);
		else
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(ms->cur_tok->value, 2);
			ft_putstr_fd("'\n", 2);
		}
		ms->exit_code = 2;
		return (-1);
	}
	if (!ms->root)
		return (-1);
	return (1);
}
