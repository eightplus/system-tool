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

#include "utils.h"

void get_computer_info(char *manufacturer, char *productName, char *version, char *serial)
{
    //ComVendo, ComProduct, ComVersion, ComSerial
    char *line = NULL;
    size_t size = 0;
    ssize_t read;
    FILE *fp = NULL;
    fp = popen("dmidecode -t system", "r");
    if (fp == NULL) {
        return;
    }
    while ((read = getline(&line, &size, fp)) != -1) {
        if (strstr(line, ":") != NULL) {//Number Of Devices
            if (strstr(line, "Manufacturer") != NULL) {//ComVendor
                sscanf(line, "%*[^:]: %[^\n]", manufacturer);//获取指定字符中间的字符串，从:到\n之间的字符
            }
            else if (strstr(line, "Product Name") != NULL) {//ComProduct
                sscanf(line, "%*[^:]: %[^\n]", productName);
            }
            else if (strstr(line, "Version") != NULL) {//ComVersion
                sscanf(line, "%*[^:]: %[^\n]", version);
            }
            else if (strstr(line, "Serial Number") != NULL) {//ComSerial
                sscanf(line, "%*[^:]: %[^\n]", serial);
            }
            else {
                continue;
            }
        }
    }
    if (line != NULL) {
        free(line);
        line = NULL;
    }
    fclose(fp);
    fp = NULL;
}


void get_cpu_info(char *manufacturer, char *corecount, char *version, char *maxspeed, char *currentspeed, char *serial)
{
    //CpuVendor, cpu_cores, CpuVersion, CpuCapacity, CpuSize, CpuSerial
    char *line = NULL;
    size_t size = 0;
    ssize_t read;
    FILE *fp = NULL;
    fp = popen("dmidecode -t processor", "r");
    if (fp == NULL) {
        return;
    }
    while ((read = getline(&line, &size, fp)) != -1) {
        if (strstr(line, ":") != NULL) {
            if (strstr(line, "Manufacturer") != NULL) {//CpuVendor
                sscanf(line, "%*[^:]: %[^\n]", manufacturer);//获取指定字符中间的字符串，从:到\n之间的字符
            }
            else if (strstr(line, "Core Count") != NULL) {//cpu_cores
                sscanf(line, "%*[^:]: %[^\n]", corecount);
            }
            else if (strstr(line, "Version") != NULL) {//CpuVersion
                sscanf(line, "%*[^:]: %[^\n]", version);
            }
            else if (strstr(line, "Max Speed") != NULL) {//CpuCapacity
                sscanf(line, "%*[^:]: %[^\n]", maxspeed);
            }
            else if (strstr(line, "Current Speed") != NULL) {//CpuSize
                sscanf(line, "%*[^:]: %[^\n]", currentspeed);
            }
            else if (strstr(line, "Serial Number") != NULL) {//CpuSerial
                sscanf(line, "%*[^:]: %[^\n]", serial);
            }
            else {
                continue;
            }
        }
    }
    if (line != NULL) {
        free(line);
        line = NULL;
    }
    fclose(fp);
    fp = NULL;
}

void get_memory_info(int *count, char *dataWidth, char *memorySize, char *bankLocator, char *memoryType, char *manufacturer, char *serial)
{
    //number, MemVendor, MemProduct, MemSize, MemSerial, MemSlot, MemWidth
    int num_memory = 0;
    FILE *pd = NULL;
    pd = popen("dmidecode -t memory |grep -e 'Number Of Devices'", "r");
    if (pd == NULL) {
        return;
    }
    fscanf(pd, "%*s%*s%*s%d", &num_memory);
    pclose(pd);
    *count = num_memory;

    char *line = NULL;
    size_t size = 0;
    ssize_t read;
    FILE *fp = NULL;
    fp = popen("dmidecode -t memory", "r");
    if (fp == NULL) {
        return;
    }

    while ((read = getline(&line, &size, fp)) != -1) {
        char result[128] = {0};
        if (strstr(line, ":") != NULL) {
            if (strstr(line, "Data Width:") != NULL) {//
                sscanf(line, "%*[^:]: %[^\n]", result);//获取指定字符中间的字符串，从:到\n之间的字符
                if (strlen(result) > 0) {
                    if (strlen(dataWidth) > 0) {
                        strcat(dataWidth, "$");
                    }
                    strncat(dataWidth, result, strlen(result));
                }
                else {
                    if (strlen(dataWidth) > 0) {
                        strcat(dataWidth, "$");
                    }
                    strcat(dataWidth, "None");
                }
            }
            else if (strstr(line, "Size:") != NULL) {//cpu_cores
                sscanf(line, "%*[^:]: %[^\n]", result);
                if (strlen(result) > 0) {
                    if (strlen(memorySize) > 0) {
                        strcat(memorySize, "$");
                    }
                    strncat(memorySize, result, strlen(result));
                }
                else {
                    if (strlen(memorySize) > 0) {
                        strcat(memorySize, "$");
                    }
                    strcat(memorySize, "None");
                }
            }
            else if (strstr(line, "Bank Locator:") != NULL) {//CpuVersion
                sscanf(line, "%*[^:]: %[^\n]", result);
                if (strlen(result) > 0) {
                    if (strlen(bankLocator) > 0) {
                        strcat(bankLocator, "$");
                    }
                    strncat(bankLocator, result, strlen(result));
                }
                else {
                    if (strlen(bankLocator) > 0) {
                        strcat(bankLocator, "$");
                    }
                    strcat(bankLocator, "None");
                }
            }
            else if (strstr(line, "Type:") != NULL && strstr(line, "Error Correction Type:") == NULL) {//CpuCapacity
                sscanf(line, "%*[^:]: %[^\n]", result);
                if (strlen(result) > 0) {
                    if (strlen(memoryType) > 0) {
                        strcat(memoryType, "$");
                    }
                    strncat(memoryType, result, strlen(result));
                }
                else {
                    if (strlen(memoryType) > 0) {
                        strcat(memoryType, "$");
                    }
                    strcat(memoryType, "None");
                }
            }
            else if (strstr(line, "Manufacturer:") != NULL) {//CpuSize
                sscanf(line, "%*[^:]: %[^\n]", result);
                if (strlen(result) > 0) {
                    if (strlen(manufacturer) > 0) {
                        strcat(manufacturer, "$");
                    }
                    strncat(manufacturer, result, strlen(result));
                }
                else {
                    if (strlen(manufacturer) > 0) {
                        strcat(manufacturer, "$");
                    }
                    strcat(manufacturer, "None");
                }
            }
            else if (strstr(line, "Serial Number:") != NULL) {//CpuSerial
                sscanf(line, "%*[^:]: %[^\n]", result);
                if (strlen(result) > 0) {
                    if (strlen(serial) > 0) {
                        strcat(serial, "$");
                    }
                    strncat(serial, result, strlen(result));
                }
                else {
                    if (strlen(serial) > 0) {
                        strcat(serial, "$");
                    }
                    strcat(serial, "None");
                }
            }
            else {
                continue;
            }
        }
    }
    if (line != NULL) {
        free(line);
        line = NULL;
    }
    fclose(fp);
    fp = NULL;
}
