#M4 

2 Player pong:
- Left player uses keys "w" and "s" to play
- Right blayer uses bluetooth rotation to control

For this pong BLE I only used the Ay measurment. Essentially since graivty is to a readin of 1, you can use this to see flat, and tilted in one direction and another with a range of -1 to 1 for a 180  degree turn

Transfer function of poistion Y of paddle for BLE paddle
    Yt = Yt-1 + Ay * Y_Sesitivity_constant