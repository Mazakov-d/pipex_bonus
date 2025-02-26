/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 13:29:16 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/26 15:37:25 by dmazari          ###   ########.fr       */
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
	splited_cmd = split_c(cmd, ' ', '\0');
	if (!splited_cmd)
		return (-1);
	while (path[i])
	{
		path_cmd = ft_strcat(path[i], splited_cmd[0]);
		if (!path_cmd)
			return (-1);
		if (access(path_cmd, F_OK) == 0 && access(path_cmd, X_OK) == 0)
		{
			free(path_cmd);
			free_strs(splited_cmd);
			return (i);
		}
		free(path_cmd);
		i++;
	}
	write(2, "Error : didn't find the command in Path\n", 41);
	free_strs(splited_cmd);
	return (-1);
}

char	*get_path_cmd(char *cmd, char **path)
{
	int		i;
	char	*path_cmd;

	if (!path || !cmd)
	{
		if (path)
			free_strs(path);
		return (NULL);
	}
	i = index_path_cmd(cmd, path);
	if (i == -1)
	{
		free_strs(path);
		return (NULL);
	}
	path_cmd = ft_strcat(path[i], cmd);
	if (!path_cmd)
	{
		free_strs(path);
		return (NULL);
	}
	free_strs(path);
	return (path_cmd);
}
