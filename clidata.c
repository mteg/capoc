#include "clidata.h"
#include "cli_defs.h"
unsigned char cli_tab[] = {
/* Listing of table:  */
	64, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 1, 227 /* .delay */ , 50, 'I', 'n', 't', 'r', 'o', 'd', 'u', 'c', 'e', ' ', 'd', 'e', 'l', 'a', 'y', ' ', 'b', 'e', 'f', 'o', 'r', 'e', ' ', 'e', 'x', 'e', 'c', 'u', 't', 'i', 'n', 'g', ' ', 'f', 'o', 'l', 'l', 'o', 'w', 'i', 'n', 'g', ' ', 'c', 'o', 'm', 'm', 'a', 'n', 'd', 5, 'd', 'e', 'l', 'a', 'y',
	52, T_CAMERA >> 8, T_CAMERA & 0xff, TOK_KEYWORD, 0, 2, 1 /* .camera */ , 37, 'A', 'l', 't', 'e', 'r', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'o', 's', 'i', 't', 'i', 'o', 'n', ' ', 'a', 'n', 'd', ' ', 'o', 'r', 'i', 'e', 'n', 't', 'a', 't', 'i', 'o', 'n', 6, 'c', 'a', 'm', 'e', 'r', 'a',
	39, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 6, 140 /* .bind */ , 26, 'B', 'i', 'n', 'd', ' ', 'a', 'n', ' ', 'a', 'c', 't', 'i', 'o', 'n', ' ', 't', 'o', ' ', 'k', 'e', 'y', ' ', 'c', 'o', 'd', 'e', 4, 'b', 'i', 'n', 'd',
	38, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 9, 201 /* .keymap */ , 23, 'W', 'o', 'r', 'k', ' ', 'o', 'n', ' ', 's', 'p', 'e', 'c', 'i', 'f', 'i', 'c', ' ', 'k', 'e', 'y', 'm', 'a', 'p', 6, 'k', 'e', 'y', 'm', 'a', 'p',
	35, T_VIEW >> 8, T_VIEW & 0xff, TOK_KEYWORD, 0, 10, 105 /* .view */ , 22, 'C', 'h', 'a', 'n', 'g', 'e', ' ', 'v', 'i', 'e', 'w', ' ', 'p', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 's', 4, 'v', 'i', 'e', 'w',
	44, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 15, 17 /* .model */ , 30, 'L', 'o', 'a', 'd', ' ', 'o', 'r', ' ', 'c', 'h', 'a', 'n', 'g', 'e', ' ', 'm', 'o', 'd', 'e', 'l', ' ', 's', 'p', 'e', 'c', 'i', 'f', 'i', 'c', 's', 5, 'm', 'o', 'd', 'e', 'l',
	27, T_LIGHT >> 8, T_LIGHT & 0xff, TOK_KEYWORD, 0, 40, 218 /* .light */ , 13, 'C', 'o', 'n', 't', 'r', 'o', 'l', ' ', 'l', 'i', 'g', 'h', 't', 5, 'l', 'i', 'g', 'h', 't',
	27, T_EXIT >> 8, T_EXIT & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 14, 'D', 'i', 's', 'c', 'o', 'n', 'n', 'e', 'c', 't', ' ', 'C', 'L', 'I', 4, 'e', 'x', 'i', 't',
	27, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 42, 152 /* .exec */ , 14, 'E', 'x', 'e', 'c', 'u', 't', 'e', ' ', 'a', ' ', 'f', 'i', 'l', 'e', 4, 'e', 'x', 'e', 'c',
	45, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 42, 178 /* .flag */ , 32, 'S', 'e', 't', ' ', 'o', 'r', ' ', 'r', 'e', 's', 'e', 't', ' ', 'm', 'i', 's', 'c', 'e', 'l', 'l', 'a', 'n', 'e', 'o', 'u', 's', ' ', 'f', 'l', 'a', 'g', 's', 4, 'f', 'l', 'a', 'g',
	44, T_LOG >> 8, T_LOG & 0xff, TOK_KEYWORD, 0, 43, 246 /* .log */ , 32, 'S', 'e', 't', ' ', 'a', 'n', 'd', ' ', 'u', 's', 'e', ' ', 'm', 'e', 'a', 's', 'u', 'r', 'e', 'm', 'e', 'n', 't', ' ', 'l', 'o', 'g', ' ', 'f', 'i', 'l', 'e', 3, 'l', 'o', 'g',
	40, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 44, 215 /* .set */ , 28, 'S', 'e', 't', ' ', 'm', 'i', 's', 'c', 'e', 'l', 'l', 'a', 'n', 'e', 'o', 'u', 's', ' ', 'p', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 's', 3, 's', 'e', 't',
TOK_END, 

/* Listing of table: .delay */
	29, T_DELAY >> 8, T_DELAY & 0xff, TOK_NUM, 0, 0, 0 /* . */ , 21, 'D', 'e', 'l', 'a', 'y', ' ', 'i', 'n', ' ', 'm', 'i', 'l', 'l', 'i', 's', 'e', 'c', 'o', 'n', 'd', 's',
TOK_END, 

/* Listing of table: .camera */
	41, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 4, 12 /* .camera.capture */ , 25, 'R', 'e', 'n', 'd', 'e', 'r', ' ', 'a', 'n', 'd', ' ', 's', 'a', 'v', 'e', ' ', 'i', 'n', 't', 'o', ' ', 'f', 'i', 'l', 'e', 7, 'c', 'a', 'p', 't', 'u', 'r', 'e',
	39, T_LOG >> 8, T_LOG & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 27, 'L', 'o', 'g', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'o', 's', 'i', 't', 'i', 'o', 'n', 3, 'l', 'o', 'g',
	42, T_SET >> 8, T_SET & 0xff, TOK_KEYWORD, 0, 4, 30 /* .camera.set */ , 30, 'S', 'e', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'o', 's', 'i', 't', 'i', 'o', 'n', ' ', 'a', 'n', 'd', ' ', 'a', 'n', 'g', 'l', 'e', 's', 3, 's', 'e', 't',
	32, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 4, 139 /* .camera.configure */ , 14, 'S', 'e', 't', ' ', 'u', 'n', 'i', 't', ' ', 's', 't', 'e', 'p', 's', 9, 'c', 'o', 'n', 'f', 'i', 'g', 'u', 'r', 'e',
	37, T_SLIGHTLY >> 8, T_SLIGHTLY & 0xff, TOK_KEYWORD, 0, 2, 1 /* .camera */ , 20, 'M', 'u', 'l', 't', 'i', 'p', 'l', 'y', ' ', 's', 't', 'e', 'p', ' ', 'b', 'y', ' ', '0', '.', '1', 8, 's', 'l', 'i', 'g', 'h', 't', 'l', 'y',
	53, T_ADVANCE >> 8, T_ADVANCE & 0xff, TOK_KEYWORD, 0, 5, 11 /* .camera.advance */ , 37, 'W', 'a', 'l', 'k', ' ', 'f', 'o', 'r', 'w', 'a', 'r', 'd', ' ', 'i', 'n', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'y', 'a', 'w', ' ', 'd', 'i', 'r', 'e', 'c', 't', 'i', 'o', 'n', 7, 'a', 'd', 'v', 'a', 'n', 'c', 'e',
	50, T_RETREAT >> 8, T_RETREAT & 0xff, TOK_KEYWORD, 0, 5, 11 /* .camera.advance */ , 34, 'W', 'a', 'l', 'k', ' ', 'b', 'a', 'c', 'k', ' ', 'i', 'n', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'y', 'a', 'w', ' ', 'd', 'i', 'r', 'e', 'c', 't', 'i', 'o', 'n', 7, 'r', 'e', 't', 'r', 'e', 'a', 't',
	48, T_LEFT >> 8, T_LEFT & 0xff, TOK_KEYWORD, 0, 5, 11 /* .camera.advance */ , 35, 'M', 'o', 'v', 'e', ' ', '9', '0', ' ', 'd', 'e', 'g', 'r', 'e', 'e', 's', ' ', 'l', 'e', 'f', 't', ' ', 't', 'o', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'y', 'a', 'w', 4, 'l', 'e', 'f', 't',
	50, T_RIGHT >> 8, T_RIGHT & 0xff, TOK_KEYWORD, 0, 5, 11 /* .camera.advance */ , 36, 'M', 'o', 'v', 'e', ' ', '9', '0', ' ', 'd', 'e', 'g', 'r', 'e', 'e', 's', ' ', 'r', 'i', 'g', 'h', 't', ' ', 't', 'o', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'y', 'a', 'w', 5, 'r', 'i', 'g', 'h', 't',
	18, T_UP >> 8, T_UP & 0xff, TOK_KEYWORD, 0, 5, 11 /* .camera.advance */ , 7, 'M', 'o', 'v', 'e', ' ', 'u', 'p', 2, 'u', 'p',
	22, T_DOWN >> 8, T_DOWN & 0xff, TOK_KEYWORD, 0, 5, 11 /* .camera.advance */ , 9, 'M', 'o', 'v', 'e', ' ', 'd', 'o', 'w', 'n', 4, 'd', 'o', 'w', 'n',
	40, T_LOOK >> 8, T_LOOK & 0xff, TOK_KEYWORD, 0, 5, 65 /* .camera.look */ , 27, 'C', 'h', 'a', 'n', 'g', 'e', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'y', 'a', 'w', ' ', 'o', 'r', ' ', 'p', 'i', 't', 'c', 'h', 4, 'l', 'o', 'o', 'k',
	50, T_XPLANE >> 8, T_XPLANE & 0xff, TOK_KEYWORD, 0, 5, 213 /* .camera.xplane */ , 35, 'C', 'o', 'n', 't', 'r', 'o', 'l', ' ', 'c', 'r', 'o', 's', 's', '-', 's', 'e', 'c', 't', 'i', 'o', 'n', ' ', 'p', 'l', 'a', 'n', 'e', ' ', 'd', 'i', 's', 'p', 'l', 'a', 'y', 6, 'x', 'p', 'l', 'a', 'n', 'e',
TOK_END, 

/* Listing of table: .camera.capture */
	17, T_CAPTURE >> 8, T_CAPTURE & 0xff, TOK_STRING, 255, 255, 255 /* End */ , 9, 'F', 'i', 'l', 'e', ' ', 'n', 'a', 'm', 'e',
TOK_END, 

/* Listing of table: .camera.set */
	20, T_X >> 8, T_X & 0xff, TOK_WORD, 0, 4, 51 /* .camera.set.<word> */ , 12, 'X', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e',
TOK_END, 

/* Listing of table: .camera.set.<word> */
	20, T_Y >> 8, T_Y & 0xff, TOK_WORD, 0, 4, 72 /* .camera.set.<word>.<word> */ , 12, 'Y', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e',
TOK_END, 

/* Listing of table: .camera.set.<word>.<word> */
	20, T_Z >> 8, T_Z & 0xff, TOK_WORD, 0, 4, 93 /* .camera.set.<word>.<word>.<word> */ , 12, 'Z', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e',
TOK_END, 

/* Listing of table: .camera.set.<word>.<word>.<word> */
	21, T_YAW >> 8, T_YAW & 0xff, TOK_WORD, 0, 4, 115 /* .camera.set.<word>.<word>.<word>.<word> */ , 13, 'Y', 'a', 'w', ' ', '(', 'd', 'e', 'g', 'r', 'e', 'e', 's', ')',
TOK_END, 

/* Listing of table: .camera.set.<word>.<word>.<word>.<word> */
	23, T_PITCH >> 8, T_PITCH & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 15, 'P', 'i', 't', 'c', 'h', ' ', '(', 'd', 'e', 'g', 'r', 'e', 'e', 's', ')',
TOK_END, 

/* Listing of table: .camera.configure */
	34, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 4, 211 /* .camera.configure.walk-step */ , 16, 'S', 'e', 't', ' ', 'w', 'a', 'l', 'k', 'i', 'n', 'g', ' ', 's', 't', 'e', 'p', 9, 'w', 'a', 'l', 'k', '-', 's', 't', 'e', 'p',
	37, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 4, 239 /* .camera.configure.rotate-step */ , 17, 'S', 'e', 't', ' ', 'r', 'o', 't', 'a', 't', 'i', 'o', 'n', ' ', 's', 't', 'e', 'p', 11, 'r', 'o', 't', 'a', 't', 'e', '-', 's', 't', 'e', 'p',
TOK_END, 

/* Listing of table: .camera.configure.walk-step */
	27, T_STEP >> 8, T_STEP & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 19, 'U', 'n', 'i', 't', ' ', 's', 't', 'e', 'p', ' ', '(', 'd', 'e', 'g', 'r', 'e', 'e', 's', ')',
TOK_END, 

/* Listing of table: .camera.configure.rotate-step */
	27, T_STEP >> 8, T_STEP & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 19, 'U', 'n', 'i', 't', ' ', 's', 't', 'e', 'p', ' ', '(', 'd', 'e', 'g', 'r', 'e', 'e', 's', ')',
TOK_END, 

/* Listing of table: .camera.advance */
	35, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 5, 47 /* .camera.advance.by */ , 24, 'W', 'a', 'l', 'k', ' ', 'b', 'y', ' ', 'a', ' ', 's', 'p', 'e', 'c', 'i', 'f', 'i', 'e', 'd', ' ', 's', 't', 'e', 'p', 2, 'b', 'y',
TOK_END, 

/* Listing of table: .camera.advance.by */
	17, T_STEP >> 8, T_STEP & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 9, 'U', 'n', 'i', 't', ' ', 's', 't', 'e', 'p',
TOK_END, 

/* Listing of table: .camera.look */
	22, T_LOOKLEFT >> 8, T_LOOKLEFT & 0xff, TOK_KEYWORD, 0, 5, 152 /* .camera.look.left */ , 9, 'L', 'o', 'o', 'k', ' ', 'l', 'e', 'f', 't', 4, 'l', 'e', 'f', 't',
	24, T_LOOKRIGHT >> 8, T_LOOKRIGHT & 0xff, TOK_KEYWORD, 0, 5, 152 /* .camera.look.left */ , 10, 'L', 'o', 'o', 'k', ' ', 'r', 'i', 'g', 'h', 't', 5, 'r', 'i', 'g', 'h', 't',
	18, T_LOOKUP >> 8, T_LOOKUP & 0xff, TOK_KEYWORD, 0, 5, 152 /* .camera.look.left */ , 7, 'L', 'o', 'o', 'k', ' ', 'u', 'p', 2, 'u', 'p',
	22, T_LOOKDOWN >> 8, T_LOOKDOWN & 0xff, TOK_KEYWORD, 0, 5, 152 /* .camera.look.left */ , 9, 'L', 'o', 'o', 'k', ' ', 'd', 'o', 'w', 'n', 4, 'd', 'o', 'w', 'n',
TOK_END, 

/* Listing of table: .camera.look.left */
	32, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 5, 185 /* .camera.look.left.by */ , 21, 'Y', 'a', 'w', ' ', 'b', 'y', ' ', 's', 'p', 'e', 'c', 'i', 'f', 'i', 'e', 'd', ' ', 's', 't', 'e', 'p', 2, 'b', 'y',
TOK_END, 

/* Listing of table: .camera.look.left.by */
	27, T_STEP >> 8, T_STEP & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 19, 'U', 'n', 'i', 't', ' ', 's', 't', 'e', 'p', ' ', '(', 'd', 'e', 'g', 'r', 'e', 'e', 's', ')',
TOK_END, 

/* Listing of table: .camera.xplane */
	30, T_ALPHA >> 8, T_ALPHA & 0xff, TOK_KEYWORD, 0, 6, 17 /* .camera.xplane.alpha */ , 16, 'S', 'e', 't', ' ', 't', 'r', 'a', 'n', 's', 'p', 'a', 'r', 'e', 'n', 'c', 'y', 5, 'a', 'l', 'p', 'h', 'a',
	29, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 6, 71 /* .camera.xplane.color */ , 15, 'S', 'e', 't', ' ', 'p', 'l', 'a', 'n', 'e', ' ', 'c', 'o', 'l', 'o', 'r', 5, 'c', 'o', 'l', 'o', 'r',
TOK_END, 

/* Listing of table: .camera.xplane.alpha */
	35, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 6, 53 /* .camera.xplane.alpha.by */ , 24, 'A', 'l', 't', 'e', 'r', ' ', 't', 'r', 'a', 'n', 's', 'p', 'a', 'r', 'e', 'n', 'c', 'y', ' ', 'b', 'y', '.', '.', '.', 2, 'b', 'y',
TOK_END, 

/* Listing of table: .camera.xplane.alpha.by */
	17, T_STEP >> 8, T_STEP & 0xff, TOK_NUM, 255, 255, 255 /* End */ , 9, 'U', 'n', 'i', 't', ' ', 's', 't', 'e', 'p',
TOK_END, 

/* Listing of table: .camera.xplane.color */
	21, T_SET_R >> 8, T_SET_R & 0xff, TOK_NUM, 0, 6, 93 /* .camera.xplane.color.<num> */ , 13, 'R', 'e', 'd', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't',
TOK_END, 

/* Listing of table: .camera.xplane.color.<num> */
	23, T_SET_G >> 8, T_SET_G & 0xff, TOK_NUM, 0, 6, 117 /* .camera.xplane.color.<num>.<num> */ , 15, 'G', 'r', 'e', 'e', 'n', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't',
TOK_END, 

/* Listing of table: .camera.xplane.color.<num>.<num> */
	22, T_SET_B >> 8, T_SET_B & 0xff, TOK_NUM, 255, 255, 255 /* End */ , 14, 'B', 'l', 'u', 'e', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't',
TOK_END, 

/* Listing of table: .bind */
	28, T_PRESS >> 8, T_PRESS & 0xff, TOK_KEYWORD, 0, 6, 255 /* .bind.press */ , 14, 'B', 'i', 'n', 'd', ' ', 'k', 'e', 'y', ' ', 'p', 'r', 'e', 's', 's', 5, 'p', 'r', 'e', 's', 's',
	26, T_HOLD >> 8, T_HOLD & 0xff, TOK_KEYWORD, 0, 6, 255 /* .bind.press */ , 13, 'B', 'i', 'n', 'd', ' ', 'k', 'e', 'y', ' ', 'h', 'o', 'l', 'd', 4, 'h', 'o', 'l', 'd',
	32, T_RELEASE >> 8, T_RELEASE & 0xff, TOK_KEYWORD, 0, 6, 255 /* .bind.press */ , 16, 'B', 'i', 'n', 'd', ' ', 'k', 'e', 'y', ' ', 'r', 'e', 'l', 'e', 'a', 's', 'e', 7, 'r', 'e', 'l', 'e', 'a', 's', 'e',
	28, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 7, 45 /* .bind.drag */ , 15, 'B', 'i', 'n', 'd', ' ', 'm', 'o', 'u', 's', 'e', ' ', 'd', 'r', 'a', 'g', 4, 'd', 'r', 'a', 'g',
TOK_END, 

/* Listing of table: .bind.press */
	21, T_BINDCODE >> 8, T_BINDCODE & 0xff, TOK_WORD, 0, 7, 21 /* .bind.press.<word> */ , 13, 'K', 'e', 'y', 'b', 'o', 'a', 'r', 'd', ' ', 'c', 'o', 'd', 'e',
TOK_END, 

/* Listing of table: .bind.press.<word> */
	23, T_COMMAND >> 8, T_COMMAND & 0xff, TOK_STRING, 255, 255, 255 /* End */ , 15, 'C', 'o', 'm', 'm', 'a', 'n', 'd', ' ', 't', 'o', ' ', 'b', 'i', 'n', 'd',
TOK_END, 

/* Listing of table: .bind.drag */
	37, T_SHIFT >> 8, T_SHIFT & 0xff, TOK_KEYWORD, 0, 7, 45 /* .bind.drag */ , 23, 'B', 'i', 'n', 'd', ' ', 's', 'h', 'i', 'f', 't', 'e', 'd', ' ', 'm', 'o', 'u', 's', 'e', ' ', 'd', 'r', 'a', 'g', 5, 's', 'h', 'i', 'f', 't',
	38, T_BUTTON >> 8, T_BUTTON & 0xff, TOK_KEYWORD_INTVAL, 0, 7, 161 /* .bind.drag.left */ , 23, 'B', 'i', 'n', 'd', ' ', 'd', 'r', 'a', 'g', ' ', 'w', '/', 'l', 'e', 'f', 't', ' ', 'b', 'u', 't', 't', 'o', 'n', 4, 'l', 'e', 'f', 't', 0, 1,
	40, T_BUTTON >> 8, T_BUTTON & 0xff, TOK_KEYWORD_INTVAL, 0, 7, 161 /* .bind.drag.left */ , 24, 'B', 'i', 'n', 'd', ' ', 'd', 'r', 'a', 'g', ' ', 'w', '/', 'r', 'i', 'g', 'h', 't', ' ', 'b', 'u', 't', 't', 'o', 'n', 5, 'r', 'i', 'g', 'h', 't', 0, 2,
TOK_END, 

/* Listing of table: .bind.drag.left */
	28, T_AXIS >> 8, T_AXIS & 0xff, TOK_KEYWORD_INTVAL, 0, 7, 218 /* .bind.drag.left.x-axis */ , 11, 'B', 'i', 'n', 'd', ' ', 'X', ' ', 'a', 'x', 'i', 's', 6, 'x', '-', 'a', 'x', 'i', 's', 0, 0,
	28, T_AXIS >> 8, T_AXIS & 0xff, TOK_KEYWORD_INTVAL, 0, 7, 218 /* .bind.drag.left.x-axis */ , 11, 'B', 'i', 'n', 'd', ' ', 'Y', ' ', 'a', 'x', 'i', 's', 6, 'y', '-', 'a', 'x', 'i', 's', 0, 1,
TOK_END, 

/* Listing of table: .bind.drag.left.x-axis */
	14, T_FACTOR >> 8, T_FACTOR & 0xff, TOK_WORD, 0, 7, 233 /* .bind.drag.left.x-axis.<word> */ , 6, 'F', 'a', 'c', 't', 'o', 'r',
TOK_END, 

/* Listing of table: .bind.drag.left.x-axis.<word> */
	39, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 24, 'U', 'n', 'b', 'i', 'n', 'd', ' ', 'm', 'o', 'u', 's', 'e', ' ', 'm', 'o', 'v', 'e', ' ', 'a', 'c', 't', 'i', 'o', 'n', 4, 'n', 'o', 'n', 'e', 0, 0,
	46, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 23, 'B', 'i', 'n', 'd', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'i', 't', 'c', 'h', ' ', 'a', 'n', 'g', 'l', 'e', 12, 'c', 'a', 'm', 'e', 'r', 'a', '-', 'p', 'i', 't', 'c', 'h', 0, 1,
	42, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 21, 'B', 'i', 'n', 'd', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'y', 'a', 'w', ' ', 'a', 'n', 'g', 'l', 'e', 10, 'c', 'a', 'm', 'e', 'r', 'a', '-', 'y', 'a', 'w', 0, 2,
	45, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 22, 'B', 'i', 'n', 'd', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'Z', ' ', 'p', 'o', 's', 'i', 't', 'i', 'o', 'n', 12, 'c', 'a', 'm', 'e', 'r', 'a', '-', 'd', 'e', 'p', 't', 'h', 0, 3,
	46, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 22, 'B', 'i', 'n', 'd', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'X', ' ', 'p', 'o', 's', 'i', 't', 'i', 'o', 'n', 13, 'c', 'a', 'm', 'e', 'r', 'a', '-', 'o', 'f', 'f', 's', 'e', 't', 0, 4,
	46, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 22, 'B', 'i', 'n', 'd', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'Y', ' ', 'p', 'o', 's', 'i', 't', 'i', 'o', 'n', 13, 'c', 'a', 'm', 'e', 'r', 'a', '-', 'h', 'e', 'i', 'g', 'h', 't', 0, 5,
	43, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 24, 'B', 'i', 'n', 'd', ' ', 'N', 'V', 'M', ' ', 'r', 'a', 's', 't', 'e', 'r', ' ', 'X', ' ', 'm', 'a', 'r', 'k', 'e', 'r', 8, 'm', 'a', 'r', 'k', 'e', 'r', '-', 'x', 0, 6,
	43, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 24, 'B', 'i', 'n', 'd', ' ', 'N', 'V', 'M', ' ', 'r', 'a', 's', 't', 'e', 'r', ' ', 'Y', ' ', 'm', 'a', 'r', 'k', 'e', 'r', 8, 'm', 'a', 'r', 'k', 'e', 'r', '-', 'y', 0, 7,
	33, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 11, 'M', 'a', 'r', 'k', ' ', 'p', 'o', 'i', 'n', 't', 's', 11, 'm', 'a', 'r', 'k', '-', 'p', 'o', 'i', 'n', 't', 's', 0, 8,
	34, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 16, 'M', 'e', 'a', 's', 'u', 'r', 'e', 'm', 'e', 'n', 't', ' ', 't', 'o', 'o', 'l', 7, 'm', 'e', 'a', 's', 'u', 'r', 'e', 0, 9,
	23, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 7, 'P', 'a', 'n', ' ', 'N', 'V', 'M', 5, 'p', 'a', 'n', '-', 'x', 0, 10,
	39, T_DRAG >> 8, T_DRAG & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 23, 'P', 'a', 'n', ' ', 'N', 'V', 'M', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', '(', 'Y', ' ', 'a', 'x', 'i', 's', ')', 5, 'p', 'a', 'n', '-', 'y', 0, 11,
TOK_END, 

/* Listing of table: .keymap */
	43, T_ACTIVATE >> 8, T_ACTIVATE & 0xff, TOK_KEYWORD, 0, 10, 8 /* .keymap.activate */ , 26, 'A', 'c', 't', 'i', 'v', 'a', 't', 'e', ' ', 'a', ' ', 's', 'p', 'e', 'c', 'i', 'f', 'i', 'c', ' ', 'k', 'e', 'y', 'm', 'a', 'p', 8, 'a', 'c', 't', 'i', 'v', 'a', 't', 'e',
	19, T_KEYMAP >> 8, T_KEYMAP & 0xff, TOK_WORD, 0, 10, 28 /* .keymap.<word> */ , 11, 'K', 'e', 'y', 'm', 'a', 'p', ' ', 'n', 'a', 'm', 'e',
TOK_END, 

/* Listing of table: .keymap.activate */
	19, T_KEYMAP >> 8, T_KEYMAP & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 11, 'K', 'e', 'y', 'm', 'a', 'p', ' ', 'n', 'a', 'm', 'e',
TOK_END, 

/* Listing of table: .keymap.<word> */
	38, T_DESCRIPTION >> 8, T_DESCRIPTION & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 18, 'K', 'e', 'y', 'm', 'a', 'p', ' ', 'd', 'e', 's', 'c', 'r', 'i', 'p', 't', 'i', 'o', 'n', 11, 'd', 'e', 's', 'c', 'r', 'i', 'p', 't', 'i', 'o', 'n',
	38, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 6, 140 /* .bind */ , 25, 'B', 'i', 'n', 'd', ' ', 'a', ' ', 'k', 'e', 'y', ' ', 'i', 'n', ' ', 't', 'h', 'i', 's', ' ', 'k', 'e', 'y', 'm', 'a', 'p', 4, 'b', 'i', 'n', 'd',
TOK_END, 

/* Listing of table: .view */
	26, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 11, 20 /* .view.set */ , 14, 'S', 'e', 't', ' ', 'u', 'n', 'i', 't', ' ', 's', 't', 'e', 'p', 's', 3, 's', 'e', 't',
	34, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 12, 23 /* .view.switch */ , 19, 'C', 'h', 'a', 'n', 'g', 'e', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'v', 'i', 'e', 'w', 6, 's', 'w', 'i', 't', 'c', 'h',
	27, T_ZOOM >> 8, T_ZOOM & 0xff, TOK_KEYWORD, 0, 12, 239 /* .view.zoom */ , 14, 'Z', 'o', 'o', 'm', ' ', 'i', 'n', ' ', 'o', 'r', ' ', 'o', 'u', 't', 4, 'z', 'o', 'o', 'm',
	27, T_CLIP >> 8, T_CLIP & 0xff, TOK_KEYWORD, 0, 12, 239 /* .view.zoom */ , 14, 'C', 'l', 'i', 'p', ' ', 'i', 'n', ' ', 'o', 'r', ' ', 'o', 'u', 't', 4, 'c', 'l', 'i', 'p',
	31, T_OFFSET >> 8, T_OFFSET & 0xff, TOK_KEYWORD, 0, 12, 239 /* .view.zoom */ , 16, 'O', 'f', 'f', 's', 'e', 't', ' ', 'i', 'n', ' ', 'o', 'r', ' ', 'o', 'u', 't', 6, 'o', 'f', 'f', 's', 'e', 't',
	25, T_FOG >> 8, T_FOG & 0xff, TOK_KEYWORD, 0, 14, 38 /* .view.fog */ , 13, 'F', 'o', 'g', ' ', 'i', 'n', ' ', 'o', 'r', ' ', 'o', 'u', 't', 3, 'f', 'o', 'g',
TOK_END, 

/* Listing of table: .view.set */
	35, T_RSIZE >> 8, T_RSIZE & 0xff, TOK_KEYWORD, 0, 11, 93 /* .view.set.raster-size */ , 15, 'S', 'e', 't', ' ', 'r', 'a', 's', 't', 'e', 'r', ' ', 's', 'i', 'z', 'e', 11, 'r', 'a', 's', 't', 'e', 'r', '-', 's', 'i', 'z', 'e',
	37, T_RRANGE >> 8, T_RRANGE & 0xff, TOK_KEYWORD, 0, 11, 186 /* .view.set.raster-range */ , 16, 'S', 'e', 't', ' ', 'r', 'a', 's', 't', 'e', 'r', ' ', 'r', 'a', 'n', 'g', 'e', 12, 'r', 'a', 's', 't', 'e', 'r', '-', 'r', 'a', 'n', 'g', 'e',
TOK_END, 

/* Listing of table: .view.set.raster-size */
	30, T_ENLARGE >> 8, T_ENLARGE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 15, 'E', 'n', 'l', 'a', 'r', 'g', 'e', ' ', 'r', 'a', 's', 't', 'e', 'r', 's', 6, 'l', 'a', 'r', 'g', 'e', 'r',
	30, T_SHRINK >> 8, T_SHRINK & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 14, 'S', 'h', 'r', 'i', 'n', 'k', ' ', 'r', 'a', 's', 't', 'e', 'r', 's', 7, 's', 'm', 'a', 'l', 'l', 'e', 'r',
	32, T_VALUE >> 8, T_VALUE & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 24, 'R', 'a', 's', 't', 'e', 'r', ' ', 's', 'i', 'z', 'e', ' ', 't', 'o', ' ', 'c', 'o', 'n', 'f', 'i', 'g', 'u', 'r', 'e',
TOK_END, 

/* Listing of table: .view.set.raster-range */
	30, T_ENLARGE >> 8, T_ENLARGE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 15, 'E', 'n', 'l', 'a', 'r', 'g', 'e', ' ', 'r', 'a', 's', 't', 'e', 'r', 's', 6, 'l', 'a', 'r', 'g', 'e', 'r',
	30, T_SHRINK >> 8, T_SHRINK & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 14, 'S', 'h', 'r', 'i', 'n', 'k', ' ', 'r', 'a', 's', 't', 'e', 'r', 's', 7, 's', 'm', 'a', 'l', 'l', 'e', 'r',
	32, T_VALUE >> 8, T_VALUE & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 24, 'R', 'a', 's', 't', 'e', 'r', ' ', 's', 'i', 'z', 'e', ' ', 't', 'o', ' ', 'c', 'o', 'n', 'f', 'i', 'g', 'u', 'r', 'e',
TOK_END, 

/* Listing of table: .view.switch */
	33, T_SETVIEW >> 8, T_SETVIEW & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 18, 'S', 'w', 'i', 't', 'c', 'h', ' ', 't', 'o', ' ', 't', 'o', 'p', ' ', 'v', 'i', 'e', 'w', 4, 'p', 'l', 'a', 'n', 0, 0,
	44, T_SETVIEW >> 8, T_SETVIEW & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 26, 'S', 'w', 'i', 't', 'c', 'h', ' ', 't', 'o', ' ', 'e', 'x', 't', 'e', 'n', 'd', 'e', 'd', ' ', 'p', 'r', 'o', 'f', 'i', 'l', 'e', 7, 'p', 'r', 'o', 'f', 'i', 'l', 'e', 0, 1,
	29, T_SETVIEW >> 8, T_SETVIEW & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 13, 'C', 'r', 'o', 's', 's', '-', 's', 'e', 'c', 't', 'i', 'o', 'n', 5, 'x', 's', 'e', 'c', 't', 0, 2,
	27, T_SETVIEW >> 8, T_SETVIEW & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 11, 'P', 'e', 'r', 's', 'p', 'e', 'c', 't', 'i', 'v', 'e', 5, 'c', 'a', 'v', 'e', 'r', 0, 3,
	27, T_SETVIEW >> 8, T_SETVIEW & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 10, 'N', 'V', 'M', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 6, 'n', 'v', 'm', 'c', 'a', 'm', 0, 4,
	55, T_SETVIEW >> 8, T_SETVIEW & 0xff, TOK_KEYWORD_INTVAL, 255, 255, 255 /* End */ , 36, 'T', 'h', 'r', 'e', 'e', ' ', 'o', 'r', 't', 'h', 'o', 'g', 'o', 'n', 'a', 'l', ' ', 'v', 'i', 'e', 'w', 's', ' ', '+', ' ', 'p', 'e', 'r', 's', 'p', 'e', 'c', 't', 'i', 'v', 'e', 8, 'f', 'o', 'u', 'r', 'v', 'i', 'e', 'w', 0, 6,
TOK_END, 

/* Listing of table: .view.zoom */
	23, T_MODVIEW >> 8, T_MODVIEW & 0xff, TOK_KEYWORD_INTVAL, 0, 13, 169 /* .view.zoom.current */ , 8, 'T', 'o', 'p', ' ', 'v', 'i', 'e', 'w', 4, 'p', 'l', 'a', 'n', 0, 0,
	30, T_MODVIEW >> 8, T_MODVIEW & 0xff, TOK_KEYWORD_INTVAL, 0, 13, 169 /* .view.zoom.current */ , 12, 'P', 'r', 'o', 'f', 'i', 'l', 'e', ' ', 'v', 'i', 'e', 'w', 7, 'p', 'r', 'o', 'f', 'i', 'l', 'e', 0, 1,
	34, T_MODVIEW >> 8, T_MODVIEW & 0xff, TOK_KEYWORD_INTVAL, 0, 13, 169 /* .view.zoom.current */ , 18, 'C', 'r', 'o', 's', 's', '-', 's', 'e', 'c', 't', 'i', 'o', 'n', ' ', 'v', 'i', 'e', 'w', 5, 'x', 's', 'e', 'c', 't', 0, 2,
	32, T_MODVIEW >> 8, T_MODVIEW & 0xff, TOK_KEYWORD_INTVAL, 0, 13, 169 /* .view.zoom.current */ , 15, 'N', 'V', 'M', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'v', 'i', 'e', 'w', 6, 'n', 'v', 'm', 'c', 'a', 'm', 0, 4,
	36, T_MODVIEW >> 8, T_MODVIEW & 0xff, TOK_KEYWORD_INTVAL, 0, 13, 169 /* .view.zoom.current */ , 20, 'C', 'h', 'a', 'n', 'g', 'e', ' ', '3', 'D', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'F', 'O', 'V', 5, 'c', 'a', 'v', 'e', 'r', 0, 3,
	30, T_MODVIEW >> 8, T_MODVIEW & 0xff, TOK_KEYWORD_INTVAL, 0, 13, 169 /* .view.zoom.current */ , 12, 'C', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'v', 'i', 'e', 'w', 7, 'c', 'u', 'r', 'r', 'e', 'n', 't', 0, 255,
TOK_END, 

/* Listing of table: .view.zoom.current */
	23, T_SET >> 8, T_SET & 0xff, TOK_KEYWORD, 0, 13, 231 /* .view.zoom.current.set-to */ , 8, 'S', 'e', 't', ' ', 'z', 'o', 'o', 'm', 6, 's', 'e', 't', '-', 't', 'o',
	18, T_IN >> 8, T_IN & 0xff, TOK_KEYWORD, 0, 13, 251 /* .view.zoom.current.in */ , 7, 'Z', 'o', 'o', 'm', ' ', 'i', 'n', 2, 'i', 'n',
	20, T_OUT >> 8, T_OUT & 0xff, TOK_KEYWORD, 0, 13, 251 /* .view.zoom.current.in */ , 8, 'Z', 'o', 'o', 'm', ' ', 'o', 'u', 't', 3, 'o', 'u', 't',
TOK_END, 

/* Listing of table: .view.zoom.current.set-to */
	19, T_BY >> 8, T_BY & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 11, 'Z', 'o', 'o', 'm', ' ', 'f', 'a', 'c', 't', 'o', 'r',
TOK_END, 

/* Listing of table: .view.zoom.current.in */
	22, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 14, 18 /* .view.zoom.current.in.by */ , 11, 'Z', 'o', 'o', 'm', ' ', 'f', 'a', 'c', 't', 'o', 'r', 2, 'b', 'y',
TOK_END, 

/* Listing of table: .view.zoom.current.in.by */
	19, T_BY >> 8, T_BY & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 11, 'Z', 'o', 'o', 'm', ' ', 'b', 'y', ' ', '.', '.', '.',
TOK_END, 

/* Listing of table: .view.fog */
	22, T_ON >> 8, T_ON & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 11, 'T', 'u', 'r', 'n', ' ', 'f', 'o', 'g', ' ', 'o', 'n', 2, 'o', 'n',
	24, T_OFF >> 8, T_OFF & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 12, 'T', 'u', 'r', 'n', ' ', 'f', 'o', 'g', ' ', 'o', 'f', 'f', 3, 'o', 'f', 'f',
	28, T_SET >> 8, T_SET & 0xff, TOK_KEYWORD, 0, 14, 206 /* .view.fog.set */ , 16, 'S', 'e', 't', ' ', 'f', 'o', 'g', ' ', 'd', 'i', 's', 't', 'a', 'n', 'c', 'e', 3, 's', 'e', 't',
	23, T_IN >> 8, T_IN & 0xff, TOK_KEYWORD, 0, 0, 0 /* .view.zoom.in */ , 12, 'B', 'r', 'i', 'n', 'g', ' ', 'f', 'o', 'g', ' ', 'i', 'n', 2, 'i', 'n',
	25, T_OUT >> 8, T_OUT & 0xff, TOK_KEYWORD, 0, 0, 0 /* .view.zoom.in */ , 13, 'B', 'r', 'i', 'n', 'g', ' ', 'f', 'o', 'g', ' ', 'o', 'u', 't', 3, 'o', 'u', 't',
	45, T_SLIGHTLY >> 8, T_SLIGHTLY & 0xff, TOK_KEYWORD, 0, 14, 224 /* .view.fog.slightly */ , 28, 'B', 'r', 'i', 'n', 'g', ' ', 'f', 'o', 'g', ' ', 'i', 'n', ' ', 'o', 'r', ' ', 'o', 'u', 't', ' ', 's', 'l', 'i', 'g', 'h', 't', 'l', 'y', 8, 's', 'l', 'i', 'g', 'h', 't', 'l', 'y',
TOK_END, 

/* Listing of table: .view.fog.set */
	17, T_LEVEL >> 8, T_LEVEL & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 9, 'F', 'o', 'g', ' ', 'l', 'e', 'v', 'e', 'l',
TOK_END, 

/* Listing of table: .view.fog.slightly */
	23, T_IN >> 8, T_IN & 0xff, TOK_KEYWORD, 0, 0, 0 /* .view.zoom.in */ , 12, 'B', 'r', 'i', 'n', 'g', ' ', 'f', 'o', 'g', ' ', 'i', 'n', 2, 'i', 'n',
	25, T_OUT >> 8, T_OUT & 0xff, TOK_KEYWORD, 0, 0, 0 /* .view.zoom.in */ , 13, 'B', 'r', 'i', 'n', 'g', ' ', 'f', 'o', 'g', ' ', 'o', 'u', 't', 3, 'o', 'u', 't',
TOK_END, 

/* Listing of table: .model */
	50, T_MODEL >> 8, T_MODEL & 0xff, TOK_KEYWORD_INTVAL, 0, 17, 136 /* .model.<num>.load */ , 35, 'L', 'o', 'a', 'd', ' ', 'm', 'o', 'd', 'e', 'l', ' ', 'i', 'n', 't', 'o', ' ', 'n', 'e', 'x', 't', ' ', 'a', 'v', 'a', 'i', 'l', 'a', 'b', 'l', 'e', ' ', 's', 'l', 'o', 't', 4, 'l', 'o', 'a', 'd', 0, 255,
	20, T_MODEL >> 8, T_MODEL & 0xff, TOK_NUM, 0, 15, 88 /* .model.<num> */ , 12, 'M', 'o', 'd', 'e', 'l', ' ', 'n', 'u', 'm', 'b', 'e', 'r',
TOK_END, 

/* Listing of table: .model.<num> */
	33, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 17, 136 /* .model.<num>.load */ , 20, 'L', 'o', 'a', 'd', ' ', 'm', 'o', 'd', 'e', 'l', ' ', 'f', 'r', 'o', 'm', ' ', 'f', 'i', 'l', 'e', 4, 'l', 'o', 'a', 'd',
	37, T_APPEND >> 8, T_APPEND & 0xff, TOK_KEYWORD, 0, 17, 136 /* .model.<num>.load */ , 22, 'A', 'p', 'p', 'e', 'n', 'd', ' ', 'm', 'o', 'd', 'e', 'l', ' ', 'f', 'r', 'o', 'm', ' ', 'f', 'i', 'l', 'e', 6, 'a', 'p', 'p', 'e', 'n', 'd',
	29, T_RECOLOR >> 8, T_RECOLOR & 0xff, TOK_KEYWORD, 0, 18, 166 /* .model.<num>.recolor */ , 13, 'R', 'e', 'c', 'o', 'l', 'o', 'r', ' ', 'm', 'o', 'd', 'e', 'l', 7, 'r', 'e', 'c', 'o', 'l', 'o', 'r',
	52, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 20, 39 /* .model.<num>.color-quality */ , 30, 'M', 'a', 'n', 'i', 'p', 'u', 'l', 'a', 't', 'e', ' ', 'c', 'o', 'l', 'o', 'r', ' ', 'q', 'u', 'a', 'l', 'i', 't', 'y', ' ', 't', 'a', 'b', 'l', 'e', 13, 'c', 'o', 'l', 'o', 'r', '-', 'q', 'u', 'a', 'l', 'i', 't', 'y',
	63, T_DELOP >> 8, T_DELOP & 0xff, TOK_KEYWORD, 0, 20, 75 /* .model.<num>.restrict */ , 46, 'D', 'e', 'l', 'e', 't', 'e', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'f', 'a', 'r', ' ', 'f', 'r', 'o', 'm', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'o', 's', 'i', 't', 'i', 'o', 'n', 8, 'r', 'e', 's', 't', 'r', 'i', 'c', 't',
	30, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 20, 154 /* .model.<num>.save */ , 17, 'S', 'a', 'v', 'e', ' ', 'm', 'o', 'd', 'e', 'l', ' ', 'a', 's', ' ', '.', '.', '.', 4, 's', 'a', 'v', 'e',
	47, T_SELECTION >> 8, T_SELECTION & 0xff, TOK_KEYWORD, 0, 20, 172 /* .model.<num>.selection */ , 29, 'O', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n', 's', ' ', 'o', 'n', ' ', 's', 'e', 'l', 'e', 'c', 't', 'e', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', 9, 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n',
	43, T_NORMALS >> 8, T_NORMALS & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 19, '(', 'R', 'e', ')', 'c', 'o', 'm', 'p', 'u', 't', 'e', ' ', 'n', 'o', 'r', 'm', 'a', 'l', 's', 15, 'c', 'o', 'm', 'p', 'u', 't', 'e', '-', 'n', 'o', 'r', 'm', 'a', 'l', 's',
	61, T_TRANSFORM >> 8, T_TRANSFORM & 0xff, TOK_KEYWORD, 0, 23, 51 /* .model.<num>.transform */ , 43, 'A', 'p', 'p', 'l', 'y', ' ', 'a', 'n', ' ', 'a', 'f', 'f', 'i', 'n', 'e', ' ', 't', 'r', 'a', 'n', 's', 'f', 'o', 'r', 'm', 'a', 't', 'i', 'o', 'n', ' ', 't', 'o', ' ', 't', 'h', 'e', ' ', 'm', 'o', 'd', 'e', 'l', 9, 't', 'r', 'a', 'n', 's', 'f', 'o', 'r', 'm',
	40, T_DISPLAY >> 8, T_DISPLAY & 0xff, TOK_KEYWORD, 0, 26, 255 /* .model.<num>.display */ , 24, 'M', 'o', 'd', 'e', 'l', ' ', 'd', 'i', 's', 'p', 'l', 'a', 'y', 'i', 'n', 'g', ' ', 'o', 'p', 't', 'i', 'o', 'n', 's', 7, 'd', 'i', 's', 'p', 'l', 'a', 'y',
	46, T_INFO >> 8, T_INFO & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 33, 'D', 'i', 's', 'p', 'l', 'a', 'y', ' ', 'i', 'n', 'f', 'o', 'r', 'm', 'a', 't', 'i', 'o', 'n', ' ', 'o', 'n', ' ', 't', 'h', 'i', 's', ' ', 'm', 'o', 'd', 'e', 'l', 4, 'i', 'n', 'f', 'o',
	43, T_POINT >> 8, T_POINT & 0xff, TOK_KEYWORD, 0, 27, 178 /* .model.<num>.point */ , 29, 'M', 'a', 'n', 'i', 'p', 'u', 'l', 'a', 't', 'e', ' ', 'm', 'o', 'd', 'e', 'l', ' ', 's', 'a', 'v', 'e', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', 5, 'p', 'o', 'i', 'n', 't',
	35, T_NVM >> 8, T_NVM & 0xff, TOK_KEYWORD, 0, 28, 127 /* .model.<num>.nvm */ , 23, 'N', '-', 'v', 'i', 'e', 'w', ' ', 'm', 'a', 't', 'c', 'h', ' ', 'o', 'p', 'e', 'r', 'a', 't', 'i', 'o', 'n', 's', 3, 'n', 'v', 'm',
TOK_END, 

/* Listing of table: .model.<num>.load */
	30, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 17, 239 /* .model.<num>.load.from */ , 17, 'S', 'p', 'e', 'c', 'i', 'f', 'y', ' ', 'f', 'i', 'l', 'e', ' ', 'n', 'a', 'm', 'e', 4, 'f', 'r', 'o', 'm',
	29, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 18, 1 /* .model.<num>.load.color */ , 15, 'S', 'e', 't', ' ', 'm', 'o', 'd', 'e', 'l', ' ', 'c', 'o', 'l', 'o', 'r', 5, 'c', 'o', 'l', 'o', 'r',
	43, T_NORMALS >> 8, T_NORMALS & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 19, '(', 'R', 'e', ')', 'c', 'o', 'm', 'p', 'u', 't', 'e', ' ', 'n', 'o', 'r', 'm', 'a', 'l', 's', 15, 'c', 'o', 'm', 'p', 'u', 't', 'e', '-', 'n', 'o', 'r', 'm', 'a', 'l', 's',
TOK_END, 

/* Listing of table: .model.<num>.load.from */
	17, T_LOAD_MODEL >> 8, T_LOAD_MODEL & 0xff, TOK_STRING, 255, 255, 255 /* End */ , 9, 'F', 'i', 'l', 'e', ' ', 'n', 'a', 'm', 'e',
TOK_END, 

/* Listing of table: .model.<num>.load.color */
	33, T_SET_R >> 8, T_SET_R & 0xff, TOK_NUM, 0, 18, 35 /* .model.<num>.load.color.<num> */ , 25, 'S', 'e', 't', ' ', 'R', 'e', 'd', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', ' ', '(', '0', '-', '2', '5', '5', ')',
TOK_END, 

/* Listing of table: .model.<num>.load.color.<num> */
	35, T_SET_G >> 8, T_SET_G & 0xff, TOK_NUM, 0, 18, 71 /* .model.<num>.load.color.<num>.<num> */ , 27, 'S', 'e', 't', ' ', 'G', 'r', 'e', 'e', 'n', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', ' ', '(', '0', '-', '2', '5', '5', ')',
TOK_END, 

/* Listing of table: .model.<num>.load.color.<num>.<num> */
	34, T_SET_B >> 8, T_SET_B & 0xff, TOK_NUM, 0, 18, 106 /* .model.<num>.load.color.<num>.<num>.<num> */ , 26, 'S', 'e', 't', ' ', 'B', 'l', 'u', 'e', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', ' ', '(', '0', '-', '2', '5', '5', ')',
TOK_END, 

/* Listing of table: .model.<num>.load.color.<num>.<num>.<num> */
	59, T_SET_A >> 8, T_SET_A & 0xff, TOK_NUM, 0, 17, 136 /* .model.<num>.load */ , 51, 'S', 'e', 't', ' ', 'A', 'l', 'p', 'h', 'a', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', ' ', '(', '0', ' ', '-', ' ', 't', 'r', 'a', 'n', 's', 'p', 'a', 'r', 'e', 'n', 't', ',', ' ', '2', '5', '5', ' ', '-', ' ', 'o', 'p', 'a', 'q', 'u', 'e', ')',
TOK_END, 

/* Listing of table: .model.<num>.recolor */
	34, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 18, 234 /* .model.<num>.recolor.from */ , 21, 'L', 'o', 'a', 'd', ' ', 'c', 'o', 'l', 'o', 'r', 's', ' ', 'f', 'r', 'o', 'm', ' ', 'f', 'i', 'l', 'e', 4, 'f', 'r', 'o', 'm',
	33, T_SET_R >> 8, T_SET_R & 0xff, TOK_NUM, 0, 19, 164 /* .model.<num>.recolor.<num> */ , 25, 'S', 'e', 't', ' ', 'R', 'e', 'd', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', ' ', '(', '0', '-', '2', '5', '5', ')',
TOK_END, 

/* Listing of table: .model.<num>.recolor.from */
	32, T_RECOLOR_FROM >> 8, T_RECOLOR_FROM & 0xff, TOK_WORD, 0, 19, 11 /* .model.<num>.recolor.from.<word> */ , 24, 'F', 'i', 'l', 'e', ' ', 't', 'o', ' ', 'r', 'e', 'a', 'd', ' ', 'c', 'o', 'l', 'o', 'r', 's', ' ', 'f', 'r', 'o', 'm',
TOK_END, 

/* Listing of table: .model.<num>.recolor.from.<word> */
	48, T_GRAY_UNCOLORABLE >> 8, T_GRAY_UNCOLORABLE & 0xff, TOK_KEYWORD, 0, 19, 11 /* .model.<num>.recolor.from.<word> */ , 23, 'G', 'r', 'a', 'y', ' ', 'u', 'n', 'c', 'o', 'l', 'o', 'r', 'a', 'b', 'l', 'e', ' ', 'p', 'o', 'i', 'n', 't', 's', 16, 'g', 'r', 'a', 'y', '-', 'u', 'n', 'c', 'o', 'l', 'o', 'r', 'a', 'b', 'l', 'e',
	52, T_DELETE_UNCOLORABLE >> 8, T_DELETE_UNCOLORABLE & 0xff, TOK_KEYWORD, 0, 19, 11 /* .model.<num>.recolor.from.<word> */ , 25, 'D', 'e', 'l', 'e', 't', 'e', ' ', 'u', 'n', 'c', 'o', 'l', 'o', 'r', 'a', 'b', 'l', 'e', ' ', 'p', 'o', 'i', 'n', 't', 's', 18, 'd', 'e', 'l', 'e', 't', 'e', '-', 'u', 'n', 'c', 'o', 'l', 'o', 'r', 'a', 'b', 'l', 'e',
	52, T_UNDELETE_COLORABLE >> 8, T_UNDELETE_COLORABLE & 0xff, TOK_KEYWORD, 0, 19, 11 /* .model.<num>.recolor.from.<word> */ , 25, 'U', 'n', 'd', 'e', 'l', 'e', 't', 'e', ' ', 'c', 'o', 'l', 'o', 'r', 'a', 'b', 'l', 'e', ' ', 'p', 'o', 'i', 'n', 't', 's', 18, 'u', 'n', 'd', 'e', 'l', 'e', 't', 'e', '-', 'c', 'o', 'l', 'o', 'r', 'a', 'b', 'l', 'e',
TOK_END, 

/* Listing of table: .model.<num>.recolor.<num> */
	35, T_SET_G >> 8, T_SET_G & 0xff, TOK_NUM, 0, 19, 200 /* .model.<num>.recolor.<num>.<num> */ , 27, 'S', 'e', 't', ' ', 'G', 'r', 'e', 'e', 'n', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', ' ', '(', '0', '-', '2', '5', '5', ')',
TOK_END, 

/* Listing of table: .model.<num>.recolor.<num>.<num> */
	34, T_SET_B >> 8, T_SET_B & 0xff, TOK_NUM, 0, 19, 235 /* .model.<num>.recolor.<num>.<num>.<num> */ , 26, 'S', 'e', 't', ' ', 'B', 'l', 'u', 'e', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', ' ', '(', '0', '-', '2', '5', '5', ')',
TOK_END, 

/* Listing of table: .model.<num>.recolor.<num>.<num>.<num> */
	59, T_SET_A >> 8, T_SET_A & 0xff, TOK_NUM, 255, 255, 255 /* End */ , 51, 'S', 'e', 't', ' ', 'A', 'l', 'p', 'h', 'a', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', ' ', '(', '0', ' ', '-', ' ', 't', 'r', 'a', 'n', 's', 'p', 'a', 'r', 'e', 'n', 't', ',', ' ', '2', '5', '5', ' ', '-', ' ', 'o', 'p', 'a', 'q', 'u', 'e', ')',
TOK_END, 

/* Listing of table: .model.<num>.color-quality */
	35, T_RESET_COLORS >> 8, T_RESET_COLORS & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 21, 'R', 'e', 's', 'e', 't', ' ', 'c', 'o', 'l', 'o', 'r', ' ', 'q', 'u', 'a', 'l', 'i', 't', 'i', 'e', 's', 5, 'r', 'e', 's', 'e', 't',
TOK_END, 

/* Listing of table: .model.<num>.restrict */
	44, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 20, 120 /* .model.<num>.restrict.to */ , 33, 'S', 'p', 'e', 'c', 'i', 'f', 'y', ' ', 't', 'a', 'r', 'g', 'e', 't', ' ', 'n', 'u', 'm', 'b', 'e', 'r', ' ', 'o', 'f', ' ', 'v', 'e', 'r', 't', 'i', 'c', 'e', 's', 2, 't', 'o',
TOK_END, 

/* Listing of table: .model.<num>.restrict.to */
	33, T_NUM_V >> 8, T_NUM_V & 0xff, TOK_NUM, 255, 255, 255 /* End */ , 25, 'T', 'a', 'r', 'g', 'e', 't', ' ', 'n', 'u', 'm', 'b', 'e', 'r', ' ', 'o', 'f', ' ', 'v', 'e', 'r', 't', 'i', 'c', 'e', 's',
TOK_END, 

/* Listing of table: .model.<num>.save */
	17, T_SAVE_AS >> 8, T_SAVE_AS & 0xff, TOK_STRING, 255, 255, 255 /* End */ , 9, 'F', 'i', 'l', 'e', ' ', 'n', 'a', 'm', 'e',
TOK_END, 

/* Listing of table: .model.<num>.selection */
	68, T_ADD_BEHIND >> 8, T_ADD_BEHIND & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 49, 'A', 'd', 'd', ' ', 'a', 'l', 'l', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'b', 'e', 'h', 'i', 'n', 'd', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 't', 'o', ' ', 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n', 10, 'a', 'd', 'd', '-', 'b', 'e', 'h', 'i', 'n', 'd',
	39, T_DELETE >> 8, T_DELETE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 24, 'D', 'e', 'l', 'e', 't', 'e', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n', 6, 'd', 'e', 'l', 'e', 't', 'e',
	48, T_UNDELETE >> 8, T_UNDELETE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 27, 'U', 'n', 'd', 'e', 'l', 'e', 't', 'e', ' ', 'a', 'l', 'l', ' ', 'd', 'e', 'l', 'e', 't', 'e', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', 12, 'u', 'n', 'd', 'e', 'l', 'e', 't', 'e', '-', 'a', 'l', 'l',
	33, T_RESET >> 8, T_RESET & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 19, 'R', 'e', 's', 'e', 't', ' ', 't', 'h', 'e', ' ', 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n', 5, 'r', 'e', 's', 'e', 't',
	42, T_FIT >> 8, T_FIT & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 24, 'F', 'i', 't', ' ', 'a', ' ', 'p', 'l', 'a', 'n', 'e', ' ', 't', 'o', ' ', 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n', 9, 'f', 'i', 't', '-', 'p', 'l', 'a', 'n', 'e',
	35, T_POPCOMPONENT >> 8, T_POPCOMPONENT & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 17, 'P', 'o', 'p', ' ', 'o', 'n', 'e', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', 9, 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't',
	54, T_GROW >> 8, T_GROW & 0xff, TOK_KEYWORD, 0, 22, 174 /* .model.<num>.selection.grow */ , 41, 'G', 'r', 'o', 'w', ' ', 'o', 'r', ' ', 's', 'h', 'r', 'i', 'n', 'k', ' ', 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n', ' ', 'b', 'y', ' ', 'e', 'd', 'g', 'e', ' ', 'd', 'i', 's', 't', 'a', 'n', 'c', 'e', 4, 'g', 'r', 'o', 'w',
	61, T_BOUNDARY >> 8, T_BOUNDARY & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 44, 'M', 'a', 'r', 'k', ' ', 'u', 'n', 'm', 'a', 'r', 'k', 'e', 'd', ' ', 'v', 'e', 'r', 't', 'i', 'c', 'e', 's', ' ', 'a', 'd', 'j', 'a', 'c', 'e', 'n', 't', ' ', 't', 'o', ' ', 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n', 8, 'b', 'o', 'u', 'n', 'd', 'a', 'r', 'y',
	50, T_WALKEDGE >> 8, T_WALKEDGE & 0xff, TOK_KEYWORD, 0, 22, 239 /* .model.<num>.selection.walk-edge */ , 32, 'M', 'a', 'r', 'k', ' ', 'a', 'd', 'j', 'a', 'c', 'e', 'n', 't', ' ', 'm', 'e', 's', 'h', ' ', 'e', 'd', 'g', 'e', ' ', 'v', 'e', 'r', 't', 'i', 'c', 'e', 's', 9, 'w', 'a', 'l', 'k', '-', 'e', 'd', 'g', 'e',
	34, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 23, 33 /* .model.<num>.selection.save */ , 21, 'S', 'a', 'v', 'e', ' ', 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n', ' ', 'a', 's', ' ', '.', '.', '.', 4, 's', 'a', 'v', 'e',
	49, T_PATCHES >> 8, T_PATCHES & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 25, 'D', 'u', 'm', 'p', ' ', 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', 's', 15, 'd', 'u', 'm', 'p', '-', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', 's',
TOK_END, 

/* Listing of table: .model.<num>.selection.grow */
	22, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 22, 197 /* .model.<num>.selection.grow.by */ , 11, 'G', 'r', 'o', 'w', ' ', 'b', 'y', ' ', '.', '.', '.', 2, 'b', 'y',
TOK_END, 

/* Listing of table: .model.<num>.selection.grow.by */
	41, T_BY >> 8, T_BY & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 33, 'A', 'f', 'f', 'i', 'n', 'e', ' ', 'u', 'n', 'i', 't', 's', ' ', '(', 'n', 'e', 'g', 'a', 't', 'i', 'v', 'e', ' ', 't', 'o', ' ', 's', 'h', 'r', 'i', 'n', 'k', ')',
TOK_END, 

/* Listing of table: .model.<num>.selection.walk-edge */
	35, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 23, 19 /* .model.<num>.selection.walk-edge.step */ , 22, 'H', 'o', 'w', ' ', 'm', 'a', 'n', 'y', ' ', 's', 't', 'e', 'p', 's', ' ', 't', 'o', ' ', 'm', 'a', 'k', 'e', 4, 's', 't', 'e', 'p',
TOK_END, 

/* Listing of table: .model.<num>.selection.walk-edge.step */
	13, T_STEPS >> 8, T_STEPS & 0xff, TOK_NUM, 255, 255, 255 /* End */ , 5, 'S', 't', 'e', 'p', 's',
TOK_END, 

/* Listing of table: .model.<num>.selection.save */
	17, T_SAVE_AS >> 8, T_SAVE_AS & 0xff, TOK_STRING, 255, 255, 255 /* End */ , 9, 'F', 'i', 'l', 'e', ' ', 'n', 'a', 'm', 'e',
TOK_END, 

/* Listing of table: .model.<num>.transform */
	36, T_MATRIX >> 8, T_MATRIX & 0xff, TOK_KEYWORD, 0, 24, 112 /* .model.<num>.transform.matrix */ , 21, 'S', 'p', 'e', 'c', 'i', 'f', 'y', ' ', 'a', 'f', 'f', 'i', 'n', 'e', ' ', 'm', 'a', 't', 'r', 'i', 'x', 6, 'm', 'a', 't', 'r', 'i', 'x',
	41, T_TRANSFORM_PITCH >> 8, T_TRANSFORM_PITCH & 0xff, TOK_KEYWORD, 0, 25, 241 /* .model.<num>.transform.pitch */ , 27, 'R', 'o', 't', 'a', 't', 'e', ' ', 'a', 'r', 'o', 'u', 'n', 'd', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'X', ' ', 'a', 'x', 'i', 's', 5, 'p', 'i', 't', 'c', 'h',
	39, T_TRANSFORM_YAW >> 8, T_TRANSFORM_YAW & 0xff, TOK_KEYWORD, 0, 26, 10 /* .model.<num>.transform.yaw */ , 27, 'R', 'o', 't', 'a', 't', 'e', ' ', 'a', 'r', 'o', 'u', 'n', 'd', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'Y', ' ', 'a', 'x', 'i', 's', 3, 'y', 'a', 'w',
	40, T_TRANSFORM_ROLL >> 8, T_TRANSFORM_ROLL & 0xff, TOK_KEYWORD, 0, 26, 35 /* .model.<num>.transform.roll */ , 27, 'R', 'o', 't', 'a', 't', 'e', ' ', 'a', 'r', 'o', 'u', 'n', 'd', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'Z', ' ', 'a', 'x', 'i', 's', 4, 'r', 'o', 'l', 'l',
	52, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 26, 60 /* .model.<num>.transform.scale */ , 38, 'S', 'c', 'a', 'l', 'e', ' ', 'u', 'n', 'i', 'f', 'o', 'r', 'm', 'l', 'y', ' ', 'u', 's', 'i', 'n', 'g', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'a', 's', ' ', 'o', 'r', 'i', 'g', 'i', 'n', 5, 's', 'c', 'a', 'l', 'e',
	37, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 26, 86 /* .model.<num>.transform.translate */ , 19, 'T', 'r', 'a', 'n', 's', 'l', 'a', 't', 'e', ' ', 't', 'h', 'e', ' ', 'm', 'o', 'd', 'e', 'l', 9, 't', 'r', 'a', 'n', 's', 'l', 'a', 't', 'e',
	71, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 26, 137 /* .model.<num>.transform.to */ , 60, 'F', 'i', 'n', 'd', ' ', 'a', 'n', ' ', 'a', 'f', 'f', 'i', 'n', 'e', ' ', 't', 'r', 'a', 'n', 's', 'f', 'o', 'r', 'm', 'a', 't', 'i', 'o', 'n', ' ', 'i', 'n', 't', 'o', ' ', 'a', 'n', 'o', 't', 'h', 'e', 'r', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e', ' ', 's', 'y', 's', 't', 'e', 'm', 2, 't', 'o',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix */
	48, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 24, 161 /* .model.<num>.transform.matrix.x */ , 38, 'S', 'p', 'e', 'c', 'i', 'f', 'y', ' ', 'f', 'i', 'r', 's', 't', ' ', 'r', 'o', 'w', ' ', 'o', 'f', ' ', 't', 'h', 'e', ' ', 'a', 'f', 'f', 'i', 'n', 'e', ' ', 'm', 'a', 't', 'r', 'i', 'x', 1, 'x',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x */
	20, T_M11 >> 8, T_M11 & 0xff, TOK_WORD, 0, 24, 182 /* .model.<num>.transform.matrix.x.<word> */ , 12, 'F', 'i', 'r', 's', 't', ' ', 'c', 'o', 'l', 'u', 'm', 'n',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word> */
	21, T_M12 >> 8, T_M12 & 0xff, TOK_WORD, 0, 24, 204 /* .model.<num>.transform.matrix.x.<word>.<word> */ , 13, 'S', 'e', 'c', 'o', 'n', 'd', ' ', 'c', 'o', 'l', 'u', 'm', 'n',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word> */
	20, T_M13 >> 8, T_M13 & 0xff, TOK_WORD, 0, 24, 225 /* .model.<num>.transform.matrix.x.<word>.<word>.<word> */ , 12, 'T', 'h', 'i', 'r', 'd', ' ', 'c', 'o', 'l', 'u', 'm', 'n',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word> */
	14, T_M14 >> 8, T_M14 & 0xff, TOK_WORD, 0, 24, 240 /* .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word> */ , 6, 'O', 'f', 'f', 's', 'e', 't',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word> */
	49, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 25, 34 /* .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y */ , 39, 'S', 'p', 'e', 'c', 'i', 'f', 'y', ' ', 's', 'e', 'c', 'o', 'n', 'd', ' ', 'r', 'o', 'w', ' ', 'o', 'f', ' ', 't', 'h', 'e', ' ', 'a', 'f', 'f', 'i', 'n', 'e', ' ', 'm', 'a', 't', 'r', 'i', 'x', 1, 'y',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y */
	20, T_M21 >> 8, T_M21 & 0xff, TOK_WORD, 0, 25, 55 /* .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word> */ , 12, 'F', 'i', 'r', 's', 't', ' ', 'c', 'o', 'l', 'u', 'm', 'n',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word> */
	21, T_M22 >> 8, T_M22 & 0xff, TOK_WORD, 0, 25, 77 /* .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word> */ , 13, 'S', 'e', 'c', 'o', 'n', 'd', ' ', 'c', 'o', 'l', 'u', 'm', 'n',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word> */
	20, T_M23 >> 8, T_M23 & 0xff, TOK_WORD, 0, 25, 98 /* .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word> */ , 12, 'T', 'h', 'i', 'r', 'd', ' ', 'c', 'o', 'l', 'u', 'm', 'n',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word> */
	14, T_M24 >> 8, T_M24 & 0xff, TOK_WORD, 0, 25, 113 /* .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word>.<word> */ , 6, 'O', 'f', 'f', 's', 'e', 't',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word>.<word> */
	48, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 25, 162 /* .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word>.<word>.z */ , 38, 'S', 'p', 'e', 'c', 'i', 'f', 'y', ' ', 't', 'h', 'i', 'r', 'd', ' ', 'r', 'o', 'w', ' ', 'o', 'f', ' ', 't', 'h', 'e', ' ', 'a', 'f', 'f', 'i', 'n', 'e', ' ', 'm', 'a', 't', 'r', 'i', 'x', 1, 'z',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word>.<word>.z */
	20, T_M31 >> 8, T_M31 & 0xff, TOK_WORD, 0, 25, 183 /* .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word>.<word>.z.<word> */ , 12, 'F', 'i', 'r', 's', 't', ' ', 'c', 'o', 'l', 'u', 'm', 'n',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word>.<word>.z.<word> */
	21, T_M32 >> 8, T_M32 & 0xff, TOK_WORD, 0, 25, 205 /* .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word>.<word>.z.<word>.<word> */ , 13, 'S', 'e', 'c', 'o', 'n', 'd', ' ', 'c', 'o', 'l', 'u', 'm', 'n',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word>.<word>.z.<word>.<word> */
	20, T_M33 >> 8, T_M33 & 0xff, TOK_WORD, 0, 25, 226 /* .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word>.<word>.z.<word>.<word>.<word> */ , 12, 'T', 'h', 'i', 'r', 'd', ' ', 'c', 'o', 'l', 'u', 'm', 'n',
TOK_END, 

/* Listing of table: .model.<num>.transform.matrix.x.<word>.<word>.<word>.<word>.y.<word>.<word>.<word>.<word>.z.<word>.<word>.<word> */
	14, T_M34 >> 8, T_M34 & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 6, 'O', 'f', 'f', 's', 'e', 't',
TOK_END, 

/* Listing of table: .model.<num>.transform.pitch */
	24, T_TRANSFORM_ANGLE >> 8, T_TRANSFORM_ANGLE & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 16, 'A', 'n', 'g', 'l', 'e', ' ', 'i', 'n', ' ', 'd', 'e', 'g', 'r', 'e', 'e', 's',
TOK_END, 

/* Listing of table: .model.<num>.transform.yaw */
	24, T_TRANSFORM_ANGLE >> 8, T_TRANSFORM_ANGLE & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 16, 'A', 'n', 'g', 'l', 'e', ' ', 'i', 'n', ' ', 'd', 'e', 'g', 'r', 'e', 'e', 's',
TOK_END, 

/* Listing of table: .model.<num>.transform.roll */
	24, T_TRANSFORM_ANGLE >> 8, T_TRANSFORM_ANGLE & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 16, 'A', 'n', 'g', 'l', 'e', ' ', 'i', 'n', ' ', 'd', 'e', 'g', 'r', 'e', 'e', 's',
TOK_END, 

/* Listing of table: .model.<num>.transform.scale */
	25, T_TRANSFORM_SCALE >> 8, T_TRANSFORM_SCALE & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 17, 'S', 'c', 'a', 'l', 'e', ' ', 'c', 'o', 'e', 'f', 'f', 'i', 'c', 'i', 'e', 'n', 't',
TOK_END, 

/* Listing of table: .model.<num>.transform.translate */
	16, T_TX >> 8, T_TX & 0xff, TOK_WORD, 0, 26, 103 /* .model.<num>.transform.translate.<word> */ , 8, 'X', ' ', 'o', 'f', 'f', 's', 'e', 't',
TOK_END, 

/* Listing of table: .model.<num>.transform.translate.<word> */
	16, T_TY >> 8, T_TY & 0xff, TOK_WORD, 0, 26, 120 /* .model.<num>.transform.translate.<word>.<word> */ , 8, 'Y', ' ', 'o', 'f', 'f', 's', 'e', 't',
TOK_END, 

/* Listing of table: .model.<num>.transform.translate.<word>.<word> */
	16, T_TZ >> 8, T_TZ & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 8, 'Z', ' ', 'o', 'f', 'f', 's', 'e', 't',
TOK_END, 

/* Listing of table: .model.<num>.transform.to */
	51, T_TRANSFORM_TO >> 8, T_TRANSFORM_TO & 0xff, TOK_NUM, 0, 26, 189 /* .model.<num>.transform.to.<num> */ , 43, 'M', 'o', 'd', 'e', 'l', ' ', 'd', 'e', 'f', 'i', 'n', 'i', 'n', 'g', ' ', 't', 'h', 'e', ' ', 't', 'a', 'r', 'g', 'e', 't', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e', ' ', 's', 'y', 's', 't', 'e', 'm',
TOK_END, 

/* Listing of table: .model.<num>.transform.to.<num> */
	65, T_COMPOSE >> 8, T_COMPOSE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 49, 'C', 'o', 'm', 'p', 'o', 's', 'e', ' ', 'c', 'o', 'm', 'p', 'u', 't', 'e', 'd', ' ', 't', 'r', 'a', 'n', 's', 'f', 'o', 'r', 'm', ' ', 'w', 'i', 't', 'h', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 't', 'r', 'a', 'n', 's', 'f', 'o', 'r', 'm', 7, 'c', 'o', 'm', 'p', 'o', 's', 'e',
TOK_END, 

/* Listing of table: .model.<num>.display */
	25, T_REFINE >> 8, T_REFINE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 10, 'A', 'd', 'd', ' ', 'd', 'e', 't', 'a', 'i', 'l', 6, 'r', 'e', 'f', 'i', 'n', 'e',
	32, T_GENERALIZE >> 8, T_GENERALIZE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 13, 'R', 'e', 'm', 'o', 'v', 'e', ' ', 'd', 'e', 't', 'a', 'i', 'l', 10, 'g', 'e', 'n', 'e', 'r', 'a', 'l', 'i', 'z', 'e',
	38, T_NONE >> 8, T_NONE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 25, 'D', 'o', ' ', 'n', 'o', 't', ' ', 'd', 'i', 's', 'p', 'l', 'a', 'y', ' ', 't', 'h', 'i', 's', ' ', 'm', 'o', 'd', 'e', 'l', 4, 'n', 'o', 'n', 'e',
	29, T_POINTS >> 8, T_POINTS & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 14, 'D', 'i', 's', 'p', 'l', 'a', 'y', ' ', 'p', 'o', 'i', 'n', 't', 's', 6, 'p', 'o', 'i', 'n', 't', 's',
	27, T_FACES >> 8, T_FACES & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 13, 'D', 'i', 's', 'p', 'l', 'a', 'y', ' ', 'f', 'a', 'c', 'e', 's', 5, 'f', 'a', 'c', 'e', 's',
	27, T_EDGES >> 8, T_EDGES & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 13, 'D', 'i', 's', 'p', 'l', 'a', 'y', ' ', 'e', 'd', 'g', 'e', 's', 5, 'e', 'd', 'g', 'e', 's',
TOK_END, 

/* Listing of table: .model.<num>.point */
	16, T_POINT_ID >> 8, T_POINT_ID & 0xff, TOK_WORD, 0, 27, 195 /* .model.<num>.point.<word> */ , 8, 'P', 'o', 'i', 'n', 't', ' ', 'I', 'D',
TOK_END, 

/* Listing of table: .model.<num>.point.<word> */
	36, T_SET >> 8, T_SET & 0xff, TOK_KEYWORD, 0, 28, 64 /* .model.<num>.point.<word>.set-to */ , 21, 'S', 'e', 't', ' ', 'p', 'o', 'i', 'n', 't', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e', 's', 6, 's', 'e', 't', '-', 't', 'o',
	32, T_DELETE >> 8, T_DELETE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 17, 'D', 'e', 'l', 'e', 't', 'e', ' ', 't', 'h', 'i', 's', ' ', 'p', 'o', 'i', 'n', 't', 6, 'd', 'e', 'l', 'e', 't', 'e',
	56, T_SEE >> 8, T_SEE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 44, 'A', 's', 's', 'u', 'm', 'e', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 's', 'o', ' ', 't', 'h', 'a', 't', ' ', 't', 'h', 'i', 's', ' ', 'p', 'o', 'i', 'n', 't', ' ', 'c', 'a', 'n', ' ', 'b', 'e', ' ', 's', 'e', 'e', 'n', 3, 's', 'e', 'e',
TOK_END, 

/* Listing of table: .model.<num>.point.<word>.set-to */
	20, T_PX >> 8, T_PX & 0xff, TOK_WORD, 0, 28, 85 /* .model.<num>.point.<word>.set-to.<word> */ , 12, 'X', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e',
TOK_END, 

/* Listing of table: .model.<num>.point.<word>.set-to.<word> */
	20, T_PY >> 8, T_PY & 0xff, TOK_WORD, 0, 28, 106 /* .model.<num>.point.<word>.set-to.<word>.<word> */ , 12, 'Y', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e',
TOK_END, 

/* Listing of table: .model.<num>.point.<word>.set-to.<word>.<word> */
	20, T_PZ >> 8, T_PZ & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 12, 'Z', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e',
TOK_END, 

/* Listing of table: .model.<num>.nvm */
	26, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 29, 28 /* .model.<num>.nvm.load */ , 13, 'L', 'o', 'a', 'd', ' ', 'N', 'V', 'M', ' ', 'f', 'i', 'l', 'e', 4, 'l', 'o', 'a', 'd',
	50, T_SEARCH >> 8, T_SEARCH & 0xff, TOK_KEYWORD, 0, 29, 106 /* .model.<num>.nvm.search */ , 35, 'D', 'u', 'm', 'p', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', 's', 'e', 'e', 'i', 'n', 'g', ' ', 's', 'e', 'l', 'e', 'c', 't', 'e', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', 6, 's', 'e', 'a', 'r', 'c', 'h',
	23, T_SHOW >> 8, T_SHOW & 0xff, TOK_KEYWORD, 0, 31, 135 /* .model.<num>.nvm.show */ , 10, 'S', 'h', 'o', 'w', ' ', 's', 't', 'u', 'f', 'f', 4, 's', 'h', 'o', 'w',
	23, T_HIDE >> 8, T_HIDE & 0xff, TOK_KEYWORD, 0, 32, 164 /* .model.<num>.nvm.hide */ , 10, 'H', 'i', 'd', 'e', ' ', 's', 't', 'u', 'f', 'f', 4, 'h', 'i', 'd', 'e',
	34, T_ALTER_CAMERA >> 8, T_ALTER_CAMERA & 0xff, TOK_KEYWORD, 0, 33, 140 /* .model.<num>.nvm.camera */ , 19, 'D', 'i', 's', 'p', 'l', 'a', 'y', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'd', 'a', 't', 'a', 6, 'c', 'a', 'm', 'e', 'r', 'a',
TOK_END, 

/* Listing of table: .model.<num>.nvm.load */
	20, T_NVM_ID >> 8, T_NVM_ID & 0xff, TOK_NUM, 0, 29, 49 /* .model.<num>.nvm.load.<num> */ , 12, 'N', 'V', 'M', ' ', 'm', 'o', 'd', 'e', 'l', ' ', 'I', 'D',
TOK_END, 

/* Listing of table: .model.<num>.nvm.load.<num> */
	38, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 29, 88 /* .model.<num>.nvm.load.<num>.from */ , 25, 'S', 'p', 'e', 'c', 'i', 'f', 'y', ' ', 'f', 'i', 'l', 'e', ' ', 't', 'o', ' ', 'l', 'o', 'a', 'd', ' ', 'f', 'r', 'o', 'm', 4, 'f', 'r', 'o', 'm',
TOK_END, 

/* Listing of table: .model.<num>.nvm.load.<num>.from */
	17, T_LOAD_NVM >> 8, T_LOAD_NVM & 0xff, TOK_STRING, 255, 255, 255 /* End */ , 9, 'F', 'i', 'l', 'e', ' ', 'n', 'a', 'm', 'e',
TOK_END, 

/* Listing of table: .model.<num>.nvm.search */
	60, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 29, 224 /* .model.<num>.nvm.search.sparse */ , 45, 'S', 'e', 'a', 'r', 'c', 'h', ' ', 'N', 'V', 'M', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'a', 'r', 'o', 'u', 'n', 'd', ' ', 't', 'h', 'e', ' ', 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n', ' ', '(', 'f', 'a', 's', 't', ')', 6, 's', 'p', 'a', 'r', 's', 'e',
	57, T_DENSE >> 8, T_DENSE & 0xff, TOK_KEYWORD, 0, 30, 15 /* .model.<num>.nvm.search.dense */ , 43, 'R', 'e', 'p', 'r', 'o', 'j', 'e', 'c', 't', ' ', 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n', ' ', 'o', 'n', 't', 'o', ' ', 'a', 'l', 'l', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', '(', 's', 'l', 'o', 'w', ')', 5, 'd', 'e', 'n', 's', 'e',
TOK_END, 

/* Listing of table: .model.<num>.nvm.search.sparse */
	46, T_REORDER >> 8, T_REORDER & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 30, 'R', 'e', 'o', 'r', 'd', 'e', 'r', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', 'b', 'y', ' ', 'p', 'o', 'i', 'n', 't', ' ', 'c', 'o', 'u', 'n', 't', 7, 'r', 'e', 'o', 'r', 'd', 'e', 'r',
TOK_END, 

/* Listing of table: .model.<num>.nvm.search.dense */
	46, T_REORDER >> 8, T_REORDER & 0xff, TOK_KEYWORD, 0, 30, 15 /* .model.<num>.nvm.search.dense */ , 30, 'R', 'e', 'o', 'r', 'd', 'e', 'r', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', 'b', 'y', ' ', 'p', 'o', 'i', 'n', 't', ' ', 'c', 'o', 'u', 'n', 't', 7, 'r', 'e', 'o', 'r', 'd', 'e', 'r',
	51, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 30, 244 /* .model.<num>.nvm.search.dense.depth-tolerance */ , 27, 'S', 'p', 'e', 'c', 'i', 'f', 'y', ' ', 'd', 'e', 'p', 't', 'h', ' ', 'm', 'a', 'p', ' ', 't', 'o', 'l', 'e', 'r', 'a', 'n', 'c', 'e', 15, 'd', 'e', 'p', 't', 'h', '-', 't', 'o', 'l', 'e', 'r', 'a', 'n', 'c', 'e',
	48, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 31, 49 /* .model.<num>.nvm.search.dense.max-depth */ , 30, 'I', 'g', 'n', 'o', 'r', 'e', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'f', 'a', 'r', 't', 'h', 'e', 'r', ' ', 't', 'h', 'a', 'n', ' ', '.', '.', '.', 9, 'm', 'a', 'x', '-', 'd', 'e', 'p', 't', 'h',
	47, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 31, 92 /* .model.<num>.nvm.search.dense.min-depth */ , 29, 'I', 'g', 'n', 'o', 'r', 'e', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'c', 'l', 'o', 's', 'e', 'r', ' ', 't', 'h', 'a', 'n', ' ', '.', '.', '.', 9, 'm', 'i', 'n', '-', 'd', 'e', 'p', 't', 'h',
	36, T_NOMAP >> 8, T_NOMAP & 0xff, TOK_KEYWORD, 0, 30, 15 /* .model.<num>.nvm.search.dense */ , 21, 'D', 'o', ' ', 'n', 'o', 't', ' ', 'u', 's', 'e', ' ', 'd', 'e', 'p', 't', 'h', ' ', 'm', 'a', 'p', 's', 6, 'n', 'o', '-', 'm', 'a', 'p',
TOK_END, 

/* Listing of table: .model.<num>.nvm.search.dense.depth-tolerance */
	60, T_ZTOL >> 8, T_ZTOL & 0xff, TOK_WORD, 0, 30, 15 /* .model.<num>.nvm.search.dense */ , 52, 'D', 'e', 'p', 't', 'h', ' ', 'm', 'a', 'p', ' ', 't', 'o', 'l', 'e', 'r', 'a', 'n', 'c', 'e', ' ', 'f', 'o', 'r', ' ', 'd', 'e', 't', 'e', 'r', 'm', 'i', 'n', 'i', 'n', 'g', ' ', 'p', 'o', 'i', 'n', 't', ' ', 'v', 'i', 's', 'i', 'b', 'i', 'l', 'i', 't', 'y',
TOK_END, 

/* Listing of table: .model.<num>.nvm.search.dense.max-depth */
	42, T_ZMAX >> 8, T_ZMAX & 0xff, TOK_WORD, 0, 30, 15 /* .model.<num>.nvm.search.dense */ , 34, 'M', 'a', 'x', 'i', 'm', 'u', 'm', ' ', 'd', 'i', 's', 't', 'a', 'n', 'c', 'e', ' ', 'f', 'r', 'o', 'm', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'l', 'a', 'n', 'e',
TOK_END, 

/* Listing of table: .model.<num>.nvm.search.dense.min-depth */
	42, T_ZMIN >> 8, T_ZMIN & 0xff, TOK_WORD, 0, 30, 15 /* .model.<num>.nvm.search.dense */ , 34, 'M', 'i', 'n', 'i', 'm', 'u', 'm', ' ', 'd', 'i', 's', 't', 'a', 'n', 'c', 'e', ' ', 'f', 'r', 'o', 'm', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'l', 'a', 'n', 'e',
TOK_END, 

/* Listing of table: .model.<num>.nvm.show */
	28, T_IMAGE >> 8, T_IMAGE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 14, 'S', 'h', 'o', 'w', ' ', 'N', 'V', 'M', ' ', 'i', 'm', 'a', 'g', 'e', 5, 'i', 'm', 'a', 'g', 'e',
	60, T_2DPOINTS >> 8, T_2DPOINTS & 0xff, TOK_KEYWORD, 0, 0, 0 /* .model.<num>.nvm.camera.show */ , 38, 'S', 'h', 'o', 'w', ' ', '3', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'a', 's', ' ', 's', 'e', 'e', 'n', ' ', 'o', 'n', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'l', 'a', 'n', 'e', 13, 'p', 'l', 'a', 'n', 'a', 'r', '-', 'p', 'o', 'i', 'n', 't', 's',
	57, T_3DPOINTS >> 8, T_3DPOINTS & 0xff, TOK_KEYWORD, 0, 0, 0 /* .model.<num>.nvm.camera.show */ , 34, 'S', 'h', 'o', 'w', ' ', '3', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 's', 'e', 'e', 'n', ' ', 'b', 'y', ' ', 't', 'h', 'i', 's', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 14, 's', 'p', 'a', 't', 'i', 'a', 'l', '-', 'p', 'o', 'i', 'n', 't', 's',
	56, T_LINES >> 8, T_LINES & 0xff, TOK_KEYWORD, 0, 0, 0 /* .model.<num>.nvm.camera.show */ , 39, 'S', 'h', 'o', 'w', ' ', 'l', 'i', 'n', 'e', 's', ' ', 'c', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'n', 'g', ' ', '2', 'd', ' ', 'w', 'i', 't', 'h', ' ', '3', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', 8, 'm', 'a', 't', 'c', 'h', 'i', 'n', 'g',
	39, T_NEXT_MATCH >> 8, T_NEXT_MATCH & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 20, 'H', 'i', 'g', 'h', 'l', 'i', 'g', 'h', 't', ' ', 'n', 'e', 'x', 't', ' ', 'm', 'a', 't', 'c', 'h', 10, 'n', 'e', 'x', 't', '-', 'm', 'a', 't', 'c', 'h',
	44, T_PREV_MATCH >> 8, T_PREV_MATCH & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 25, 'H', 'i', 'g', 'h', 'l', 'i', 'g', 'h', 't', ' ', 'p', 'r', 'e', 'v', 'i', 'o', 'u', 's', ' ', 'm', 'a', 't', 'c', 'h', ' ', 10, 'p', 'r', 'e', 'v', '-', 'm', 'a', 't', 'c', 'h',
TOK_END, 

/* Listing of table: .model.<num>.nvm.hide */
	28, T_IMAGE >> 8, T_IMAGE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 14, 'H', 'i', 'd', 'e', ' ', 'N', 'V', 'M', ' ', 'i', 'm', 'a', 'g', 'e', 5, 'i', 'm', 'a', 'g', 'e',
	60, T_2DPOINTS >> 8, T_2DPOINTS & 0xff, TOK_KEYWORD, 0, 0, 0 /* .model.<num>.nvm.camera.hide */ , 38, 'H', 'i', 'd', 'e', ' ', '3', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'a', 's', ' ', 's', 'e', 'e', 'n', ' ', 'o', 'n', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'l', 'a', 'n', 'e', 13, 'p', 'l', 'a', 'n', 'a', 'r', '-', 'p', 'o', 'i', 'n', 't', 's',
	56, T_3DPOINTS >> 8, T_3DPOINTS & 0xff, TOK_KEYWORD, 0, 0, 0 /* .model.<num>.nvm.camera.hide */ , 33, 'H', 'i', 'd', 'e', '3', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 's', 'e', 'e', 'n', ' ', 'b', 'y', ' ', 't', 'h', 'i', 's', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 14, 's', 'p', 'a', 't', 'i', 'a', 'l', '-', 'p', 'o', 'i', 'n', 't', 's',
	56, T_LINES >> 8, T_LINES & 0xff, TOK_KEYWORD, 0, 0, 0 /* .model.<num>.nvm.camera.hide */ , 39, 'H', 'i', 'd', 'e', ' ', 'l', 'i', 'n', 'e', 's', ' ', 'c', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'n', 'g', ' ', '2', 'd', ' ', 'w', 'i', 't', 'h', ' ', '3', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', 8, 'm', 'a', 't', 'c', 'h', 'i', 'n', 'g',
	31, T_HIDEALL >> 8, T_HIDEALL & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 19, 'H', 'i', 'd', 'e', ' ', 't', 'h', 'e', ' ', 'N', 'V', 'M', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 3, 'a', 'l', 'l',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera */
	30, T_ORDER >> 8, T_ORDER & 0xff, TOK_KEYWORD, 0, 34, 97 /* .model.<num>.nvm.camera.order-by */ , 13, 'O', 'r', 'd', 'e', 'r', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', 8, 'o', 'r', 'd', 'e', 'r', '-', 'b', 'y',
	31, T_NEXT >> 8, T_NEXT & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 18, 'S', 'e', 'l', 'e', 'c', 't', ' ', 'n', 'e', 'x', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 4, 'n', 'e', 'x', 't',
	35, T_PREV >> 8, T_PREV & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 22, 'S', 'e', 'l', 'e', 'c', 't', ' ', 'p', 'r', 'e', 'v', 'i', 'o', 'u', 's', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 4, 'p', 'r', 'e', 'v',
	37, T_CURRENT >> 8, T_CURRENT & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 21, 'S', 'e', 'l', 'e', 'c', 't', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 7, 'c', 'u', 'r', 'r', 'e', 'n', 't',
	46, T_ALL >> 8, T_ALL & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 36, 'L', 'o', 'o', 'p', ' ', 't', 'h', 'r', 'o', 'u', 'g', 'h', ' ', 'a', 'l', 'l', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', 'i', 'f', ' ', 'p', 'o', 's', 's', 'i', 'b', 'l', 'e', 1, '*',
	33, T_CAMERA_NAME >> 8, T_CAMERA_NAME & 0xff, TOK_WORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 25, 'C', 'a', 'm', 'e', 'r', 'a', ' ', 'i', 'n', 'd', 'e', 'x', ' ', 'o', 'r', ' ', 'f', 'i', 'l', 'e', ' ', 'n', 'a', 'm', 'e',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.order-by */
	69, T_DISTANCE >> 8, T_DISTANCE & 0xff, TOK_KEYWORD, 0, 35, 85 /* .model.<num>.nvm.camera.order-by.depth */ , 52, 'O', 'r', 'd', 'e', 'r', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', 'b', 'y', ' ', 'a', 'b', 's', 'o', 'l', 'u', 't', 'e', ' ', 'd', 'i', 's', 't', 'a', 'n', 'c', 'e', ' ', 'f', 'r', 'o', 'm', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'l', 'a', 'n', 'e', 8, 'd', 'i', 's', 't', 'a', 'n', 'c', 'e',
	57, T_DEPTH >> 8, T_DEPTH & 0xff, TOK_KEYWORD, 0, 35, 85 /* .model.<num>.nvm.camera.order-by.depth */ , 43, 'O', 'r', 'd', 'e', 'r', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', 'b', 'y', ' ', 'd', 'i', 's', 't', 'a', 'n', 'c', 'e', ' ', 'f', 'r', 'o', 'm', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'l', 'a', 'n', 'e', 5, 'd', 'e', 'p', 't', 'h',
	34, T_NAME >> 8, T_NAME & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 21, 'O', 'r', 'd', 'e', 'r', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', 'b', 'y', ' ', 'n', 'a', 'm', 'e', 4, 'n', 'a', 'm', 'e',
	40, T_INDEX >> 8, T_INDEX & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 26, 'O', 'r', 'd', 'e', 'r', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', 'b', 'y', ' ', 'N', 'V', 'M', ' ', 'i', 'n', 'd', 'e', 'x', 5, 'i', 'n', 'd', 'e', 'x',
	43, T_SEEN >> 8, T_SEEN & 0xff, TOK_KEYWORD, 0, 36, 2 /* .model.<num>.nvm.camera.order-by.seen */ , 30, 'O', 'r', 'd', 'e', 'r', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', 'b', 'y', ' ', 's', 'e', 'e', 'n', ' ', 'f', 'e', 'a', 't', 'u', 'r', 'e', 's', 4, 's', 'e', 'e', 'n',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.order-by.depth */
	44, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 35, 183 /* .model.<num>.nvm.camera.order-by.depth.offset */ , 29, 'F', 'i', 'n', 'd', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 's', ' ', 's', 'l', 'i', 'g', 'h', 't', 'l', 'y', ' ', 'f', 'o', 'r', 'w', 'a', 'r', 'd', 6, 'o', 'f', 'f', 's', 'e', 't',
	53, T_FIRST >> 8, T_FIRST & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 39, 'C', 'o', 'n', 'f', 'i', 'g', 'u', 'r', 'e', ' ', 'a', 'c', 't', 'i', 'o', 'n', 's', ' ', 'w', 'i', 't', 'h', ' ', 't', 'h', 'e', ' ', 'f', 'i', 'r', 's', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 5, 'f', 'i', 'r', 's', 't',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.order-by.depth.offset */
	20, T_OFFSET >> 8, T_OFFSET & 0xff, TOK_WORD, 0, 35, 204 /* .model.<num>.nvm.camera.order-by.depth.offset.<word> */ , 12, 'D', 'e', 'p', 't', 'h', ' ', 'o', 'f', 'f', 's', 'e', 't',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.order-by.depth.offset.<word> */
	53, T_FIRST >> 8, T_FIRST & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 39, 'C', 'o', 'n', 'f', 'i', 'g', 'u', 'r', 'e', ' ', 'a', 'c', 't', 'i', 'o', 'n', 's', ' ', 'w', 'i', 't', 'h', ' ', 't', 'h', 'e', ' ', 'f', 'i', 'r', 's', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 5, 'f', 'i', 'r', 's', 't',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.order-by.seen */
	53, T_FIRST >> 8, T_FIRST & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 39, 'C', 'o', 'n', 'f', 'i', 'g', 'u', 'r', 'e', ' ', 'a', 'c', 't', 'i', 'o', 'n', 's', ' ', 'w', 'i', 't', 'h', ' ', 't', 'h', 'e', ' ', 'f', 'i', 'r', 's', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 5, 'f', 'i', 'r', 's', 't',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.<word> */
	59, T_SET_ACTIVE >> 8, T_SET_ACTIVE & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 40, 'M', 'a', 'r', 'k', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'a', 's', ' ', 'a', 'c', 't', 'i', 'v', 'e', ' ', 'a', 'n', 'd', ' ', 's', 'h', 'o', 'w', ' ', 'i', 't', 's', ' ', 'i', 'm', 'a', 'g', 'e', 10, 's', 'e', 't', '-', 'a', 'c', 't', 'i', 'v', 'e',
	61, T_REFRESH >> 8, T_REFRESH & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 45, 'D', 'o', ' ', 'n', 'o', 't', ' ', 'c', 'h', 'a', 'n', 'g', 'e', ' ', 'a', 'n', 'y', 't', 'h', 'i', 'n', 'g', ',', ' ', 'j', 'u', 's', 't', ' ', 'r', 'e', 'f', 'r', 'e', 's', 'h', ' ', 't', 'h', 'e', ' ', 'v', 'i', 'e', 'w', 7, 'r', 'e', 'f', 'r', 'e', 's', 'h',
	67, T_SEE >> 8, T_SEE & 0xff, TOK_KEYWORD, 0, 36, 56 /* .model.<num>.nvm.camera.<word> */ , 55, 'M', 'o', 'v', 'e', ' ', 's', 'c', 'e', 'n', 'e', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 's', 'o', ' ', 't', 'h', 'a', 't', ' ', 'i', 't', ' ', 'a', 'l', 'm', 'o', 's', 't', ' ', 'm', 'a', 't', 'c', 'h', 'e', 's', ' ', 't', 'h', 'i', 's', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 3, 's', 'e', 'e',
	54, T_REPROJECT >> 8, T_REPROJECT & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 36, 'R', 'e', 'p', 'r', 'o', 'j', 'e', 'c', 't', ' ', '3', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'o', 'n', 't', 'o', ' ', 't', 'h', 'i', 's', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 9, 'r', 'e', 'p', 'r', 'o', 'j', 'e', 'c', 't',
	24, T_DIAG >> 8, T_DIAG & 0xff, TOK_KEYWORD, 0, 40, 16 /* .model.<num>.nvm.camera.<word>.diag */ , 11, 'D', 'i', 'a', 'g', 'n', 'o', 's', 't', 'i', 'c', 's', 4, 'd', 'i', 'a', 'g',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.<word>.reproject */
	50, T_ONLY_SELECTION >> 8, T_ONLY_SELECTION & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 32, 'R', 'e', 'p', 'r', 'o', 'j', 'e', 'c', 't', ' ', 'o', 'n', 'l', 'y', ' ', 's', 'e', 'l', 'e', 'c', 't', 'e', 'd', ' ', 'v', 'e', 'r', 't', 'i', 'c', 'e', 's', 9, 's', 'e', 'l', 'e', 'c', 't', 'i', 'o', 'n',
	55, T_THROW >> 8, T_THROW & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 34, 'T', 'h', 'r', 'o', 'w', ' ', 'r', 'a', 'y', 's', ' ', 'f', 'r', 'o', 'm', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'N', 'V', 'M', ' ', 'c', 'u', 'r', 's', 'o', 'r', 12, 't', 'h', 'r', 'o', 'w', '-', 'c', 'u', 'r', 's', 'o', 'r',
	73, T_RECOLOR >> 8, T_RECOLOR & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 45, 'R', 'e', 'c', 'o', 'l', 'o', 'r', ' ', 'a', 'l', 'l', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'a', 'b', 'l', 'e', ' ', 'b', 'y', ' ', 't', 'h', 'i', 's', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 19, 'r', 'e', 'c', 'o', 'l', 'o', 'r', '-', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'a', 'b', 'l', 'e',
	67, T_UNDEL_COL >> 8, T_UNDEL_COL & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 40, 'U', 'n', 'd', 'e', 'l', 'e', 't', 'e', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'c', 'o', 'l', 'o', 'r', 'a', 'b', 'l', 'e', ' ', 'b', 'y', ' ', 't', 'h', 'i', 's', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 18, 'u', 'n', 'd', 'e', 'l', 'e', 't', 'e', '-', 'c', 'o', 'l', 'o', 'r', 'a', 'b', 'l', 'e',
	76, T_DEL_UNPROJ >> 8, T_DEL_UNPROJ & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 47, 'D', 'e', 'l', 'e', 't', 'e', ' ', '3', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'n', 'o', 't', ' ', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'a', 'b', 'l', 'e', ' ', 'b', 'y', ' ', 't', 'h', 'i', 's', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 20, 'd', 'e', 'l', 'e', 't', 'e', '-', 'u', 'n', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'a', 'b', 'l', 'e',
	63, T_GRAY_UNPROJ >> 8, T_GRAY_UNPROJ & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 36, 'M', 'a', 'r', 'k', ' ', 'u', 'n', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'a', 'b', 'l', 'e', ' ', '3', 'd', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'a', 's', ' ', 'g', 'r', 'a', 'y', 18, 'g', 'r', 'a', 'y', '-', 'u', 'n', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'a', 'b', 'l', 'e',
	67, T_DUMP >> 8, T_DUMP & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 42, 'D', 'u', 'm', 'p', ' ', 'R', 'G', 'B', ' ', 'c', 'o', 'l', 'o', 'r', ' ', 'd', 'a', 't', 'a', ' ', 'f', 'o', 'r', ' ', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'a', 'b', 'l', 'e', ' ', 'p', 'o', 'i', 'n', 't', 's', 16, 'd', 'u', 'm', 'p', '-', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'a', 'b', 'l', 'e',
	65, T_MARK >> 8, T_MARK & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 40, 'M', 'a', 'r', 'k', ' ', 'v', 'e', 'r', 't', 'i', 'c', 'e', 's', ' ', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'a', 'b', 'l', 'e', ' ', 'b', 'y', ' ', 't', 'h', 'i', 's', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 16, 'm', 'a', 'r', 'k', '-', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'a', 'b', 'l', 'e',
	49, T_SUMMARY >> 8, T_SUMMARY & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 33, 'D', 'u', 'm', 'p', ' ', 'a', ' ', 's', 'u', 'm', 'm', 'a', 'r', 'y', ' ', 'o', 'f', ' ', 't', 'h', 'i', 's', ' ', 'r', 'e', 'c', 'o', 'l', 'o', 'r', 'i', 'n', 'g', 7, 's', 'u', 'm', 'm', 'a', 'r', 'y',
	36, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 39, 254 /* .model.<num>.nvm.camera.<word>.reproject.save-depths */ , 16, 'S', 'a', 'v', 'e', ' ', 'a', ' ', 'd', 'e', 'p', 't', 'h', ' ', 'm', 'a', 'p', 11, 's', 'a', 'v', 'e', '-', 'd', 'e', 'p', 't', 'h', 's',
	37, T_NOMAP >> 8, T_NOMAP & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 22, 'D', 'o', ' ', 'n', 'o', 't', ' ', 'u', 's', 'e', ' ', 'a', ' ', 'd', 'e', 'p', 't', 'h', ' ', 'm', 'a', 'p', 6, 'n', 'o', '-', 'm', 'a', 'p',
	61, T_RESET_QUALITY >> 8, T_RESET_QUALITY & 0xff, TOK_KEYWORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 39, 'R', 'e', 's', 'e', 't', ' ', 'c', 'o', 'l', 'o', 'r', ' ', 'q', 'u', 'a', 'l', 'i', 't', 'y', ' ', 'b', 'e', 'f', 'o', 'r', 'e', ' ', 'r', 'e', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'i', 'n', 'g', 13, 'r', 'e', 's', 'e', 't', '-', 'q', 'u', 'a', 'l', 'i', 't', 'y',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.<word>.reproject.save-depths */
	17, T_DEPTH_MAP >> 8, T_DEPTH_MAP & 0xff, TOK_WORD, 0, 37, 66 /* .model.<num>.nvm.camera.<word>.reproject */ , 9, 'F', 'i', 'l', 'e', ' ', 'n', 'a', 'm', 'e',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.<word>.diag */
	36, T_NVMINFO >> 8, T_NVMINFO & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 23, 'D', 'u', 'm', 'p', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'i', 'n', 'f', 'o', 'r', 'm', 'a', 't', 'i', 'o', 'n', 4, 'i', 'n', 'f', 'o',
	58, T_MEAS >> 8, T_MEAS & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 37, 'D', 'u', 'm', 'p', ' ', 'm', 'e', 'a', 's', 'u', 'r', 'e', 'm', 'e', 'n', 't', 's', ' ', 's', 'e', 'e', 'n', ' ', 'b', 'y', ' ', 't', 'h', 'i', 's', ' ', 'c', 'a', 'm', 'e', 'r', 'a', 12, 'm', 'e', 'a', 's', 'u', 'r', 'e', 'm', 'e', 'n', 't', 's',
	44, T_REPROJECT_POINT >> 8, T_REPROJECT_POINT & 0xff, TOK_KEYWORD, 0, 40, 155 /* .model.<num>.nvm.camera.<word>.diag.reproject */ , 26, 'C', 'o', 'm', 'p', 'u', 't', 'e', ' ', 'p', 'o', 'i', 'n', 't', ' ', 'r', 'e', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'i', 'o', 'n', 9, 'r', 'e', 'p', 'r', 'o', 'j', 'e', 'c', 't',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.<word>.diag.reproject */
	20, T_X >> 8, T_X & 0xff, TOK_WORD, 0, 40, 176 /* .model.<num>.nvm.camera.<word>.diag.reproject.<word> */ , 12, 'X', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.<word>.diag.reproject.<word> */
	20, T_Y >> 8, T_Y & 0xff, TOK_WORD, 0, 40, 197 /* .model.<num>.nvm.camera.<word>.diag.reproject.<word>.<word> */ , 12, 'Y', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e',
TOK_END, 

/* Listing of table: .model.<num>.nvm.camera.<word>.diag.reproject.<word>.<word> */
	20, T_Z >> 8, T_Z & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 12, 'Z', ' ', 'c', 'o', 'o', 'r', 'd', 'i', 'n', 'a', 't', 'e',
TOK_END, 

/* Listing of table: .light */
	27, T_ENABLE >> 8, T_ENABLE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 12, 'E', 'n', 'a', 'b', 'l', 'e', ' ', 'l', 'i', 'g', 'h', 't', 6, 'e', 'n', 'a', 'b', 'l', 'e',
	29, T_DISABLE >> 8, T_DISABLE & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 13, 'D', 'i', 's', 'a', 'b', 'l', 'e', ' ', 'l', 'i', 'g', 'h', 't', 7, 'd', 'i', 's', 'a', 'b', 'l', 'e',
	25, T_RAISE >> 8, T_RAISE & 0xff, TOK_KEYWORD, 0, 41, 213 /* .light.raise */ , 11, 'R', 'a', 'i', 's', 'e', ' ', 'l', 'i', 'g', 'h', 't', 5, 'r', 'a', 'i', 's', 'e',
	27, T_OFFSET >> 8, T_OFFSET & 0xff, TOK_KEYWORD, 0, 41, 227 /* .light.offset */ , 12, 'O', 'f', 'f', 's', 'e', 't', ' ', 'l', 'i', 'g', 'h', 't', 6, 'o', 'f', 'f', 's', 'e', 't',
	32, T_SET >> 8, T_SET & 0xff, TOK_KEYWORD, 0, 41, 241 /* .light.brighten */ , 20, 'S', 'e', 't', ' ', 'l', 'i', 'g', 'h', 't', ' ', 'b', 'r', 'i', 'g', 'h', 't', 'n', 'e', 's', 's', 3, 's', 'e', 't',
	31, T_BRIGHTEN >> 8, T_BRIGHTEN & 0xff, TOK_KEYWORD, 0, 41, 241 /* .light.brighten */ , 14, 'B', 'r', 'i', 'g', 'h', 't', 'e', 'n', ' ', 'l', 'i', 'g', 'h', 't', 8, 'b', 'r', 'i', 'g', 'h', 't', 'e', 'n',
	50, T_LDROP >> 8, T_LDROP & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 37, 'D', 'r', 'o', 'p', ' ', 'l', 'i', 'g', 'h', 't', ' ', 'a', 't', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'o', 's', 'i', 't', 'i', 'o', 'n', 4, 'd', 'r', 'o', 'p',
	29, T_LDRAG >> 8, T_LDRAG & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 16, 'D', 'r', 'a', 'g', ' ', 'l', 'i', 'g', 'h', 't', ' ', 'a', 'l', 'o', 'n', 'g', 4, 'd', 'r', 'a', 'g',
TOK_END, 

/* Listing of table: .light.raise */
	13, T_BY >> 8, T_BY & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 5, 'U', 'n', 'i', 't', 's',
TOK_END, 

/* Listing of table: .light.offset */
	13, T_BY >> 8, T_BY & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 5, 'U', 'n', 'i', 't', 's',
TOK_END, 

/* Listing of table: .light.brighten */
	44, T_QUADRATIC >> 8, T_QUADRATIC & 0xff, TOK_KEYWORD, 0, 42, 110 /* .light.brighten.quadratic */ , 26, 'C', 'h', 'a', 'n', 'g', 'e', ' ', 'q', 'u', 'a', 'd', 'r', 'a', 't', 'i', 'c', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', 9, 'q', 'u', 'a', 'd', 'r', 'a', 't', 'i', 'c',
	38, T_LINEAR >> 8, T_LINEAR & 0xff, TOK_KEYWORD, 0, 42, 124 /* .light.brighten.linear */ , 23, 'C', 'h', 'a', 'n', 'g', 'e', ' ', 'l', 'i', 'n', 'e', 'a', 'r', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', 6, 'l', 'i', 'n', 'e', 'a', 'r',
	42, T_CONSTANT >> 8, T_CONSTANT & 0xff, TOK_KEYWORD, 0, 42, 138 /* .light.brighten.constant */ , 25, 'C', 'h', 'a', 'n', 'g', 'e', ' ', 'c', 'o', 'n', 's', 't', 'a', 'n', 't', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't', 8, 'c', 'o', 'n', 's', 't', 'a', 'n', 't',
TOK_END, 

/* Listing of table: .light.brighten.quadratic */
	13, T_BY >> 8, T_BY & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 5, 'U', 'n', 'i', 't', 's',
TOK_END, 

/* Listing of table: .light.brighten.linear */
	13, T_BY >> 8, T_BY & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 5, 'U', 'n', 'i', 't', 's',
TOK_END, 

/* Listing of table: .light.brighten.constant */
	13, T_BY >> 8, T_BY & 0xff, TOK_WORD, 255, 255, 255 /* End */ , 5, 'U', 'n', 'i', 't', 's',
TOK_END, 

/* Listing of table: .exec */
	25, T_EXEC >> 8, T_EXEC & 0xff, TOK_STRING, 255, 255, 255 /* End */ , 17, 'F', 'i', 'l', 'e', ' ', 'n', 'a', 'm', 'e', ' ', 't', 'o', ' ', 'l', 'o', 'a', 'd',
TOK_END, 

/* Listing of table: .flag */
	22, T_SET >> 8, T_SET & 0xff, TOK_KEYWORD, 0, 42, 255 /* .flag.set */ , 10, 'S', 'e', 't', ' ', 'a', ' ', 'f', 'l', 'a', 'g', 3, 's', 'e', 't',
	26, T_CLEAR >> 8, T_CLEAR & 0xff, TOK_KEYWORD, 0, 42, 255 /* .flag.set */ , 12, 'C', 'l', 'e', 'a', 'r', ' ', 'a', ' ', 'f', 'l', 'a', 'g', 5, 'c', 'l', 'e', 'a', 'r',
	28, T_TOGGLE >> 8, T_TOGGLE & 0xff, TOK_KEYWORD, 0, 42, 255 /* .flag.set */ , 13, 'T', 'o', 'g', 'g', 'l', 'e', ' ', 'a', ' ', 'f', 'l', 'a', 'g', 6, 't', 'o', 'g', 'g', 'l', 'e',
TOK_END, 

/* Listing of table: .flag.set */
	67, T_MISCFLAG >> 8, T_MISCFLAG & 0xff, TOK_KEYWORD_INTVAL, 0, 42, 255 /* .flag.set */ , 49, 'A', 'c', 't', ' ', 'l', 'i', 'k', 'e', ' ', 'l', 'e', 'f', 't', ' ', 'm', 'o', 'u', 's', 'e', ' ', 'b', 'u', 't', 't', 'o', 'n', ' ', 'w', 'a', 's', ' ', 'c', 'o', 'n', 's', 't', 'a', 'n', 't', 'l', 'y', ' ', 'p', 'r', 'e', 's', 's', 'e', 'd', 7, 'f', 'i', 'x', '-', 'l', 'm', 'b', 0, 1,
	43, T_MISCFLAG >> 8, T_MISCFLAG & 0xff, TOK_KEYWORD_INTVAL, 0, 42, 255 /* .flag.set */ , 25, 'L', 'o', 'g', ' ', 'e', 'v', 'e', 'r', 'y', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'm', 'o', 'v', 'e', 'm', 'e', 'n', 't', 7, 'l', 'o', 'g', '-', 'a', 'l', 'l', 0, 2,
	41, T_MISCFLAG >> 8, T_MISCFLAG & 0xff, TOK_KEYWORD_INTVAL, 0, 42, 255 /* .flag.set */ , 19, 'K', 'e', 'e', 'p', ' ', 'm', 'o', 'v', 'i', 'n', 'g', ' ', 'f', 'o', 'r', 'w', 'a', 'r', 'd', 11, 'k', 'e', 'e', 'p', '-', 'm', 'o', 'v', 'i', 'n', 'g', 0, 4,
	50, T_MISCFLAG >> 8, T_MISCFLAG & 0xff, TOK_KEYWORD_INTVAL, 0, 42, 255 /* .flag.set */ , 24, 'O', 'n', 'l', 'y', ' ', 'd', 'i', 's', 'p', 'l', 'a', 'y', ' ', 'N', 'V', 'M', ' ', ' ', 'i', 'm', 'a', 'g', 'e', 's', 15, 'o', 'n', 'l', 'y', '-', 'n', 'v', 'm', '-', 'i', 'm', 'a', 'g', 'e', 's', 0, 32,
	45, T_MISCFLAG >> 8, T_MISCFLAG & 0xff, TOK_KEYWORD_INTVAL, 0, 42, 255 /* .flag.set */ , 25, 'E', 'n', 'a', 'b', 'l', 'e', ' ', 'J', 'o', 'h', 'n', 'n', 'y', 'Q', 'u', 'e', 's', 't', ' ', 's', 'h', 'a', 'd', 'e', 'r', 9, 'j', 'q', '-', 's', 'h', 'a', 'd', 'e', 'r', 0, 64,
TOK_END, 

/* Listing of table: .log */
	29, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 44, 193 /* .log.file */ , 16, 'S', 'e', 't', ' ', 'n', 'e', 'w', ' ', 'l', 'o', 'g', ' ', 'f', 'i', 'l', 'e', 4, 'f', 'i', 'l', 'e',
	42, T_CAMERA >> 8, T_CAMERA & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 27, 'L', 'o', 'g', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'o', 's', 'i', 't', 'i', 'o', 'n', 6, 'c', 'a', 'm', 'e', 'r', 'a',
	44, T_NEXT >> 8, T_NEXT & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 23, 'A', 'd', 'v', 'a', 'n', 'c', 'e', ' ', 's', 't', 'a', 't', 'i', 'o', 'n', ' ', 'p', 'o', 'i', 'n', 't', 'e', 'r', 12, 'n', 'e', 'x', 't', '-', 's', 't', 'a', 't', 'i', 'o', 'n',
	44, T_PREV >> 8, T_PREV & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 23, 'R', 'e', 't', 'r', 'e', 'a', 't', ' ', 's', 't', 'a', 't', 'i', 'o', 'n', ' ', 'p', 'o', 'i', 'n', 't', 'e', 'r', 12, 'p', 'r', 'e', 'v', '-', 's', 't', 'a', 't', 'i', 'o', 'n',
	43, T_COMMIT >> 8, T_COMMIT & 0xff, TOK_KEYWORD, 255, 255, 255 /* End */ , 28, 'D', 'u', 'm', 'p', ' ', 'l', 'a', 's', 't', ' ', 'm', 'e', 'a', 's', 'u', 'r', 'e', 'm', 'e', 'n', 't', ' ', 't', 'o', ' ', 'l', 'o', 'g', 6, 'c', 'o', 'm', 'm', 'i', 't',
TOK_END, 

/* Listing of table: .log.file */
	21, T_CHANGE_LOG >> 8, T_CHANGE_LOG & 0xff, TOK_STRING, 255, 255, 255 /* End */ , 13, 'L', 'o', 'g', ' ', 'f', 'i', 'l', 'e', ' ', 'n', 'a', 'm', 'e',
TOK_END, 

/* Listing of table: .set */
	30, T_SET_NVM >> 8, T_SET_NVM & 0xff, TOK_KEYWORD, 0, 45, 38 /* .set.nvm */ , 18, 'S', 'e', 't', ' ', 'N', 'V', 'M', ' ', 'p', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 's', 3, 'n', 'v', 'm',
	48, T_SET_REPROJECTION >> 8, T_SET_REPROJECTION & 0xff, TOK_KEYWORD, 0, 45, 81 /* .set.reprojection */ , 27, 'S', 'e', 't', ' ', 'r', 'e', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'i', 'o', 'n', ' ', 'p', 'a', 'r', 'a', 'm', 'e', 't', 'e', 'r', 's', 12, 'r', 'e', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'i', 'o', 'n',
TOK_END, 

/* Listing of table: .set.nvm */
	14, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 45, 53 /* .set.nvm.alpha */ , 0, 5, 'a', 'l', 'p', 'h', 'a',
TOK_END, 

/* Listing of table: .set.nvm.alpha */
	27, T_NVM_ALPHA >> 8, T_NVM_ALPHA & 0xff, TOK_NUM, 255, 255, 255 /* End */ , 19, 'S', 'e', 't', ' ', 'a', 'l', 'p', 'h', 'a', ' ', 'c', 'o', 'm', 'p', 'o', 'n', 'e', 'n', 't',
TOK_END, 

/* Listing of table: .set.reprojection */
	51, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 46, 87 /* .set.reprojection.depth-tolerance */ , 27, 'S', 'p', 'e', 'c', 'i', 'f', 'y', ' ', 'd', 'e', 'p', 't', 'h', ' ', 'm', 'a', 'p', ' ', 't', 'o', 'l', 'e', 'r', 'a', 'n', 'c', 'e', 15, 'd', 'e', 'p', 't', 'h', '-', 't', 'o', 'l', 'e', 'r', 'a', 'n', 'c', 'e',
	48, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 46, 148 /* .set.reprojection.max-depth */ , 30, 'I', 'g', 'n', 'o', 'r', 'e', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'f', 'a', 'r', 't', 'h', 'e', 'r', ' ', 't', 'h', 'a', 'n', ' ', '.', '.', '.', 9, 'm', 'a', 'x', '-', 'd', 'e', 'p', 't', 'h',
	47, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 46, 191 /* .set.reprojection.min-depth */ , 29, 'I', 'g', 'n', 'o', 'r', 'e', ' ', 'p', 'o', 'i', 'n', 't', 's', ' ', 'c', 'l', 'o', 's', 'e', 'r', ' ', 't', 'h', 'a', 'n', ' ', '.', '.', '.', 9, 'm', 'i', 'n', '-', 'd', 'e', 'p', 't', 'h',
	65, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 46, 234 /* .set.reprojection.dot-range */ , 47, 'A', 'c', 'c', 'e', 'p', 't', 'e', 'd', ' ', 'n', 'o', 'r', 'm', 'a', 'l', ' ', '*', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'a', 'x', 'i', 's', ' ', 'd', 'o', 't', ' ', 'p', 'r', 'o', 'd', 'u', 'c', 't', ' ', 'v', 'a', 'l', 'u', 'e', 9, 'd', 'o', 't', '-', 'r', 'a', 'n', 'g', 'e',
	50, T_KEYWORD >> 8, T_KEYWORD & 0xff, TOK_KEYWORD, 0, 47, 34 /* .set.reprojection.cache */ , 36, 'E', 'n', 'a', 'b', 'l', 'e', ' ', 'o', 'r', ' ', 'd', 'i', 's', 'a', 'b', 'l', 'e', ' ', 'r', 'e', 'p', 'r', 'o', 'j', 'e', 'c', 't', 'i', 'o', 'n', ' ', 'c', 'a', 'c', 'h', 'e', 5, 'c', 'a', 'c', 'h', 'e',
TOK_END, 

/* Listing of table: .set.reprojection.depth-tolerance */
	60, T_ZTOL >> 8, T_ZTOL & 0xff, TOK_WORD, 0, 45, 81 /* .set.reprojection */ , 52, 'D', 'e', 'p', 't', 'h', ' ', 'm', 'a', 'p', ' ', 't', 'o', 'l', 'e', 'r', 'a', 'n', 'c', 'e', ' ', 'f', 'o', 'r', ' ', 'd', 'e', 't', 'e', 'r', 'm', 'i', 'n', 'i', 'n', 'g', ' ', 'p', 'o', 'i', 'n', 't', ' ', 'v', 'i', 's', 'i', 'b', 'i', 'l', 'i', 't', 'y',
TOK_END, 

/* Listing of table: .set.reprojection.max-depth */
	42, T_ZMAX >> 8, T_ZMAX & 0xff, TOK_WORD, 0, 45, 81 /* .set.reprojection */ , 34, 'M', 'a', 'x', 'i', 'm', 'u', 'm', ' ', 'd', 'i', 's', 't', 'a', 'n', 'c', 'e', ' ', 'f', 'r', 'o', 'm', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'l', 'a', 'n', 'e',
TOK_END, 

/* Listing of table: .set.reprojection.min-depth */
	42, T_ZMIN >> 8, T_ZMIN & 0xff, TOK_WORD, 0, 45, 81 /* .set.reprojection */ , 34, 'M', 'i', 'n', 'i', 'm', 'u', 'm', ' ', 'd', 'i', 's', 't', 'a', 'n', 'c', 'e', ' ', 'f', 'r', 'o', 'm', ' ', 'c', 'a', 'm', 'e', 'r', 'a', ' ', 'p', 'l', 'a', 'n', 'e',
TOK_END, 

/* Listing of table: .set.reprojection.dot-range */
	27, T_DOTMIN >> 8, T_DOTMIN & 0xff, TOK_WORD, 0, 47, 6 /* .set.reprojection.dot-range.<word> */ , 19, 'M', 'i', 'n', 'i', 'm', 'a', 'l', ' ', 'd', 'o', 't', ' ', 'p', 'r', 'o', 'd', 'u', 'c', 't',
TOK_END, 

/* Listing of table: .set.reprojection.dot-range.<word> */
	27, T_DOTMAX >> 8, T_DOTMAX & 0xff, TOK_WORD, 0, 45, 81 /* .set.reprojection */ , 19, 'M', 'a', 'x', 'i', 'm', 'a', 'l', ' ', 'd', 'o', 't', ' ', 'p', 'r', 'o', 'd', 'u', 'c', 't',
TOK_END, 

/* Listing of table: .set.reprojection.cache */
	37, T_ENABLE_CACHE >> 8, T_ENABLE_CACHE & 0xff, TOK_KEYWORD, 0, 45, 81 /* .set.reprojection */ , 22, 'E', 'n', 'a', 'b', 'l', 'e', ' ', 'd', 'e', 'p', 't', 'h', ' ', 'm', 'a', 'p', ' ', 'c', 'a', 'c', 'h', 'e', 6, 'e', 'n', 'a', 'b', 'l', 'e',
	39, T_DISABLE_CACHE >> 8, T_DISABLE_CACHE & 0xff, TOK_KEYWORD, 0, 45, 81 /* .set.reprojection */ , 23, 'D', 'i', 's', 'a', 'b', 'l', 'e', ' ', 'd', 'e', 'p', 't', 'h', ' ', 'm', 'a', 'p', ' ', 'c', 'a', 'c', 'h', 'e', 7, 'd', 'i', 's', 'a', 'b', 'l', 'e',
TOK_END, 

};
