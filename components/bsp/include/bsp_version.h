/*
 * bsp_version.h
 * 
 * Created on: 05 Mar 2025
 *     Author: Vo Van Buong (BRT-SG)
 */

#ifndef BSP_VERSION_HDR_H_
#define BSP_VERSION_HDR_H_

#define BSP_VERSION_MAJOR   0
#define BSP_VERSION_MINOR   0
#define BSP_VERSION_PATCH   2

#define STRINGIFY(x)        #x
#define TOSTRING(x)         STRINGIFY(x)
#define BSP_VERSION_STR     TOSTRING(BSP_VERSION_MAJOR) "." TOSTRING(BSP_VERSION_MINOR) "." TOSTRING(BSP_VERSION_PATCH)

#endif BSP_VERSION_HDR_H_
