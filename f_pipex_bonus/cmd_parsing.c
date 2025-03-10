/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 22:55:12 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/10 16:56:22 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	is_flag(char *str)
{
	if (str && str[0] == '-')
		return (0);
	return (1);
}

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

int	count_flags(char **args, int i)
{
	int	count;
	int	comp;

	comp = 0;
	while (args && args[comp])
		comp++;
	i++;
	count = 1;
	while (args && i < comp && args[i])
	{
		if (is_flag(args[i]) == 0)
		{
			count++;
			i++;
		}
		else
			break;
	}
	return (count);
}

char	**parse_cmd_args(char **args, int *i, int j, int k)
{
	char	**cmd;

	cmd = malloc(sizeof(char *) * (count_flags(args, *i) + 3));
	if (!cmd)
		return (NULL);
	cmd[j] = ft_strndup(args[*i], ft_strclen(args[*i], 0));
	if (!cmd[j])
		return (free_str_array(cmd));
	j++;
	*i = *i + 1;
	while (args && args[*i])
	{
		if (is_flag(args[*i]) == 0 || is_grep(cmd[0], &k) == 0)
		{
			cmd[j] = ft_strndup(args[*i], ft_strclen(args[*i], 0));
			if (!cmd[j])
				return (free_str_array(cmd));
			*i = *i + 1;
			j++;
		}
		else
			break;
	}
	cmd[j] = 0;
	return (cmd);
}

t_cmd *cmd_new(t_cmd *current, int position)
{
    t_cmd *next;

    next = malloc(sizeof(t_cmd));
    if (!next)
        return (NULL);
    if (position == 0)
        current->prev = NULL;
    current->next = next;
    current->position = position;
    next->prev = current;
    next->args = NULL;
    next->next = NULL;
    next->position = position + 1;
    return (next);
}

t_cmd	*parse_commands(char **args, int cmd_count)
{
	t_cmd	*cmd;
	t_cmd	*save;
	int		i;
	int		position;
	int     cmd_parsed;

	if (!args || !args[0] || cmd_count <= 0)
		return (NULL);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	save = cmd;
	i = 0;
	position = 0;
	cmd_parsed = 0;
	while (args && args[i] && cmd_parsed < cmd_count)
	{
		cmd->args = parse_cmd_args(args, &i, 0, 0);
		if (!cmd->args)
			return (free_cmd_list(save));
		cmd_parsed++;
		if (cmd_parsed >= cmd_count)
			break;
		cmd = cmd_new(cmd, position);
		if (!cmd)
			return (free_cmd_list(save));
		position++;
	}
	cmd->next = NULL;
	return (save);
}
