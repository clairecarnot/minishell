/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:31:46 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/05 09:46:19 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "lexer.h"
# include "struct.h"
# include "builtins.h"

//----------------------- exec.c ------------------------
int	exec_andif(t_ast *node, t_ms *ms);
int	exec_orif(t_ast *node, t_ms *ms);
int	execute(t_ast *node, t_ms *ms);
int	pre_exec(t_ms *ms);

//----------------------- cmd_exec.c ------------------------
t_cmd	*node_to_cmd(t_ms *ms, t_ast *node, char **env);
int	exec_builtin(t_ms *ms, t_cmd *cmd);
int	do_cmd(t_cmd *cmd, t_ms *ms, char **env);
int	exec_cmd(t_ast *node, t_ms *ms);

//----------------------- cmd_path.c ------------------------
void	abs_rel_path(t_cmd *cmd);
char	**get_bin_paths(char **env);
void	build_path(t_cmd *cmd);

//----------------------- cmd_utils.c ------------------------
int	equals(char *s1, char *s2);
t_builtin_type	builtin_type_is(char *builtin);
char	**lst_to_tab(t_list *lst);
char    *ft_strjoin_slash(char const *s1, char const *s2);
char	**tab_cpy(t_ms *ms, char **tab);

//----------------------- cmd_utils2.c ------------------------
int	tab_size(char **tab);
char	**resize_tab(char **tab, int i);
//int	post_expand_adj(t_ms *ms, char **args, char **tmp);
char	**post_expand_adj(t_ms *ms, char **args, char **tmp);

//----------------------- pipe_exec.c ------------------------
t_list	*ft_lstnew_int(int pid);
int	do_cmdpipe(t_cmd *cmd, t_ms *ms, char **env);
int	exec_cmdpipe(t_ms *ms, t_ast *node, int tmp_fd);
int pipex(t_ms *ms, t_ast *node, int tmp_fd, int *fd);
int	exec_pipeline(t_ast *node, t_ms *ms);

//----------------------- clear_exec.c ------------------------
void	free_tab(char **tab);
void	free_cmd(t_cmd *cmd);
int	close_if(int *fd);

//----------------------- expand.c ------------------------
char	*get_varvalue(t_ms *ms, char *arg, int i, int j);
char	*skip_dol(char *arg, int i, int j, int data[2]);
char	*repl_dol(char *arg, char *var, int i, int j);
char	*keep_one_dol_only(t_ms *ms, char *arg, int i, t_dol **dol);
int	dol_standalone(char *arg, t_dol **dol);
char	*expand_dol(t_ms *ms, char *arg, int data[2], t_dol **dol);
int	cmd_expand(t_ms *ms, char **args, t_dol *dol);


#endif
