/*
 * (C) Copyright 2002-2008
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * To build the utility with the run-time configuration
 * uncomment the next line.
 * See included "fw_env.config" sample file (TRAB board)
 * for notes on configuration.
 */
#define CONFIG_FILE     "/etc/fw_env.config"

#define HAVE_REDUND /* For systems with 2 env sectors */
#define DEVICE1_NAME      "/dev/mtd1"
#define DEVICE2_NAME      "/dev/mtd2"
#define DEVICE1_OFFSET    0x0000
#define ENV1_SIZE         0x4000
#define DEVICE1_ESIZE     0x4000
#define DEVICE2_OFFSET    0x0000
#define ENV2_SIZE         0x4000
#define DEVICE2_ESIZE     0x4000

#define CONFIG_BAUDRATE		115200
#define CONFIG_BOOTDELAY    0

#define CONFIG_NETMASK	    255.255.255.0		/* talk on MY local net */
#define CONFIG_IPADDR	    192.168.1.120		/* default static IP */
#define CONFIG_GATEWAYIP    192.168.1.1
#define CONFIG_SERVERIP	    192.168.1.211		/* default tftp server ip */
#define CONFIG_ETHADDR	    74:37:2f:00:00:00

#define CONFIG_BOOTCOMMAND  \
"sf probe 0;sf read 0x82000000 0x30000 0x300000;bootm 0x82000000"

#define CONFIG_BOOTARGS   \
"mem=72M console=ttyAMA0,115200 "   \
"root=/dev/mtdblock3 rootfstype=squashfs rw init=/init "  \
"mtdparts=hi_sfc:128k(u-boot)ro,64k(env),3m(kernel),4608k(rootfs),512k(config),-(app)"


extern int   fw_printenv(int argc, char *argv[]);
extern char *fw_getenv  (char *name);
extern int fw_setenv  (int argc, char *argv[]);

extern unsigned	long  crc32	 (unsigned long, const unsigned char *, unsigned);

