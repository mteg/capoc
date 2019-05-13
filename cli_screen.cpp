/* This file is mostly inspired by lib/vty.c from GNU Zebra */
/* Below is the original header */

/*
 * Virtual terminal [aka TeletYpe] interface routine.
 * Copyright (C) 1997, 98 Kunihiro Ishiguro
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Zebra; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */


#include <string.h>
#include <stdlib.h>
#ifndef WINDOWS
#define CLI_TELNET_SUPPORT
#endif
#ifdef CLI_TELNET_SUPPORT
#include <arpa/inet.h>
#include <arpa/telnet.h>
#include <cstring>
#include <cstdlib>

#endif
#include "cli.h"


void cli::telnetInit()
{
#ifdef CLI_TELNET_SUPPORT
    unsigned char init_commands[] =
		{ IAC, WILL, TELOPT_ECHO,
		  IAC, WILL, TELOPT_SGA,
		  IAC, DONT, TELOPT_LINEMODE,
		  IAC, DO, TELOPT_NAWS,
		};

	this->write((char*) init_commands, sizeof(init_commands));
#endif
}

#define T_BACK  0x08
#define T_SPACE ' '

void cli::sInsert(char c)
{
    int to_end = this->len - this->pos;
    int i;

    if((this->len + 2) >= this->alloc_len) return;
    if(to_end) memmove(&this->buffer[this->pos + 1], &this->buffer[this->pos], to_end);

    this->buffer[this->pos] = (unsigned char) c;
    this->buffer[this->len + 1] = 0;
    this->print((char*) &this->buffer[this->pos]);

    for(i = 0; i<to_end; i++)
        this->writech(T_BACK);

    this->pos++;
    this->len++;
}

void cli::sInsertOverwrite(char c)
{
    if((this->len + 2) >= this->alloc_len) return;

    this->buffer[this->pos++] = (unsigned char)  c;
    if(this->pos > this->len) this->buffer[++this->len] = 0;

    this->writech(c);
}

void cli::sRedraw()
{
    this->print((char*) this->buffer);
    this->pos = this->len;
}

void cli::sFwd()
{
    if(this->pos < this->len)
    {
        this->writech(this->buffer[this->pos]);
        this->pos++;
    }
}

void cli::sRew()
{
    if(this->pos > 0)
    {
        this->pos--;
        this->writech(T_BACK);
    }
}

void cli::seol() {

}

void cli::sBol()
{
    while(this->pos)
        this->sRew();
}

void cli::sEol()
{
    while(this->pos < this->len)
        this->sFwd();
}

void cli::sFwdWord()
{
    while(this->pos < this->len && this->buffer[this->pos] != ' ')
        this->sFwd();

    while(this->pos < this->len && this->buffer[this->pos] == ' ')
        this->sFwd();
}

void cli::sRewWord()
{
    while(this->pos > 0 && this->buffer[this->pos - 1] == ' ')
        this->sRew();

    while(this->pos > 0 && this->buffer[this->pos - 1] != ' ')
        this->sRew();
}

void cli::sDel()
{
    int to_end, i;

    if(!this->len) return;
    if(this->pos == this->len) return;

    to_end = this->len - this->pos;
    this->len--;

    memmove(&this->buffer[this->pos], &this->buffer[this->pos + 1], (size_t) (to_end - 1));
    this->buffer[this->len] = 0;

    this->print((char*) &this->buffer[this->pos]);
    this->writech(T_SPACE);

    for(i = 0; i<to_end; i++)
        this->writech(T_BACK);
}

void cli::sBackspace()
{
    if(!this->pos) return;
    this->sRew();
    this->sDel();
}

void cli::sKline()
{
    int i, to_end = this->len - this->pos;
    if(!to_end) return;
    for(i = 0; i<to_end; i++)
        this->writech(T_SPACE);

    for(i = 0; i<to_end; i++)
        this->writech(T_BACK);

    this->len = this->pos;
    this->buffer[this->len] = 0;
}

void cli::sErase()
{
    this->sBol();
    this->sKline();

    this->hist_browse = this->hist_last + 1;
}

void cli::sDelWord()
{
    while(this->pos < this->len && this->buffer[this->pos] == ' ')
        this->sDel();
    while(this->pos < this->len && this->buffer[this->pos] != ' ')
        this->sDel();
}

void cli::sBackspaceWord()
{
    while(this->pos > 0 && this->buffer[this->pos - 1] == ' ')
        this->sBackspace();
    while(this->pos > 0 && this->buffer[this->pos - 1] != ' ')
        this->sBackspace();
}


void cli::histAdd()
{
    if(this->hist_ent)
        if(!strcmp(this->hist_ent[this->hist_last], (char*) this->buffer))
        {
            /* Ignore entry same as previous */
            this->hist_browse = this->hist_last + 1;
            return;
        }

    if(this->hist_count < CLI_HIST_MAX)
    {
        this->assert((this->hist_ent = (char**) realloc(this->hist_ent, sizeof(char*) * (++this->hist_count))));
        this->hist_ent[this->hist_count - 1] = NULL;
    }

    this->hist_last++;
    if(this->hist_last >= this->hist_count) this->hist_last = 0;

    free(this->hist_ent[this->hist_last]);
    assert((this->hist_ent[this->hist_last] = strdup((char*) this->buffer)));

    this->hist_browse = this->hist_last + 1;
}

void cli::histDisplay()
{
    if(this->hist_browse >= this->hist_count) this->hist_browse = 0;
    if(this->hist_browse < 0) this->hist_browse = this->hist_count - 1;

    if(this->hist_ent)
    {
        unsigned int l;

        this->sBol();
        this->sKline();

        l = (int) strlen(this->hist_ent[this->hist_browse]);
        strncpy((char*) this->buffer, this->hist_ent[this->hist_browse], this->alloc_len);
        if(l >= this->alloc_len) l = this->alloc_len - 1;

        this->buffer[l] = 0;
        this->pos = l;
        this->len = l;

        this->sRedraw();
    }
}

void cli::histPrev()
{
    this->hist_browse--;
    this->histDisplay();
}

void cli::histNext()
{
    this->hist_browse++;
    this->histDisplay();
}
