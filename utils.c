/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:07:40 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/16 14:07:12 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	ft_strclen(char *str, int c)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != c)
		i++;
	return (i);
}

char	*ft_strndup(char *str, int n)
{
	char	*dup;
	int		i;

	if (n < 0)
		return (NULL);
	i = 0;
	dup = malloc(sizeof(char) * n + 1);
	if (!dup)
		return (NULL);
	while (str && str[i] && i < n)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}

char	*ft_strcat(char *str, char *s)
{
	int		i;
	int		j;
	char	*cat;

	cat = malloc(sizeof(char) * (ft_strclen(str, 0) + ft_strclen(s, 0) + 1));
	if (!cat)
		return (NULL);
	i = 0;
	j = 0;
	while (str && str[i])
	{
		cat[i] = str[i];
		i++;
	}
	while (s && s[j])
	{
		cat[i + j] = s[j];
		j++;
	}
	cat[i + j] = 0;
	return (cat);
}

int	count_cmd(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args && args[i])
	{
		if (is_flag(args[i]) == 0)
			i++;
		else
		{
			i++;
			count++;
		}
	}
	return (count);
}
