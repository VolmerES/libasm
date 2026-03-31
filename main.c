/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: volmer <volmer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:53:39 by volmer            #+#    #+#             */
/*   Updated: 2026/03/31 17:23:13 by volmer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>

extern size_t ft_strlen(const char *s);
extern char *ft_strcpy(char *dst, const char *src);

static const char *g_reset = "\033[0m";
static const char *g_green = "\033[32m";
static const char *g_red = "\033[31m";
static const char *g_cyan = "\033[36m";
static const char *g_yellow = "\033[33m";

#define COL_STATUS 4
#define COL_NAME 24
#define COL_ORIGINAL 20
#define COL_LIBASM 20

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
    print_repeat('-', COL_ORIGINAL + 2);
    printf("+");
    print_repeat('-', COL_LIBASM + 2);
    printf("+\n");
}

static void	print_table_header(const char *title)
{
    printf("\n%s== %s ==%s\n", g_cyan, title, g_reset);
    print_table_border();
    printf("| %-*s | %-*s | %-*s | %-*s |\n",
        COL_STATUS, "RES",
        COL_NAME, "TEST",
        COL_ORIGINAL, "ORIGINAL",
        COL_LIBASM, "LIBASM");
    print_table_border();
}

static int	report_result(const char *label, int ok,
        const char *original, const char *libasm)
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
    printf("| %s%-*s%s | %-*s | %s%-*.*s%s | %s%-*.*s%s |\n",
        status_color,
        COL_STATUS,
        status_text,
        g_reset,
        COL_NAME,
        label,
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
    char	original[32];
    char	libasm[32];

    tiny_animation();
    libc_len = strlen(input);
    ft_len = ft_strlen(input);
    ok = (libc_len == ft_len);
    snprintf(original, sizeof(original), "%zu", libc_len);
    snprintf(libasm, sizeof(libasm), "%zu", ft_len);
    return (report_result(label, ok, original, libasm));
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
    return (report_result(label, ok, dst_libc, dst_ft));
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
    if (passed == total)
        printf("\n%sSummary: %d/%d tests OK%s\n", g_green, passed, total, g_reset);
    else
        printf("\n%sSummary: %d/%d tests OK%s\n", g_red, passed, total, g_reset);
    if (passed != total)
        return (1);
    return (0);
}