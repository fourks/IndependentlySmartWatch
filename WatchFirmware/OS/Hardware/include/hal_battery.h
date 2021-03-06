//==============================================================================
//  Copyright 2011 Meta Watch Ltd. - http://www.MetaWatch.org/
// 
//  Licensed under the Meta Watch License, Version 1.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  
//      http://www.MetaWatch.org/licenses/license-1.0.html
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//==============================================================================

/******************************************************************************/
/*! \file hal_battery.h
 *
 * Battery charging and power good detection.
 */
/******************************************************************************/

#ifndef HAL_BATTERY_H
#define HAL_BATTERY_H
#ifdef __cplusplus
extern "C" {
#endif

/*! The state of the battery charging circuit is dedcoded from the bits on 
 *  port 6 
 *
 *   Stat2    Stat1      Status                  Hex value
 *    0          0       Precharge                   0x00
 *    1          0       Fast charge                 0x10
 *    0          1       Charge Done                 0x08
 *    1          1       Timer Fault or Sleep        0x18
 */
#define BATTERY_PRECHARGE              ( 0x00 )
#define BATTERY_FAST_CHARGE            ( 0x02 )
#define BATTERY_CHARGE_DONE            ( 0x01 )
#define BATTERY_CHARGE_OFF_FAULT_SLEEP ( 0x03 )


/*! Configure the pins used for reading the battery state and 
 * charging the battery.
 */
void ConfigureBatteryPins(void);

#ifdef __cplusplus
}
#endif
#endif // HAL_BATTERY_H
