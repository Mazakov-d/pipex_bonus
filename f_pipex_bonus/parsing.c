/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:04:37 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 21:35:12 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	is_grep(char *str, int *k)
{
	if (*k == 0)
		*k = 1;
	else
		return (1);
	if (str && str[0] == 'g' && str[1] == 'r' && str[2] == 'e'
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

t_data	*new_cmd(t_data *actu, int i)
{
	t_data	*next;

	next = malloc(sizeof(t_data));
	if (!next)
		return (NULL);
	if (i == 0)
		actu->prev = NULL;
	actu->next = next;
	actu->place = i;
	next->prev = actu;
	next->cmd = NULL;
	next->next = NULL;
	return (next);
}

t_data	*get_commands(char **args)
{
	t_data	*cmd;
	t_data	*save;
	int		i;
	int		j;

	if (!args || !args[0])
		return (NULL);
	cmd = malloc(sizeof(t_data));
	if (!cmd)
		return (NULL);
	save = cmd;
	i = 0;
	j = 0;
	while (args && args[i] && args[i + 1])
	{
		cmd->cmd = command_flags(args, &i, 0, 0);
		if (!cmd->cmd)
			return (free_data(save));
		cmd = new_cmd(cmd, j);
		if (!cmd)
			return (free_data(save));
		j++;
	}
	cmd->next = NULL;
	return (save);
}
