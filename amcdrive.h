#ifndef _AMCDRIVE_H_
#define _AMCDRIVE_H_

#include <ntcan.h>
#include "byteorder.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    NTCAN_HANDLE handle;
    
    uint canopen_id; // CANopen identifier
    int8_t current_sign; // Flip current?
    int8_t error_state; // Is the dirve currently in a state of error?
    
    int16_t k_i;    // feedback position interp
    uint32_t k_s;    // switch freq
    uint16_t k_p;    // max rated current
    int32_t raw_position; // raw position in counts as reported by the drive
    double position; // offset position in counts as a double precision float to avoid wraparound
    double vel_cps;  //velocity in counts per second
    double i_ref; // target current
    double i_act; // actual current

    // PDO->COB mappings
    uint16_t rpdo_position;
    uint16_t rpdo_velocity;
    uint16_t rpdo_current;

    uint16_t tpdo_position;
    uint16_t tpdo_velocity;
    uint16_t tpdo_current;

    int fresh_position;
    int fresh_velocity;
    int fresh_current;
} servo_vars_t;

#define ENABLE_RPDO_POSITION 0x01
#define ENABLE_RPDO_VELOCITY 0x02
#define ENABLE_RPDO_CURRENT  0x04

#define REQUEST_TPDO_POSITION 0x10
#define REQUEST_TPDO_VELOCITY 0x20
#define REQUEST_TPDO_CURRENT  0x40

NTCAN_RESULT amcdrive_init_drive(NTCAN_HANDLE network, uint identifier, uint pdos, uint update_freq, servo_vars_t *drive_info);
NTCAN_RESULT amcdrive_init_drives(NTCAN_HANDLE network, uint *identifiers, uint count, uint pdos, uint update_freq, servo_vars_t *drive_infos);

NTCAN_RESULT amcdrive_open_drives(uint network, uint *identifiers, uint count, uint pdos, uint update_freq, servo_vars_t *drive_infos);
NTCAN_RESULT amcdrive_update_drives(servo_vars_t *drives, int count);

NTCAN_RESULT amcdrive_set_current(servo_vars_t *drive, double amps);

NTCAN_RESULT amcdrive_start_drive(servo_vars_t *drive);
NTCAN_RESULT amcdrive_stop_drive(servo_vars_t *drive);

#ifdef __cplusplus
}
#endif

#endif
