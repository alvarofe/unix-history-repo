/*
 *	Copyright (c) 1984-1987 by the Regents of the
 *	University of California and by Gregory Glenn Minshall.
 *
 *	Permission to use, copy, modify, and distribute these
 *	programs and their documentation for any purpose and
 *	without fee is hereby granted, provided that this
 *	copyright and permission appear on all copies and
 *	supporting documentation, the name of the Regents of
 *	the University of California not be used in advertising
 *	or publicity pertaining to distribution of the programs
 *	without specific prior permission, and notice be given in
 *	supporting documentation that copying and distribution is
 *	by permission of the Regents of the University of California
 *	and by Gregory Glenn Minshall.  Neither the Regents of the
 *	University of California nor Gregory Glenn Minshall make
 *	representations about the suitability of this software
 *	for any purpose.  It is provided "as is" without
 *	express or implied warranty.
 */

#ifndef lint
static char sccsid[] = "@(#)system.c	1.6 (Berkeley) %G%";
#endif	/* not lint */

#include <stdio.h>

#include "../general/general.h"
#include "../ctlr/api.h"
#include "spint.h"

#include "../general/globals.h"


static Spint spinted;
static char command[256];
static int need_to_start = 0;

/*
 * shell_continue() actually runs the command, and looks for API
 * requests coming back in.
 *
 * We are called from the main loop in telnet.c.
 */

int
shell_continue()
{
    /*
     * spint_start() returns when either the command has finished, or when
     * the required interrupt comes in.  In the latter case, the appropriate
     * thing to do is to process the interrupt, and then return to
     * the interrupt issuer by calling spint_continue().
     */
    if (need_to_start) {
	need_to_start = 0;
	spint_start(command, &spinted);
    }

    if (spinted.done == 0) {
	/* Process request */
	handle_api(&spinted.regs, &spinted.sregs);
	spint_continue(&spinted);
    } else {
	char inputbuffer[100];

	if (spinted.rc != 0) {
	    fprintf(stderr, "Process generated a return code of 0x%x.\n",
								spinted.rc);
	}
	printf("[Hit return to continue]");
	fflush(stdout);
	(void) gets(inputbuffer);
	shell_active = 0;
	setconnmode();
	ConnectScreen();
    }
    return shell_active;
}


/*
 * Called from telnet.c to fork a lower command.com.  We
 * use the spint... routines so that we can pick up
 * interrupts generated by application programs.
 */


int
shell(argc,argv)
int	argc;
char	*argv[];
{

    ClearElement(spinted);
    spinted.int_no = API_INTERRUPT_NUMBER;
    if (argc == 1) {
	command[0] = 0;
    } else {
	char *cmdptr;
	int length;

	argc--;
	argv++;
	strcpy(command, " /c");
	cmdptr = command+strlen(command);
	while (argc) {
	    if ((cmdptr+strlen(*argv)) >= (command+sizeof command)) {
		fprintf(stderr, "Argument list too long at argument *%s*.\n",
			    *argv);
		return 0;
	    }
	    *cmdptr++ = ' ';		/* Blank separators */
	    strcpy(cmdptr, *argv);
	    cmdptr += strlen(cmdptr);
	    argc--;
	    argv++;
	}
	length = strlen(command)-1;
	if (length < 0) {
	    length = 0;
	}
	command[0] = length;
    }
    need_to_start = 1;
    shell_active = 1;
    return 1;			/* Go back to main loop */
}
