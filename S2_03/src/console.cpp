#include "console.h"
#include "common.h"

#if defined(WIN32) || defined(_WIN32) /* si vous êtes sous Windows */
const char* convTable[] = {
	"Ç", "ü", "é", "â", "ä", "à", "å", "ç", "ê", "ë", "è", "ï", "î", "ì", "Ä", "Å",
	"É", "æ", "Æ", "ô", "ö", "ò", "û", "ù", "ÿ", "Ö", "Ü", "ø", "£", "Ø", "×", "ƒ",
	"á", "í", "ó", "ú", "ñ", "Ñ", "ª", "º", "¿", "®", "¬", "½", "¼", "¡", "«", "»",
	"░", "▒", "▓", "│", "┤", "Á", "Â", "À", "©", "╣", "║", "╗", "╝", "¢", "¥", "┐",
	"└", "┴", "┬", "├", "─", "┼", "ã", "Ã", "╚", "╔", "╩", "╦", "╠", "═", "╬", "¤",
	"ð", "Ð", "Ê", "Ë", "È", "ı", "Í", "Î", "Ï", "┘", "┌", "█", "▄", "¦", "Ì", "▀",
	"Ó", "ß", "Ô", "Ò", "õ", "Õ", "µ", "þ", "Þ", "Ú", "Û", "Ù", "ý", "Ý", "¯", "´",
	NULL, "±", "‗", "¾", "¶", "§", "÷", "¸", "°", "¨", "·", "¹", "³", "²", "■", NULL

};

// Convertit le Code page 850 de Microsoft vers l'UTF-8 pour une intercompatiblité des systèmes d'exploitation
const char* CP850ToUTF8(char chr) {
	if(chr > 0)
		return NULL;

	return convTable[chr + 0x80];
}
#endif

// Cette fonction reproduit le comportement de std::cin (ou scanf("%s", &str))
// À la différence que le texte est stocké en temps réel et que la fonctione peut être interrompue pour être réappelée plus tard
// L'intérêt étant de pouvoir déplacer la zone d'écriture à volonté
void fakeCin(std::string &str, bool* interrupt, bool printNewLine) {
	unsigned int col = 0;

	if(str.length() > 0) {
		printf("%s", str.data());
		col = str.length();
	}

	int skipNext = 0;
	while((interrupt == NULL) || !(*interrupt)) {
		#if defined (linux) /* si vous êtes sous Linux */
		fflush(stdout);
		#endif

		while(!_kbhit() && ((interrupt == NULL) || !(*interrupt))) {}
		if(!((interrupt == NULL) || !(*interrupt)))
			break;

		int chrI = _getch();
		char chr = chrI;
		if(chr == '\t')
			continue;
		
		if(skipNext) {
			skipNext--;
			continue;
		}
		if(chr == 8 || chr == 127) {
			if(col > 0) {
				int nbChr = (str[col-1] < 0) + 1;
				str.erase(col-nbChr, nbChr);

				printf("%c", 8);
				ClearEndOfLine();
				col -= nbChr;
				printf("%s", str.data()+col);
				MoveCursorCol(- (str.length() - col));
				if(col >= str.length())
					MoveCursorCol(-1);
			}
		}
		#if defined(WIN32) || defined(_WIN32) /* si vous êtes sous Windows */
		else if(chr == -32) {
			while(!_kbhit()) {}
			chr = _getch();

			if(chr == 'K' && col > 0) {
				MoveCursorCol(-1);
				col -= (str[col-1] < 0) + 1;
			}
			else if(chr == 'M' && col < str.length()) {
				MoveCursorCol(1);
				col += (str[col+1] < 0) + 1;
			}
			else if(chr == 'S' && col < str.length()) {
		#elif defined (linux) /* si vous êtes sous Linux */
		else if(chr == 126) {
		#endif
				int nbChr = (str[col] < 0) + 1;
				str.erase(col, nbChr);

				ClearEndOfLine();
				printf("%s", str.data()+col);
				MoveCursorCol(- (str.length() - col));
				if(col >= str.length())
					MoveCursorCol(-1);
			}
		#if defined(WIN32) || defined(_WIN32) /* si vous êtes sous Windows */
		}
		#elif defined (linux) /* si vous êtes sous Linux */
		else if(chr == 27) {
			while(!_kbhit()) {}
			chr = _getch();
			while(!_kbhit()) {}
			chr = _getch();

			if(chr == 'D' && col > 0) {
				MoveCursorCol(-1);
				col -= (str[col-1] < 0) + 1;
			}
			else if(chr == 'C' && col < str.length()) {
				MoveCursorCol(1);
				col += (str[col+1] < 0) + 1;
			}
		}
		else if(chr == 3) {
			quitApp(0);
			break;
		}
		#endif
		else if(chr == '\n' || chr == '\r') {
			if(str.length() > 0) {
				if(printNewLine)
					printf("\n");
				break;
			}
		}
		else {
			std::string tp;
			tp.push_back(chr);
			
			#if defined(WIN32) || defined(_WIN32) /* si vous êtes sous Windows */
			if(chr < 0) {
				const char* converted = CP850ToUTF8(chr);
				if(!converted)
					continue;
					
				tp = converted;
			}
			#elif defined (linux) /* si vous êtes sous Linux */
			if(chr < 0) {
				while(!_kbhit()) {}
				tp.push_back(_getch());
			}
			#endif

			int nbChr = tp.length();
			for(int i = 0; i < nbChr; i++)
				str.insert(col+i, 1, tp[i]);

			printf("%s", str.data()+col);
			col += nbChr;
			MoveCursorCol(- (str.length() - col));
			if(col >= str.length())
				MoveCursorCol(-1);
		}
	}
}