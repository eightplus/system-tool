/*
 * Copyright (C) 2018 lixiang
 *
 * Authors:
 *  lixiang kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void get_computer_info(char *manufacturer, char *productName, char *version, char *serial);
void get_cpu_info(char *manufacturer, char *corecount, char *version, char *maxspeed, char *currentspeed, char *serial);
void get_memory_info(int *count, char *dataWidth, char *memorySize, char *bankLocator, char *memoryType, char *manufacturer, char *serial);
