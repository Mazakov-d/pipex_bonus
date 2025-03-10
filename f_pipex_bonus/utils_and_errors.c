/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_and_errors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 00:15:22 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 22:41:52 by dorianmazar      ###   ########.fr       */
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
	cat[i + j] = '\0';
	return (cat);
}

void	print_error(char *msg)
{
	if (msg)
		write(STDERR_FILENO, msg, ft_strlen(msg));
}

int	error_return(char *msg, int ret_val)
{
	print_error(msg);
	print_error("\n");
	return (ret_val);
}

void	*error_malloc(void)
{
	print_error("Error: malloc failed\n");
	return (NULL);
}