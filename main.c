/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: volmer <volmer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:53:39 by volmer            #+#    #+#             */
/*   Updated: 2026/05/10 22:15:48 by volmer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

extern size_t ft_strlen(const char *s);
extern char *ft_strcpy(char *dst, const char *src);
extern int ft_strcmp(const char *s1, const char *s2);
extern ssize_t ft_write(int fd, const void *buf, size_t count);
extern ssize_t ft_read(int fd, void *buf, size_t count);
extern char *ft_strdup(const char *s);

static const char *g_reset = "\033[0m";
static const char *g_green = "\033[32m";
static const char *g_red = "\033[31m";
static const char *g_cyan = "\033[36m";
static const char *g_yellow = "\033[33m";

#define COL_STATUS 4
#define COL_NAME 24
#define COL_TESTED 18
#define COL_ORIGINAL 16
#define COL_LIBASM 16

static int	should_animate(void)
{
    char	*no_anim;

    if (!isatty(STDOUT_FILENO))
        return (0);
    no_anim = getenv("NO_ANIM");
    if (no_anim && no_anim[0] == '1')
        return (0);
    return (1);
}

static void	tiny_animation(void)
{
    const char	frames[] = "|/-\\";
    int			i;

    if (!should_animate())
        return ;
    i = 0;
    while (i < 8)
    {
        printf("\r%s   running %c%s", g_cyan, frames[i % 4], g_reset);
        fflush(stdout);
        usleep(22000);
        i++;
    }
    printf("\r\033[K");
}

static void	print_repeat(char c, int count)
{
    int	i;

    i = 0;
    while (i < count)
    {
        putchar(c);
        i++;
    }
}

static void	print_table_border(void)
{
    printf("+");
    print_repeat('-', COL_STATUS + 2);
    printf("+");
    print_repeat('-', COL_NAME + 2);
    printf("+");
    print_repeat('-', COL_TESTED + 2);
    printf("+");
    print_repeat('-', COL_ORIGINAL + 2);
    printf("+");
    print_repeat('-', COL_LIBASM + 2);
    printf("+\n");
}

static void	print_table_header(const char *title)
{
    printf("\n%s== %s ==%s\n", g_cyan, title, g_reset);
    print_table_border();
    printf("| %-*s | %-*s | %-*s | %-*s | %-*s |\n",
        COL_STATUS, "RES",
        COL_NAME, "TEST",
        COL_TESTED, "TESTED",
        COL_ORIGINAL, "ORIGINAL",
        COL_LIBASM, "LIBASM");
    print_table_border();
}

static int	report_result(const char *label, int ok,
        const char *tested, const char *original, const char *libasm)
{
    const char  *status_text;
    const char  *status_color;

    if (ok)
    {
        status_text = "OK";
        status_color = g_green;
    }
    else
    {
        status_text = "KO";
        status_color = g_red;
    }
    printf("| %s%-*s%s | %-*s | %-*.*s | %s%-*.*s%s | %s%-*.*s%s |\n",
        status_color,
        COL_STATUS,
        status_text,
        g_reset,
        COL_NAME,
        label,
        COL_TESTED,
        COL_TESTED,
        tested,
        g_cyan,
        COL_ORIGINAL,
        COL_ORIGINAL,
        original,
        g_reset,
        g_yellow,
        COL_LIBASM,
        COL_LIBASM,
        libasm,
        g_reset);
    return (ok);
}

static int	test_strlen_case(const char *label, const char *input)
{
    size_t	libc_len;
    size_t	ft_len;
    int		ok;
    char	tested[32];
    char	original[32];
    char	libasm[32];

    tiny_animation();
    libc_len = strlen(input);
    ft_len = ft_strlen(input);
    ok = (libc_len == ft_len);
    snprintf(tested, sizeof(tested), "%s", input);
    snprintf(original, sizeof(original), "%zu", libc_len);
    snprintf(libasm, sizeof(libasm), "%zu", ft_len);
    return (report_result(label, ok, tested, original, libasm));
}

static int	check_guard(const unsigned char *mem, size_t guard_size, size_t dst_size)
{
    size_t	i;

    i = 0;
    while (i < guard_size)
    {
        if (mem[i] != 0xA5)
            return (0);
        if (mem[guard_size + dst_size + i] != 0xA5)
            return (0);
        i++;
    }
    return (1);
}

static int	test_strcpy_case(const char *label, const char *src)
{
    enum { GUARD_SIZE = 16, DST_SIZE = 128 };
    unsigned char	mem_ft[GUARD_SIZE + DST_SIZE + GUARD_SIZE];
    unsigned char	mem_libc[GUARD_SIZE + DST_SIZE + GUARD_SIZE];
    char			*dst_ft;
    char			*dst_libc;
    char			*ret_ft;
    char			*ret_libc;
    char			tested[32];
    int			ok;

    tiny_animation();
    memset(mem_ft, 0xA5, sizeof(mem_ft));
    memset(mem_libc, 0xA5, sizeof(mem_libc));
    dst_ft = (char *)(mem_ft + GUARD_SIZE);
    dst_libc = (char *)(mem_libc + GUARD_SIZE);
    ret_ft = ft_strcpy(dst_ft, src);
    ret_libc = strcpy(dst_libc, src);
    ok = (strcmp(dst_ft, dst_libc) == 0);
    ok = ok && (strcmp(dst_ft, src) == 0);
    ok = ok && (ret_ft == dst_ft);
    ok = ok && (ret_libc == dst_libc);
    ok = ok && check_guard(mem_ft, GUARD_SIZE, DST_SIZE);
    ok = ok && check_guard(mem_libc, GUARD_SIZE, DST_SIZE);
    snprintf(tested, sizeof(tested), "%s", src);
    return (report_result(label, ok, tested, dst_libc, dst_ft));
}

static int	test_strcmp_case(const char *label, const char *s1, const char *s2)
{
    int		libc_cmp;
    int		ft_cmp;
    int		ok;
    char	tested[32];
    char	original[32];
    char	libasm[32];

    tiny_animation();
    libc_cmp = strcmp(s1, s2);
    ft_cmp = ft_strcmp(s1, s2);
    ok = (libc_cmp == ft_cmp);
    snprintf(tested, sizeof(tested), "%s,%s", s1, s2);
    snprintf(original, sizeof(original), "%d", libc_cmp);
    snprintf(libasm, sizeof(libasm), "%d", ft_cmp);
    return (report_result(label, ok, tested, original, libasm));
}

static int	test_write_pipe_case(const char *label, const char *buf, size_t count)
{
    int		pipe_libc[2];
    int		pipe_ft[2];
    ssize_t	libc_ret;
    ssize_t	ft_ret;
    int		libc_errno;
    int		ft_errno;
    char		read_libc[256];
    char		read_ft[256];
    ssize_t	read_libc_ret;
    ssize_t	read_ft_ret;
    int		ok;
    char	tested[32];
    char	original[32];
    char	libasm[32];

    tiny_animation();
    if (pipe(pipe_libc) == -1 || pipe(pipe_ft) == -1)
        return (report_result(label, 0, "pipe", "pipe err", "pipe err"));
    errno = 0;
    libc_ret = write(pipe_libc[1], buf, count);
    libc_errno = errno;
    errno = 0;
    ft_ret = ft_write(pipe_ft[1], buf, count);
    ft_errno = errno;
    read_libc_ret = 0;
    read_ft_ret = 0;
    if (libc_ret > 0)
        read_libc_ret = read(pipe_libc[0], read_libc, sizeof(read_libc));
    if (ft_ret > 0)
        read_ft_ret = read(pipe_ft[0], read_ft, sizeof(read_ft));
    close(pipe_libc[0]);
    close(pipe_libc[1]);
    close(pipe_ft[0]);
    close(pipe_ft[1]);
    ok = (libc_ret == ft_ret);
    ok = ok && (libc_errno == ft_errno);
    ok = ok && (read_libc_ret == read_ft_ret);
    if (read_libc_ret > 0 && read_ft_ret > 0)
        ok = ok && (memcmp(read_libc, read_ft, (size_t)read_libc_ret) == 0);
    snprintf(tested, sizeof(tested), "n=%zu", count);
    snprintf(original, sizeof(original), "%zd/e%d", libc_ret, libc_errno);
    snprintf(libasm, sizeof(libasm), "%zd/e%d", ft_ret, ft_errno);
    return (report_result(label, ok, tested, original, libasm));
}

static int	test_write_bad_fd_case(const char *label)
{
    const char	*msg;
    ssize_t		libc_ret;
    ssize_t		ft_ret;
    int			libc_errno;
    int			ft_errno;
    int			ok;
    char		original[32];
    char		libasm[32];

    tiny_animation();
    msg = "X";
    errno = 0;
    libc_ret = write(-1, msg, 1);
    libc_errno = errno;
    errno = 0;
    ft_ret = ft_write(-1, msg, 1);
    ft_errno = errno;
    ok = (libc_ret == ft_ret);
    ok = ok && (libc_errno == ft_errno);
    snprintf(original, sizeof(original), "%zd/e%d", libc_ret, libc_errno);
    snprintf(libasm, sizeof(libasm), "%zd/e%d", ft_ret, ft_errno);
    return (report_result(label, ok, "fd=-1,n=1", original, libasm));
}

static int	test_read_pipe_case(const char *label, const char *input)
{
    int		pipe_libc[2];
    int		pipe_ft[2];
    ssize_t	libc_ret;
    ssize_t	ft_ret;
    int		libc_errno;
    int		ft_errno;
    char	buf_libc[256];
    char	buf_ft[256];
    int		ok;
    char	tested[32];
    char	original[32];
    char	libasm[32];
    size_t	input_len;

    tiny_animation();
    input_len = strlen(input);
    if (pipe(pipe_libc) == -1 || pipe(pipe_ft) == -1)
        return (report_result(label, 0, "pipe", "pipe err", "pipe err"));
    write(pipe_libc[1], input, input_len);
    write(pipe_ft[1], input, input_len);
    memset(buf_libc, 0, sizeof(buf_libc));
    memset(buf_ft, 0, sizeof(buf_ft));
    errno = 0;
    libc_ret = read(pipe_libc[0], buf_libc, input_len);
    libc_errno = errno;
    errno = 0;
    ft_ret = ft_read(pipe_ft[0], buf_ft, input_len);
    ft_errno = errno;
    close(pipe_libc[0]);
    close(pipe_libc[1]);
    close(pipe_ft[0]);
    close(pipe_ft[1]);
    ok = (libc_ret == ft_ret);
    ok = ok && (libc_errno == ft_errno);
    if (libc_ret > 0)
        ok = ok && (memcmp(buf_libc, buf_ft, (size_t)libc_ret) == 0);
    snprintf(tested, sizeof(tested), "n=%zu", input_len);
    snprintf(original, sizeof(original), "%zd/e%d", libc_ret, libc_errno);
    snprintf(libasm, sizeof(libasm), "%zd/e%d", ft_ret, ft_errno);
    return (report_result(label, ok, tested, original, libasm));
}

static int	test_read_bad_fd_case(const char *label)
{
    char	buf_libc[10];
    char	buf_ft[10];
    ssize_t	libc_ret;
    ssize_t	ft_ret;
    int		libc_errno;
    int		ft_errno;
    int		ok;
    char	original[32];
    char	libasm[32];

    tiny_animation();
    errno = 0;
    libc_ret = read(-1, buf_libc, 1);
    libc_errno = errno;
    errno = 0;
    ft_ret = ft_read(-1, buf_ft, 1);
    ft_errno = errno;
    ok = (libc_ret == ft_ret);
    ok = ok && (libc_errno == ft_errno);
    snprintf(original, sizeof(original), "%zd/e%d", libc_ret, libc_errno);
    snprintf(libasm, sizeof(libasm), "%zd/e%d", ft_ret, ft_errno);
    return (report_result(label, ok, "fd=-1,n=1", original, libasm));
}

static int	test_strdup_case(const char *label, const char *src)
{
    char	*ret_libc;
    char	*ret_ft;
    int		ok;
    char	tested[32];
    char	original[32];
    char	libasm[32];

    tiny_animation();
    ret_libc = strdup(src);
    ret_ft = ft_strdup(src);
    
    ok = 1;
    if (ret_libc == NULL || ret_ft == NULL)
    {
        if (ret_libc != ret_ft)
            ok = 0;
    }
    else
    {
        ok = (strcmp(ret_libc, ret_ft) == 0);
    }
    
    snprintf(tested, sizeof(tested), "%s", src);
    snprintf(original, sizeof(original), "%s", ret_libc ? ret_libc : "NULL");
    snprintf(libasm, sizeof(libasm), "%s", ret_ft ? ret_ft : "NULL");
    
    if (ret_libc)
        free(ret_libc);
    if (ret_ft)
        free(ret_ft);
        
    return (report_result(label, ok, tested, original, libasm));
}

int	main(void)
{
    int	passed;
    int	total;

    passed = 0;
    total = 0;
    print_table_header("ft_strlen");
    passed += test_strlen_case("strlen empty", "");
    total++;
    passed += test_strlen_case("strlen one char", "a");
    total++;
    passed += test_strlen_case("strlen normal", "Hola 42");
    total++;
    passed += test_strlen_case("strlen symbols", "!@#$%^&*()_+");
    total++;
    passed += test_strlen_case("strlen spaces", "   42   ");
    total++;
    passed += test_strlen_case("strlen mixed", "line\twith\ncontrols");
    total++;
    print_table_border();
    print_table_header("ft_strcpy");
    passed += test_strcpy_case("strcpy empty", "");
    total++;
    passed += test_strcpy_case("strcpy one char", "Z");
    total++;
    passed += test_strcpy_case("strcpy normal", "Assembler 42");
    total++;
    passed += test_strcpy_case("strcpy symbols", "!@#$%^&*()_+");
    total++;
    passed += test_strcpy_case("strcpy spaces", "   hello 42   ");
    total++;
    passed += test_strcpy_case("strcpy alphabet", "abcdefghijklmnopqrstuvwxyz");
    total++;
    print_table_border();
    print_table_header("ft_strcmp");
    passed += test_strcmp_case("strcmp equal", "42", "42");
    total++;
    passed += test_strcmp_case("strcmp diff char", "abc", "abd");
    total++;
    passed += test_strcmp_case("strcmp prefix", "abc", "abcd");
    total++;
    passed += test_strcmp_case("strcmp reversed", "abcd", "abc");
    total++;
    passed += test_strcmp_case("strcmp empty", "", "");
    total++;
    passed += test_strcmp_case("strcmp symbols", "!@#", "!@$");
    total++;
    print_table_border();
    print_table_header("ft_write");
    passed += test_write_pipe_case("write pipe text", "Hola42", 6);
    total++;
    passed += test_write_pipe_case("write pipe empty", "", 0);
    total++;
    passed += test_write_pipe_case("write null count0", NULL, 0);
    total++;
    passed += test_write_bad_fd_case("write bad fd");
    total++;
    print_table_border();
    print_table_header("ft_read");
    passed += test_read_pipe_case("read pipe text", "Hola42");
    total++;
    passed += test_read_pipe_case("read pipe empty", "");
    total++;
    passed += test_read_bad_fd_case("read bad fd");
    total++;
    print_table_border();
    print_table_header("ft_strdup");
    passed += test_strdup_case("strdup normal", "Hola 42");
    total++;
    passed += test_strdup_case("strdup empty", "");
    total++;
    passed += test_strdup_case("strdup long", "This is a much longer string testing allocation");
    total++;
    print_table_border();
    if (passed == total)
        printf("\n%sSummary: %d/%d tests OK%s\n", g_green, passed, total, g_reset);
    else
        printf("\n%sSummary: %d/%d tests OK%s\n", g_red, passed, total, g_reset);
    if (passed != total)
        return (1);
    return (0);
}