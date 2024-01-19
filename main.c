#include "./include/lexer.h"
#include "./include/parser.h"
#include "./include/struct.h"
#include "./include/env.h"
#include "./include/signals.h"
#include "./include/exec.h"
#include "./include/builtins.h"

char	*tok_to_str(t_token *token)//temporaire
{
	if (token->type == T_WORD)
		return ("T_WORD");
	if (token->type == T_PIPE)
		return ("T_PIPE");
	if (token->type == T_AND_IF)
		return ("T_AND_IF");
	if (token->type == T_OR_IF)
		return ("T_OR_IF");
	if (token->type == T_LPAR)
		return ("T_LPAR");
	if (token->type == T_RPAR)
		return ("T_RPAR");
	if (token->type == T_LESS)
		return ("T_LESS");
	if (token->type == T_GREAT)
		return ("T_GREAT");
	if (token->type == T_DLESS)
		return ("T_DLESS");
	if (token->type == T_DGREAT)
		return ("T_DGREAT");
	if (token->type == T_NEWLINE)
		return ("T_NEWLINE");
	if (token->type == T_EOF)
		return ("T_EOF");
	else
		return ("UNKNOWN");
}

char	*node_to_str(t_ast *node)//temporaire
{
	if (node->type == CMD)
		return ("CMD");
	if (node->type == PIPE)
		return ("PIPE");
	if (node->type == AND_IF)
		return ("AND_IF");
	if (node->type == OR_IF)
		return ("OR_IF");
	if (node->type == LESS)
		return ("LESS");
	if (node->type == GREAT)
		return ("GREAT");
	if (node->type == DLESS)
		return ("DLESS");
	if (node->type == DGREAT)
		return ("DGREAT");
	if (node->type == NEWLINE)
		return ("NEWLINE");
	else
		return ("UNKNOWN");
}

void	print_token_lst(t_token *token)//temporaire
{
	t_token	*token_lst;

	token_lst = token;
	while (token_lst)
	{
		dprintf(2, "%s est de type %s\n", (token_lst)->value, tok_to_str(token_lst));
		if (token_lst->dol)
			print_lst(token_lst->dol->d);
		if (token_lst->dol)
			print_lst(token_lst->dol->c);
		token_lst = (token_lst)->next_token;
	}
}

void	print_redirs(t_redirs *args_enter)//temporaire
{
	t_redirs	*args;

	args = args_enter;
	while (args)
	{
		printf("redirs filename = %s\n", (char *)args->filename);
		args = args->next_redir;
	}
}

void	print_lst(t_list *args_enter)//temporaire
{
	t_list	*args;
	int		i = 0;

	args = args_enter;
	while (args)
	{
//		dprintf(2, "args content = %s\n", (char *)args->content);
		dprintf(2, "int = %d\n", args->n);
		// dprintf(2, "%d\n", i);
		i++;
		args = args->next;
	}
}

void	visit_node(t_ast *root)//temporaire
{
	if (!root)
		return ;
	visit_node(root->left);
	dprintf(2, "node type : %s subshell : %d\n", node_to_str(root), root->subsh);
	dprintf(2, "ARGS\n");
	if (root->args)
		print_lst(root->args);
	dprintf(2, "REDIRS\n");
	if (root->redirs)
		print_redirs(root->redirs);
	dprintf(2, "ARGS DOLS\n");
	if (root->dol)
	{
		print_lst(root->dol->d);
		print_lst(root->dol->c);
	}
	dprintf(2, "REDIRS DOLS\n");
	if (root->redirs)
	{
		if (root->redirs->dol)
		{
			print_lst(root->redirs->dol->d);
			print_lst(root->redirs->dol->c);
		}
	}
	dprintf(2, "WILDS\n");
	if (root->wil)
	{
		if (root->wil)
		{
			print_lst(root->wil->w);
		}
	}
	visit_node(root->right);
//	printf("exiting node %s\n", node_to_str(root));
}

t_ms	*init_ms(char **env)
{
	t_ms	*minishell;

	minishell = ft_calloc(1, sizeof(t_ms));
	if (!minishell)
		return (NULL);
	minishell->lexer = NULL;
	minishell->cur_tok = NULL;
	minishell->root = NULL;
	minishell->env = NULL;
	minishell->exp = NULL;
	minishell->wkdir = NULL;
	minishell->old_wkdir = NULL;
	minishell->cur_node = NULL;
	minishell->pidlst = NULL;
	minishell->exit_code = 0;
	minishell->previous_exit_code = 0;
	minishell->in = -1;
	minishell->out = -1;
	minishell->hdlst = NULL;
	if (init_env(minishell, env))
		free_minishell(minishell, 1);
	init_workdir(minishell, 0);
	if (init_exp(minishell))
		free_minishell(minishell, 1);
	minishell->i = 0;//index lexer pour norme
	minishell->j = 0;//lexer pour norme
	minishell->i_w = 0;
	minishell->a = 0;//a enlever
	return (minishell);
}

char	*display_prompt(t_ms *ms)
{
	// dprintf(2, "display prompt\n");
	char	*line;

	line = NULL;
	line = readline("minishell$ ");
	// if (isatty(fileno(stdin)))
	// 	line = readline("minishell$ ");
	// else
	// {
	// 	char *line2;
	// 	line2 = get_next_line(fileno(stdin), 0);
	// 	line = ft_strtrim(line2, "\n");
	// 	free(line2);
	// }
	if (!line)
		free_minishell(ms, 1);
	add_history(line);
	// dprintf(2, "end display prompt\n");
	return (line);
}

void	print_tree(t_ast *root, int space)
{
	int i;

	if (!root)
		return ;
	space += 10;
	print_tree(root->right, space);
	printf("\n");
	for (i = 10; i < space; i++)
		printf(" ");
	if (!root->parent)
		printf("%s", node_to_str(root));
	else
		printf("%s par:%s", node_to_str(root), node_to_str(root->parent));
	if (root->type == CMD)
		printf("%s\n", (char *)root->args->content);
		// printf("%s ss:%d dol:%d\n", (char *)root->args->content, root->subsh, root->dol);

	print_tree(root->left, space);
}

// char	*ft_sjoin_char(t_ms *ms, char *s1, char s2)
// {
// 	char	*dest;
// 	int		i;

// 	i = 0;
// 	if (ms)
// 	dest = malloc(sizeof(char) * ft_slen(s1) + 1 + 1);// a proteger
// 	if (s1)
// 	{
// 		while (s1[i])
// 		{
// 			dest[i] = s1[i];
// 			i++;
// 		}
// 	}
// 	if (s2)
// 		dest[i] = s2;
// 	dest[i + 1] = '\0';
// 	return (dest);
// }

// int	count_dolq(char *line, int i)
// {
// 	int	dol;

// 	dol = 0;
// 	while(line[i + dol] && line[i + dol] == '$')
// 		dol++;
// 	if (line[i + dol] && (line[i + dol] == '\'' || line[i + dol] == '\"'))
// 		return (dol);
// 	return (0);
// }

// char	*remove_dolq(t_ms *ms)
// {
// 	int		i;
// 	int		nb_dol;
// 	char	*new;

// 	i = 0;
// 	new = NULL;
// 	while (ms->line[i])
// 	{
// 		nb_dol = 0;
// 		if (ms->line[i] == '$')
// 			nb_dol = count_dolq(ms->line, i);
// 		if (nb_dol)
// 			i += nb_dol;
// 		new = ft_sjoin_char(ms, new, ms->line[i]);
// 		i++;
// 	}
// 	return (new);
// }

int	non_interactive_mode(t_ms *minishell, char **env)
{
	char	*line;

	line = get_next_line(STDIN_FILENO, 0);
	if (!line)
		(get_next_line(0, 1), exit(0));
	line[ft_strlen(line) - 1] = '\0';
	minishell = init_ms(env);
	if (!minishell)
		exit(1);
//	dprintf(2, "%s\n", line);
	minishell->line = line;
	if (!check_error_prelexer(minishell->line))
	{
		if (*minishell->line == '\"' || *minishell->line == '\'')
			minishell->flag_q = 1;
		if (!lexer(minishell, minishell->line))// if no error
		{
			if (!minishell->lexer)
				return (free_minishell(minishell, 1), 1);
			//print_token_lst(minishell->lexer->token_lst);
			minishell->cur_tok = minishell->lexer->token_lst;
			if (parse(minishell) == -1)
			{
				if (minishell->exit_code == 255)
				{
					ft_putstr_fd("minishell: malloc error\n", 2);
					get_next_line(0, 1);
					free_minishell(minishell, 255);
				}
				else if (minishell->exit_code == 2)
				{
					get_next_line(0, 1);
					free_minishell(minishell, 2);
				}
			}
			else
			{
				// print_tree(minishell->root, 0);
				//					visit_node(minishell->root);
				//exec_env(minishell);
				//exec_export(minishell, minishell->root);
				pre_exec(minishell);
				//					dprintf(2, "after exec\n");
				get_next_line(0, 1);
				free_exit(minishell);
				exit(0);
			}
		}
	}
	(get_next_line(0, 1), exit(1));
}

/*
 * note {a}
 * ./minishell(1) | ./minishell(2)
 * Le minishell(2) n'est pas un terminal, il va exit
 * Pour le minishell(1):
 * Le pipeline va remplacer la STDOUT par un pipe[1] => comme le ms(2) va exit, 
 * il y aura un SIGPIPE si le ms(1) essaye d'ecrire dans le pipe[1]
 * On doit donc re-rediriger la sortie pipe[1] vers STDOUT afin d'eviter le SIGPIPE
 * Comme STDOUT est deja remplace par pipe[1], on utilise STDIN pour rediriger pipe[1]
 * (effectivement, stdin et stdout se rapportent au meme fichier, le terminal
 * donc bien que differents on peut remplacer l'un par l'autre)
 */

int	main(int argc, char **argv, char **env)
{

	t_ms	*minishell;
	int	fd;
	int	i;

	(void)argc;
	(void)argv;
	i = 0;
	minishell = NULL;
	if (!isatty(0)) //pour ./minishell | ./minishell (SIGPIPE - note {a})
		non_interactive_mode(minishell, env);
	if (isatty(0))
	{
		fd = open("/dev/stdin", O_RDWR); //A PROTEGER
		dup2(fd, STDOUT_FILENO); //A PROTEGER
		close_if(&fd);
	}
	minishell = NULL;
	minishell = init_ms(env);
	if (!minishell)
		return (1);
	while (1)
	{
		// dprintf(2, "nvelle boucle\n");
		preprompt_signals();
		// dprintf(2, "ms->wkdir = %s\n", minishell->wkdir);
		// dprintf(2, "ms->oldwkdir = %s\n", minishell->old_wkdir);
		minishell->line = display_prompt(minishell);
		if (!minishell->line)
			return (free_minishell(minishell, 0), 0);//verifier protec
		if (g_exit_code == 2)
		{
			minishell->exit_code = 130;
			g_exit_code = 0;
		}
		minishell->previous_exit_code = minishell->exit_code;
		minishell->exit_code = 0;
//		postprompt_signals();
		// dprintf(2, "ms->line = %s\n", minishell->line);
		// minishell->line = remove_dolq(minishell);
		// dprintf(2, "ms->line = %s\n", minishell->line);
//		if (!check_error_prelexer(minishell->line) && !lexer(minishell, minishell->line))// if no error
		if (!check_error_prelexer(minishell->line))
		{
			if (*minishell->line == '\"' || *minishell->line == '\'')
				minishell->flag_q = 1;
			if (!lexer(minishell, minishell->line))// if no error
			{
				if (!minishell->lexer)
					return (free_minishell(minishell, 1), 1);
//				print_token_lst(minishell->lexer->token_lst);
				minishell->cur_tok = minishell->lexer->token_lst;
				if (parse(minishell) == -1)
				{
					if (minishell->exit_code == 255)
					{
						ft_putstr_fd("minishell: malloc error\n", 2);
						free_minishell(minishell, 255);
					}
					else if (minishell->exit_code == 2)
						free_minishell(minishell, 0);
				}
				else
				{
					// print_tree(minishell->root, 0);
					// visit_node(minishell->root);
					//exec_env(minishell);
					//exec_export(minishell, minishell->root);
					pre_exec(minishell);
//					dprintf(2, "after exec\n");
					free_minishell(minishell, 0);
				}
			}
		}
		init_workdir(minishell, 1);
		i++;
		minishell->a++;
	}
	free_minishell(minishell, 1);
	return (0);
}
