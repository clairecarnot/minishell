/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpy.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/13 15:12:27 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

/*
 * new_node:
 * Cree et initialise une structure AST du type indique
 */

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

/*
 * handle_par:
 * Boucle tant qu'on n'a pas recupere une ')'
 * La boucle recupere une structure AST du type correspondant au token suivant la parenthese
 * La boucle ajoute chaque AST recupere au-dessus dans l'ancien AST, et renvoie l'AST final avec un flag subshell = 1
 */

t_ast	*handle_par(t_ms *ms)
{
	t_ast	*new_ast;
	t_ast	*tmp_tree;

	new_ast = NULL;
	eat_token(ms, T_LPAR);
	while (ms->cur_tok && ms->cur_tok->type != T_RPAR)
	{
		if (ms->cur_tok->type == T_PIPE)
			tmp_tree = handle_pipe(ms);
		else if (ms->cur_tok->type == T_AND_IF || ms->cur_tok->type == T_OR_IF)
			tmp_tree = handle_op(ms);
		else
			tmp_tree = handle_cmd(ms);
		if (!tmp_tree)
			return (free_root_ast(new_ast), NULL);
		if (!new_ast)
			new_ast = tmp_tree;
		else
			add_subtree(tmp_tree, &new_ast);
	}
	eat_token(ms, T_RPAR);
	new_ast->subsh = 1;
	return (new_ast);
}

/*
 * is_redir:
 * Verifie que le type envoye est un token redirection
 */

int	is_redir(int type)
{
	if (type == T_LESS || type == T_GREAT
		|| type == T_DLESS || type == T_DGREAT)
		return (1);
	return (0);
}

/*
 * handle_red:
 * Cree une nouvelle structure t_redirs qui contient le nom du fichier correspondant a cette redirection. Cette structure est ajoutee a la liste des redirections de l'AST
 */

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

/*
 * add_cmd_args:
 * Cree une nouvelle structure t_list qui a pour content la valeur de la commande ou de l'argument qu'on ajoute a la liste d'arguments de l'AST
 */

t_list	*add_cmd_args(t_ms *ms, t_ast *new_ast)
{
	char	*cmd;
	t_list	*new_arg;

	cmd = ft_strdup(ms->cur_tok->value);
	if (!cmd)
		return (NULL);
	new_arg = ft_lstnew(cmd);
	if (!new_arg)
		return (free(cmd), NULL);
	ft_lstadd_back(&new_ast->args, new_arg);
	eat_token(ms, T_WORD);
	return (new_ast->args);
}

/*
 * handle_cmd
 * Si le token actuel est une parenthese => redirige vers la fonction handle_par
 * Sinon cree une structure AST de type commande
 * On cree les arguments de cet AST a travers une boucle qui continue tant que les tokens sont des words:
 * 	si word = redirection, le token suivant = le nom du fichier de redirection est ajoute en argument de la liste t_redirs
 * 	si word simple, on ajoute ce word a la t_list des arguments de la commande
 */

t_ast	*handle_cmd(t_ms *ms)
{
	t_ast	*new_ast;

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
			if (!add_cmd_args(ms, new_ast))
				return (free_root_ast(new_ast), NULL);
		}
	}
	return (new_ast);
}

/*
 * handle_op:
 * Cree une structure AST de type operateur (AND ou OR)
 * AND/OR est normalement forcement suivi d'un token word (word, parenthese ou redirection): on cree donc une autre structure AST qui sera la branche droite de l'AST operateur
 * Il n'y a pas de boucle puisque l'operateur n'a qu'une seule commande a sa droite
 * L'AST operateur renvoye sera implante au-dessus de l'ancienne root, a partir de sa branche gauche
 */

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

/*
 * handle_pipe:
 * Cree une structure AST de type PIPE
 * Le cas echeant, un autre AST qui deviendra la branche droite du pipe, est cree a travers une boucle (tant qu'on ne rencontre pas ')' ou '|')
 * On renvoie cet AST pipe qui sera ajoute "au-dessus" des noeuds precedants (a partir de sa branche gauche)
 */

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
		if (ms->cur_tok->type == T_AND_IF || ms->cur_tok->type == T_OR_IF)
			tmp_tree = handle_op(ms);
		else
			tmp_tree = handle_cmd(ms);
		if (!tmp_tree)
			return (free_root_ast(new_ast), NULL);
		add_subtree(tmp_tree, &new_ast->right);
	}
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
