#ifndef FLEXPTP_PORT_PTP_PORT_MK64F_LWIP
#define FLEXPTP_PORT_PTP_PORT_MK64F_LWIP

#include "flexptp/timeutils.h"

/**
 * Get time right from the hardware clock.
 * 
 * @param pTime pointer to a TimestampU object.
 */
void ptphw_gettime(TimestampU * pTime);

#endif /* FLEXPTP_PORT_PTP_PORT_MK64F_LWIP */
