#include "get_next_line.h"

static size_t  ft_strlen(char *str)
{
    size_t  len;

    if (!str)
        return (0);
    len = 0;
    while (str[len])
        len++;
    return (len);
}

static void    *ft_memset(void *buff, char c, size_t n)
{
    char    *buff_p;
    size_t  idx;

    buff_p = buff;
    idx = 0;
    while (idx < n)
    {
        buff_p[idx] = (unsigned char)c;
        idx++;
    }
    return (buff);
}

static void    *ft_memcpy(void *dest, void *src, size_t n)
{
    char    *dest_p;
    char    *src_p;
    size_t  idx;

    dest_p = dest;
    src_p = src;
    idx = 0;
    while (idx < n)
    {
        dest_p[idx] = src_p[idx]; 
        idx++;
    }
    return (dest);
}

static size_t  ft_strlen_del(char *str, char del)
{
    size_t len = 0;
    if (!str)
        return (0);
    while (str[len] && str[len] != del)
        len++;
    if (str[len] == '\n')
        len++;
    return (len);
}

static char    *ft_strjoin(char *s1, char *s2)
{
    char    *ret;
    size_t  s1_len;
    size_t  s2_len;

    s1_len = ft_strlen(s1);
    s2_len = ft_strlen_del(s2, '\n');
    ret = malloc(sizeof(char) * (s1_len + s2_len + 1));
    if (!ret)
        return (NULL);
    ft_memcpy(ret, s1, s1_len);
    ft_memcpy(ret + s1_len, s2, s2_len);
    ret[s1_len + s2_len] = '\0';
    if (s1)
        free(s1);
    return (ret);
}

static char    *ft_strchr(char *str, char c)
{
    size_t  idx;

    idx = 0;
    while (str[idx])
    {
        if (str[idx] == c)
            return (&str[idx]);
        idx++;
    }
    if (c == '\0')
        return (&str[idx]);
    return (NULL);
}

static char    *ft_read_line(char *buffer, int fd)
{
    char    *new_line;
    char    *line;
    ssize_t b_read;

    new_line = NULL;
    line = NULL;
    b_read = 1;
    while (!new_line)
    {
        if (!*buffer)
            b_read = read(fd, buffer, BUFFER_SIZE);
        if (b_read == -1)
        {
            free(line);
            return (NULL);
        }
        if ((0 == b_read && !*buffer))
            return (line);
        line = ft_strjoin(line, buffer);
        if (!line)
            return (NULL);
        new_line = ft_strchr(buffer, '\n');
        if (!new_line)
            ft_memset(buffer, '\0', BUFFER_SIZE + 1);
    }
    new_line++;
    ft_memcpy(buffer, new_line, ft_strlen(new_line));
    ft_memset(buffer + ft_strlen(new_line) , '\0', BUFFER_SIZE - ft_strlen(new_line));
    return (line);
}

char	*get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE + 1];
    
    if (BUFFER_SIZE <= 0 || fd  < 0)
        return (NULL);
    return (ft_read_line(buffer, fd));
}
