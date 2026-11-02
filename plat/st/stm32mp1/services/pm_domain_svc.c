/*
 * Copyright (c) 2026, onedots
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * STM32_SMC_PD_DOMAIN SiP service.
 *
 * Linux's stm32-pm-domain driver (ST v5.4 and v6.6 trees) reports the
 * state of the VSW / CORE_RET / CORE power domains through this call. ST's
 * TF-A up to v2.8 recorded it to decide which SoC low-power mode
 * (Stop / LP-Stop / Standby) a later PSCI system suspend may enter. v2.10
 * dropped both that power-mode machinery in SP_MIN and this call, so every
 * runtime-PM transition of a CORE-domain device (the console UART on each
 * keystroke, for instance) printed "Unimplemented STM32MP1 Service Call".
 *
 * SP_MIN in v2.10 has no SoC low-power mode selection to feed, so keep the
 * ABI and record the state only. Nothing reads it yet.
 */

#include <stdbool.h>

#include <stm32mp_svc_setup.h>

#include "pm_domain_svc.h"

#define STM32MP1_PD_MAX		3U
#define STM32_SMC_PD_ON		0U
#define STM32_SMC_PD_OFF	1U

static bool pm_domain_on[STM32MP1_PD_MAX] = { true, true, true };

uint32_t pm_domain_scv_handler(uint32_t x1, uint32_t x2)
{
	if ((x1 >= STM32MP1_PD_MAX) ||
	    ((x2 != STM32_SMC_PD_ON) && (x2 != STM32_SMC_PD_OFF))) {
		return STM32_SMC_INVALID_PARAMS;
	}

	pm_domain_on[x1] = (x2 == STM32_SMC_PD_ON);

	return STM32_SMC_OK;
}
