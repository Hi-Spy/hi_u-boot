/******************************************************************************
*    Copyright (c) 2009-2012 by Hisi.
*    All rights reserved.
* ***
*
******************************************************************************/

#include <asm/arch/platform.h>
#include <config.h>

#ifdef CONFIG_DDR_DATAEYE_TRAINING_STARTUP
extern int ddr_dataeye_training(void *param);
#endif /* CONFIG_DDR_DATAEYE_TRAINING_STARTUP */

extern void ddr_training_info(int value);
extern int ddrphy_train_route(void);

extern void reset_cpu(unsigned long addr);

static inline void DWB(void) /* drain write buffer */
{
}

static inline unsigned int readl(unsigned addr)
{
	unsigned int val;
	val = (*(volatile unsigned int *)(addr));
	return val;
}

static inline void writel(unsigned val, unsigned addr)
{
	DWB();
	(*(volatile unsigned *) (addr)) = (val);
	DWB();
}

static inline void delay(void)
{
	__asm__ __volatile__("nop");
}

void start_ddr_training(unsigned int base)
{
	int ret = 0;

	if (!(readl(base + REG_SC_GEN20) & 0x1)) {
			ret = ddrphy_train_route();
			if (ret) {
				ddr_training_info(ret);
				reset_cpu(0);
			}
		}

#ifdef CONFIG_DDR_DATAEYE_TRAINING_STARTUP
	if (!(readl(base + REG_SC_GEN20) & (0x1 << 16))) {
			ret = ddr_dataeye_training(0);
			if (ret)
				reset_cpu(0);
		}
#endif /* CONFIG_DDR_DATAEYE_TRAINING_STARTUP */

}
