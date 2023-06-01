
//{{BLOCK(title)

//======================================================================
//
//	title, 240x88@8, 
//	+ palette 16 entries, not compressed
//	+ bitmap not compressed
//	Total size: 32 + 21120 = 21152
//
//	Time-stamp: 2023-05-24, 16:29:55
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLE_H
#define GRIT_TITLE_H

#ifdef CARTRIDGE
#define titleBitmapLen 3360
extern const unsigned char titleBitmap[3360];
#else
#define titleBitmapLen 21120
extern const unsigned char titleBitmap[21120];
#endif

#define titlePalLen 32
extern const unsigned short titlePal[16];

#endif // GRIT_TITLE_H

//}}BLOCK(title)
