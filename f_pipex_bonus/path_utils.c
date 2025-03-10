/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 00:35:32 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 22:41:35 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*str_dup_c(char *str, char c_limit, char c_join)
{
	char	*new_str;
	int		i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c_limit)
		i++;
	new_str = malloc(sizeof(char) * (i + 2));
	if (!new_str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c_limit)
	{
		new_str[i] = str[i];
		i++;
	}
	if (c_join != '\0')
		new_str[i++] = c_join;
	new_str[i] = '\0';
	return (new_str);
}

int	count_str_c_limit(char *str, char c_limit)
{
	int	i;
	int	count;

	if (!str)
		return (0);
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

int	find_path_in_env(char **env)
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

char	**split_c(char *str, char c_limit, char c_join)
{
	char	**strs;
	int		i;
	int		str_count;

	if (!str)
		return (NULL);
	str_count = count_str_c_limit(str, c_limit);
	strs = malloc(sizeof(char *) * (str_count + 1));
	if (!strs)
		return (error_malloc());
	i = 0;
	while (*str)
	{
		strs[i] = str_dup_c(str, c_limit, c_join);
		if (!strs[i])
			return (free_str_array(strs));
		while (*str && *str != c_limit)
			str++;
		if (*str == c_limit)
			str++;
		i++;
	}
	strs[i] = NULL;
	return (strs);
}

char	**get_path_from_env(char **env)
{
	int		i;
	char	**path;

	i = find_path_in_env(env);
	if (i == -1)
		return (NULL);
	path = split_c(env[i] + 5, ':', '/');
	if (!path)
		return (NULL);
	return (path);
}
