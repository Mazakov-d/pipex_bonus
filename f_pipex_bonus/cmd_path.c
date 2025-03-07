/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:29:16 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 21:34:16 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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

int	index_path_cmd(char *cmd, char **path)
{
	int		i;
	char	*path_cmd;
	char	**splited_cmd;

	i = 0;
	if (!cmd || !path)
		return (-1);
	splited_cmd = split_c(cmd, ' ', '\0');
	if (!splited_cmd)
		return (-1);
	while (path && path[i])
	{
		path_cmd = ft_strcat(path[i], splited_cmd[0]);
		if (!path_cmd)
			return (-1);
		if (access(path_cmd, F_OK | X_OK) == 0)
		{
			free(path_cmd);
			free_strs(splited_cmd);
			return (i);
		}
		free(path_cmd);
		i++;
	}
	free_strs(splited_cmd);
	return (-1);
}

char	*get_path_cmd(char *cmd, char **path)
{
	int		i;
	char	*path_cmd;
	char	**splited_cmd;

	if (!path || !cmd)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strndup(cmd, ft_strlen(cmd)));
		return (NULL);
	}
	splited_cmd = split_c(cmd, ' ', '\0');
	if (!splited_cmd)
		return (NULL);
	i = index_path_cmd(cmd, path);
	if (i == -1)
	{
		free_strs(splited_cmd);
		return (NULL);
	}
	path_cmd = ft_strcat(path[i], splited_cmd[0]);
	free_strs(splited_cmd);
	return (path_cmd);
}
