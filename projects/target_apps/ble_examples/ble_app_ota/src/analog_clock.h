/**
 ****************************************************************************************
 *
 * @file analog_clock.h
 *
 * @brief Analog clock display functionality header file
 *
 ****************************************************************************************
 */

#ifndef __ANALOG_CLOCK_H
#define __ANALOG_CLOCK_H

#include <stdbool.h>
#include <stdint.h>


/**
 * @brief Vẽ đồng hồ kim
 * @param x Tọa độ X góc trên bên trái đồng hồ
 * @param y Tọa độ Y góc trên bên trái đồng hồ
 * @param size Độ dài cạnh hình vuông của đồng hồ
 * @param unix_time Dấu thời gian Unix hiện tại
 * @param force_redraw Có bắt buộc vẽ lại tất cả các phần tử hay không (true: vẽ
 * lại hoàn toàn, false: chỉ cập nhật các kim)
 */
void draw_analog_clock(uint16_t x, uint16_t y, uint16_t size,
                       uint32_t unix_time, bool force_redraw);

/**
 * @brief Vẽ trang lịch kèm đồng hồ kim
 * @param unix_time Dấu thời gian Unix hiện tại
 * @param force_redraw Có bắt buộc vẽ lại đồng hồ hay không
 */
void draw_calendar_with_analog_clock(uint32_t unix_time, bool force_redraw);

#endif // __ANALOG_CLOCK_H
