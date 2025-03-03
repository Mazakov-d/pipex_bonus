/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:40:34 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/26 15:33:14 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*str_dup_c(char *str, char c_limit, char c_join)
{
	char	*new_str;
	int		i;

	i = 0;
	while (str[i] && str[i] != c_limit)
		i++;
	new_str = malloc(sizeof(char) * i + 2);
	if (!new_str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c_limit)
	{
		new_str[i] = str[i];
		i++;
	}
	if (c_join)
		new_str[i++] = c_join;
	new_str[i] = 0;
	return (new_str);
}

int	count_str_c_limit(char *str, char c_limit)
{
	int	i;
	int	count;

	i = 0;
	count = 1;
	while (str[i])
	{
		if (str[i] == c_limit)
			count++;
		i++;
	}
	return (count);
}

int	find_path_string(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (env[i][0] == 'P' && env[i][1] == 'A' && env[i][2] == 'T'
		&& env[i][3] == 'H' && env[i][4] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	**split_c(char	*str, char c_limit, char c_join)
{
	char	**strs;
	int		i;

	strs = malloc(sizeof(char *) * count_str_c_limit(str, c_limit) * 2);
	if (!strs)
		return (error_malloc());
	i = 0;
	while (*str)
	{
		strs[i] = str_dup_c(str, c_limit, c_join);
		if (!strs[i])
			return (free_strs(strs));
		while (*str && *str != c_limit)
			str++;
		if (*str == c_limit)
			str++;
		i++;
	}
	strs[i] = 0;
	return (strs);
}

char	**get_path_env(char **env)
{
	int		i;
	char	**path;

	i = find_path_string(env);
	if (i == -1)
	{
		write(2, "Error : didn't find the path\n", 30);
		return (NULL);
	}
	path = split_c(env[i] + 5, ':', '/');
	if (!path)
		return (NULL);
	return (path);
}
