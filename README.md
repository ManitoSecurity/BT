================================================================
BT
================================================================

Arduino sketches to prototype communication over BT with rn-41
data sheet here: https://www.sparkfun.com/datasheets/Wireless/Bluetooth/rn-bluetooth-um.pdf

----------------------------------------------------------------------------

**bt_phone**
----------------------------------------------------------------------------
  * prototype communication arduino to phone with rn41
  * indicates "detection" when IR sensor picks up movement
  * master can send 'A' or 'D' to arm or disarm the device

  **TO DO**
  * improve alerts to wait after sending and avoid false positives

  **Known Problems**
  * none- we're perfect!
  * it just doesn't have enough functionality yet

  **Versions**

  *bt_phone_v2_2*
   * current working
   * reliable reciept of messages from phone

  *bt_phone_v2*
   * adds arming and disarming capability

  *bt_phone_v1*
   * initial prototype
   * indicates "detection" when IR sensor picks up movement
   * also checks connection with phone
   * functions to send BT commands and recieve response

----------------------------------------------------------------------------

**other stuff**
----------------------------------------------------------------------------
**trials and tribulations**
 * precursors to bt_phone_v1
 * ugly mess

**bt_start**
 * example communication between two rn41 chips
 * found online- gross code that doesn't work
 * may still provide workflow for future iterations

