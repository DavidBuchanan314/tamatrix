#ifndef TAMAEMU_H
#define TAMAEMU_H

#include <stdint.h>
#include "M6502/M6502.h"

#define R_BANK		0x3000
#define R_CLKCTL	0x3001
#define R_C32KCTL	0x3002
#define R_WDTCTL	0x3004
#define R_RESFL		0x3005
#define R_SPUCTL	0x3006
#define R_WAKEEN	0x3007
#define R_WAKEFL	0x3008
#define R_RESENA	0x300B
#define R_RESCTL	0x300C
#define R_PACFG		0x3010
#define R_PADIR		0x3011
#define R_PADATA	0x3012
#define R_PASTR		0x3013
#define R_PBCFG		0x3014
#define R_PBDIR		0x3015
#define R_PBDATA	0x3016
#define R_PCCFG		0x3017
#define R_PCDIR		0x3018
#define R_PCDATA	0x3019
#define R_TIMBASE	0x3030
#define R_TIMCTL	0x3031
#define R_TM0LOW	0x3032
#define R_TM0HIGH	0x3033
#define R_TM0LOW	0x3034
#define R_TM0HIGH	0x3035
#define R_CARCTL	0x303C
#define R_KEYSCTL	0x303D
#define R_KEYSP1	0x303E
#define R_KEYSP2	0x303F
#define R_LCDS1		0x3040
#define R_LCDS2		0x3041
#define R_LCDC1		0x3042
#define R_LCDC2		0x3043
#define R_LCDSEG	0x3044
#define R_LCDCOM	0x3045
#define R_LCDFRMCTL	0x3046
#define R_LCDBUFCTL	0x3047
#define R_VLCDCTL	0x3048
#define R_PUMPCTL	0x3049
#define R_BIASCTL	0x304A
#define R_MVCL		0x3051
#define R_MIXR		0x3052
#define R_MIXL		0x3053
#define R_SPUEN		0x3054
#define R_SPUINTSTS	0x3055
#define R_SPUINTEN	0x3056
#define R_CONC		0x3057
#define R_MULA		0x3058
#define R_MULB		0x3059
#define R_MULOUTH	0x305A
#define R_MULOUTL	0x305B
#define R_MULACT	0x305C
#define R_AUTOMUTE	0x305E
#define R_DAC1H		0x3060
#define R_DAC1L		0x3062
#define R_SOFTCTL	0x3064
#define R_DACPWM	0x3065
#define R_INTCTLLO	0x3070
#define R_INTCTLMI	0x3071
#define R_INTCLRLO	0x3073
#define R_INTCLRMI	0x3074
#define R_NMICTL	0x3076
#define R_LVCTL		0x3077
//data regs omitted
#define R_IFFPCLR	0x3090
#define R_IF8KCLR	0x3093
#define R_IF2KCLR	0x3094
#define R_IFTM0CLR	0x3097
#define R_IFTBLCLR	0x309A
#define R_IFTBHCLR	0x309B
#define R_IFTM1CLR	0x309D
#define R_SPICTL	0x30B0
#define R_SPITXSTS	0x30B1
#define R_SPITXCTL	0x30B2
#define R_SPITXDAT	0x30B3
#define R_SPIRXSTS	0x30B4
#define R_SPIRXCTL	0x30B5
#define R_SPIRXDAT	0x30B6
#define R_SPIMISC	0x30B7
#define R_SPIPORT	0x30BA

typedef struct {
	uint8_t strobe;
	uint8_t dir;
	uint8_t config;
	uint8_t data;
} TamaGpio;

typedef struct {
	uint8_t bankSel;
	uint8_t clkCtl;
	TamaGpio gpioa;
	TamaGpio gpiob;
} TamaHw;


typedef struct {
	M6502 *cpu;
	unsigned char **rom;
	char ram[1536];
	char dram[512];
	char ioreg[255];
	TamaHw hw;
} Tamagotchi;


unsigned char **loadRoms();
void freeRoms(unsigned char **roms);

Tamagotchi *tamaInit(unsigned char **rom);
void tamaRun(Tamagotchi *tama, int cycles);


#endif