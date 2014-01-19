/*
 * Standard NAND flash commands
 */
#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1
#define NAND_CMD_RNDOUT		5
#define NAND_CMD_PAGEPROG	0x10
#define NAND_CMD_READOOB	0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_RNDIN		0x85
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_PARAM		0xec
#define NAND_CMD_RESET		0xff
#define NAND_CMD_LOCK		0x2a
#define NAND_CMD_UNLOCK1	0x23
#define NAND_CMD_UNLOCK2	0x24
/* Extended commands for large page devices */
#define NAND_CMD_READSTART	0x30
#define NAND_CMD_RNDOUTSTART	0xE0
#define NAND_CMD_CACHEDPROG	0x15
#define NAND_CMD_ONFI		0xEC

#define NAND_STATUS_FAIL 			(1<<0) /* HIGH - FAIL,  LOW - PASS */
#define NAND_STATUS_IDLE 			(1<<5) /* HIGH - IDLE,  LOW - ACTIVE */
#define NAND_STATUS_READY 			(1<<6) /* HIGH - READY, LOW - BUSY */
#define NAND_STATUS_NOT_PROTECTED 	(1<<7) /* HIGH - NOT,   LOW - PROTECTED */
