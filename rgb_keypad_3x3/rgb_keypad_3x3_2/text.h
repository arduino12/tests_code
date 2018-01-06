#ifndef TEXT_H
#define TEXT_H

// #define TEXT_ARRAY

//{ Hebrew letters
#ifdef TEXT_ARRAY
	#define ALEF				(0x80)
	#define BET					(0x81)
	#define GIMEL				(0x82)
	#define DALED				(0x83)
	#define HEY					(0x84)
	#define VAV					(0x85)
	#define ZAIN				(0x86)
	#define HET					(0x87)
	#define TET					(0x88)
	#define YOD					(0x89)
	#define HAF_S				(0x8A)
	#define HAF					(0x8B)
	#define LAMED				(0x8C)
	#define MEM_S				(0x8D)
	#define MEM					(0x8E)
	#define NON_S				(0x8F)
	#define NON					(0x90)
	#define SAMECH				(0x91)
	#define AIN					(0x92)
	#define PEY_S	 			(0x93)
	#define PEY					(0x94)
	#define ZADIK_S		 		(0x95)
	#define ZADIK				(0x96)
	#define KOF					(0x97)
	#define REISH				(0x98)
	#define SHEN				(0x99)
	#define TAF					(0x9A)
	
	char hello_hebrew[] = {' ', MEM_SOFIT, LAMED, VAV, AIN, ' ', MEM_SOFIT, VAV, LAMED, SHEN, 0};
#else
	#define ALEF				"\x80"
	#define BET					"\x81"
	#define GIMEL				"\x82"
	#define DALED				"\x83"
	#define HEY					"\x84"
	#define VAV					"\x85"
	#define ZAIN				"\x86"
	#define HET					"\x87"
	#define TET					"\x88"
	#define YOD					"\x89"
	#define HAF_S				"\x8A"
	#define HAF					"\x8B"
	#define LAMED				"\x8C"
	#define MEM_S				"\x8D"
	#define MEM					"\x8E"
	#define NON_S				"\x8F"
	#define NON					"\x90"
	#define SAMECH				"\x91"
	#define AIN					"\x92"
	#define PEY_S				"\x93"
	#define PEY					"\x94"
	#define ZADI_S				"\x95"
	#define ZADI				"\x96"
	#define KOF					"\x97"
	#define REISH				"\x98"
	#define SHIN				"\x99"
	#define TAF					"\x9A"

	#define TEXT_INIT_1			("3x3 rgb keys V1.0")
	#define TEXT_INIT_2			("!2016 " REISH MEM GIMEL " " TET KOF YOD VAV REISH PEY)
	#define TEXT_X_O			(LAMED VAV GIMEL YOD AIN " " SAMECH KOF YOD ALEF)
	#define TEXT_SIMON			(NON VAV MEM YOD YOD SAMECH)
#endif

#endif /* TEXT_H */
