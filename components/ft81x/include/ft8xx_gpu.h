/*

Copyright (c) Future Technology Devices International 2014

THIS SOFTWARE IS PROVIDED BY FUTURE TECHNOLOGY DEVICES INTERNATIONAL LIMITED "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
FUTURE TECHNOLOGY DEVICES INTERNATIONAL LIMITED BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES LOSS OF USE, DATA, OR PROFITS OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

FTDI DRIVERS MAY BE USED ONLY IN CONJUNCTION WITH PRODUCTS BASED ON FTDI PARTS.

FTDI DRIVERS MAY BE DISTRIBUTED IN ANY FORM AS LONG AS LICENSE INFORMATION IS NOT MODIFIED.

IF A CUSTOM VENDOR ID AND/OR PRODUCT ID OR DESCRIPTION STRING ARE USED, IT IS THE
RESPONSIBILITY OF THE PRODUCT MANUFACTURER TO MAINTAIN ANY CHANGES AND SUBSEQUENT WHQL
RE-CERTIFICATION AS A RESULT OF MAKING THESE CHANGES.

Author : FTDI

Revision History:
0.1 - date 2013.04.24 - Initial Version
0.2 - date 2013.08.19 - added few registers.
1.0 - date 2013.12.10 - Version 1.0.
2.0 - date 2014.11.24 - version 2.0 which includes support for FT81x series.


*/

#ifndef _FT_GPU_H_
#define _FT_GPU_H_

#include <stdint.h>
#include "bsp_board.h"

/* Definitions used for FT800 co-processor command buffer */
#define FT_DL_SIZE		  (8 * 1024L) // 8KB Display List buffer size
#define FT_CMD_FIFO_SIZE  (4 * 1024L) // 4KB co-processor FIFO size
#define FT_CMD_SIZE		  (4)		  // 4 bytes per EVE co-processor command
#define FT_PNG_SCRATCHPAD 0xF5800	  // defined in BT81X Prog Guide v2.1

#ifndef FT_SONARQ_STR_MAXLEN
#define FT_SONARQ_STR_MAXLEN   1024 // SonarQube insistence of a string maxlen
#endif /* FT_SONARQ_STR_MAXLEN */

#undef POINTS // this for windef.h in MSVC

#if defined(FT800_ENABLE) || defined(FT801_ENABLE)

/* For FT801 enable the switch in platform.h file */
/* Lower boundary of trimming */
#define LOW_FREQ_BOUND 47040000L // 98% of 48Mhz

#define FT800_VERSION	 "1.9.0".
#define RAM_G_SIZE		 256 * 1024L
#define ADC_DIFFERENTIAL 1UL
#define ADC_SINGLE_ENDED 0UL
#define ADPCM_SAMPLES	 2UL
#define ALWAYS			 7UL
#define ARGB1555		 0UL
#define ARGB2			 5UL
#define ARGB4			 6UL
#define BARGRAPH		 11UL
#define BILINEAR		 1UL
#define BITMAPS			 1UL
#define BORDER			 0UL

#define CMDBUF_SIZE			 4096UL
#define CMD_APPEND			 4294967070UL
#define CMD_BGCOLOR			 4294967049UL
#define CMD_BITMAP_TRANSFORM 4294967073UL
#define CMD_BUTTON			 4294967053UL
#define CMD_CALIBRATE		 4294967061UL
#define CMD_CSKETCH			 4294967093UL
#define CMD_CLOCK			 4294967060UL
#define CMD_COLDSTART		 4294967090UL
#define CMD_CRC				 4294967043UL
#define CMD_DIAL			 4294967085UL
#define CMD_DLSTART			 4294967040UL
#define CMD_EXECUTE			 4294967047UL
#define CMD_FGCOLOR			 4294967050UL
#define CMD_GAUGE			 4294967059UL
#define CMD_GETMATRIX		 4294967091UL
#define CMD_GETPOINT		 4294967048UL
#define CMD_GETPROPS		 4294967077UL
#define CMD_GETPTR			 4294967075UL
#define CMD_GRADCOLOR		 4294967092UL
#define CMD_GRADIENT		 4294967051UL
#define CMD_HAMMERAUX		 4294967044UL
#define CMD_IDCT			 4294967046UL
#define CMD_INFLATE			 4294967074UL
#define CMD_INTERRUPT		 4294967042UL
#define CMD_KEYS			 4294967054UL
#define CMD_LOADIDENTITY	 4294967078UL
#define CMD_LOADIMAGE		 4294967076UL
#define CMD_LOGO			 4294967089UL
#define CMD_MARCH			 4294967045UL
#define CMD_MEMCPY			 4294967069UL
#define CMD_MEMCRC			 4294967064UL
#define CMD_MEMSET			 4294967067UL
#define CMD_MEMWRITE		 4294967066UL
#define CMD_MEMZERO			 4294967068UL
#define CMD_NUMBER			 4294967086UL
#define CMD_PROGRESS		 4294967055UL
#define CMD_REGREAD			 4294967065UL
#define CMD_ROTATE			 4294967081UL
#define CMD_SCALE			 4294967080UL
#define CMD_SCREENSAVER		 4294967087UL
#define CMD_SCROLLBAR		 4294967057UL
#define CMD_SETFONT			 4294967083UL
#define CMD_SETMATRIX		 4294967082UL
#define CMD_SKETCH			 4294967088UL
#define CMD_SLIDER			 4294967056UL
#define CMD_SNAPSHOT		 4294967071UL
#define CMD_SPINNER			 4294967062UL
#define CMD_STOP			 4294967063UL
#define CMD_SWAP			 4294967041UL
#define CMD_TEXT			 4294967052UL
#define CMD_TOGGLE			 4294967058UL
#define CMD_TOUCH_TRANSFORM	 4294967072UL
#define CMD_TRACK			 4294967084UL
#define CMD_TRANSLATE		 4294967079UL

#define DECR			 4UL
#define DECR_WRAP		 7UL
#define DLSWAP_DONE		 0UL
#define DLSWAP_FRAME	 2UL
#define DLSWAP_LINE		 1UL
#define DST_ALPHA		 3UL
#define EDGE_STRIP_A	 7UL
#define EDGE_STRIP_B	 8UL
#define EDGE_STRIP_L	 6UL
#define EDGE_STRIP_R	 5UL
#define EQUAL			 5UL
#define GEQUAL			 4UL
#define GREATER			 3UL
#define INCR			 3UL
#define INCR_WRAP		 6UL
#define INT_CMDEMPTY	 32UL
#define INT_CMDFLAG		 64UL
#define INT_CONVCOMPLETE 128UL
#define INT_PLAYBACK	 16UL
#define INT_SOUND		 8UL
#define INT_SWAP		 1UL
#define INT_TAG			 4UL
#define INT_TOUCH		 2UL
#define INVERT			 5UL

#define KEEP				1UL
#define L1					1UL
#define L4					2UL
#define L8					3UL
#define LEQUAL				2UL
#define LESS				1UL
#define LINEAR_SAMPLES		0UL
#define LINES				3UL
#define LINE_STRIP			4UL
#define NEAREST				0UL
#define NEVER				0UL
#define NOTEQUAL			6UL
#define ONE					1UL
#define ONE_MINUS_DST_ALPHA 5UL
#define ONE_MINUS_SRC_ALPHA 4UL
#define OPT_CENTER			1536UL
#define OPT_CENTERX			512UL
#define OPT_CENTERY			1024UL
#define OPT_FLAT			256UL
#define OPT_MONO			1UL
#define OPT_NOBACK			4096UL
#define OPT_NODL			2UL
#define OPT_NOHANDS			49152UL
#define OPT_NOHM			16384UL
#define OPT_NOPOINTER		16384UL
#define OPT_NOSECS			32768UL
#define OPT_NOTICKS			8192UL
#define OPT_RIGHTX			2048UL
#define OPT_SIGNED			256UL
#define PALETTED			8UL
#define FTPOINTS			2UL
#define RECTS				9UL

#define RAM_CMD 1081344UL
#define RAM_DL	1048576UL
#define RAM_G	0UL
#define RAM_PAL 1056768UL
#define RAM_REG 1057792UL

#define ROMFONT_TABLEADDRESS 1048572UL

#define REG_ANALOG			  1058104UL
#define REG_ANA_COMP		  1058160UL
#define REG_CLOCK			  1057800UL
#define REG_CMD_DL			  1058028UL
#define REG_CMD_READ		  1058020UL
#define REG_CMD_WRITE		  1058024UL
#define REG_CPURESET		  1057820UL
#define REG_CRC				  1058152UL
#define REG_CSPREAD			  1057892UL
#define REG_CYA0			  1058000UL
#define REG_CYA1			  1058004UL
#define REG_CYA_TOUCH		  1058100UL
#define REG_DATESTAMP		  1058108UL
#define REG_DITHER			  1057884UL
#define REG_DLSWAP			  1057872UL
#define REG_FRAMES			  1057796UL
#define REG_FREQUENCY		  1057804UL
#define REG_GPIO			  1057936UL
#define REG_GPIO_DIR		  1057932UL
#define REG_HCYCLE			  1057832UL
#define REG_HOFFSET			  1057836UL
#define REG_HSIZE			  1057840UL
#define REG_HSYNC0			  1057844UL
#define REG_HSYNC1			  1057848UL
#define REG_ID				  1057792UL
#define REG_INT_EN			  1057948UL
#define REG_INT_FLAGS		  1057944UL
#define REG_INT_MASK		  1057952UL
#define REG_MACRO_0			  1057992UL
#define REG_MACRO_1			  1057996UL
#define REG_OUTBITS			  1057880UL
#define REG_PCLK			  1057900UL
#define REG_PCLK_POL		  1057896UL
#define REG_PLAY			  1057928UL
#define REG_PLAYBACK_FORMAT	  1057972UL
#define REG_PLAYBACK_FREQ	  1057968UL
#define REG_PLAYBACK_LENGTH	  1057960UL
#define REG_PLAYBACK_LOOP	  1057976UL
#define REG_PLAYBACK_PLAY	  1057980UL
#define REG_PLAYBACK_READPTR  1057964UL
#define REG_PLAYBACK_START	  1057956UL
#define REG_PWM_DUTY		  1057988UL
#define REG_PWM_HZ			  1057984UL
#define REG_RENDERMODE		  1057808UL
#define REG_ROMSUB_SEL		  1058016UL
#define REG_ROTATE			  1057876UL
#define REG_SNAPSHOT		  1057816UL
#define REG_SNAPY			  1057812UL
#define REG_SOUND			  1057924UL
#define REG_SWIZZLE			  1057888UL
#define REG_TAG				  1057912UL
#define REG_TAG_X			  1057904UL
#define REG_TAG_Y			  1057908UL
#define REG_TAP_CRC			  1057824UL
#define REG_TAP_MASK		  1057828UL
#define REG_TOUCH_ADC_MODE	  1058036UL
#define REG_TOUCH_CHARGE	  1058040UL
#define REG_TOUCH_DIRECT_XY	  1058164UL
#define REG_TOUCH_DIRECT_Z1Z2 1058168UL
#define REG_TOUCH_MODE		  1058032UL
#define REG_TOUCH_OVERSAMPLE  1058048UL
#define REG_TOUCH_RAW_XY	  1058056UL
#define REG_TOUCH_RZ		  1058060UL
#define REG_TOUCH_RZTHRESH	  1058052UL
#define REG_TOUCH_SCREEN_XY	  1058064UL
#define REG_TOUCH_SETTLE	  1058044UL
#define REG_TOUCH_TAG		  1058072UL
#define REG_TOUCH_TAG_XY	  1058068UL
#define REG_TOUCH_TRANSFORM_A 1058076UL
#define REG_TOUCH_TRANSFORM_B 1058080UL
#define REG_TOUCH_TRANSFORM_C 1058084UL
#define REG_TOUCH_TRANSFORM_D 1058088UL
#define REG_TOUCH_TRANSFORM_E 1058092UL
#define REG_TOUCH_TRANSFORM_F 1058096UL
#define REG_TRACKER			  1085440UL
#define REG_TRIM			  1058156UL
#define REG_VCYCLE			  1057852UL
#define REG_VOFFSET			  1057856UL
#define REG_VOL_PB			  1057916UL
#define REG_VOL_SOUND		  1057920UL
#define REG_VSIZE			  1057860UL
#define REG_VSYNC0			  1057864UL
#define REG_VSYNC1			  1057868UL
/* FT801 Registers */
#define REG_CTOUCH_EXTENDED	 1058036UL
#define REG_CTOUCH_GESTURE	 1058104UL
#define REG_CTOUCH_IDS		 1058060UL
#define REG_CTOUCH_TOUCH0_XY 1058064UL
#define REG_CTOUCH_TOUCH1_XY 1058056UL
#define REG_CTOUCH_TOUCH2_XY 1058164UL
#define REG_CTOUCH_TOUCH3_XY 1058168UL
#define REG_CTOUCH_TOUCH4_X	 1058104UL
#define REG_CTOUCH_TOUCH4_Y	 1058060UL

#define REPEAT				 1UL
#define REPLACE				 2UL
#define RGB332				 4UL
#define RGB565				 7UL
#define SRC_ALPHA			 2UL
#define TEXT8X8				 9UL
#define TEXTVGA				 10UL
#define TOUCHMODE_CONTINUOUS 3UL
#define TOUCHMODE_FRAME		 2UL
#define TOUCHMODE_OFF		 0UL
#define TOUCHMODE_ONESHOT	 1UL
#define ULAW_SAMPLES		 1UL
#define ZERO				 0UL

#define VERTEX2F(x, y)									 ((1UL << 30) | (((x)&32767UL) << 15) | (((y)&32767UL) << 0))
#define VERTEX2II(x, y, handle, cell)					 ((2UL << 30) | (((x)&511UL) << 21) | (((y)&511UL) << 12) | (((handle)&31UL) << 7) | (((cell)&127UL) << 0))
#define BITMAP_SOURCE(addr)								 ((1UL << 24) | (((addr)&1048575UL) << 0))
#define CLEAR_COLOR_RGB(red, green, blue)				 ((2UL << 24) | (((red)&255UL) << 16) | (((green)&255UL) << 8) | (((blue)&255UL) << 0))
#define TAG(s)											 ((3UL << 24) | (((s)&255UL) << 0))
#define COLOR_RGB(red, green, blue)						 ((4UL << 24) | (((red)&255UL) << 16) | (((green)&255UL) << 8) | (((blue)&255UL) << 0))
#define BITMAP_HANDLE(handle)							 ((5UL << 24) | (((handle)&31UL) << 0))
#define CELL(cell)										 ((6UL << 24) | (((cell)&127UL) << 0))
#define BITMAP_LAYOUT(format, linestride, height)		 ((7UL << 24) | (((format)&31UL) << 19) | (((linestride)&1023UL) << 9) | (((height)&511UL) << 0))
#define BITMAP_SIZE(filter, wrapx, wrapy, width, height) ((8UL << 24) | (((filter)&1UL) << 20) | (((wrapx)&1UL) << 19) | (((wrapy)&1UL) << 18) | (((width)&511UL) << 9) | (((height)&511UL) << 0))
#define ALPHA_FUNC(func, ref)							 ((9UL << 24) | (((func)&7UL) << 8) | (((ref)&255UL) << 0))
#define STENCIL_FUNC(func, ref, mask)					 ((10UL << 24) | (((func)&7UL) << 16) | (((ref)&255UL) << 8) | (((mask)&255UL) << 0))
#define BLEND_FUNC(src, dst)							 ((11UL << 24) | (((src)&7UL) << 3) | (((dst)&7UL) << 0))
#define STENCIL_OP(sfail, spass)						 ((12UL << 24) | (((sfail)&7UL) << 3) | (((spass)&7UL) << 0))
#define POINT_SIZE(size)								 ((13UL << 24) | (((size)&8191UL) << 0))
#define LINE_WIDTH(width)								 ((14UL << 24) | (((width)&4095UL) << 0))
#define CLEAR_COLOR_A(alpha)							 ((15UL << 24) | (((alpha)&255UL) << 0))
#define COLOR_A(alpha)									 ((16UL << 24) | (((alpha)&255UL) << 0))
#define CLEAR_STENCIL(s)								 ((17UL << 24) | (((s)&255UL) << 0))
#define CLEAR_TAG(s)									 ((18UL << 24) | (((s)&255UL) << 0))
#define STENCIL_MASK(mask)								 ((19UL << 24) | (((mask)&255UL) << 0))
#define TAG_MASK(mask)									 ((20UL << 24) | (((mask)&1UL) << 0))
#define BITMAP_TRANSFORM_A(a)							 ((21UL << 24) | (((a)&131071UL) << 0))
#define BITMAP_TRANSFORM_B(b)							 ((22UL << 24) | (((b)&131071UL) << 0))
#define BITMAP_TRANSFORM_C(c)							 ((23UL << 24) | (((c)&16777215UL) << 0))
#define BITMAP_TRANSFORM_D(d)							 ((24UL << 24) | (((d)&131071UL) << 0))
#define BITMAP_TRANSFORM_E(e)							 ((25UL << 24) | (((e)&131071UL) << 0))
#define BITMAP_TRANSFORM_F(f)							 ((26UL << 24) | (((f)&16777215UL) << 0))
#define SCISSOR_XY(x, y)								 ((27UL << 24) | (((x)&511UL) << 9) | (((y)&511UL) << 0))
#define SCISSOR_SIZE(width, height)						 ((28UL << 24) | (((width)&1023UL) << 10) | (((height)&1023UL) << 0))
#define CALL(dest)										 ((29UL << 24) | (((dest)&65535UL) << 0))
#define JUMP(dest)										 ((30UL << 24) | (((dest)&65535UL) << 0))
#define BEGIN(prim)										 ((31UL << 24) | (((prim)&15UL) << 0))
#define COLOR_MASK(r, g, b, a)							 ((32UL << 24) | (((r)&1UL) << 3) | (((g)&1UL) << 2) | (((b)&1UL) << 1) | (((a)&1UL) << 0))
#define CLEAR(c, s, t)									 ((38UL << 24) | (((c)&1UL) << 2) | (((s)&1UL) << 1) | (((t)&1UL) << 0))
#define END()											 ((33UL << 24))
#define SAVE_CONTEXT()									 ((34UL << 24))
#define RESTORE_CONTEXT()								 ((35UL << 24))
#define RETURN()										 ((36UL << 24))
#define MACRO(m)										 ((37UL << 24) | (((m)&1UL) << 0))
#define DISPLAY()										 ((0UL << 24))

/* End FT80x parameters */
#elif defined(FT810_ENABLE) || defined(FT811_ENABLE) || defined(FT812_ENABLE) || defined(FT813_ENABLE) || defined(FT81x_ENABLE)
#define LOW_FREQ_BOUND 58800000L // 98% of 60Mhz

#define RAM_G_SIZE	  1024 * 1024L
#define FT81X_VERSION "1.0.4"

#define ADC_DIFFERENTIAL	 1UL
#define ADC_SINGLE_ENDED	 0UL
#define ADPCM_SAMPLES		 2UL
#define ALWAYS				 7UL
#define ARGB1555			 0UL
#define ARGB2				 5UL
#define ARGB4				 6UL
#define BARGRAPH			 11UL
#define BILINEAR			 1UL
#define BITMAPS				 1UL
#define BORDER				 0UL
#define CMDBUF_SIZE			 4096UL
#define CMD_APPEND			 4294967070UL
#define CMD_BGCOLOR			 4294967049UL
#define CMD_BITMAP_TRANSFORM 4294967073UL
#define CMD_BUTTON			 4294967053UL
#define CMD_CALIBRATE		 4294967061UL
#define CMD_CLOCK			 4294967060UL
#define CMD_COLDSTART		 4294967090UL
#define CMD_CRC				 4294967043UL
#define CMD_CSKETCH			 4294967093UL
#define CMD_DIAL			 4294967085UL
#define CMD_DLSTART			 4294967040UL
#define CMD_EXECUTE			 4294967047UL
#define CMD_FGCOLOR			 4294967050UL
#define CMD_GAUGE			 4294967059UL
#define CMD_GETMATRIX		 4294967091UL
#define CMD_GETPOINT		 4294967048UL
#define CMD_GETPROPS		 4294967077UL
#define CMD_GETPTR			 4294967075UL
#define CMD_GRADCOLOR		 4294967092UL
#define CMD_GRADIENT		 4294967051UL
#define CMD_HAMMERAUX		 4294967044UL
#define CMD_IDCT_DELETED	 4294967046UL
#define CMD_INFLATE			 4294967074UL
#define CMD_INTERRUPT		 4294967042UL
#define CMD_INT_RAMSHARED	 4294967101UL
#define CMD_INT_SWLOADIMAGE	 4294967102UL
#define CMD_KEYS			 4294967054UL
#define CMD_LOADIDENTITY	 4294967078UL
#define CMD_LOADIMAGE		 4294967076UL
#define CMD_LOGO			 4294967089UL
#define CMD_MARCH			 4294967045UL
#define CMD_MEDIAFIFO		 4294967097UL
#define CMD_MEMCPY			 4294967069UL
#define CMD_MEMCRC			 4294967064UL
#define CMD_MEMSET			 4294967067UL
#define CMD_MEMWRITE		 4294967066UL
#define CMD_MEMZERO			 4294967068UL
#define CMD_NUMBER			 4294967086UL
#define CMD_PLAYVIDEO		 4294967098UL
#define CMD_PROGRESS		 4294967055UL
#define CMD_REGREAD			 4294967065UL
#define CMD_ROMFONT			 4294967103UL
#define CMD_ROTATE			 4294967081UL
#define CMD_SCALE			 4294967080UL
#define CMD_SCREENSAVER		 4294967087UL
#define CMD_SCROLLBAR		 4294967057UL
#define CMD_SETBASE			 4294967096UL
#define CMD_SETBITMAP		 4294967107UL
#define CMD_SETFONT			 4294967083UL
#define CMD_SETFONT2		 4294967099UL
#define CMD_SETMATRIX		 4294967082UL
#define CMD_SETROTATE		 4294967094UL
#define CMD_SETSCRATCH		 4294967100UL
#define CMD_SKETCH			 4294967088UL
#define CMD_SLIDER			 4294967056UL
#define CMD_SNAPSHOT		 4294967071UL
#define CMD_SNAPSHOT2		 4294967095UL
#define CMD_SPINNER			 4294967062UL
#define CMD_STOP			 4294967063UL
#define CMD_SWAP			 4294967041UL
#define CMD_SYNC			 4294967106UL
#define CMD_TEXT			 4294967052UL
#define CMD_TOGGLE			 4294967058UL
#define CMD_TOUCH_TRANSFORM	 4294967072UL
#define CMD_TRACK			 4294967084UL
#define CMD_TRANSLATE		 4294967079UL
#define CMD_VIDEOFRAME		 4294967105UL
#define CMD_VIDEOSTART		 4294967104UL
#define DECR				 4UL
#define DLSWAP_DONE			 0UL
#define DLSWAP_FRAME		 2UL
#define DLSWAP_LINE			 1UL
#define DST_ALPHA			 3UL
#define EDGE_STRIP_A		 7UL
#define EDGE_STRIP_B		 8UL
#define EDGE_STRIP_L		 6UL
#define EDGE_STRIP_R		 5UL
#define EQUAL				 5UL
#define GEQUAL				 4UL
#define GREATER				 3UL
#define INCR				 3UL
#define INT_CMDEMPTY		 32UL
#define INT_CMDFLAG			 64UL
#define INT_CONVCOMPLETE	 128UL
#define INT_G8				 18UL
#define INT_L8C				 12UL
#define INT_PLAYBACK		 16UL
#define INT_SOUND			 8UL
#define INT_SWAP			 1UL
#define INT_TAG				 4UL
#define INT_TOUCH			 2UL
#define INT_VGA				 13UL
#define INVERT				 5UL
#define KEEP				 1UL
#define L1					 1UL
#define L2					 17UL
#define L4					 2UL
#define L8					 3UL
#define LEQUAL				 2UL
#define LESS				 1UL
#define LINEAR_SAMPLES		 0UL
#define LINES				 3UL
#define LINE_STRIP			 4UL
#define NEAREST				 0UL
#define NEVER				 0UL
#define NOTEQUAL			 6UL
#define ONE					 1UL
#define ONE_MINUS_DST_ALPHA	 5UL
#define ONE_MINUS_SRC_ALPHA	 4UL
#define OPT_CENTER			 1536UL
#define OPT_CENTERX			 512UL
#define OPT_CENTERY			 1024UL
#define OPT_FLAT			 256UL
#define OPT_FULLSCREEN		 8UL
#define OPT_MEDIAFIFO		 16UL
#define OPT_MONO			 1UL
#define OPT_NOBACK			 4096UL
#define OPT_NODL			 2UL
#define OPT_NOHANDS			 49152UL
#define OPT_NOHM			 16384UL
#define OPT_NOPOINTER		 16384UL
#define OPT_NOSECS			 32768UL
#define OPT_NOTEAR			 4UL
#define OPT_NOTICKS			 8192UL
#define OPT_RIGHTX			 2048UL
#define OPT_SIGNED			 256UL
#define OPT_SOUND			 32UL
#define PALETTED			 8UL
#define PALETTED4444		 15UL
#define PALETTED565			 14UL
#define PALETTED8			 16UL
#define FTPOINTS			 2UL
#define RAM_CMD				 3178496UL
#define RAM_DL				 3145728UL
#define RAM_G				 0UL
#define RAM_REG				 3153920UL
#define RAM_ROMSUB			 3186688UL
#define RECTS				 9UL

#define ROMFONT_TABLEADDRESS 3145724UL

#define REG_ANALOG										 3154284UL
#define REG_ANA_COMP									 3154308UL
#define REG_BIST_EN										 3154292UL
#define REG_BUSYBITS									 3154152UL
#define REG_CLOCK										 3153928UL
#define REG_CMDB_SPACE									 3155316UL
#define REG_CMDB_WRITE									 3155320UL
#define REG_CMD_DL										 3154176UL
#define REG_CMD_READ									 3154168UL
#define REG_CMD_WRITE									 3154172UL
#define REG_CPURESET									 3153952UL
#define REG_CRC											 3154296UL
#define REG_CSPREAD										 3154024UL
#define REG_CTOUCH_EXTENDED								 3154184UL
// modified below to reflect the same as datasheet
#define REG_CTOUCH_TOUCH0_XY							 3154212UL
#define REG_CTOUCH_TOUCH4_X								 3154284UL
#define REG_CTOUCH_TOUCH4_Y								 3154208UL
#define REG_CTOUCH_TOUCH1_XY							 3154204UL
#define REG_CTOUCH_TOUCH2_XY							 3154316UL
#define REG_CTOUCH_TOUCH3_XY							 3154320UL
#define REG_CYA_TOUCH									 3154280UL
#define REG_DATESTAMP									 3155300UL
#define REG_DITHER										 3154016UL
#define REG_DLSWAP										 3154004UL
#define REG_FRAMES										 3153924UL
#define REG_FREQUENCY									 3153932UL
#define REG_GPIO										 3154068UL
#define REG_GPIOX										 3154076UL
#define REG_GPIOX_DIR									 3154072UL
#define REG_GPIO_DIR									 3154064UL
#define REG_HCYCLE										 3153964UL
#define REG_HOFFSET										 3153968UL
#define REG_HSIZE										 3153972UL
#define REG_HSYNC0										 3153976UL
#define REG_HSYNC1										 3153980UL
#define REG_ID											 3153920UL
#define REG_INT_EN										 3154092UL
#define REG_INT_FLAGS									 3154088UL
#define REG_INT_MASK									 3154096UL
#define REG_MACRO_0										 3154136UL
#define REG_MACRO_1										 3154140UL
#define REG_MEDIAFIFO_READ								 3182612UL
#define REG_MEDIAFIFO_WRITE								 3182616UL
#define REG_OUTBITS										 3154012UL
#define REG_PATCHED_ANALOG								 3154288UL
#define REG_PATCHED_TOUCH_FAULT							 3154284UL
#define REG_PCLK										 3154032UL
#define REG_PCLK_POL									 3154028UL
#define REG_PLAY										 3154060UL
#define REG_PLAYBACK_FORMAT								 3154116UL
#define REG_PLAYBACK_FREQ								 3154112UL
#define REG_PLAYBACK_LENGTH								 3154104UL
#define REG_PLAYBACK_LOOP								 3154120UL
#define REG_PLAYBACK_PLAY								 3154124UL
#define REG_PLAYBACK_READPTR							 3154108UL
#define REG_PLAYBACK_START								 3154100UL
#define REG_PWM_DUTY									 3154132UL
#define REG_PWM_HZ										 3154128UL
#define REG_RENDERMODE									 3153936UL
#define REG_ROMSUB_SEL									 3154160UL
#define REG_ROTATE										 3154008UL
#define REG_SNAPFORMAT									 3153948UL
#define REG_SNAPSHOT									 3153944UL
#define REG_SNAPY										 3153940UL
#define REG_SOUND										 3154056UL
#define REG_SPI_EARLY_TX								 3154300UL
#define REG_SPI_WIDTH									 3154312UL
#define REG_SWIZZLE										 3154020UL
#define REG_TAG											 3154044UL
#define REG_TAG_X										 3154036UL
#define REG_TAG_Y										 3154040UL
#define REG_TAP_CRC										 3153956UL
#define REG_TAP_MASK									 3153960UL
#define REG_TOUCH_ADC_MODE								 3154184UL
#define REG_TOUCH_CHARGE								 3154188UL
#define REG_TOUCH_DIRECT_XY								 3154316UL
#define REG_TOUCH_DIRECT_Z1Z2							 3154320UL
#define REG_TOUCH_FAULT									 3154288UL
#define REG_TOUCH_MODE									 3154180UL
#define REG_TOUCH_OVERSAMPLE							 3154196UL
#define REG_TOUCH_RAW_XY								 3154204UL
#define REG_TOUCH_RZ									 3154208UL
#define REG_TOUCH_RZTHRESH								 3154200UL
#define REG_TOUCH_SCREEN_XY								 3154212UL
#define REG_TOUCH_SETTLE								 3154192UL
#define REG_TOUCH_TAG									 3154220UL
#define REG_TOUCH_TAG1									 3154228UL
#define REG_TOUCH_TAG1_XY								 3154224UL
#define REG_TOUCH_TAG2									 3154236UL
#define REG_TOUCH_TAG2_XY								 3154232UL
#define REG_TOUCH_TAG3									 3154244UL
#define REG_TOUCH_TAG3_XY								 3154240UL
#define REG_TOUCH_TAG4									 3154252UL
#define REG_TOUCH_TAG4_XY								 3154248UL
#define REG_TOUCH_TAG_XY								 3154216UL
#define REG_TOUCH_TRANSFORM_A							 3154256UL
#define REG_TOUCH_TRANSFORM_B							 3154260UL
#define REG_TOUCH_TRANSFORM_C							 3154264UL
#define REG_TOUCH_TRANSFORM_D							 3154268UL
#define REG_TOUCH_TRANSFORM_E							 3154272UL
#define REG_TOUCH_TRANSFORM_F							 3154276UL
#define REG_TRACKER										 3182592UL
#define REG_TRACKER_1									 3182596UL
#define REG_TRACKER_2									 3182600UL
#define REG_TRACKER_3									 3182604UL
#define REG_TRACKER_4									 3182608UL
#define REG_TRIM										 3154304UL
#define REG_VCYCLE										 3153984UL
#define REG_VOFFSET										 3153988UL
#define REG_VOL_PB										 3154048UL
#define REG_VOL_SOUND									 3154052UL
#define REG_VSIZE										 3153992UL
#define REG_VSYNC0										 3153996UL
#define REG_VSYNC1										 3154000UL
#define REPEAT											 1UL
#define REPLACE											 2UL
#define RGB332											 4UL
#define RGB565											 7UL
#define SRC_ALPHA										 2UL
#define TEXT8X8											 9UL
#define TEXTVGA											 10UL
#define TOUCHMODE_CONTINUOUS							 3UL
#define TOUCHMODE_FRAME									 2UL
#define TOUCHMODE_OFF									 0UL
#define TOUCHMODE_ONESHOT								 1UL
#define ULAW_SAMPLES									 1UL
#define ZERO											 0UL
#define VERTEX2F(x, y)									 ((1UL << 30) | (((x)&32767UL) << 15) | (((y)&32767UL) << 0))
#define VERTEX2II(x, y, handle, cell)					 ((2UL << 30) | (((x)&511UL) << 21) | (((y)&511UL) << 12) | (((handle)&31UL) << 7) | (((cell)&127UL) << 0))
#define BITMAP_SOURCE(addr)								 ((1UL << 24) | (((addr)&4194303UL) << 0))
#define CLEAR_COLOR_RGB(red, green, blue)				 ((2UL << 24) | (((red)&255UL) << 16) | (((green)&255UL) << 8) | (((blue)&255UL) << 0))
#define TAG(s)											 ((3UL << 24) | (((s)&255UL) << 0))
#define COLOR_RGB(red, green, blue)						 ((4UL << 24) | (((red)&255UL) << 16) | (((green)&255UL) << 8) | (((blue)&255UL) << 0))
#define BITMAP_HANDLE(handle)							 ((5UL << 24) | (((handle)&31UL) << 0))
#define CELL(cell)										 ((6UL << 24) | (((cell)&127UL) << 0))
#define BITMAP_LAYOUT(format, linestride, height)		 ((7UL << 24) | (((format)&31UL) << 19) | (((linestride)&1023UL) << 9) | (((height)&511UL) << 0))
#define BITMAP_SIZE(filter, wrapx, wrapy, width, height) ((8UL << 24) | (((filter)&1UL) << 20) | (((wrapx)&1UL) << 19) | (((wrapy)&1UL) << 18) | (((width)&511UL) << 9) | (((height)&511UL) << 0))
#define ALPHA_FUNC(func, ref)							 ((9UL << 24) | (((func)&7UL) << 8) | (((ref)&255UL) << 0))
#define STENCIL_FUNC(func, ref, mask)					 ((10UL << 24) | (((func)&7UL) << 16) | (((ref)&255UL) << 8) | (((mask)&255UL) << 0))
#define BLEND_FUNC(src, dst)							 ((11UL << 24) | (((src)&7UL) << 3) | (((dst)&7UL) << 0))
#define STENCIL_OP(sfail, spass)						 ((12UL << 24) | (((sfail)&7UL) << 3) | (((spass)&7UL) << 0))
#define POINT_SIZE(size)								 ((13UL << 24) | (((size)&8191UL) << 0))
#define LINE_WIDTH(width)								 ((14UL << 24) | (((width)&4095UL) << 0))
#define CLEAR_COLOR_A(alpha)							 ((15UL << 24) | (((alpha)&255UL) << 0))
#define COLOR_A(alpha)									 ((16UL << 24) | (((alpha)&255UL) << 0))
#define CLEAR_STENCIL(s)								 ((17UL << 24) | (((s)&255UL) << 0))
#define CLEAR_TAG(s)									 ((18UL << 24) | (((s)&255UL) << 0))
#define STENCIL_MASK(mask)								 ((19UL << 24) | (((mask)&255UL) << 0))
#define TAG_MASK(mask)									 ((20UL << 24) | (((mask)&1UL) << 0))
#define BITMAP_TRANSFORM_A(a)							 ((21UL << 24) | (((a)&131071UL) << 0))
#define BITMAP_TRANSFORM_B(b)							 ((22UL << 24) | (((b)&131071UL) << 0))
#define BITMAP_TRANSFORM_C(c)							 ((23UL << 24) | (((c)&16777215UL) << 0))
#define BITMAP_TRANSFORM_D(d)							 ((24UL << 24) | (((d)&131071UL) << 0))
#define BITMAP_TRANSFORM_E(e)							 ((25UL << 24) | (((e)&131071UL) << 0))
#define BITMAP_TRANSFORM_F(f)							 ((26UL << 24) | (((f)&16777215UL) << 0))
#define SCISSOR_XY(x, y)								 ((27UL << 24) | (((x)&2047UL) << 11) | (((y)&2047UL) << 0))
#define SCISSOR_SIZE(width, height)						 ((28UL << 24) | (((width)&4095UL) << 12) | (((height)&4095UL) << 0))
#define CALL(dest)										 ((29UL << 24) | (((dest)&65535UL) << 0))
#define JUMP(dest)										 ((30UL << 24) | (((dest)&65535UL) << 0))
#define BEGIN(prim)										 ((31UL << 24) | (((prim)&15UL) << 0))
#define COLOR_MASK(r, g, b, a)							 ((32UL << 24) | (((r)&1UL) << 3) | (((g)&1UL) << 2) | (((b)&1UL) << 1) | (((a)&1UL) << 0))
#define CLEAR(c, s, t)									 ((38UL << 24) | (((c)&1UL) << 2) | (((s)&1UL) << 1) | (((t)&1UL) << 0))
#define VERTEX_FORMAT(frac)								 ((39UL << 24) | (((frac)&7UL) << 0))
#define BITMAP_LAYOUT_H(linestride, height)				 ((40UL << 24) | (((linestride)&3UL) << 2) | (((height)&3UL) << 0))
#define BITMAP_SIZE_H(width, height)					 ((41UL << 24) | (((width)&3UL) << 2) | (((height)&3UL) << 0))
#define PALETTE_SOURCE(addr)							 ((42UL << 24) | (((addr)&4194303UL) << 0))
#define VERTEX_TRANSLATE_X(x)							 ((43UL << 24) | (((x)&131071UL) << 0))
#define VERTEX_TRANSLATE_Y(y)							 ((44UL << 24) | (((y)&131071UL) << 0))
#define NOP()											 ((45UL << 24))
#define END()											 ((33UL << 24))
#define SAVE_CONTEXT()									 ((34UL << 24))
#define RESTORE_CONTEXT()								 ((35UL << 24))
#define RETURN()										 ((36UL << 24))
#define MACRO(m)										 ((37UL << 24) | (((m)&1UL) << 0))
#define DISPLAY()										 ((0UL << 24))

/* End FT81x parameters */
#elif defined(BT815_ENABLE) || defined(BT816_ENABLE) || defined(BT817_ENABLE)
#define LOW_FREQ_BOUND 58800000L // 98% of 60Mhz

#define RAM_G_SIZE					   (1024 * 1024L)

/* Copied from Auto-generated vc.h */
#define VC1_VERSION					   "1.5.8"
#define FLASH_DEFAULT_SHA1KEY		   0xf589cf07
#define PPC							   16
#define BANKS						   16
#define BANKW						   16
#define ADC_DIFFERENTIAL			   1UL
#define ADC_SINGLE_ENDED			   0UL
#define ADPCM_SAMPLES				   2UL
#define ALPHA						   5UL
#define ALWAYS						   7UL
#define ANIM_HOLD					   2UL
#define ANIM_LOOP					   1UL
#define ANIM_ONCE					   0UL
#define ARGB1555					   0UL
#define ARGB2						   5UL
#define ARGB4						   6UL
#define BARGRAPH					   11UL
#define BILINEAR					   1UL
#define BITMAPS						   1UL
#define BLUE						   4UL
#define BORDER						   0UL
#define CMDBUF_SIZE					   4096UL
#define CMD_ANIMDRAW				   4294967126UL
#define CMD_ANIMFRAME				   4294967130UL
#define CMD_ANIMSTART				   4294967123UL
#define CMD_ANIMSTOP				   4294967124UL
#define CMD_ANIMXY					   4294967125UL
#define CMD_APPEND					   4294967070UL
#define CMD_APPENDF					   4294967129UL
#define CMD_BGCOLOR					   4294967049UL
#define CMD_BITMAP_TRANSFORM		   4294967073UL
#define CMD_BUTTON					   4294967053UL
#define CMD_CALIBRATE				   4294967061UL
#define CMD_CLEARCACHE				   4294967119UL
#define CMD_CLOCK					   4294967060UL
#define CMD_COLDSTART				   4294967090UL
#define CMD_CRC						   4294967043UL
#define CMD_DEPRECATED_CSKETCH		   4294967093UL
#define CMD_DIAL					   4294967085UL
#define CMD_DLSTART					   4294967040UL
#define CMD_EXECUTE					   4294967047UL
#define CMD_FGCOLOR					   4294967050UL
#define CMD_FILLWIDTH				   4294967128UL
#define CMD_FLASHATTACH				   4294967113UL
#define CMD_FLASHDETACH				   4294967112UL
#define CMD_FLASHERASE				   4294967108UL
#define CMD_FLASHFAST				   4294967114UL
#define CMD_FLASHREAD				   4294967110UL
#define CMD_FLASHSOURCE				   4294967118UL
#define CMD_FLASHSPIDESEL			   4294967115UL
#define CMD_FLASHSPIRX				   4294967117UL
#define CMD_FLASHSPITX				   4294967116UL
#define CMD_FLASHUPDATE				   4294967111UL
#define CMD_FLASHWRITE				   4294967109UL
#define CMD_GAUGE					   4294967059UL
#define CMD_GETMATRIX				   4294967091UL
#define CMD_GETPOINT				   4294967048UL
#define CMD_GETPROPS				   4294967077UL
#define CMD_GETPTR					   4294967075UL
#define CMD_GRADCOLOR				   4294967092UL
#define CMD_GRADIENT				   4294967051UL
#define CMD_GRADIENTA				   4294967127UL
#define CMD_HAMMERAUX				   4294967044UL
#define CMD_HMAC					   4294967133UL
#define CMD_IDCT_DELETED			   4294967046UL
#define CMD_INFLATE					   4294967074UL
#define CMD_INFLATE2				   4294967120UL
#define CMD_INTERRUPT				   4294967042UL
#define CMD_INT_RAMSHARED			   4294967101UL
#define CMD_INT_SWLOADIMAGE			   4294967102UL
#define CMD_KEYS					   4294967054UL
#define CMD_LAST_					   4294967134UL
#define CMD_LOADIDENTITY			   4294967078UL
#define CMD_LOADIMAGE				   4294967076UL
#define CMD_LOGO					   4294967089UL
#define CMD_MARCH					   4294967045UL
#define CMD_MEDIAFIFO				   4294967097UL
#define CMD_MEMCPY					   4294967069UL
#define CMD_MEMCRC					   4294967064UL
#define CMD_MEMSET					   4294967067UL
#define CMD_MEMWRITE				   4294967066UL
#define CMD_MEMZERO					   4294967068UL
#define CMD_NOP						   4294967131UL
#define CMD_NUMBER					   4294967086UL
#define CMD_PLAYVIDEO				   4294967098UL
#define CMD_PROGRESS				   4294967055UL
#define CMD_REGREAD					   4294967065UL
#define CMD_RESETFONTS				   4294967122UL
#define CMD_ROMFONT					   4294967103UL
#define CMD_ROTATE					   4294967081UL
#define CMD_ROTATEAROUND			   4294967121UL
#define CMD_SCALE					   4294967080UL
#define CMD_SCREENSAVER				   4294967087UL
#define CMD_SCROLLBAR				   4294967057UL
#define CMD_SETBASE					   4294967096UL
#define CMD_SETBITMAP				   4294967107UL
#define CMD_SETFONT					   4294967083UL
#define CMD_SETFONT2				   4294967099UL
#define CMD_SETMATRIX				   4294967082UL
#define CMD_SETROTATE				   4294967094UL
#define CMD_SETSCRATCH				   4294967100UL
#define CMD_SHA1					   4294967132UL
#define CMD_SKETCH					   4294967088UL
#define CMD_SLIDER					   4294967056UL
#define CMD_SNAPSHOT				   4294967071UL
#define CMD_SNAPSHOT2				   4294967095UL
#define CMD_SPINNER					   4294967062UL
#define CMD_STOP					   4294967063UL
#define CMD_SWAP					   4294967041UL
#define CMD_SYNC					   4294967106UL
#define CMD_TEXT					   4294967052UL
#define CMD_TOGGLE					   4294967058UL
#define CMD_TOUCH_TRANSFORM			   4294967072UL
#define CMD_TRACK					   4294967084UL
#define CMD_TRANSLATE				   4294967079UL
#define CMD_VIDEOFRAME				   4294967105UL
#define CMD_VIDEOSTART				   4294967104UL
#define CMD_VIDEOSTARTF				   4294967135UL
#define COMPRESSED_RGBA_ASTC_10x10_KHR 37819UL
#define COMPRESSED_RGBA_ASTC_10x5_KHR  37816UL
#define COMPRESSED_RGBA_ASTC_10x6_KHR  37817UL
#define COMPRESSED_RGBA_ASTC_10x8_KHR  37818UL
#define COMPRESSED_RGBA_ASTC_12x10_KHR 37820UL
#define COMPRESSED_RGBA_ASTC_12x12_KHR 37821UL
#define COMPRESSED_RGBA_ASTC_4x4_KHR   37808UL
#define COMPRESSED_RGBA_ASTC_5x4_KHR   37809UL
#define COMPRESSED_RGBA_ASTC_5x5_KHR   37810UL
#define COMPRESSED_RGBA_ASTC_6x5_KHR   37811UL
#define COMPRESSED_RGBA_ASTC_6x6_KHR   37812UL
#define COMPRESSED_RGBA_ASTC_8x5_KHR   37813UL
#define COMPRESSED_RGBA_ASTC_8x6_KHR   37814UL
#define COMPRESSED_RGBA_ASTC_8x8_KHR   37815UL
#define CTOUCH_MODE_COMPATIBILITY	   1UL
#define CTOUCH_MODE_EXTENDED		   0UL
#define DECR						   4UL
#define DLSWAP_DONE					   0UL
#define DLSWAP_FRAME				   2UL
#define DLSWAP_LINE					   1UL
#define DST_ALPHA					   3UL
#define EDGE_STRIP_A				   7UL
#define EDGE_STRIP_B				   8UL
#define EDGE_STRIP_L				   6UL
#define EDGE_STRIP_R				   5UL
#define EQUAL						   5UL
#define FLASH_STATUS_BASIC			   2UL
#define FLASH_STATUS_DETACHED		   1UL
#define FLASH_STATUS_FULL			   3UL
#define FLASH_STATUS_INIT			   0UL
#define GEQUAL						   4UL
#define GLFORMAT					   31UL
#define GREATER						   3UL
#define GREEN						   3UL
#define INCR						   3UL
#define INT_CMDEMPTY				   32UL
#define INT_CMDFLAG					   64UL
#define INT_CONVCOMPLETE			   128UL
#define INT_G8						   18UL
#define INT_L8C						   12UL
#define INT_PLAYBACK				   16UL
#define INT_SOUND					   8UL
#define INT_SWAP					   1UL
#define INT_TAG						   4UL
#define INT_TOUCH					   2UL
#define INT_VGA						   13UL
#define INVERT						   5UL
#define JT_CYA						   9UL
#define JT_DRIVES					   4UL
#define JT_FRAMES					   6UL
#define JT_ONESHOT					   7UL
#define JT_PEN						   5UL
#define JT_PWM						   8UL
#define JT_RI_ADC_MODE				   14UL
#define JT_RI_CHARGE				   13UL
#define JT_RI_CTOUCH_EXTENDED		   14UL
#define JT_RI_EHOST_ID				   16UL
#define JT_RI_EHOST_X				   13UL
#define JT_RI_EHOST_Y				   17UL
#define JT_RI_INT					   45UL
#define JT_RI_MODE					   12UL
#define JT_RI_OVERSAMPLE			   16UL
#define JT_RI_RZTHRESH				   17UL
#define JT_RI_SETTLE				   15UL
#define JT_RI_SHORTS				   31UL
#define JT_RI_UART_BUSY				   30UL
#define JT_RI_XF_AH					   19UL
#define JT_RI_XF_AL					   18UL
#define JT_RI_XF_BH					   21UL
#define JT_RI_XF_BL					   20UL
#define JT_RI_XF_CH					   23UL
#define JT_RI_XF_CL					   22UL
#define JT_RI_XF_DH					   25UL
#define JT_RI_XF_DL					   24UL
#define JT_RI_XF_EH					   27UL
#define JT_RI_XF_EL					   26UL
#define JT_RI_XF_FH					   29UL
#define JT_RI_XF_FL					   28UL
#define JT_RO_ANALOG				   37UL
#define JT_RO_CTOUCH_TOUCH0_XY		   34UL
#define JT_RO_CTOUCH_TOUCH4X		   37UL
#define JT_RO_CTOUCH_TOUCH4Y		   33UL
#define JT_RO_CTOUCH_TOUCHA_XY		   32UL
#define JT_RO_CTOUCH_TOUCHB_XY		   35UL
#define JT_RO_CTOUCH_TOUCHC_XY		   36UL
#define JT_RO_DIRECT_XY				   35UL
#define JT_RO_DIRECT_Z1Z2			   36UL
#define JT_RO_EHOST_ACK				   41UL
#define JT_RO_FAULT					   41UL
#define JT_RO_INTERRUPT				   39UL
#define JT_RO_RAW_XY				   32UL
#define JT_RO_RZ					   33UL
#define JT_RO_SCREEN_XY				   34UL
#define JT_RO_SHORT_DETECT			   40UL
#define JT_RO_UART					   38UL
#define JT_RO_WAKE					   44UL
#define JT_R_SCRATCH				   11UL
#define JT_SCL						   42UL
#define JT_SDA						   43UL
#define JT_TICK						   10UL
#define JT_XD						   3UL
#define JT_XEOC						   2UL
#define JT_XMCLK					   0UL
#define JT_XSAMP_CLK				   1UL
#define KEEP						   1UL
#define L1							   1UL
#define L2							   17UL
#define L4							   2UL
#define L8							   3UL
#define LEQUAL						   2UL
#define LESS						   1UL
#define LINEAR_SAMPLES				   0UL
#define LINES						   3UL
#define LINE_STRIP					   4UL
#define NEAREST						   0UL
#define NEVER						   0UL
#define NOTEQUAL					   6UL
#define ONE							   1UL
#define ONE_MINUS_DST_ALPHA			   5UL
#define ONE_MINUS_SRC_ALPHA			   4UL
#define OPT_CENTER					   1536UL
#define OPT_CENTERX					   512UL
#define OPT_CENTERY					   1024UL
#define OPT_FILL					   8192UL
#define OPT_FLASH					   64UL
#define OPT_FLAT					   256UL
#define OPT_FORMAT					   4096UL
#define OPT_FULLSCREEN				   8UL
#define OPT_MEDIAFIFO				   16UL
#define OPT_MONO					   1UL
#define OPT_NOBACK					   4096UL
#define OPT_NODL					   2UL
#define OPT_NOHANDS					   49152UL
#define OPT_NOHM					   16384UL
#define OPT_NOPOINTER				   16384UL
#define OPT_NOSECS					   32768UL
#define OPT_NOTEAR					   4UL
#define OPT_NOTICKS					   8192UL
#define OPT_OVERLAY					   128UL
#define OPT_RIGHTX					   2048UL
#define OPT_SIGNED					   256UL
#define OPT_SOUND					   32UL
#define PALETTED					   8UL
#define PALETTED4444				   15UL
#define PALETTED565					   14UL
#define PALETTED8					   16UL
#define POINTS						   2UL
#define RAM_BIST					   3670016UL
#define RAM_CMD						   3178496UL
#define RAM_DL						   3145728UL
#define RAM_G						   0UL
#define RAM_J1RAM					   3182592UL
#define RAM_JTBOOT					   3190784UL
#define RAM_REG						   3153920UL
#define RAM_ROMSUB					   3186688UL
#define RAM_TOP						   3162112UL
#define RECTS						   9UL
#define RED							   2UL
#define REG_ADAPTIVE_FRAMERATE		   3155324UL
#define REG_ANALOG					   3154284UL
#define REG_ANA_COMP				   3154308UL
#define REG_ANIM_ACTIVE				   3182636UL
#define REG_BIST_EN					   3154292UL
#define REG_BUSYBITS				   3154152UL
#define REG_CLOCK					   3153928UL
#define REG_CMDB_SPACE				   3155316UL
#define REG_CMDB_WRITE				   3155320UL
#define REG_CMD_DL					   3154176UL
#define REG_CMD_READ				   3154168UL
#define REG_CMD_WRITE				   3154172UL
#define REG_CPURESET				   3153952UL
#define REG_CRC						   3154296UL
#define REG_CSPREAD					   3154024UL
#define REG_CTOUCH_EXTENDED			   3154184UL
#define REG_CTOUCH_TOUCH0_XY		   3154212UL
#define REG_CTOUCH_TOUCH4_X			   3154284UL
#define REG_CTOUCH_TOUCH4_Y			   3154208UL
#define REG_CTOUCH_TOUCHA_XY		   3154204UL
#define REG_CTOUCH_TOUCHB_XY		   3154316UL
#define REG_CTOUCH_TOUCHC_XY		   3154320UL
#define REG_CYA0					   3154144UL
#define REG_CYA1					   3154148UL
#define REG_CYA_TOUCH				   3154280UL
#define REG_DATESTAMP				   3155300UL
#define REF_UNDERRUN				   0x30260Cul
#define REG_AH_HCYCLE_MAX			   0x302610UL
#define REG_DF_TUNED				   3182640UL
#define REG_DITHER					   3154016UL
#define REG_DLSWAP					   3154004UL
#define REG_EHOST_TOUCH_ACK			   3154288UL
#define REG_EHOST_TOUCH_ID			   3154196UL
#define REG_EHOST_TOUCH_X			   3154188UL
#define REG_EHOST_TOUCH_Y			   3154200UL
#define REG_EJPG_ACC				   3154776UL
#define REG_EJPG_BUSY				   3154328UL
#define REG_EJPG_DAT				   3154332UL
#define REG_EJPG_DCC				   3154752UL
#define REG_EJPG_DEBUG				   3155292UL
#define REG_EJPG_DST				   3154340UL
#define REG_EJPG_FORMAT				   3154352UL
#define REG_EJPG_H					   3154348UL
#define REG_EJPG_HT					   3154496UL
#define REG_EJPG_OPTIONS			   3154336UL
#define REG_EJPG_Q					   3154368UL
#define REG_EJPG_READY				   3154324UL
#define REG_EJPG_RI					   3154356UL
#define REG_EJPG_SCALE				   3155288UL
#define REG_EJPG_TDA				   3154364UL
#define REG_EJPG_TQ					   3154360UL
#define REG_EJPG_W					   3154344UL
#define REG_ESPIM_ADD				   3155356UL
#define REG_ESPIM_COUNT				   3155360UL
#define REG_ESPIM_DUMMY				   3155428UL
#define REG_ESPIM_READSTART			   3155336UL
#define REG_ESPIM_SEQ				   3155340UL
#define REG_ESPIM_TRIG				   3155432UL
#define REG_ESPIM_WINDOW			   3155364UL
#define REG_FLASH_SIZE				   3182628UL
#define REG_FLASH_STATUS			   3155440UL
#define REG_FRAMES					   3153924UL
#define REG_FREQUENCY				   3153932UL
#define REG_FULLBUSYBITS			   3155444UL
#define REG_GPIO					   3154068UL
#define REG_GPIOX					   3154076UL
#define REG_GPIOX_DIR				   3154072UL
#define REG_GPIO_DIR				   3154064UL
#define REG_HCYCLE					   3153964UL
#define REG_HOFFSET					   3153968UL
#define REG_HSIZE					   3153972UL
#define REG_HSYNC0					   3153976UL
#define REG_HSYNC1					   3153980UL
#define REG_ID						   3153920UL
#define REG_INT_EN					   3154092UL
#define REG_INT_FLAGS				   3154088UL
#define REG_INT_MASK				   3154096UL
#define REG_J1_COLD					   3154080UL
#define REG_J1_INT					   3154084UL
#define REG_MACRO_0					   3154136UL
#define REG_MACRO_1					   3154140UL
#define REG_MEDIAFIFO_BASE			   3182620UL
#define REG_MEDIAFIFO_READ			   3182612UL
#define REG_MEDIAFIFO_SIZE			   3182624UL
#define REG_MEDIAFIFO_WRITE			   3182616UL
#define REG_OUTBITS					   3154012UL
#define REG_PATCHED_ANALOG			   3154288UL
#define REG_PATCHED_TOUCH_FAULT		   3154284UL
#define REG_PCLK					   3154032UL
#define REG_PCLK_POL				   3154028UL
#define REG_PLAY					   3154060UL
#define REG_PLAYBACK_FORMAT			   3154116UL
#define REG_PLAYBACK_FREQ			   3154112UL
#define REG_PLAYBACK_LENGTH			   3154104UL
#define REG_PLAYBACK_LOOP			   3154120UL
#define REG_PLAYBACK_PAUSE			   3155436UL
#define REG_PLAYBACK_PLAY			   3154124UL
#define REG_PLAYBACK_READPTR		   3154108UL
#define REG_PLAYBACK_START			   3154100UL
#define REG_PLAY_CONTROL			   3182926UL
#define REG_PWM_DUTY				   3154132UL
#define REG_PWM_HZ					   3154128UL
#define REG_RAM_FOLD				   3154164UL
#define REG_RASTERY					   3155296UL
#define REG_RENDERMODE				   3153936UL
#define REG_ROMSUB_SEL				   3154160UL
#define REG_ROTATE					   3154008UL
#define REG_SHA1KEY					   3155460UL
#define REG_SNAPFORMAT				   3153948UL
#define REG_SNAPSHOT				   3153944UL
#define REG_SNAPY					   3153940UL
#define REG_SOUND					   3154056UL
#define REG_SPIM					   3155332UL
#define REG_SPIM_DIR				   3155328UL
#define REG_SPI_EARLY_TX			   3154300UL
#define REG_SPI_WIDTH				   3154312UL
#define REG_SWIZZLE					   3154020UL
#define REG_TAG						   3154044UL
#define REG_TAG_X					   3154036UL
#define REG_TAG_Y					   3154040UL
#define REG_TAP_CRC					   3153956UL
#define REG_TAP_MASK				   3153960UL
#define REG_TOUCH_ADC_MODE			   3154184UL
#define REG_TOUCH_CHARGE			   3154188UL
#define REG_TOUCH_DIRECT_XY			   3154316UL
#define REG_TOUCH_DIRECT_Z1Z2		   3154320UL
#define REG_TOUCH_FAULT				   3154288UL
#define REG_TOUCH_MODE				   3154180UL
#define REG_TOUCH_OVERSAMPLE		   3154196UL
#define REG_TOUCH_RAW_XY			   3154204UL
#define REG_TOUCH_RZ				   3154208UL
#define REG_TOUCH_RZTHRESH			   3154200UL
#define REG_TOUCH_SCREEN_XY			   3154212UL
#define REG_TOUCH_SETTLE			   3154192UL
#define REG_TOUCH_TAG				   3154220UL
#define REG_TOUCH_TAG1				   3154228UL
#define REG_TOUCH_TAG1_XY			   3154224UL
#define REG_TOUCH_TAG2				   3154236UL
#define REG_TOUCH_TAG2_XY			   3154232UL
#define REG_TOUCH_TAG3				   3154244UL
#define REG_TOUCH_TAG3_XY			   3154240UL
#define REG_TOUCH_TAG4				   3154252UL
#define REG_TOUCH_TAG4_XY			   3154248UL
#define REG_TOUCH_TAG_XY			   3154216UL
#define REG_TOUCH_TRANSFORM_A		   3154256UL
#define REG_TOUCH_TRANSFORM_B		   3154260UL
#define REG_TOUCH_TRANSFORM_C		   3154264UL
#define REG_TOUCH_TRANSFORM_D		   3154268UL
#define REG_TOUCH_TRANSFORM_E		   3154272UL
#define REG_TOUCH_TRANSFORM_F		   3154276UL
#define REG_TRACKER					   3182592UL
#define REG_TRACKER_1				   3182596UL
#define REG_TRACKER_2				   3182600UL
#define REG_TRACKER_3				   3182604UL
#define REG_TRACKER_4				   3182608UL
#define REG_TRIM					   3154304UL
#define REG_VCYCLE					   3153984UL
#define REG_VOFFSET					   3153988UL
#define REG_VOL_PB					   3154048UL
#define REG_VOL_SOUND				   3154052UL
#define REG_VSIZE					   3153992UL
#define REG_VSYNC0					   3153996UL
#define REG_VSYNC1					   3154000UL
#define REPEAT						   1UL
#define REPLACE						   2UL
#define RGB332						   4UL
#define RGB565						   7UL
#define SRC_ALPHA					   2UL
#define SS_A0						   19UL
#define SS_A1						   20UL
#define SS_A2						   21UL
#define SS_A3						   22UL
#define SS_A4						   23UL
#define SS_A5						   24UL
#define SS_A6						   25UL
#define SS_A7						   26UL
#define SS_PAUSE					   18UL
#define SS_Q0						   0UL
#define SS_Q1						   1UL
#define SS_Q2						   2UL
#define SS_Q3						   3UL
#define SS_Q4						   4UL
#define SS_Q5						   5UL
#define SS_Q6						   6UL
#define SS_Q7						   7UL
#define SS_Q8						   8UL
#define SS_Q9						   9UL
#define SS_QA						   10UL
#define SS_QB						   11UL
#define SS_QC						   12UL
#define SS_QD						   13UL
#define SS_QE						   14UL
#define SS_QF						   15UL
#define SS_QI						   31UL
#define SS_S0						   16UL
#define SS_S1						   17UL
#define TEXT8X8						   9UL
#define TEXTVGA						   10UL
#define TOUCHMODE_CONTINUOUS		   3UL
#define TOUCHMODE_FRAME				   2UL
#define TOUCHMODE_OFF				   0UL
#define TOUCHMODE_ONESHOT			   1UL
#define ULAW_SAMPLES				   1UL
#define ZERO						   0UL

#define VERTEX2F(x, y)									 ((1UL << 30) | (((x)&32767UL) << 15) | (((y)&32767UL) << 0))
#define VERTEX2II(x, y, handle, cell)					 ((2UL << 30) | (((x)&511UL) << 21) | (((y)&511UL) << 12) | (((handle)&31UL) << 7) | (((cell)&127UL) << 0))
#define BITMAP_SOURCE(addr)								 ((1UL << 24) | (((addr)&4194303UL) << 0))
#define BITMAP_SOURCE2(flash_or_ram, addr)				 ((1UL << 24) | ((flash_or_ram) << 23) | (((addr)&8388607UL) << 0))
#define CLEAR_COLOR_RGB(red, green, blue)				 ((2UL << 24) | (((red)&255UL) << 16) | (((green)&255UL) << 8) | (((blue)&255UL) << 0))
#define TAG(s)											 ((3UL << 24) | (((s)&255UL) << 0))
#define COLOR_RGB(red, green, blue)						 ((4UL << 24) | (((red)&255UL) << 16) | (((green)&255UL) << 8) | (((blue)&255UL) << 0))
#define BITMAP_HANDLE(handle)							 ((5UL << 24) | (((handle)&31UL) << 0))
#define CELL(cell)										 ((6UL << 24) | (((cell)&127UL) << 0))
#define BITMAP_LAYOUT(format, linestride, height)		 ((7UL << 24) | (((format)&31UL) << 19) | (((linestride)&1023UL) << 9) | (((height)&511UL) << 0))
#define BITMAP_SIZE(filter, wrapx, wrapy, width, height) ((8UL << 24) | (((filter)&1UL) << 20) | (((wrapx)&1UL) << 19) | (((wrapy)&1UL) << 18) | (((width)&511UL) << 9) | (((height)&511UL) << 0))
#define ALPHA_FUNC(func, ref)							 ((9UL << 24) | (((func)&7UL) << 8) | (((ref)&255UL) << 0))
#define STENCIL_FUNC(func, ref, mask)					 ((10UL << 24) | (((func)&7UL) << 16) | (((ref)&255UL) << 8) | (((mask)&255UL) << 0))
#define BLEND_FUNC(src, dst)							 ((11UL << 24) | (((src)&7UL) << 3) | (((dst)&7UL) << 0))
#define STENCIL_OP(sfail, spass)						 ((12UL << 24) | (((sfail)&7UL) << 3) | (((spass)&7UL) << 0))
#define POINT_SIZE(size)								 ((13UL << 24) | (((size)&8191UL) << 0))
#define LINE_WIDTH(width)								 ((14UL << 24) | (((width)&4095UL) << 0))
#define CLEAR_COLOR_A(alpha)							 ((15UL << 24) | (((alpha)&255UL) << 0))
#define COLOR_A(alpha)									 ((16UL << 24) | (((alpha)&255UL) << 0))
#define CLEAR_STENCIL(s)								 ((17UL << 24) | (((s)&255UL) << 0))
#define CLEAR_TAG(s)									 ((18UL << 24) | (((s)&255UL) << 0))
#define STENCIL_MASK(mask)								 ((19UL << 24) | (((mask)&255UL) << 0))
#define TAG_MASK(mask)									 ((20UL << 24) | (((mask)&1UL) << 0))
#define BITMAP_TRANSFORM_A(p, v)						 ((21UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_B(p, v)						 ((22UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_C(c)							 ((23UL << 24) | (((c)&16777215UL) << 0))
#define BITMAP_TRANSFORM_D(p, v)						 ((24UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_E(p, v)						 ((25UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_F(f)							 ((26UL << 24) | (((f)&16777215UL) << 0))
#define SCISSOR_XY(x, y)								 ((27UL << 24) | (((x)&2047UL) << 11) | (((y)&2047UL) << 0))
#define SCISSOR_SIZE(width, height)						 ((28UL << 24) | (((width)&4095UL) << 12) | (((height)&4095UL) << 0))
#define CALL(dest)										 ((29UL << 24) | (((dest)&65535UL) << 0))
#define JUMP(dest)										 ((30UL << 24) | (((dest)&65535UL) << 0))
#define BEGIN(prim)										 ((31UL << 24) | (((prim)&15UL) << 0))
#define COLOR_MASK(r, g, b, a)							 ((32UL << 24) | (((r)&1UL) << 3) | (((g)&1UL) << 2) | (((b)&1UL) << 1) | (((a)&1UL) << 0))
#define CLEAR(c, s, t)									 ((38UL << 24) | (((c)&1UL) << 2) | (((s)&1UL) << 1) | (((t)&1UL) << 0))
#define VERTEX_FORMAT(frac)								 ((39UL << 24) | (((frac)&7UL) << 0))
#define BITMAP_LAYOUT_H(linestride, height)				 ((40UL << 24) | (((linestride)&3UL) << 2) | (((height)&3UL) << 0))
#define BITMAP_SIZE_H(width, height)					 ((41UL << 24) | (((width)&3UL) << 2) | (((height)&3UL) << 0))
#define PALETTE_SOURCE(addr)							 ((42UL << 24) | (((addr)&4194303UL) << 0))
#define VERTEX_TRANSLATE_X(x)							 ((43UL << 24) | (((x)&131071UL) << 0))
#define VERTEX_TRANSLATE_Y(y)							 ((44UL << 24) | (((y)&131071UL) << 0))
#define NOP()											 ((45UL << 24))
#define BITMAP_EXT_FORMAT(format)						 ((46UL << 24) | (((format)&65535UL) << 0))
#define BITMAP_SWIZZLE(r, g, b, a)						 ((47UL << 24) | (((r)&7UL) << 9) | (((g)&7UL) << 6) | (((b)&7UL) << 3) | (((a)&7UL) << 0))
#define INT_FRR()										 ((48UL << 24))
#define END()											 ((33UL << 24))
#define SAVE_CONTEXT()									 ((34UL << 24))
#define RESTORE_CONTEXT()								 ((35UL << 24))
#define RETURN()										 ((36UL << 24))
#define MACRO(m)										 ((37UL << 24) | (((m)&1UL) << 0))
#define DISPLAY()										 ((0UL << 24))

/*--- End Auto-generated vc.h ---*/

#define RAM_ERR_REPORT		 0x309800
/* For BT815, the location is same as ft810 */
#define ROMFONT_TABLEADDRESS 3145724UL
/* Added to ensure previous macros are fine */
#define REG_CTOUCH_TOUCH1_XY 3154204UL
#define REG_CTOUCH_TOUCH2_XY 3154316UL
#define REG_CTOUCH_TOUCH3_XY 3154320UL
#define FTPOINTS			 POINTS

#undef BITMAP_TRANSFORM_A // New BT815 file do not compatible with the legacy code
#undef BITMAP_TRANSFORM_B // New BT815 file do not compatible with the legacy code
#undef BITMAP_TRANSFORM_D // New BT815 file do not compatible with the legacy code
#undef BITMAP_TRANSFORM_E // New BT815 file do not compatible with the legacy code

#define BITMAP_TRANSFORM_A_EXT(p, v) ((21UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_B_EXT(p, v) ((22UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_C(c)		 ((23UL << 24) | (((c)&16777215UL) << 0))
#define BITMAP_TRANSFORM_D_EXT(p, v) ((24UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_E_EXT(p, v) ((25UL << 24) | (((p)&1UL) << 17) | (((v)&131071UL) << 0))
#define BITMAP_TRANSFORM_F(f)		 ((26UL << 24) | (((f)&16777215UL) << 0))

/* Added to ensure previous macros are fine */
#define BITMAP_TRANSFORM_A(a)		 BITMAP_TRANSFORM_A_EXT(0, a)
#define BITMAP_TRANSFORM_B(b)		 BITMAP_TRANSFORM_B_EXT(0, b)
#define BITMAP_TRANSFORM_D(d)		 BITMAP_TRANSFORM_D_EXT(0, d)
#define BITMAP_TRANSFORM_E(e)		 BITMAP_TRANSFORM_E_EXT(0, e)

/* End BT81x parameters */
#else
#error "Please select a proper chip"
#endif

/* For FT801, FT811 and FT813 */
#ifndef CTOUCH_MODE_COMPATIBILITY
#define CTOUCH_MODE_COMPATIBILITY 1
#endif
#ifndef CTOUCH_MODE_EXTENDED
#define CTOUCH_MODE_EXTENDED 0
#endif

/* ==================================================================================== */

#define GPU_NUMCHAR_PERFONT (128)
#define GPU_FONT_TABLE_SIZE (148)

/* FT81x and FT80x font table structure */
/* Font table address in ROM can be found by reading the address from 0xFFFFC location. */
/* 16 font tables are present at the address read from location 0xFFFFC */
typedef struct Gpu_Fonts {
    /* All the values are in bytes */
    /* Width of each character font from 0 to 127 */
    uint8_t FontWidth[GPU_NUMCHAR_PERFONT];
    /* Bitmap format of font wrt bitmap formats supported by FT800 - L1, L4, L8 */
    uint32_t FontBitmapFormat;
    /* Font line stride in FT800 ROM */
    uint32_t FontLineStride;
    /* Font width in pixels */
    uint32_t FontWidthInPixels;
    /* Font height in pixels */
    uint32_t FontHeightInPixels;
    /* Pointer to font graphics raw data */
    uint32_t PointerToFontGraphicsData;
} Gpu_Fonts_t;

/* BT815 inclusion - extended font table for unicode support */
typedef struct Gpu_FontsExt_t {
    uint32_t Signature;
    uint32_t Size;
    uint32_t Format;
    uint32_t Swizzle;
    uint32_t LayoutWidth;
    uint32_t LayoutHeight;
    uint32_t PixelWidth;
    uint32_t pixelHeight;
    uint32_t StartOfGraphicData;
    uint32_t OffsetGlyphData;
    // uint32_t OffsetWidthData;//to be OffsetWidthData=Starting adress+StartOfGraphicData*4.
} Gpu_FontsExt_t;

#endif /* #ifndef _FT_GPU_H_ */

/* Nothing beyond this */
