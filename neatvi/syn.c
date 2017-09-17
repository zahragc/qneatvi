#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vi.h"

#define NFTS		16

/* mapping filetypes to regular expression sets */
static struct ftmap {
    char ft[32];
    struct rset *rs;
} ftmap[NFTS];

static struct rset *syn_ftrs;

static struct rset *syn_find(char *ft)
{
    int i;
    for (i = 0; i < LEN(ftmap); i++)
        if (!strcmp(ft, ftmap[i].ft))
            return ftmap[i].rs;
    return NULL;
}

int syn_merge(int old, int new1)
{
    int fg = SYN_FGSET(new1) ? SYN_FG(new1) : SYN_FG(old);
    int bg = SYN_BGSET(new1) ? SYN_BG(new1) : SYN_BG(old);
    return ((old | new1) & SYN_FLG) | (bg << 8) | fg;
}

// file type highlight
int *syn_highlight(char *ft, char *s)
{
    int subs[16 * 2];
    int n = uc_slen(s);
    int *att = malloc(n * sizeof(att[0]));
    int sidx = 0;
    struct rset *rs = syn_find(ft);
    int flg = 0;
    int hl, j, i;
    memset(att, 0, n * sizeof(att[0]));
    if (!rs)
        return att;
    while ((hl = rset_find(rs, s + sidx, LEN(subs) / 2, subs, flg)) >= 0) {
        int grp = 0;
        int cend = 1;
        int *catt;
        conf_highlight(hl, NULL, &catt, NULL, &grp);
        for (i = 0; i < LEN(subs) / 2; i++) {
            if (subs[i * 2] >= 0) {
                int beg = uc_off(s, sidx + subs[i * 2 + 0]);
                int end = uc_off(s, sidx + subs[i * 2 + 1]);
                for (j = beg; j < end; j++)
                    att[j] = syn_merge(att[j], catt[i]);
                if (i == grp)
                    cend = MAX(cend, subs[i * 2 + 1]);
            }
        }
        sidx += cend;
        flg = RE_NOTBOL;
    }
    return att;
}

static void syn_initft(char *name)
{
    char *pats[128] = {NULL};
    char *ft, *pat;
    int i, n;
    for (i = 0; !conf_highlight(i, &ft, NULL, &pat, NULL) && i < LEN(pats); i++)
        if (!strcmp(ft, name))
            pats[i] = pat;
    n = i;
    for (i = 0; i < LEN(ftmap); i++) {
        if (!ftmap[i].ft[0]) {
            strcpy(ftmap[i].ft, name);
            ftmap[i].rs = rset_make(n, pats, 0);
            return;
        }
    }
}

char *syn_filetype(char *path)
{
    int hl = rset_find(syn_ftrs, path, 0, NULL, 0);
    char *ft;
    if (!conf_filetype(hl, &ft, NULL))
        return ft;
    return "";
}

void syn_init(void)
{
    char *pats[128] = {NULL};
    char *pat, *ft;
    int i;
    for (i = 0; !conf_highlight(i, &ft, NULL, NULL, NULL); i++)
        if (!syn_find(ft))
            syn_initft(ft);
    for (i = 0; !conf_filetype(i, NULL, &pat) && i < LEN(pats); i++)
        pats[i] = pat;
    syn_ftrs = rset_make(i, pats, 0);
}

void syn_done(void)
{
    int i;
    for (i = 0; i < LEN(ftmap); i++)
        if (ftmap[i].rs)
            rset_free(ftmap[i].rs);
    rset_free(syn_ftrs);
}
