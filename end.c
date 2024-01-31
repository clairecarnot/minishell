#include "./include/general.h"

void	prefree_minishell_cmd(t_ms *ms, t_cmd *cmd)
{
	ms->exit_code = 255;
	free_cmd(cmd);
	free_minishell(ms, 255);
}

void	free_root_ast(t_ast *root)
{
	if (!root)
		return ;
	if (root->left)
		free_root_ast(root->left);
	if (root->right)
		free_root_ast(root->right);
	if (root->type == CMD)
		ft_lstfree(&root->args);
	if (root->redirs)
		redirs_free(&root->redirs);
	if (root->dol)
		dol_free(&root->dol);
	if (root->wil)
		wil_free(&root->wil);
	free(root);
	root = NULL;
}

/*
peut-etre a changer: dans les conditions if exit_status != 0 
(en cas d'erreur free certaines choses si non erreur(= passage
a la ligne suivante) ne pas les free)
*/

void	free_minishell_bis(t_ms *ms, int exit_status, int exit_code)
{
	(void) exit_code;
	if (ms->env && exit_status != 0)
		ft_lstfree(&ms->env);
	if (ms->exp && exit_status != 0)
		ft_lstfree(&ms->exp);
	if (ms->wkdir && exit_status != 0)
		free(ms->wkdir);
	if (ms->old_wkdir && exit_status != 0)
		free(ms->old_wkdir);
	if (ms->home && exit_status != 0)
		free(ms->home);
	if (ms->line)
		free(ms->line);
	ms->line = NULL;
	if (ms->hdlst)
		ft_lstfree(&ms->hdlst);
	ms->flag_q = 0;
	if (ms && exit_status != 0)
		free(ms);
	if (exit_status != 0)
		exit(exit_status);
//		exit(exit_code);
}

void	free_minishell(t_ms *ms, int exit_status)
{
	int		exit_code;
	t_list	*hdtmp;

	exit_code = ms->exit_code;
	close_if(&ms->in);
	close_if(&ms->out);
	hdtmp = ms->hdlst;
	while (hdtmp && isatty(0) && isatty(1))
	{
		if (hdtmp->content)
			unlink(hdtmp->content);
		hdtmp = hdtmp->next;
	}
	if (ms->pidlst)
		ft_intlstfree(&ms->pidlst);
	free_wil_dol(ms);
	if (ms->root)
		free_root_ast(ms->root);
	ms->root = NULL;
	if (ms->lexer)
		(token_lst_free(&ms->lexer->token_lst), free(ms->lexer));
	ms->lexer = NULL;
	free_minishell_bis(ms, exit_status, exit_code);
}
