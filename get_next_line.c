/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 14:13:36 by dorianmazar       #+#    #+#             */
/*   Updated: 2024/12/09 13:25:58 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_free(char **ptr)
{
	if (ptr && *ptr)
		free(*ptr);
	*ptr = NULL;
	return (NULL);
}

char	*ft_strdup(char *str)
{
	char	*result;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = 0;
	return (result);
}

char	*ft_split_save(char **save)
{
	char	*line;
	char	*ptr;
	int		i;

	if (!(*save))
		return (NULL);
	i = ft_is_n(*save);
	if (i == -1 || i + 1 == ft_strlen(*save))
	{
		line = *save;
		*save = NULL;
		return (line);
	}
	line = ft_strndup(*save, i);
	if (!line)
		return (NULL);
	ptr = ft_strdup(*save + i + 1);
	free(*save);
	*save = ptr;
	if (!(*save))
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*ft_read(int fd, char *save)
{
	char	*buff;
	char	*ptr;
	int		rd;

	rd = 1;
	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (ft_free(&save));
	while (rd > 0 && ft_is_n(save) == -1)
	{
		rd = read(fd, buff, BUFFER_SIZE);
		if (rd == -1)
			return (free(buff), ft_free(&save));
		if (rd == 0)
			break ;
		buff[rd] = 0;
		ptr = save;
		save = ft_strjoin(save, buff);
		free(ptr);
		if (!save)
			return (free(buff), NULL);
	}
	free(buff);
	return (save);
}

char	*get_next_line(int fd)
{
	static char	*save;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_free(&save));
	if (!save || (save && ft_is_n(save) == -1))
		save = ft_read(fd, save);
	if (!save)
		return (NULL);
	if (save[0] == 0)
		return (ft_free(&save));
	line = ft_split_save(&save);
	if (!line)
		return (ft_free(&save));
	return (line);
}
