/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:20:15 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/16 09:33:10 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

int	check_redir4(t_token *tok_lst)
{
	if ((tok_lst->type == T_LESS || tok_lst->type == T_GREAT \
	|| tok_lst->type == T_DLESS || tok_lst->type == T_DGREAT) \
	&& (tok_lst->next_token->type != T_WORD))
		return (printf("minishell: syntax error near unexpected token\n"), 1);
	return (0);
}

int	check_redir3(t_token *tok_lst)
{
	if ((tok_lst->type == T_LESS || tok_lst->type == T_GREAT || \
	tok_lst->type == T_DGREAT || tok_lst->type == T_DLESS) \
	&& tok_lst->next_token->type == T_EOF)// case : ...<eof or ...>eof or ...<<eof or ...>>eof		
		return (printf("minishell: \
		syntax error near unexpected token `newline'\n"), 1);
	else if (tok_lst->type == T_DLESS && tok_lst->next_token\
	->type == T_DLESS && (tok_lst->next_token->next_token->type\
	 && tok_lst->next_token->next_token->type == T_DLESS))// case t_word <<<<<< t_word
		return (printf("minishell: \
		syntax error near unexpected token `<<<'\n"), 1);
	else if (tok_lst->type == T_DLESS && tok_lst->next_token\
	->type == T_DLESS && (tok_lst->next_token->next_token->type\
	 && tok_lst->next_token->next_token->type == T_LESS))// case t_word <<<<< t_word
		return (printf("minishell: \
		syntax error near unexpected token `<<'\n"), 1);
	else if (tok_lst->type == T_DLESS && tok_lst->next_token\
	->type == T_DLESS)// case : t_word <<<< t_word
		return (printf("minishell: \
		syntax error near unexpected token `<'\n"), 1);
	else if (tok_lst->type == T_DGREAT && tok_lst->next_token\
	->type == T_DLESS)
		return (printf("minishell:\
		 syntax error near unexpected token `<<'\n"));// case : >> << 
	return (0);	
}

int	check_redir2(t_token *tok_lst)
{
	if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT &&
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_LESS)// case : <><
		return (printf("minishell: syntax error near unexpected token `<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT &&
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_DLESS
	&& tok_lst->next_token->next_token->next_token && tok_lst->next_token->next_token->next_token->type == T_LESS)// case : <><<<<
		return (printf("minishell: syntax error near unexpected token `<<<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT &&
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_DLESS
	&& tok_lst->next_token->next_token->next_token && tok_lst->next_token->next_token->next_token->type == T_DLESS)// case : <><<<<
		return (printf("minishell: syntax error near unexpected token `<<<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT &&
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_DLESS)// case : <><<
		return (printf("minishell: syntax error near unexpected token `<<'\n"), 1);
	else if (tok_lst->type == T_GREAT && tok_lst->next_token->type == T_LESS)// case : >< 
		return (printf("minishell: syntax error near unexpected token `<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT)// case : <     >
		return (printf("minishell: syntax error near unexpected token `>'\n"), 1);
	return (0);
}

int	check_redir1(t_token *tok_lst)
{
	if (tok_lst->type == T_DGREAT && tok_lst->next_token->type == T_DGREAT)// case t_word >>>> t_word
		return (printf("minishell: syntax error near unexpected token `>>'\n"), 1);
	else if (tok_lst->type == T_GREAT && tok_lst->next_token->type == T_GREAT)// case >  >
		return (printf("minishell: synthax error near unexpected token `>'\n"));
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_DGREAT && 
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_GREAT)// case : <>>>
		return (printf("minishell: syntax error near unexpected token `>>'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_DGREAT && 
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_DGREAT)// case : <>>>>
		return (printf("minishell: syntax error near unexpected token `>>'\n"), 1);
	else if (tok_lst->type == T_DGREAT && tok_lst->next_token->type == T_GREAT)// case : >>>eof or cmd >>> cmd		
		return (printf("minishell: syntax error near unexpected token `>'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_DGREAT)// case : <>>
		return (printf("minishell: syntax error near unexpected token `>'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT &&\
	 tok_lst->next_token->next_token && tok_lst->next_token->next_token->type\
	 == T_LESS && tok_lst->next_token->next_token->next_token && \
	 tok_lst->next_token->next_token->next_token->type == T_GREAT)// case : <  > <  >
		return (printf("minishell: syntax error near unexpected \
		token `>'\n"), 1);
	return (0);
}

int	check_redir(t_ms *ms)
{
	t_token	*tok_lst;

	tok_lst = ms->lexer->token_lst;
	while (tok_lst && tok_lst->next_token)
	{
		if (check_redir1(tok_lst))
			return (1);
		else if (check_redir2(tok_lst))
			return (1);
		else if (check_redir3(tok_lst))
			return (1);
		else if (check_redir4(tok_lst))
			return (1);
		tok_lst = tok_lst->next_token;
	}
	return (0);
}
