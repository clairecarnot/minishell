#ifndef STRUCT_H
# define STRUCT_H

# include "../libft/libft.h"
# include "lexer.h"

extern int	g_exit_code;

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
	char	**args;
	t_builtin_type	builtin;
}		t_command;

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

typedef struct s_redirs
{
	t_node_type		type;
	char			*filename;
	struct s_redirs	*next_redir;
}		t_redirs;

struct	s_ast
{
	t_node_type	type;
	t_ast		*right;
	t_ast		*left;
	t_list		*args;
	int			subsh;
	t_redirs	*redirs;
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
	t_lexer		*lexer;
	char		*line;
	t_token		*cur_tok;
	t_ast		*root;
	t_list		*env;
	t_list		*exp;
	char		*wkdir;
	char		*old_wkdir;
	t_ast		*cur_node;
	t_list		*pidlst;
}				t_ms;

#endif 