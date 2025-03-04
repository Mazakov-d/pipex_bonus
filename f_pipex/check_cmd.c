/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 19:41:59 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/04 14:45:24 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	*free_strs(char **strs)
{
	int	i;

	i = 0;
	if (strs)
	{
		while (strs[i])
		{
			free(strs[i]);
			i++;
		}
		free(strs);
	}
	return (NULL);
}

int	ft_strlen(char	*str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	*ft_strcat(char *str_a, char *str_b)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	str = malloc(sizeof(char) * (ft_strlen(str_a) + ft_strlen(str_b) + 1));
	if (!str)
		return (NULL);
	while (str_a[i])
	{
		str[i] = str_a[i];
		i++;
	}
	j = 0;
	while (str_b[j])
	{
		str[i] = str_b[j];
		j++;
		i++;
	}
	str[i] = 0;
	return (str);
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
	while (path[i])
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
	{
		if (path)
			free_strs(path);
		return (NULL);
	}
	splited_cmd = split_c(cmd, ' ', '\0');
	if (!splited_cmd)
	{
		free_strs(path);
		return (NULL);
	}
	i = index_path_cmd(cmd, path);
	if (i == -1)
	{
		free_strs(path);
		free_strs(splited_cmd);
		fprintf(stderr, "Command not found: %s\n", cmd);
		return (NULL);
	}
	path_cmd = ft_strcat(path[i], splited_cmd[0]);
	free_strs(path);
	free_strs(splited_cmd);
	return (path_cmd);
}
