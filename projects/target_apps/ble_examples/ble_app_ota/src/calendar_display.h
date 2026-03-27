/**
 ****************************************************************************************
 *
 * @file calendar_display_modified.h
 *
 * @brief Modified calendar display with analog clock functionality header file
 *
 ****************************************************************************************
 */

#ifndef __CALENDAR_DISPLAY_MODIFIED_H
#define __CALENDAR_DISPLAY_MODIFIED_H

#include <stdbool.h>
#include <stdint.h>


/**
 * @brief Vẽ trang lịch đầy đủ (phiên bản gốc, duy trì tính tương thích)
 * @param unix_time Dấu thời gian Unix hiện tại
 */
void draw_calendar_page(uint32_t unix_time);

/**
 * @brief Vẽ trang lịch kèm đồng hồ kim (phiên bản mới)
 * @param unix_time Dấu thời gian Unix hiện tại
 * @param force_redraw Có bắt buộc vẽ lại đồng hồ hay không
 */
// void draw_calendar_page_with_analog_clock(uint32_t unix_time, bool
// force_redraw);

#endif // __CALENDAR_DISPLAY_MODIFIED_H
