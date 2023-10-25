/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.io/license.html
 */

#ifndef _SSD1322_H
#define _SSD1322_H

#define CMD_DISPLAY_START_LINE	0x40
#define CMD_CONTRAST_CONTROL	0x81
#define CMD_CHARGE_PUMP			0x8D
#define CMD_SEGMENT_REMAP		0xA1
#define CMD_MULTIPLEX_RATIO		0xA8
#define CMD_REFERENCE			0xAD
#define CMD_DISPLAY_OFF			0xAE
#define CMD_DISPLAY_ON			0xAF
#define CMD_COM_OUTPUT_SCAN_DIR	0xC0
#define CMD_DISPLAY_OFFSET		0xD3
#define CMD_CLOCK_DIVIDER		0xD5
#define CMD_PRECHARGE_PERIOD	0xD9
#define CMD_COM_HARDWARE_CFG	0xDA
#define CMD_VCOMH_SELECT_LEVEL	0xDB
#define CMD_LOCK				0xFD

#endif /* _SSD1322_H */

