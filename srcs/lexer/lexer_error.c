/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 10:46:00 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/31 12:16:19 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

int	check_par(t_ms *ms)
{
	int		lpar;
	int		rpar;
	t_token	*tok_lst;

	lpar = 0;
	rpar = 0;
	tok_lst = ms->lexer->token_lst;
	while (tok_lst && tok_lst->next_token)
	{
		if (tok_lst->type == T_LPAR)
			lpar++;
		else if (tok_lst->type == T_RPAR)
			rpar++;
		tok_lst = tok_lst->next_token;
	}
	if (lpar != rpar)
	{
		
		return (ft_putstr_fd("minishell: parenthesis error\n", 2), 1);
	}
	return (0);
}

//check les erreurs detectables au niveau lexer, return 1 if error, 0 if success
// line 94-95 the first token is an operator
// line 98 parenthesis pb
int	error_in_lexer(t_ms *ms)
{
	if (check_par(ms) != 0)
		return (1);
	return (0);
}
