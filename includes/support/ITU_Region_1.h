
#ifndef	__ITU_Region_1__
#define	__ITU_Region_1__

#include	<QString>
#include	<stdint.h>


QString		find_Country (uint8_t ecc, uint8_t countryId);
QString		find_ITU_code (uint8_t ecc, uint8_t countryId);
#endif



