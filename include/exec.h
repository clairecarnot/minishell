#ifndef EXEC_H
# define EXEC_H

# include "lexer.h"
# include "struct.h"
# include "builtins.h"
# include <dirent.h>

//----------------------- exec.c ------------------------
int				exec_andif(t_ast *node, t_ms *ms);
int				exec_orif(t_ast *node, t_ms *ms);
int				execute(t_ast *node, t_ms *ms);
int				pre_exec(t_ms *ms);

//----------------------- cmd_hdname.c ------------------------
void			update_hdlst(t_ms *ms, char *name);
char			random_char(t_ms *ms, char *name);
char			*generate_hdname(t_ms *ms);

//----------------------- cmd_hdoc.c ------------------------
void			hdoc_ctrl_d(t_ms *ms, int nb_line);
int				hdoc_sigint(int fd);
int				hdoc_create(t_ms *ms, int fd, char *filename, int limlen);
char			*handle_dless(t_ms *ms, t_redirs *redirs, char *limiter);
int				open_heredocs(t_ms *ms, t_ast *node);

//----------------------- cmd_redirs.c ------------------------
void			print_msg_redir(char *invalid_io);
int				handle_less(t_ms *ms, t_redirs *redirs);
int				handle_great(t_ms *ms, t_redirs *redirs);
int				handle_dgreat(t_ms *ms, t_redirs *redirs);
int				cmd_redirs(t_ms *ms, t_ast *node, t_cmd *cmd);

//----------------------- cmd_exec.c ------------------------
int				node_to_cmd_bis(t_ms *ms, t_ast *node, t_cmd *cmd, t_list *tmp_w);
int				node_to_cmd(t_ms *ms, t_ast *node, t_cmd *cmd);
int				exec_builtin(t_ms *ms, t_cmd *cmd);
int				do_cmd(t_cmd *cmd, t_ms *ms, char **env);
int				exec_cmd(t_ast *node, t_ms *ms);

//----------------------- cmd_exec_bis.c ------------------------
t_cmd			*init_cmd(t_ms *ms, char **env);
int				redef_cmdargs_bis(t_ms *ms, t_cmd *cmd, int i);
int				redef_cmdargs(t_ms *ms, t_cmd *cmd);
int				msg_do_cmd(t_ms *ms, t_cmd *cmd);
void			replace_var_underscore(t_ms *ms, t_cmd *cmd);

//----------------------- cmd_path.c ------------------------
int				abs_rel_path(t_ms *ms, t_cmd *cmd);
char			*get_bin_path_underscore(t_ms *ms, t_cmd *cmd, char *env_path,
					char **env);
void			free_path_cmd_ms(t_ms *ms, t_cmd *cmd, char *env_path);
char			**get_bin_paths(t_ms *ms, char **env, t_cmd *cmd);
int				build_path(t_ms *ms, t_cmd *cmd);

//----------------------- cmd_utils1.c ------------------------
int				equals(char *s1, char *s2);
t_builtin_type	builtin_type_is(char *builtin);
char			**lst_to_tab(t_list *lst);
char			*ft_strjoin_slash(char const *s1, char const *s2);
char			*ft_slash_addback(char const *s1);

//----------------------- cmd_utils2.c ------------------------
void			save_ptrs(t_list **s1, t_list **s2, t_list **t1, t_list **t2);
char			**tab_cpy(t_ms *ms, char **tab);
t_list			*ft_lstnew_int(int pid);

//----------------------- pipe_exec.c ------------------------
int				pipe_end_cmd(t_ms *ms, t_ast *node, int tmp_fd);
int				parent_middle(t_ms *ms, int tmp_fd, int *fd, pid_t pid);
int				pipe_middle_cmd(t_ms *ms, t_ast *node, int tmp_fd, int *fd);
int				pipex(t_ms *ms, t_ast *node, int tmp_fd, int *fd);
int				exec_pipeline(t_ast *node, t_ms *ms);

//----------------------- pipe_exec_bis.c ------------------------
int				do_cmdpipe(t_cmd *cmd, t_ms *ms, char **env);
int				exec_cmdpipe_bis(t_ms *ms, char **env, t_cmd *cmd);
int				exec_cmdpipe(t_ms *ms, t_ast *node, int tmp_fd);
void			kill_loop(t_ms *ms);

//----------------------- clear_exec.c ------------------------
void			free_tab(char **tab);
void			free_cmd(t_cmd *cmd);
int				close_if(int *fd);
void			go_garbage(int i, t_trash *trash);

//----------------------- EXPAND FILES ------------------------

//----------------------- expand_getvar.c ------------------------
char			*ft_getenv(t_ms *ms, char *var);
char			*exp_exitcode(t_ms *ms);
char			*trim_beg(char const *s1, char const *set);
char			*ft_trimvar(char *value);
char			*get_varvalue(t_ms *ms, char *arg, int i, int j);

//----------------------- expand_utils1.c ------------------------
int				is_same_len(char *s1, char *s2);
int				isset(char c, char const *set);
int				count_consec_spc(char *value);
int				find_cur_dol(char *arg, int data[5]);

//----------------------- expand_utils2.c ------------------------
int				delimitate_var(char *arg, int i, t_dol **dol);
int				hd_delimitate_var(char *arg, int i);
void			free_if(char *str);
int				tab_size(char **tab);
int				contains_spc(char *arg, int j, int data[5]);

//----------------------- expand_do_dols.c ------------------------
char			*skip_dol(char *arg, int i, int j, int data[5]);
char			*repl_dol(char *arg, char *var, int i, int j);
char			*keep_one_dol_only(t_ms *ms, char *arg, int i, t_dol **dol);
int				dol_standalone_return(char c);
int				dol_standalone(char *arg, t_dol **dol);

//----------------------- expand_redef_args.c ------------------------
void			redefine_datapos(char **new_args, int i, int j, int data[5]);
char			**redefine_args_bis(t_cmd *cmd, char *d[2], char **new_args,
					int i);
char			**redefine_args(t_cmd *cmd, int i, int j, int data[5]);
int				args_redef(t_cmd *cmd, int i, int j, int data[5]);

//----------------------- expand.c ------------------------
char			*expand_dol(t_ms *ms, char *arg, int data[5], t_dol **dol);
int				init_data(int data[5], int *j);
void			update_expand_pos(int data[5], int *i, int *j, t_dol **dol);
int				cmd_expand(t_ms *ms, t_cmd *cmd, t_dol *dol);

//----------------------- expand_first.c ------------------------
char			*ft_trimvar_first(char *value);
char			*get_varvalue_first(t_ms *ms, char *arg, int i, int j);
char			*expfirst(t_ms *ms, char *arg, int data[5], t_dol **dol);

//----------------------- expand_redirs.c ------------------------
char			*get_varvalue_redir(t_ms *ms, char *arg, int i, int j);
char			*expand_dol_redir(t_ms *ms, char *arg, int data[5],
					t_dol **dol);
void			r_update_expand_pos(int data[5], int *j, t_dol **dol);
char			*expand_redir(t_ms *ms, char *arg, t_dol *dol);

//----------------------- expand_hdoc.c ------------------------
char			*hd_keep_one_dol_only(t_ms *ms, char *arg, int i);
int				hd_dol_standalone(char *arg);
char			*hd_expand_dol(t_ms *ms, char *arg, int data[5]);
void			hd_update_expand_pos(int data[5], int *j);
char			*expand_hdoc(t_ms *ms, char *arg);

//----------------------- wildcards.c ------------------------
char			**cpy_end_args(char **new, char **args, int k, int i);
char			**make_new_args(char **args, char **ad, int w_i, int i);
char			**wildcards(char **args, t_wildcard *wildc);
t_wildcard		*init_wildc(t_wil *wil);
int				cmd_wildcard(t_cmd *cmd, t_wil *wil);

//----------------------- wildcards2.c ------------------------
int				same_as_wildcard(char *d_name, char *arg, int *flag);
char			**wldc2_split(char *new_arg);
char			*wldc2_join(char *d_name, char *new_arg);
char			*wildc2_dup_join(char *d_name, t_wildcard *wc, char *new_arg);
char			**wildcards_2(DIR *d, struct dirent *file, char **args, t_wildcard *wildc);

//----------------------- wildcards_utils.c ------------------------
int				*lstint_to_tab(t_list *lst);
int				has_asterisk(char *str);
void			advance_in_lst(t_list **lst, int size);
int				len_dchar(char **tab);
char			**copy_args(char **str);

//----------------------- wildcards_free.c ------------------------
int				cmd_wildcard_free(t_wildcard *wildc);
int				wildcards_init(DIR **d, struct dirent **file);

#endif
