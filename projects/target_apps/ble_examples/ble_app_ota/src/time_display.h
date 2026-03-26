/**
 ****************************************************************************************
 *
 * @file time_display.h
 *
 * @brief Digital time display component header file
 *
 ****************************************************************************************
 */

#ifndef _TIME_DISPLAY_H_
#define _TIME_DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Draw the digital time page (includes date, week, image, and lunar date)
 * @param unix_time Current Unix timestamp
 * @param force_redraw Whether to force a full redraw of the screen
 */
void draw_time_page(uint32_t unix_time, bool force_redraw);

#endif // _TIME_DISPLAY_H_
