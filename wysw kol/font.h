#include <avr/io.h>
#include <stdint.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

/* Schriftendefinition
 * Die Definition ist f�r Proportionalschriften ausgelegt.
 * Festbreitenschriften sind (momentan) auch m�glich, ben�tigen aber unn�tig viel Platz.
 * Die Daten sind folgenderma�en aufgebaut:
 * 1. Byte
 *   1. Nibble: Reserviert
 *   2. Nibble: H�he der Schrift in Bytes (1: 8 Pixel, 2: 16 Pixel, ...)
 * 2. bis 113. Byte
 *   Breitenangabe der Zeichen. Die Definition beginnt bei Charcode 32 (Leerzeichen)
 *   und endet mit Charcode 255. Jedes Byte beschreibt (um Platz zu sparen) die Breite
 *   von zwei Zeichen:
 *   1. Nibble: Breite erstes Zeichen
 *   2. Nibble: Breite zweites Zeichen
 *
 */
static uint8_t charset[] PROGMEM = {
	0x00,//Definition der Zeichenh�he
	32,  //Beginn des Charsets
	224, //Anzahl der Zeichen
  //L�nge der einzelnen Character
	0x21, 0x35, 0x55, 0x51, 0x22, //032-041
	0x55, 0x24, 0x13, 0x52, 0x44, //042-051
	0x44, 0x44, 0x44, 0x12, 0x44, //052-061
	0x45, 0x54, 0x44, 0x44, 0x44, //062-071
	0x41, 0x34, 0x45, 0x55, 0x45, //072-081
	0x44, 0x54, 0x55, 0x55, 0x42, //082-091
	0x32, 0x54, 0x24, 0x44, 0x44, //092-101
	0x24, 0x41, 0x44, 0x15, 0x44, //102-111
	0x44, 0x33, 0x24, 0x55, 0x44, //112-121
	0x43, 0x13, 0x45, 0x55, 0x55, //122-131
	0x55, 0x55, 0x55, 0x55, 0x55, //132-141
	0x55, 0x55, 0x55, 0x55, 0x55, //142-151
	0x55, 0x55, 0x55, 0x55, 0x53, //152-161
	0x33, 0x32, 0x53, 0x45, 0x55, //162-171
	0x55, 0x45, 0x35, 0x55, 0x24, //172-181
	0x55, 0x55, 0x55, 0x55, 0x55, //182-191
	0x55, 0x55, 0x45, 0x55, 0x55, //192-201
	0x55, 0x55, 0x55, 0x45, 0x55, //202-211
	0x55, 0x55, 0x55, 0x55, 0x45, //212-221
	0x44, 0x54, 0x55, 0x45, 0x55, //222-231
	0x55, 0x23, 0x55, 0x55, 0x55, //232-241
	0x55, 0x55, 0x45, 0x55, 0x55, //242-251
	0x45, 0x55,                   //252-255
	//Der eigentlichen Charset
	0x00, 0x00,                   //032:
	0x5F,                         //033: !
	0x03, 0x00, 0x03,             //034: "
	0x14, 0x7F, 0x14, 0x7F, 0x14, //035: #
	0x4C, 0x5A, 0xFF, 0x32, 0x00, //036: $
	0x63, 0x1B, 0x6C, 0x63, 0x00, //037: %
	0x36, 0x49, 0x56, 0x20, 0x50, //038: &
	0x03,                         //039: '
	0x3E, 0x41,                   //040: (
	0x41, 0x3E,                   //041: )
	0x08, 0x2A, 0x1C, 0x2A, 0x08, //042: *
	0x08, 0x08, 0x3E, 0x08, 0x08, //043: +
	0x80, 0x40,                   //044: ,
	0x08, 0x08, 0x08, 0x08,       //045: -
	0x40,                         //046: .
	0x60, 0x1C, 0x03,             //047: /
	0x3E, 0x41, 0x41, 0x41, 0x3E, //048: 0
	0x02, 0x7F,                   //049: 1
	0x71, 0x49, 0x49, 0x46,       //050: 2
	0x41, 0x49, 0x49, 0x36,       //051: 3
	0x18, 0x14, 0x12, 0x7F,       //052: 4
	0x47, 0x45, 0x45, 0x38,       //053: 5
	0x3E, 0x49, 0x49, 0x30,       //054: 6
	0x01, 0x71, 0x0D, 0x03,       //055: 7
	0x36, 0x49, 0x49, 0x36,       //056: 8
	0x06, 0x49, 0x49, 0x3E,       //057: 9
	0x24,                         //058: :
	0x80, 0x48,                   //059: ;
	0x08, 0x14, 0x22, 0x41,       //060: <
	0x14, 0x14, 0x14, 0x14,       //061: =
	0x41, 0x22, 0x14, 0x08,       //062: >
	0x00, 0x01, 0x59, 0x05, 0x02, //063: ?
	0x3E, 0x49, 0x55, 0x5D, 0x2E, //064: @
	0x7E, 0x05, 0x05, 0x7E,       //065: A
	0x7F, 0x45, 0x45, 0x3A,       //066: B
	0x3E, 0x41, 0x41, 0x41,       //067: C
	0x7F, 0x41, 0x41, 0x3E,       //068: D
	0x7F, 0x45, 0x45, 0x41,       //069: E
	0x7F, 0x05, 0x05, 0x01,       //070: F
	0x3E, 0x41, 0x45, 0x7D,       //071: G
	0x7F, 0x04, 0x04, 0x7F,       //072: H
	0x7F,                         //073: I
	0x40, 0x40, 0x3F,             //074: J
	0x7F, 0x14, 0x22, 0x41,       //075: K
	0x7F, 0x40, 0x40, 0x40,       //076: L
	0x7F, 0x02, 0x0C, 0x02, 0x7F, //077: M
	0x7F, 0x02, 0x04, 0x08, 0x7F, //078: N
	0x3E, 0x41, 0x41, 0x41, 0x3E, //079: O
	0x7F, 0x11, 0x11, 0x0E,       //080: P
	0x3E, 0x41, 0x61, 0x41, 0xBE, //081: Q
	0x7F, 0x11, 0x11, 0x6E,       //082: R
	0x46, 0x49, 0x49, 0x31,       //083: S
	0x01, 0x01, 0x7F, 0x01, 0x01, //084: T
	0x3F, 0x40, 0x40, 0x3F,       //085: U
	0x07, 0x18, 0x60, 0x18, 0x07, //086: V
	0x7F, 0x20, 0x18, 0x20, 0x7F, //087: W
	0x63, 0x14, 0x08, 0x14, 0x63, //088: X
	0x03, 0x04, 0x78, 0x04, 0x03, //089: Y
	0x71, 0x49, 0x45, 0x43,       //090: Z
	0x7F, 0x41,                   //091:
	0x03, 0x1C, 0x60,             //092:
	0x41, 0x7F,                   //093:
	0x04, 0x02, 0x01, 0x02, 0x04, //094:
	0x40, 0x40, 0x40, 0x40,       //095:
	0x01, 0x02,                   //096:
	0x20, 0x54, 0x54, 0x78,       //097: a
	0x7F, 0x44, 0x44, 0x38,       //098: b
	0x38, 0x44, 0x44, 0x44,       //099: c
	0x38, 0x44, 0x44, 0x7F,       //100: d
	0x38, 0x54, 0x54, 0x18,       //101: e
	0x7E, 0x09,                   //102: f
	0x18, 0xA4, 0xA4, 0x7C,       //103: g
	0x7F, 0x04, 0x04, 0x78,       //104: h
	0x7D,                         //105: i
	0x40, 0x80, 0x84, 0x7D,       //106: j
	0x7F, 0x10, 0x28, 0x44,       //107: k
	0x7F,                         //108: l
	0x7C, 0x04, 0x7C, 0x04, 0x78, //109: m
	0x7C, 0x04, 0x04, 0x78,       //110: n
	0x38, 0x44, 0x44, 0x38,       //111: o
	0xFC, 0x24, 0x24, 0x18,       //112: p
	0x18, 0x24, 0x24, 0xFC,       //113: q
	0x7C, 0x08, 0x04,             //114: r
	0x48, 0x54, 0x24,             //115: s
	0x3F, 0x44,                   //116: t
	0x3C, 0x40, 0x40, 0x7C,       //117: u
	0x1C, 0x20, 0x40, 0x20, 0x1C, //118: v
	0x1C, 0x60, 0x18, 0x60, 0x1C, //119:
	0x6C, 0x10, 0x10, 0x6C,       //120:
	0x1C, 0xA0, 0xA0, 0x7C,       //121:
	0x64, 0x54, 0x4C, 0x44,       //122:
	0x08, 0x36, 0x41,             //123:
	0xFF,                         //124:
	0x41, 0x36, 0x08,             //125:
	0x04, 0x02, 0x04, 0x02,       //126:
	0x08, 0x1C, 0x2A, 0x08, 0x08, //127:
	0x18, 0x3C, 0x5A, 0x4A, 0x24, //128:
	0x00, 0x00, 0x00, 0x00, 0x60, //129:
	0x00, 0x00, 0x00, 0x00, 0x70, //130:
	0x00, 0x00, 0x00, 0x00, 0x78, //131:
	0x00, 0x00, 0x00, 0x00, 0x7C, //132:
	0x00, 0x00, 0x00, 0x00, 0x7E, //133:
	0x00, 0x00, 0x00, 0x00, 0x7F, //134:
	0x00, 0x00, 0x00, 0x40, 0x7F, //135:
	0x00, 0x00, 0x00, 0x60, 0x7F, //136:
	0x00, 0x00, 0x00, 0x70, 0x7F, //137:
	0x00, 0x00, 0x00, 0x78, 0x7F, //138:
	0x00, 0x00, 0x00, 0x7C, 0x7F, //139:
	0x00, 0x00, 0x00, 0x7E, 0x7F, //140:
	0x00, 0x00, 0x00, 0x7F, 0x7F, //141:
	0x00, 0x00, 0x40, 0x7F, 0x7F, //142:
	0x00, 0x00, 0x60, 0x7F, 0x7F, //143:
	0x00, 0x00, 0x70, 0x7F, 0x7F, //144:
	0x00, 0x00, 0x78, 0x7F, 0x7F, //145:
	0x00, 0x00, 0x7C, 0x7F, 0x7F, //146:
	0x00, 0x00, 0x7E, 0x7F, 0x7F, //147:
	0x00, 0x00, 0x7F, 0x7F, 0x7F, //148:
	0x00, 0x40, 0x7F, 0x7F, 0x7F, //149:
	0x00, 0x60, 0x7F, 0x7F, 0x7F, //150:
	0x00, 0x70, 0x7F, 0x7F, 0x7F, //151:
	0x00, 0x78, 0x7F, 0x7F, 0x7F, //152:
	0x00, 0x7C, 0x7F, 0x7F, 0x7F, //153:
	0x00, 0x7E, 0x7F, 0x7F, 0x7F, //154:
	0x00, 0x7F, 0x7F, 0x7F, 0x7F, //155:
	0x40, 0x7F, 0x7F, 0x7F, 0x7F, //156:
	0x60, 0x7F, 0x7F, 0x7F, 0x7F, //157:
	0x70, 0x7F, 0x7F, 0x7F, 0x7F, //158:
	0x78, 0x7F, 0x7F, 0x7F, 0x7F, //159:
	0x7C, 0x7F, 0x7F, 0x7F, 0x7F, //160:
	0x70, 0x50, 0x70,             //161:
	0x0F, 0x01, 0x01,             //162:
	0x40, 0x40, 0x78,             //163:
	0x10, 0x20, 0x40,             //164:
	0x18, 0x18,                   //165:
	0x0A, 0x0A, 0x4A, 0x2A, 0x1E, //166:
	0x2E, 0x55, 0x3A,             //167:
	0x20, 0x10, 0x78, 0x04,       //168:
	0x08, 0x08, 0x4C, 0x48, 0x38, //169:
	0x48, 0x48, 0x78, 0x48, 0x48, //170:
	0x48, 0x28, 0x18, 0x7C, 0x08, //171:
	0x08, 0x7C, 0x08, 0x28, 0x18, //172:
	0x40, 0x48, 0x48, 0x78, 0x40, //173:
	0x54, 0x54, 0x54, 0x7C,       //174:
	0x18, 0x00, 0x58, 0x40, 0x38, //175:
	0x02, 0x05, 0x02,             //176:
	0x01, 0x41, 0x3D, 0x09, 0x07, //177:
	0x10, 0x08, 0x7C, 0x02, 0x01, //178:
	0x0E, 0x02, 0x43, 0x22, 0x1E, //179:
	0x02, 0x01,                   //180:
	0xFC, 0x20, 0x1C, 0x20,       //181:
	0x42, 0x3F, 0x02, 0x42, 0x3E, //182:
	0x0A, 0x0A, 0x7F, 0x0A, 0x0A, //183:
	0x04, 0x43, 0x41, 0x21, 0x1F, //184:
	0x04, 0x03, 0x42, 0x3E, 0x02, //185:
	0x42, 0x42, 0x42, 0x42, 0x7E, //186:
	0x02, 0x4F, 0x22, 0x1F, 0x02, //187:
	0x4A, 0x4A, 0x40, 0x20, 0x18, //188:
	0x42, 0x22, 0x12, 0x2A, 0x46, //189:
	0x02, 0x1F, 0x22, 0x2A, 0x26, //190:
	0x06, 0x48, 0x40, 0x20, 0x1E, //191:
	0x08, 0x46, 0x4A, 0x32, 0x1E, //192:
	0x0A, 0x4A, 0x3E, 0x09, 0x08, //193:
	0x0E, 0x00, 0x4E, 0x20, 0x1E, //194:
	0x04, 0x45, 0x3D, 0x05, 0x04, //195:
	0x79, 0x14, 0x14, 0x79,       //196:
	0x44, 0x24, 0x1F, 0x04, 0x04, //197:
	0x40, 0x42, 0x42, 0x42, 0x40, //198:
	0x42, 0x2A, 0x12, 0x2A, 0x06, //199:
	0x22, 0x12, 0x7B, 0x16, 0x22, //200:
	0x00, 0x40, 0x20, 0x1F, 0x00, //201:
	0x78, 0x01, 0x02, 0x04, 0x78, //202:
	0x3F, 0x42, 0x42, 0x42, 0x42, //203:
	0x02, 0x02, 0x42, 0x22, 0x1E, //204:
	0x04, 0x02, 0x04, 0x08, 0x30, //205:
	0x32, 0x02, 0x7F, 0x02, 0x32, //206:
	0x02, 0x12, 0x22, 0x52, 0x0E, //207:
	0x2A, 0x2A, 0x2A, 0x40,       //208:
	0x38, 0x24, 0x22, 0x20, 0x70, //209:
	0x40, 0x28, 0x10, 0x28, 0x06, //210:
	0x0A, 0x3E, 0x4A, 0x4A, 0x4A, //211:
	0x04, 0x7F, 0x04, 0x14, 0x0C, //212:
	0x40, 0x42, 0x42, 0x7E, 0x40, //213:
	0x38, 0x45, 0x44, 0x45, 0x38, //214:
	0x04, 0x05, 0x45, 0x25, 0x1C, //215:
	0x0F, 0x40, 0x20, 0x1F, 0x00, //216:
	0x7E, 0x00, 0x7F, 0x40, 0x30, //217:
	0x7E, 0x40, 0x20, 0x10, 0x08, //218:
	0x7E, 0x42, 0x42, 0x42, 0x7E, //219:
	0x3D, 0x40, 0x40, 0x3D,       //220:
	0x42, 0x42, 0x40, 0x20, 0x18, //221:
	0x02, 0x04, 0x01, 0x02,       //222:
	0xFE, 0x41, 0x49, 0x36,       //223:
	0x38, 0x44, 0x48, 0x30, 0x4C, //224:
	0x20, 0x55, 0x54, 0x79,       //225:
	0xFC, 0x2A, 0x2A, 0x2A, 0x14, //226:
	0x28, 0x54, 0x54, 0x44, 0x20, //227:
	0x20, 0x55, 0x54, 0x79,       //228:
	0x38, 0x44, 0x4C, 0x54, 0x24, //229:
	0xF0, 0x48, 0x44, 0x44, 0x38, //230:
	0x38, 0x44, 0x44, 0x44, 0xFC, //231:
	0x20, 0x40, 0x3C, 0x04, 0x04, //232:
	0x02, 0x02, 0x00, 0x07, 0x00, //233:
	0x02, 0xFF,                   //234:
	0x0A, 0x04, 0x0A,             //235:
	0x18, 0x24, 0x7E, 0x24, 0x10, //236:
	0x14, 0x7F, 0x54, 0x40, 0x40, //237:
	0x7C, 0x09, 0x05, 0x05, 0x78, //238:
	0x38, 0x45, 0x44, 0x45, 0x38, //239:
	0xFC, 0x48, 0x44, 0x44, 0x38, //240:
	0x38, 0x44, 0x44, 0x48, 0xFC, //241:
	0x3C, 0x4A, 0x4A, 0x4A, 0x3C, //242:
	0x30, 0x28, 0x10, 0x28, 0x18, //243:
	0x58, 0x64, 0x04, 0x64, 0x58, //244:
	0x3C, 0x41, 0x40, 0x21, 0x7C, //245:
	0x39, 0x44, 0x44, 0x39,       //246:
	0x44, 0x3C, 0x04, 0x7C, 0x44, //247:
	0x45, 0x29, 0x11, 0x29, 0x45, //248:
	0x3C, 0x40, 0x40, 0x40, 0x7C, //249:
	0x14, 0x14, 0x7C, 0x14, 0x12, //250:
	0x44, 0x3C, 0x14, 0x14, 0x74, //251:
	0x3D, 0x40, 0x40, 0x7D,       //252:
	0x10, 0x10, 0x54, 0x10, 0x10, //253:
	0x00, 0x00, 0x00, 0x00, 0x00, //254:
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF  //255:
};
