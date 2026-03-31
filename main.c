/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: volmer <volmer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:53:39 by volmer            #+#    #+#             */
/*   Updated: 2026/03/31 17:10:33 by volmer           ###   ########.fr       */
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

static int	report_result(const char *label, int ok, const char *detail)
{
    if (ok)
        printf("%s[OK]%s %s %s(%s)%s\n", g_green, g_reset, label, g_yellow, detail, g_reset);
    else
        printf("%s[KO]%s %s %s(%s)%s\n", g_red, g_reset, label, g_yellow, detail, g_reset);
    return (ok);
}

static int	test_strlen_case(const char *label, const char *input)
{
    size_t	libc_len;
    size_t	ft_len;
    int		ok;
    char	detail[160];

    tiny_animation();
    libc_len = strlen(input);
    ft_len = ft_strlen(input);
    ok = (libc_len == ft_len);
    snprintf(detail, sizeof(detail), "original=>%zu, libasm=>%zu", libc_len, ft_len);
    return (report_result(label, ok, detail));
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
    char			detail[320];

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
    snprintf(detail, sizeof(detail), "original=>\"%s\", libasm=>\"%s\"", dst_libc, dst_ft);
    return (report_result(label, ok, detail));
}

int	main(void)
{
    int	passed;
    int	total;

    passed = 0;
    total = 0;
    printf("%s== ft_strlen ==%s\n", g_cyan, g_reset);
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
    printf("\n%s== ft_strcpy ==%s\n", g_cyan, g_reset);
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
    if (passed == total)
        printf("\n%sSummary: %d/%d tests OK%s\n", g_green, passed, total, g_reset);
    else
        printf("\n%sSummary: %d/%d tests OK%s\n", g_red, passed, total, g_reset);
    if (passed != total)
        return (1);
    return (0);
}