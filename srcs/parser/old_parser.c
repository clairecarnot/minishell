/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/24 12:40:13 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

/*
 * new_node:
 * Cree et initialise une structure AST du type indique
 */

t_ast	*new_node(t_ms *ms, t_node_type type)
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
	new_ast->dol = ms->cur_tok->dol;
	return (new_ast);
}

/*
 * add_subtree:
 * Si la structure AST "root" n'existe pas, elle devient la nouvelle structure AST "node"
 * Sinon, node est ajoute "au-dessus" de l'ancienne structure root: sa branche gauche devient l'ancien root, et root pointe maintenant sur node
 */

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

/*
 * parse:
 * Boucle sur la liste de tokens jusqu'a EOF
 * en fonction du type de token, la boucle renvoie vers les fonctions qui gerent respectivement: les pipes, les and/or, les commandes (words, parentheses et redirections)
 * ces fonctions renvoient un tmp_tree soit une structure AST (types: pipe/and or/cmd)
 * chaque nouveau tmp_tree est ajoute "au-dessus" de l'ancien noeud root
 */
void	parse(t_ms *ms)
{
	t_ast	*tmp_tree;

	tmp_tree = NULL;
	while (ms->cur_tok && ms->cur_tok->type != T_EOF)
	{
		if (ms->cur_tok->type == T_PIPE)
			tmp_tree = handle_pipe(ms);
		else if (ms->cur_tok->type == T_AND_IF || ms->cur_tok->type == T_OR_IF)
			tmp_tree = handle_op(ms);
		else
			tmp_tree = handle_cmd(ms);
		if (!tmp_tree)
			free_minishell(ms, 1);
		add_subtree(tmp_tree, &ms->root);
	}
}
