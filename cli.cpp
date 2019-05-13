/*
 *
 * Copyright (c) 2009 - 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "cli.h"
#define CTRL(x) ((x) - '@')

#ifndef WINDOWS
#define CLI_TELNET_SUPPORT
#endif
#ifdef CLI_TELNET_SUPPORT
#include <arpa/telnet.h>

#endif

void cli::printf(const char *fmt, ...)
{
    va_list ap;
#define CLI_MSG_BUFFER_SIZE	2048
    static char buffer[CLI_MSG_BUFFER_SIZE];

    va_start(ap, fmt);
    vsnprintf(buffer, CLI_MSG_BUFFER_SIZE - 1, fmt, ap);
    this->write(buffer, strlen(buffer));

    va_end(ap);
}

void cli::print(const char *str)
{
    this->write(str, strlen(str));
}

void cli::writech(char c)
{
    char str[2] = {c, 0};
    this->write(str, 1);
}

void cli::prompt()
{
    this->print(this->promptString);
}

void cli::setPrompt(char *p)
{
    snprintf(this->promptString, CLI_PROMPT_MAX, "%s", p);
}

unsigned char * cli::skipBlanks(unsigned char *str)
{
    while(*str == ' ') str++;
    return str;
}

int cli::help(const unsigned char *current)
{
    this->print( "\r\n");
    while(!CLI_EOT(current))
    {
        unsigned char type = CLI_TYPE(current);
        unsigned char hlen = CLI_HELP_LEN(current);
        unsigned char klen;
        const unsigned char *str;
        const unsigned char *hstr = CLI_HELP(current);
        unsigned char i;

        klen = CLI_KWORD_LEN_A(current, hlen);
        str = CLI_KWORD_A(current, hlen);

        if(type != TOK_KEYWORD && type != TOK_KEYWORD_INTVAL)
        {
            str = (const unsigned char*) "<?>";
            if(type == TOK_STRING) str = (const unsigned char*) "<string>";
            if(type == TOK_WORD)  str = (const unsigned char*) "<word>";
            if(type == TOK_NUM) str = (const unsigned char*) "<number>";
            if(type == TOK_IP) str = (const unsigned char*) "<ip>";
            if(type == TOK_CR) str = (const unsigned char*) "<CR>";
            klen = (unsigned char) strlen((char*) str);
        }

        /* Watch out for hidden keywords */
        if(*str != '$')
        {

            this->print( "  ");
            for(i = 0; i<klen; i++)
                this->writech(str[i]);

            for(; i<16; i++)
                this->writech(' ');

            this->writech(' ');
            for(i = 0; i<hlen; i++)
                this->writech(hstr[i]);

            this->print( "\r\n");
        }
        current = CLI_NEXT(current);
    }
#ifndef CLI_OWN_PROMPTS
    this->prompt();

    if(noTerminal)
        this->buffer[this->pos = this->len = 0] = 0;
    else {
        this->buffer[this->len] = 0;
        this->print((char *) this->buffer);
    }
#endif
    return 1;

}



struct cliToken * cli::tokenWithin(clitok_id id, struct cliToken *t, int n)
{
    unsigned char i = 0;
    if(t) i = (unsigned char) (t->num + 1);
    for( ; i<this->token_pos; i++)
    {
        if(!n) return(NULL);
        n--;
        if(this->token_tab[i].id == id)
            return(&this->token_tab[i]);
        if(this->token_tab[i].flags & CLITOK_BARRIER)
            return(NULL);
    }
    return(NULL);
}

struct cliToken * cli::token(clitok_id id)
{
    return this->nextToken(id, NULL);
}

void cli::fence(struct cliToken *t)
{
    t->flags |= CLITOK_BARRIER;
}

struct cliToken * cli::nextToken(clitok_id id, struct cliToken *t)
{
    return this->tokenWithin(id, t, CLI_MAX_TOKENS);
}

const char * cli::strToken(clitok_id id, const char *def)
{
    struct cliToken * t = this->token(id);
    if(!t) return def;
    return ((char*) t->data.str);
}

bool cli::boolToken(clitok_id id)
{
    return this->token(id) ? true : false;
}

int cli::numToken(clitok_id id, int def)
{
    struct cliToken * t = this->token(id);
    if(!t) return def;
    return t->data.num;
}

int cli::isLastToken(clitok_id id)
{
    struct cliToken * t = this->token(id);
    if(!t) return 0;
    return (t->flags & CLITOK_LAST);
}

void cli::assert(void *m)
{
    if(!m) abort();
}

void cli::enterContext()
{
    if(this->ctx_pos >= CLI_CTX_MAX) return;
    this->assert((this->ctx[this->ctx_pos].b = strdup((char*) this->buffer)));
    this->assert((this->ctx[this->ctx_pos].return_prompt = strdup((char*) this->promptString)));
    this->ctx_pos++;
}

int cli::exitContext()
{
    if(!this->ctx_pos) return 0;
    this->ctx_pos--;
    free(this->ctx[this->ctx_pos].b);
    snprintf(this->promptString, CLI_PROMPT_MAX, "%s", this->ctx[this->ctx_pos].return_prompt);
    free(this->ctx[this->ctx_pos].return_prompt);
    return 1;
}

void cli::parseAppend(unsigned char *cli_parse_buffer, int *cpos, int *clen, unsigned char *str)
{
//	snprintf((char*) cli_parse_buffer, CLI_BUFFER_LEN, "%s %s", cli_parse_buffer, str);
    unsigned int l = (int) strlen((char*) str);
    if(((int) l) >= (int) ((*clen)-1)) l = (unsigned int) (*clen - 1);
    if(l <= 0) return;

    memcpy(cli_parse_buffer + *cpos, str, (size_t) l);
    cli_parse_buffer[(*cpos) + l] = ' ';
    *cpos += l + 1;
    *clen -= l + 1;
    cli_parse_buffer[*cpos] = 0;
}

unsigned char cli::parse(char mode)
{
    unsigned char cli_parse_buffer[CLI_BUFFER_LEN + CLI_MAX_COMPLETION];
    unsigned char *str;
    const unsigned char *tab = (unsigned char*) this->context;
    unsigned int l_tok = 0;
    const unsigned char *current;
    unsigned int i;
    int cpos = 0, clen = CLI_BUFFER_LEN - 1;
    const unsigned char * match = NULL;

    this->token_pos = 0;
    cli_parse_buffer[cpos = 0] = 0;


    for(i = 0; i<this->ctx_pos; i++)
        this->parseAppend(cli_parse_buffer, &cpos, &clen, (unsigned char*) this->ctx[i].b);

    this->parseAppend(cli_parse_buffer, &cpos, &clen, this->buffer);
    cli_parse_buffer[--cpos] = 0;
    str = cli_parse_buffer;


    while(true)
    {
        unsigned char multiple_match = 0;

        if(this->token_pos >= CLI_MAX_TOKENS)
            break;
        str = this->skipBlanks(str);
//		printf("::nextTok: %s\n", str);
        if(!*str)		/* End of parsing here! */
            break;

        /* How long is this token? */
        l_tok = 0;
        while(str[l_tok] && str[l_tok] != ' ') l_tok++;

        match = NULL;
        /* Loop through our table */
        current = tab;
        while(!CLI_EOT(current))
        {
            /* Is this a keyword? */
            if(CLI_TYPE(current) == TOK_KEYWORD || CLI_TYPE(current) == TOK_KEYWORD_INTVAL)
            {
                /* Does this match? */
                unsigned char k_len = CLI_KWORD_LEN(current);
//				printf("testing: %s %s\n", current + 7 + current[5], str);
                if(l_tok <= k_len && CLI_KWORD(current)[0] != '$') /* Chances for ANY match? */
                {
//					printf("any match chance\n");
                    if(!strncmp((char*) str, (char*) CLI_KWORD(current), l_tok))	/* We have match */
                    {
                        if(match)
                            multiple_match = 1;
                        else
                        {
                            if(CLI_TYPE(current) == TOK_KEYWORD_INTVAL)
                                this->token_tab[this->token_pos].data.num = CLI_KWORD_INTVAL(current);

                            this->token_tab[this->token_pos++].id = CLI_ID(current);
                            match = current;
                        }
                    }
                }
            }
            if(CLI_TYPE(current) == TOK_STRING)
            {
                if(!match)
                {
                    match = current;
                    this->token_tab[this->token_pos  ].id = CLI_ID(current);
                    this->token_tab[this->token_pos++].data.str = str;
                }
            }

            if(CLI_TYPE(current) == TOK_WORD)
            {
                if(!match)
                {
                    match = current;
                    this->token_tab[this->token_pos  ].id = CLI_ID(current);
                    this->token_tab[this->token_pos++].data.str = str;

                    if(str[l_tok] == ' ')
                    {
                        str[l_tok] = 0;
                        l_tok++;
                    }
                }
            }

            if(CLI_TYPE(current) == TOK_NUM)
            {
                unsigned char *estr;
                unsigned long int num;
                unsigned char slen;

                num = strtoul((char*) str, (char**) &estr, 0);
                slen = (unsigned char) (estr - str);
                if(slen == l_tok)
                {
                    if(!match)
                    {
                        match = current;
                        this->token_tab[this->token_pos  ].id = CLI_ID(current);
                        this->token_tab[this->token_pos++].data.num = (unsigned int) num;
                    }
                }
            }

#ifndef NO_SOCKETS
            if(CLI_TYPE(current) == TOK_IP)
            {
                in_addr_t ip;

                ip = inet_addr((char*) str);
                if(!match)
                {
                    match = current;
                    this->token_tab[this->token_pos  ].id = CLI_ID(current);
                    this->token_tab[this->token_pos].data.ip = ip;
                    this->token_pos++;
                }
            }
#endif

            /* Try next one! */
            current = CLI_NEXT(current);
        }

        if(multiple_match)
        {
            if(mode == CLI_PARSE_COMPLETE || mode == CLI_PARSE_HELP)
            {
                this->help(tab);
                return(1);
            }
            else
                return(0);
        }

        if(match)
        {
            if(CLI_FIN(match))
            {
                if(mode == CLI_PARSE_HELP) return(1);
                break;
            }

            tab = CLI_LINK(match) + (unsigned char*) this->context;
            str += l_tok;
//			printf("Going to %d with %s\n", match, str);
            continue;
        }

        /* Cannot match... */
        return(0);
    }
    if(mode == CLI_PARSE_HELP)
        return (unsigned char) this->help(tab);

    if(mode == CLI_PARSE_COMPLETE)
    {
        if(match && this->pos)
        {
//			printf("testing\n");
            /* If the buffer does not have any spaces... */
            if(this->buffer[this->pos - 1] != ' ' && (CLI_TYPE(match) == TOK_STRING || CLI_TYPE(match) == TOK_WORD))
            {
                char *str = (char*) this->token_tab[this->token_pos - 1].data.str;
                if(this->complete(CLI_ID(match), str, CLI_MAX_COMPLETION))
                {
                    int i;
                    for(i = l_tok; str[i]; i++)
                    {
                        if(!str[i + 1])
                            if(str[i] == ' ' && this->buffer[this->pos] == ' ') break;
                        this->sInsert(str[i]);
                    }
                }
            }

            if(this->buffer[this->pos - 1] != ' ' && (CLI_TYPE(match) == TOK_KEYWORD || CLI_TYPE(match) == TOK_KEYWORD_INTVAL))
            {
                unsigned char klen;
                const unsigned char *str;
                unsigned char i;

                klen = CLI_KWORD_LEN(match);
                str = CLI_KWORD(match);

//				printf("conditions ok klen = %d l_tok = %d\n", klen, l_tok);

                for(i = (unsigned char) l_tok; i<klen; i++)
                    this->sInsert(str[i]);

                if(this->buffer[this->pos] != ' ')
                    this->sInsert(' ');
            }
        }
    }
    if(mode == CLI_PARSE_EXEC)
    {
        unsigned int i;
        for(i = 0; i<this->token_pos; i++)
        {
            this->token_tab[i].flags = 0;
            this->token_tab[i].num = (unsigned char) i;
        }

        if(i) this->token_tab[i - 1].flags = CLITOK_LAST;

        this->exec();
        return(1);
    }
    return(0);
}


void cli::input(const unsigned char *buf, unsigned int blen)
{
    this->input(buf, blen, false);
}

void cli::input(const unsigned char *buf, unsigned int blen, bool treatNlAsCr)
{
    unsigned int pos;
    for(pos = 0; pos<blen; pos++)
    {
#ifdef CLI_TELNET_SUPPORT
        if(buf[pos] == IAC)
        {
            this->iac = 1;
            continue;
        }

        if(this->iac)
        {
            switch(buf[pos])
            {
                case SB:
                    this->iac_sb = 1;
                    this->iac_pos = 0;
                    this->iac = 0;
                    break;
                case SE:
                    this->iac_sb = 0;
                    if(this->iac_pos < 5) break;
                    this->width = this->iac_buffer[2];
                    this->height = this->iac_buffer[4];
                    this->iac = 0;
                    break;
                default:
                    this->iac = 0;
                    break;
            }
            continue;
        }

        if(this->iac_sb)
        {
            if(this->iac_pos < CLI_IAC_MAX)
                this->iac_buffer[this->iac_pos++] = buf[pos];
            continue;
        }

        if(this->esc == CLI_ESC_ESC)
        {
            this->esc = CLI_ESC_NONE;
            switch(buf[pos])
            {
                case 'A': this->histPrev(); break;
                case 'B': this->histNext(); break;
                case 'C': this->sFwd(); break;
                case 'D': this->sRew(); break;
            }
            continue;
        }
        if(this->esc == CLI_ESC_PRE)
        {
            this->esc = CLI_ESC_NONE;
            switch(buf[pos])
            {
                case '[': this->esc = CLI_ESC_ESC; break;
                case 'b': this->sRewWord(); break;
                case 'f': this->sFwdWord(); break;
                case 'd': this->sDelWord(); break;
                case CTRL('H'):
                case 0x7f: this->sBackspaceWord(); break;
            }
            continue;
        }
#endif
        switch(buf[pos])
        {
            case CTRL('A'): this->sBol(); break;
            case CTRL('B'): this->sRew(); break;
            case CTRL('C'): this->sErase(); break;
            case CTRL('D'): this->sDel(); break;
            case CTRL('E'): this->sEol(); break;
            case CTRL('F'): this->sFwd(); break;
            case CTRL('H'):
            case 0x7f:	this->sBackspace(); break;
            case CTRL('K'): this->sKline(); break;
            case CTRL('U'): this->sErase(); break;
            case CTRL('W'): this->sBackspaceWord(); break;
            case '\033': this->esc = CLI_ESC_PRE; break;

            case '\t': this->parse(CLI_PARSE_COMPLETE); break;
            case '?':  this->parse(CLI_PARSE_HELP); break;
			case '\n':
			    if(!treatNlAsCr)
			        break;
			    /* otherwise,continue */
            case '\r':
            {
                unsigned char retcode;

                this->print( "\r\n");
                this->pos = this->len;

                if(!this->len) { this->prompt(); break; }
                this->histAdd();

                retcode = this->parse(CLI_PARSE_EXEC);
                this->len = 0;
                this->pos = 0;
                this->buffer[this->len] = 0;

                if(!retcode)
                    this->parseError("Unambigious command!\r\n");

                this->prompt();
            }
                break;
            default:
            {
                if(buf[pos] < 32 || buf[pos] >= 127) break;
                this->sInsert(buf[pos]);
                break;
            }
        }
        this->buffer[this->len] = 0;
    }
}

void cli::parseError(const char *msg)
{
    this->print(msg);
}


cli::cli(const char *context)
{
    this->context = context;

    this->max_tokens = CLI_MAX_TOKENS;
    this->alloc_len = CLI_BUFFER_LEN;

    this->width = 80;
    this->height = 25;

    cli::assert((this->buffer = (unsigned char*) malloc(this->alloc_len + CLI_MAX_TOKEN_LEN + 16)));
    cli::assert((this->token_tab = (struct cliToken*) malloc(sizeof(struct cliToken) * this->max_tokens)));
    snprintf(this->promptString, CLI_PROMPT_MAX, "# ");

    this->token_pos = 0;
}

void cli::startChat()
{
#ifdef CLI_TELNET_SUPPORT
    this->telnetInit();
#endif
#ifndef CLI_OWN_PROMPTS
    this->prompt();
#endif
}

cli::~cli() {
    unsigned int i;
    free(this->buffer);
    free(this->token_tab);
    if (this->hist_ent) {
        for (i = 0; i < this->hist_count; i++)
            free(this->hist_ent[i]);
        free(this->hist_ent);
    }
    for (i = 0; i < this->ctx_pos; i++) {
        free(this->ctx[i].b);
        free(this->ctx[i].return_prompt);
    }
}


void cli::setContext(char *context)
{
    this->context = context;
}

void  cli::execString(const char *line)
{
    this->input((unsigned char*) line, (unsigned int) strlen(line), true);
}


void cli::execStream(FILE *fh)
{
    char line[CLI_BUFFER_LEN + 16];

    while(fgets(line, CLI_BUFFER_LEN, fh))
        this->execString(line);

}