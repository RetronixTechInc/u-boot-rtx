/*
 * Copyright (C) 2010-2016 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __RECOVERY_H_
#define __RECOVERY_H_

struct reco_envs {
	char *cmd;
	char *args;
};

void check_recovery_mode(void);
bool recovery_check_and_clean_command(void);
int check_recovery_cmd_file(void);
void board_recovery_setup(void);
int read_boot_reason(void);
void clear_boot_reason(void);

#endif
