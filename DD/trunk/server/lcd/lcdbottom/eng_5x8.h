/*���ļ�Ϊ5��8���������ֿ��ļ�,�ֵĺ���8�㹹��һ�ֽ�,��ߵ����ֽڵĸ�λ,�ַ������Ľǰ����Ͻǡ����Ͻǡ����½ǡ����½�ȡ��*/
/*���������Ժ������HZDotReader���ļ�,׷�Ӻ��ֵ�������,�벻Ҫ�޸Ļ���ɾ����ע��*/

const unsigned char font_eng[768] = {
/*    CHAR_20 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*!   CHAR_21 */
0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00,

/*"   CHAR_22 */
0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00,

/*#   CHAR_23 */
0x50,0x50,0xF8,0x50,0xF8,0x50,0x50,0x00,

/*$   CHAR_24 */
0x20,0x78,0xC0,0x70,0x28,0xF0,0x20,0x00,

/*%   CHAR_25 */
0xC0,0xC8,0x10,0x20,0x40,0x98,0x18,0x00,

/*&   CHAR_26 */
0x40,0xA0,0xA0,0x40,0xA8,0x90,0x68,0x00,

/*'   CHAR_27 */
0x30,0x20,0x40,0x00,0x00,0x00,0x00,0x00,

/*(   CHAR_28 */
0x10,0x20,0x40,0x40,0x40,0x20,0x10,0x00,

/*)   CHAR_29 */
0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00,

/**   CHAR_2A */
0x20,0xA8,0x70,0x20,0x70,0xA8,0x20,0x00,

/*+   CHAR_2B */
0x20,0x20,0x20,0xF8,0x20,0x20,0x20,0x00,

/*,   CHAR_2C */
0x00,0x00,0x00,0x00,0x60,0x40,0x80,0x00,

/*-   CHAR_2D */
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,

/*.   CHAR_2E */
0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,

/*/   CHAR_2F */
0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,

/*0   CHAR_30 */
0x70,0x88,0x98,0xA8,0xC8,0x88,0x70,0x00,

/*1   CHAR_31 */
0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00,

/*2   CHAR_32 */
0x70,0x88,0x08,0x30,0x40,0x80,0xF8,0x00,

/*3   CHAR_33 */
0xF8,0x08,0x10,0x30,0x08,0x88,0x70,0x00,

/*4   CHAR_34 */
0x10,0x30,0x50,0x90,0xF8,0x10,0x10,0x00,

/*5   CHAR_35 */
0xF8,0x80,0xF0,0x08,0x08,0x88,0x70,0x00,

/*6   CHAR_36 */
0x38,0x40,0x80,0xF0,0x88,0x88,0x70,0x00,

/*7   CHAR_37 */
0xF8,0x08,0x10,0x20,0x40,0x40,0x40,0x00,

/*8   CHAR_38 */
0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00,

/*9   CHAR_39 */
0x70,0x88,0x88,0x78,0x08,0x10,0xE0,0x00,

/*:   CHAR_3A */
0x00,0x60,0x60,0x00,0x60,0x60,0x00,0x00,

/*;   CHAR_3B */
0x00,0x60,0x60,0x00,0x60,0x60,0xC0,0x00,

/*<   CHAR_3C */
0x10,0x20,0x40,0x80,0x40,0x20,0x10,0x00,

/*=   CHAR_3D */
0x00,0x00,0xF8,0x00,0xF8,0x00,0x00,0x00,

/*>   CHAR_3E */
0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00,

/*?   CHAR_3F */
0x70,0x88,0x10,0x20,0x20,0x00,0x20,0x00,

/*@   CHAR_40 */
0x70,0x88,0xB8,0xA8,0xB8,0x80,0x78,0x00,

/*A   CHAR_41 */
0x20,0x50,0x88,0x88,0xF8,0x88,0x88,0x00,

/*B   CHAR_42 */
0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0,0x00,

/*C   CHAR_43 */
0x70,0x88,0x80,0x80,0x80,0x88,0x70,0x00,

/*D   CHAR_44 */
0xF0,0x88,0x88,0x88,0x88,0x88,0xF0,0x00,

/*E   CHAR_45 */
0xF8,0x80,0x80,0xF0,0x80,0x80,0xF8,0x00,

/*F   CHAR_46 */
0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0x00,

/*G   CHAR_47 */
0x70,0x88,0x80,0x80,0xB8,0x88,0x78,0x00,

/*H   CHAR_48 */
0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,

/*I   CHAR_49 */
0x70,0x20,0x20,0x20,0x20,0x20,0x70,0x00,

/*J   CHAR_4A */
0x38,0x10,0x10,0x10,0x10,0x90,0x60,0x00,

/*K   CHAR_4B */
0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88,0x00,

/*L   CHAR_4C */
0x80,0x80,0x80,0x80,0x80,0x80,0xF8,0x00,

/*M   CHAR_4D */
0x88,0xD8,0xA8,0xA8,0x88,0x88,0x88,0x00,

/*N   CHAR_4E */
0x88,0x88,0xC8,0xA8,0x98,0x88,0x88,0x00,

/*O   CHAR_4F */
0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00,

/*P   CHAR_50 */
0xF0,0x88,0x88,0xF0,0x80,0x80,0x80,0x00,

/*Q   CHAR_51 */
0x70,0x88,0x88,0x88,0xA8,0x90,0x68,0x00,

/*R   CHAR_52 */
0xF0,0x88,0x88,0xF0,0xA0,0x90,0x88,0x00,

/*S   CHAR_53 */
0x70,0x88,0x80,0x70,0x08,0x88,0x70,0x00,

/*T   CHAR_54 */
0xF8,0x20,0x20,0x20,0x20,0x20,0x20,0x00,

/*U   CHAR_55 */
0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,

/*V   CHAR_56 */
0x88,0x88,0x88,0x88,0x88,0x50,0x20,0x00,

/*W   CHAR_57 */
0x88,0x88,0x88,0xA8,0xA8,0xD8,0x88,0x00,

/*X   CHAR_58 */
0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00,

/*Y   CHAR_59 */
0x88,0x88,0x50,0x20,0x20,0x20,0x20,0x00,

/*Z   CHAR_5A */
0xF8,0x08,0x10,0x20,0x40,0x80,0xF8,0x00,

/*[   CHAR_5B */
0xF0,0xC0,0xC0,0xC0,0xC0,0xC0,0xF0,0x00,

/*\   CHAR_5C */
0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,

/*]   CHAR_5D */
0x78,0x18,0x18,0x18,0x18,0x18,0x78,0x00,

/*^   CHAR_5E */
0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00,

/*_   CHAR_5F */
0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,

/*`   CHAR_60 */
0x20,0x10,0x08,0x00,0x00,0x00,0x00,0x00,

/*a   CHAR_61 */
0x00,0x00,0xE0,0x10,0x70,0x90,0x68,0x00,

/*b   CHAR_62 */
0x80,0x80,0xB0,0xC8,0x88,0xC8,0xB0,0x00,

/*c   CHAR_63 */
0x00,0x00,0x70,0x88,0x80,0x80,0x70,0x00,

/*d   CHAR_64 */
0x08,0x08,0x68,0x98,0x88,0x98,0x68,0x00,

/*e   CHAR_65 */
0x00,0x00,0x70,0x88,0xF0,0x80,0x70,0x00,

/*f   CHAR_66 */
0x30,0x48,0x40,0xF0,0x40,0x40,0x40,0x00,

/*g   CHAR_67 */
0x00,0x00,0x70,0x88,0x88,0x78,0x08,0xF0,

/*h   CHAR_68 */
0x80,0x80,0xB0,0xC8,0x88,0x88,0x88,0x00,

/*i   CHAR_69 */
0x20,0x00,0x00,0x20,0x20,0x20,0x20,0x00,

/*j   CHAR_6A */
0x10,0x00,0x00,0x30,0x10,0x10,0x10,0x60,

/*k   CHAR_6B */
0x80,0x80,0x90,0xA0,0xC0,0xA0,0x98,0x00,

/*l   CHAR_6C */
0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00,

/*m   CHAR_6D */
0x00,0x00,0x50,0xA8,0xA8,0xA8,0xA8,0x00,

/*n   CHAR_6E */
0x00,0x00,0xB0,0x48,0x48,0x48,0x48,0x00,

/*o   CHAR_6F */
0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00,

/*p   CHAR_70 */
0x00,0x00,0xF0,0x88,0x88,0xF0,0x80,0x80,

/*q   CHAR_71 */
0x00,0x00,0x78,0x88,0x88,0x78,0x08,0x08,

/*r   CHAR_72 */
0x00,0x00,0xB0,0x48,0x40,0x40,0x40,0x00,

/*s   CHAR_73 */
0x00,0x00,0x78,0x80,0x70,0x08,0xF0,0x00,

/*t   CHAR_74 */
0x40,0x40,0xF8,0x40,0x40,0x48,0x30,0x00,

/*u   CHAR_75 */
0x00,0x00,0x90,0x90,0x90,0x90,0x68,0x00,

/*v   CHAR_76 */
0x00,0x00,0x88,0x88,0x88,0x50,0x20,0x00,

/*w   CHAR_77 */
0x00,0x00,0xA8,0xA8,0xA8,0xA8,0x50,0x00,

/*x   CHAR_78 */
0x00,0x00,0x88,0x50,0x20,0x50,0x88,0x00,

/*y   CHAR_79 */
0x00,0x00,0x88,0x88,0x98,0x68,0x08,0xF0,

/*z   CHAR_7A */
0x00,0x00,0xF8,0x10,0x20,0x40,0xF8,0x00,

/*{   CHAR_7B */
0x20,0x40,0x40,0x80,0x40,0x40,0x20,0x00,

/*|   CHAR_7C */
0x20,0x20,0x20,0x00,0x20,0x20,0x20,0x00,

/*}   CHAR_7D */
0x20,0x10,0x10,0x08,0x10,0x10,0x20,0x00,

/*~   CHAR_7E */
0x00,0x00,0x40,0xA8,0x10,0x00,0x00,0x00,

/*   CHAR_7F */
0xA8,0x50,0xA8,0x50,0xA8,0x50,0xA8,0x00

};