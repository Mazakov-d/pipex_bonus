/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:04:37 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/16 14:35:43 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	is_grep(char *str, int *k)
{
	if (*k == 0)
		*k = 1;
	else
		return (1);
	if (str[0] == 'g' && str[1] == 'r' && str[2] == 'e'
		&& str[3] == 'p')
		return (0);
	return (1);
}

int	is_flag(char *str)
{
	if (str && str[0] == '-')
		return (0);
	return (1);
}
int	count_flags(char **args, int i)
{
	int	count;
	int	comp;

	comp = 0;
	while (args && args[comp])
		comp++;
	i++;
	count = 1;
	while (args && i <= comp && args[i])
	{
		if (is_flag(args[i]) == 0)
		{
			count++;
			i++;
		}
		else
			break ;
	}
	return (count);
}

char	**command_flags(char **args, int *i, int j, int k)
{
	char	**cmd;

	cmd = malloc(sizeof(char *) * (count_flags(args, *i) + 3));
	if (!cmd)
		return (NULL);
	cmd[j] = ft_strndup(args[*i], ft_strclen(args[*i], 0));
	if (!cmd[j])
		return (free_strs(cmd));
	j++;
	*i = *i + 1;
	while (args && args[*i])
	{
		if (is_flag(args[*i]) == 0 || is_grep(cmd[0], &k) == 0)
		{
			cmd[j] = ft_strndup(args[*i], ft_strclen(args[*i], 0));
			if (!cmd[j])
				return (free_strs(cmd));
			*i = *i + 1;
			j++;
		}
		else
			break ;
	}
	cmd[j] = 0;
	return (cmd);
}

t_cmd	*get_commands(char **args)
{
	t_cmd	*cmd;
	int		i;
	int		j;

	cmd = malloc(sizeof(t_cmd) * (count_cmd(args) + 2));
	if (!cmd)
		return (NULL);
	i = 0;
	j = 0;
	while (args && args[i])
	{
		cmd[j].cmd = command_flags(args, &i, 0, 0);
		j++;
	}
	cmd[j].cmd = NULL;
	return (cmd);
}