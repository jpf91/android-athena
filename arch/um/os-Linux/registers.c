/*
 * Copyright (C) 2004 PathScale, Inc
 * Licensed under the GPL
 */

#include <errno.h>
#include <string.h>
#include <sys/ptrace.h>
#include "user.h"
#include "sysdep/ptrace.h"

/* This is set once at boot time and not changed thereafter */

static unsigned long exec_regs[MAX_REG_NR];

void init_thread_registers(union uml_pt_regs *to)
{
	memcpy(to->skas.regs, exec_regs, sizeof(to->skas.regs));
}

void save_registers(int pid, union uml_pt_regs *regs)
{
	int err;

	err = ptrace(PTRACE_GETREGS, pid, 0, regs->skas.regs);
	if(err < 0)
		panic("save_registers - saving registers failed, errno = %d\n",
		      errno);
}

void restore_registers(int pid, union uml_pt_regs *regs)
{
	int err;

	err = ptrace(PTRACE_SETREGS, pid, 0, regs->skas.regs);
	if(err < 0)
		panic("restore_registers - saving registers failed, "
		      "errno = %d\n", errno);
}

void init_registers(int pid)
{
	int err;

	err = ptrace(PTRACE_GETREGS, pid, 0, exec_regs);
	if(err)
		panic("check_ptrace : PTRACE_GETREGS failed, errno = %d",
		      errno);
}

void get_safe_registers(unsigned long *regs)
{
	memcpy(regs, exec_regs, sizeof(exec_regs));
}