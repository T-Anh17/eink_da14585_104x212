/**
 ****************************************************************************************
 *
 * @file display_manager.h
 *
 * @brief Common definitions and management for E-Ink display modes
 *
 ****************************************************************************************
 */

#ifndef _DISPLAY_MANAGER_H_
#define _DISPLAY_MANAGER_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Enumeration of available display modes
 */
typedef enum {
    DISPLAY_MODE_TIME = 0,             // Chế độ hiển thị thời gian
    DISPLAY_MODE_CALENDAR = 1,          // Chế độ hiển thị lịch
    DISPLAY_MODE_CALENDAR_ANALOG = 2,   // Chế độ hiển thị lịch (kèm đồng hồ kim nhỏ)
    DISPLAY_MODE_IMAGE = 3,             // Chế độ hiển thị hình ảnh
    DISPLAY_MODE_FABRIC_RECORD = 4      // Chế độ thẻ kho / ghi chép xả vải
} display_mode_t;

#endif // _DISPLAY_MANAGER_H_
