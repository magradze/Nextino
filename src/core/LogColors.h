/**
 * @file        LogColors.h
 * @title       ANSI Color Codes for Logger
 * @description Defines standard ANSI escape codes for colored console output,
 *              used by the Logger component.
 *
 * @author      Giorgi Magradze
 * @date        2025-08-19
 * @version     0.1.0
 *
 * @copyright   (c) 2025 Nextino. All rights reserved.
 * @license     MIT License
 */

#pragma once

#define LOG_COLOR_RESET   "\x1B[0m"
#define LOG_COLOR_RED     "\x1B[31m"     
#define LOG_COLOR_GREEN   "\x1B[32m"     
#define LOG_COLOR_YELLOW  "\x1B[33m"     
#define LOG_COLOR_BLUE    "\x1B[34m"     
#define LOG_COLOR_MAGENTA "\x1B[35m"     
#define LOG_COLOR_CYAN    "\x1B[36m"     

#define LOG_COLOR_NEON_PURPLE "\x1B[1;35m"