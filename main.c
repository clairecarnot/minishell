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
		printf("redirs = %s\n", (char *)args->filename);
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
//		printf("args content = %s\n", (char *)args->content);
		// printf("%s\n", (char *)args->content);
		// dprintf(2, "%d\n", *((pid_t *)args->content));
		dprintf(2, "%d\n", args->n);
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
	/*
	printf("node type : %s subshell : %d\n", node_to_str(root), root->subsh);
	if (root->args)
		print_lst(root->args);
	if (root->redirs)
		print_redirs(root->redirs);
		*/
	if (root->dol)
	{
		print_lst(root->dol->d);
		print_lst(root->dol->c);
	}
	else
		dprintf(2, "pas de dol\n");
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
	if (init_env(minishell, env))
		free_minishell(minishell, 1);
	if (init_workdir(minishell))
		free_minishell(minishell, 1);
	if (init_exp(minishell))
		free_minishell(minishell, 1);
	minishell->i = 0;//index lexer pour norme
	minishell->j = 0;//lexer pour norme
	return (minishell);
}

char	*display_prompt()
{
	char	*line;

	// line = readline("minishell$ ");
	if (isatty(fileno(stdin)))
		line = readline("minishell$ ");
	else
	{
		char *line2;
		line2 = get_next_line(fileno(stdin), 0);
		line = ft_strtrim(line2, "\n");
		free(line2);
	}
	if (!line)
		return (NULL);
	add_history(line);
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

char	*ft_sjoin_char(t_ms *ms, char *s1, char s2)
{
	char	*dest;
	int		i;

	i = 0;
	if (ms)
	dest = malloc(sizeof(char) * ft_slen(s1) + 1 + 1);// a proteger
	if (s1)
	{
		while (s1[i])
		{
			dest[i] = s1[i];
			i++;
		}
	}
	if (s2)
		dest[i] = s2;
	dest[i + 1] = '\0';
	return (dest);
}

int	count_dolq(char *line, int i)
{
	int	dol;

	dol = 0;
	while(line[i + dol] && line[i + dol] == '$')
		dol++;
	if (line[i + dol] && (line[i + dol] == '\'' || line[i + dol] == '\"'))
		return (dol);
	return (0);
}

char	*remove_dolq(t_ms *ms)
{
	int		i;
	int		nb_dol;
	char	*new;

	i = 0;
	new = NULL;
	while (ms->line[i])
	{
		nb_dol = 0;
		if (ms->line[i] == '$')
			nb_dol = count_dolq(ms->line, i);
		if (nb_dol)
			i += nb_dol;
		new = ft_sjoin_char(ms, new, ms->line[i]);
		i++;
	}
	return (new);
}

int	main(int argc, char **argv, char **env)
{

	t_ms	*minishell;
	int	fd;

	(void)argc;
	(void)argv;
	if (!isatty(0)) //pour ./minishell | ./minishell (SIGPIPE)
		exit(0);
	else
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
//		preprompt_signals();
		minishell->line = display_prompt();
		if (!minishell->line)
			return (free_minishell(minishell, 0), 0);//verifier protec
//		postprompt_signals();
		// dprintf(2, "ms->line = %s\n", minishell->line);
		// minishell->line = remove_dolq(minishell);
		// dprintf(2, "ms->line = %s\n", minishell->line);
		if (!check_error_prelexer(minishell->line) && !lexer(minishell, minishell->line))// if no error
		{	
			if (!minishell->lexer)
				return (free_minishell(minishell, 1), 1);
//			print_token_lst(minishell->lexer->token_lst);

			minishell->cur_tok = minishell->lexer->token_lst;
			if (parse(minishell) == -1)
			{
				if (minishell->exit_code == 255)
				{
					printf("minishell: malloc error\n");
					free_minishell(minishell, 1);
				}
				else if (minishell->exit_code == 2)
					free_minishell(minishell, 0);
			}
			else
			{
				// print_tree(minishell->root, 0);
				// visit_node(minishell->root);
				pre_exec(minishell);
				free_minishell(minishell, 0);
			}
		}
	}
	free_minishell(minishell, 1);
	return (0);
}
