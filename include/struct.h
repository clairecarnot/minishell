#ifndef STRUCT_H
# define STRUCT_H

# include "../libft/libft.h"
# include "lexer.h"

extern int	g_exit_code;

typedef struct s_dol
{
	t_list			*d;// expand du $ = 1, non expand = 0
	t_list			*c;
}		t_dol;

typedef enum e_builtin_type
{
	NOBUILT,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
}		t_builtin_type;

typedef struct s_cmd
{
	char	**env;
//	char	**tmp_args;
	char	**args;
	t_builtin_type	builtin;
	char	**bin_paths;
	int	abs_or_rel;
	int	valid_path;
	int	redir;
	int	valid_redir;
	char	*invalid_io;
}		t_cmd;

typedef struct s_ast	t_ast;

typedef enum e_node_type
{
	CMD,
	PIPE,
	AND_IF,
	OR_IF,
	LESS,
	GREAT,
	DLESS,
	DGREAT,
}			t_node_type;

// typedef	struct s_redirtype
// {
// 	t_node_type			type;
// 	struct s_redirtype	*next;
// }	t_redirtype;

typedef struct s_redirs
{
	// t_redirtype		*redirtype;
	t_node_type		type;
	char			*filename;
	struct s_redirs	*next_redir;
	t_dol		*dol;
}		t_redirs;

struct	s_ast
{
	t_node_type	type;
	t_ast		*right;
	t_ast		*left;
	t_list		*args;
	int			subsh;
	t_redirs	*redirs;
	int			*pipe;
	t_ast		*parent;
//	t_list		*dol;
	t_dol		*dol;
};

typedef enum e_type
{
	T_WORD,
	T_PIPE,
	T_AND_IF,
	T_OR_IF,
	T_LPAR,
	T_RPAR,
	T_LESS,
	T_GREAT,
	T_DLESS,
	T_DGREAT,
	T_NEWLINE,
	T_EOF,
}		t_type;

typedef struct s_token
{
	t_type			type;
	char			*value;
	struct s_token	*next_token;
	size_t			tok_nb;
	t_dol		*dol; // expand du $ = 1, non expand = 0
	//t_list			*dol;// expand du $ = 1, non expand = 0
}		t_token;

typedef struct s_lexer
{
	char	*src;
	size_t	src_size;
	char	cur_c;
	
	size_t	cur_pos;
	size_t	next_pos;
	t_token	*token_lst;
	size_t	tok_count;//nb de token final
}		t_lexer;

typedef struct s_ms
{
	int			i;//index lexer
	int			j;//lexer pour norme
	int			nb_q;//lexer pour norme
	int			flag_q;

	t_lexer		*lexer;
	char		*line;
	t_token		*cur_tok;
	t_ast		*root;
	t_list		*env;
	t_list		*exp;
	char		*wkdir;
	char		*old_wkdir;
	char		*home;
	t_ast		*cur_node;
	t_list		*pidlst;
	int			exit_code;
	int			previous_exit_code;
	int			in;
	int			out;
	t_list		*hdlst;

	int	a;//test malloc
}				t_ms;

#endif