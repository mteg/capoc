//
// Created by mteg on 5/6/18.
//

#ifndef CAPOC2_CLI_H
#define CAPOC2_CLI_H

#ifndef NO_SOCKETS
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <cstdio>

#define EXT_CLI

#ifdef EXT_CLI
#define CLI_ID(obj) (((obj)[1] << 8) | ((obj)[2]))
#define CLI_LINK(obj) ((((obj)[4]) << 16) | ((obj)[5] << 8) | ((obj)[6]))
#define CLI_FIN(obj) (CLI_LINK(obj) ==  0xffffff)
#define CLI_TYPE_OFFSET 3
#define CLI_HLEN_OFFSET 7
typedef unsigned short clitok_id;
#else
#define CLI_ID(obj) ((obj)[1])
#define CLI_LINK(obj) ((((obj)[3]) << 8) | ((obj)[4]))
#define CLI_FIN(obj) (CLI_LINK(obj) == 0xffff)
#define CLI_TYPE_OFFSET 2
#define CLI_HLEN_OFFSET 5
typedef unsigned char clitok_id;
#endif
#define CLI_HSTR_OFFSET (CLI_HLEN_OFFSET+1)

#define CLI_TYPE(obj) ((obj)[CLI_TYPE_OFFSET])
#define CLI_LEN(obj) ((obj)[0])
#define CLI_HELP_LEN(obj) ((obj)[CLI_HLEN_OFFSET])
#define CLI_HELP(obj) ((obj) + CLI_HSTR_OFFSET)
#define CLI_KWORD_LEN(obj) ((obj)[CLI_HSTR_OFFSET + CLI_HELP_LEN(obj)])
#define CLI_KWORD_LEN_A(obj, hlen) ((obj)[CLI_HSTR_OFFSET + hlen])
#define CLI_KWORD(obj) ((obj) + CLI_HSTR_OFFSET + CLI_HELP_LEN(obj) + 1)
#define CLI_KWORD_A(obj, hlen) ((obj) + CLI_HSTR_OFFSET + hlen + 1)
#define CLI_NEXT(obj) ((obj) + (obj)[0])
#define CLI_EOT(obj) (!((obj)[0]))
#define CLI_KWORD_INTVAL_OFFS(obj) (CLI_HSTR_OFFSET + 1 + CLI_HELP_LEN(obj) + CLI_KWORD_LEN(obj))
#define CLI_KWORD_INTVAL(obj) (((obj)[CLI_KWORD_INTVAL_OFFS(obj)] << 8) | ((obj)[CLI_KWORD_INTVAL_OFFS(obj) + 1]))


#define CLI_BUFFER_LEN 2048
#define CLI_MAX_COMPLETION 128
#define CLI_MAX_TOKEN_LEN 1024
#define CLI_MAX_TOKENS 64



struct cliToken
{
    clitok_id id;
    unsigned char flags;
#define CLITOK_LAST 1
#define CLITOK_BARRIER 2
    unsigned char num;
    union
    {
        unsigned int num;
        unsigned char *str;
        char *sstr;
#ifndef NO_SOCKETS
        in_addr_t ip;
#endif
    } data;
};

struct cliContext
{
    char *b;
    char *return_prompt;
};


class cli {

public:
    explicit cli(const char *context);
    void startChat();   /* to be called in constructor of derived class */
    virtual ~cli();

    const char *context;

    unsigned char *buffer;
    unsigned int alloc_len;

    unsigned int pos = 0;
    unsigned int len = 0;

    unsigned int width;
    unsigned int height;

#define CLI_PROMPT_MAX 48
    char promptString[CLI_PROMPT_MAX];

    struct cliToken * token_tab;
    unsigned int token_pos = 0;
    unsigned int max_tokens;

#define CLI_CTX_MAX 8
    struct cliContext ctx[CLI_CTX_MAX];
    unsigned int ctx_pos = 0;

#define CLI_IAC_MAX 32
    unsigned char iac_buffer[CLI_IAC_MAX] = {0};
    unsigned char iac_pos = 0;

    unsigned char iac = 0, iac_sb = 0, esc = 0;

    char **hist_ent = NULL;
    unsigned int hist_count = 0;

    unsigned int hist_last = 0;
    unsigned int hist_browse = 0;

    bool noTerminal = false;

#define CLI_HIST_MAX 8

#define CLI_ESC_NONE 0
#define CLI_ESC_PRE  1
#define CLI_ESC_ESC  2

    virtual void printf(const char *fmt, ...);
    virtual void write(const char *buf, size_t) = 0;
    virtual int complete(clitok_id, char*, int) = 0;
    virtual void exec() = 0;


    void writech(char c);

    void print(const char *str);

    void setPrompt(char *p);

    void prompt();

    static unsigned char *skipBlanks(unsigned char *str);

    int help(const unsigned char *current);

    cliToken *tokenWithin(clitok_id id, cliToken *t, int n);

    cliToken *token(clitok_id id);

    void fence(cliToken *t);

    cliToken *nextToken(clitok_id id, cliToken *t);

    const char *strToken(clitok_id id, const char *def);

    int numToken(clitok_id id, int def);

    int isLastToken(clitok_id id);

    void assert(void *m);

    void enterContext();

    int exitContext();

    void parseAppend(unsigned char *cli_parse_buffer, int *cpos, int *clen, unsigned char *str);

#define CLI_PARSE_HELP 0
#define CLI_PARSE_COMPLETE 1
#define CLI_PARSE_EXEC 2
    unsigned char parse(char mode);

    void input(const unsigned char *buf, unsigned int blen);
    void input(const unsigned char *buf, unsigned int blen, bool treatNlAsCr);

    void parseError(const char *msg);

    void setContext(char *context);

    void telnetInit();

    void sInsert(char c);

    void sInsertOverwrite(char c);

    void sRedraw();

    void sFwd();

    void sRew();

    void sBol();

    void seol();

    void sEol();

    void sFwdWord();

    void sRewWord();

    void sDel();

    void sBackspace();

    void sKline();

    void sDelWord();

    void sBackspaceWord();

    void histAdd();

    void histDisplay();

    void histPrev();

    void histNext();

    void sErase();

    bool boolToken(clitok_id id);

    void execString(const char *line);

    void execStream(FILE *fh);
};

#define TOK_KEYWORD 1
#define TOK_STRING 2
#define TOK_NUM 3
#define TOK_IP 4
#define TOK_WORD 5
#define TOK_CR 6
#define TOK_KEYWORD_INTVAL 7
#define TOK_END 0


#endif //CAPOC2_CLI_H
