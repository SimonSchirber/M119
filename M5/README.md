# M5

## 2 Player BLE pong:
- Load Right player BLE arduino device first into nano 33 Iot (Player_1.ino)
- Load Left Player Second into Nicla Sense me board  (Player_2_states.ino)
- Start pong game python script ("2_BLE_Pong.py") and wait to connect to devices (takes ~10 seconds)

## Player 1 (33 Iot)
Moves in continuos motion by "roling" the 33 Iot around when holding the breacboard 90 degreess to the cord

## Player 2 (Nicla sesne)
3 states possible when holding the board parallel with the wire like flipping a pan
Flip up (move character up, point/flip upward)
Steady/Flat (stay still, hold bord level)
FLip down (move character up, point/flip downward)