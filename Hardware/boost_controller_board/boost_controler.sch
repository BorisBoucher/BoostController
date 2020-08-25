EESchema Schematic File Version 4
LIBS:boost_controler-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Transistor_BJT:BD139 Q3
U 1 1 5E2D2794
P 7000 4050
F 0 "Q3" H 7192 4096 50  0000 L CNN
F 1 "BD139" H 7192 4005 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-126_Vertical" H 7200 3975 50  0001 L CIN
F 3 "http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/CD00001225.pdf" H 7000 4050 50  0001 L CNN
	1    7000 4050
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC337 Q2
U 1 1 5E2D32E5
P 6450 3700
F 0 "Q2" H 6641 3746 50  0000 L CNN
F 1 "BC337" H 6641 3655 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Wide" H 6650 3625 50  0001 L CIN
F 3 "https://diotec.com/tl_files/diotec/files/pdf/datasheets/bc337.pdf" H 6450 3700 50  0001 L CNN
	1    6450 3700
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5E2D4688
P 4550 3000
F 0 "A1" H 4550 1700 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 4550 1600 50  0000 C CNN
F 2 "Library_Loader:ARDNANO30NP" H 4700 2050 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 4550 2000 50  0001 C CNN
	1    4550 3000
	-1   0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LM7805_TO220 U1
U 1 1 5E2D6693
P 8000 1500
F 0 "U1" H 8000 1742 50  0000 C CNN
F 1 "LM7805_TO220" H 8000 1651 50  0000 C CNN
F 2 "TO_SOT_Packages_THT:TO-220-3_Vertical" H 8000 1725 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM7805.pdf" H 8000 1450 50  0001 C CNN
	1    8000 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:D_TVS D1
U 1 1 5E2D7CB5
P 7000 1750
F 0 "D1" H 7000 1966 50  0000 C CNN
F 1 "18V" H 7000 1875 50  0000 C CNN
F 2 "Diodes_THT:D_5W_P10.16mm_Horizontal" H 7000 1750 50  0001 C CNN
F 3 "~" H 7000 1750 50  0001 C CNN
	1    7000 1750
	0    1    1    0   
$EndComp
Text GLabel 1450 1750 2    50   Input ~ 0
WG_SOL
Text GLabel 1450 1850 2    50   Input ~ 0
AIR_FLOW
Text GLabel 1450 2350 2    50   Input ~ 0
MAP
Text GLabel 1450 2450 2    50   Input ~ 0
GND
Text GLabel 1450 3950 2    50   Input ~ 0
+12V
Text GLabel 1450 4050 2    50   Input ~ 0
GND
Text GLabel 1450 4150 2    50   Input ~ 0
RPM
Text GLabel 1450 4250 2    50   Input ~ 0
SPEED
Text GLabel 1450 4350 2    50   Input ~ 0
THROTTLE
Text GLabel 1450 3200 2    50   Input ~ 0
TX
Text GLabel 1450 3300 2    50   Input ~ 0
GND
Text GLabel 1450 3400 2    50   Input ~ 0
RX
$Comp
L Device:CP C1
U 1 1 5E2E0F6D
P 7500 1750
F 0 "C1" H 7618 1796 50  0000 L CNN
F 1 "10µF" H 7618 1705 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D8.0mm_P5.00mm" H 7538 1600 50  0001 C CNN
F 3 "~" H 7500 1750 50  0001 C CNN
	1    7500 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5E2E15E4
P 8500 1750
F 0 "C2" H 8615 1796 50  0000 L CNN
F 1 "0.1µF" H 8615 1705 50  0000 L CNN
F 2 "Capacitors_THT:C_Disc_D3.0mm_W2.0mm_P2.50mm" H 8538 1600 50  0001 C CNN
F 3 "~" H 8500 1750 50  0001 C CNN
	1    8500 1750
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR04
U 1 1 5E2E1DCD
P 8000 2200
F 0 "#PWR04" H 8000 2000 50  0001 C CNN
F 1 "GNDPWR" H 8004 2046 50  0000 C CNN
F 2 "" H 8000 2150 50  0001 C CNN
F 3 "" H 8000 2150 50  0001 C CNN
	1    8000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 2200 8000 2050
Wire Wire Line
	7500 1900 7500 2050
Wire Wire Line
	7500 2050 8000 2050
Connection ~ 8000 2050
Wire Wire Line
	8000 2050 8000 1800
Wire Wire Line
	7000 1900 7000 2050
Wire Wire Line
	7000 2050 7500 2050
Connection ~ 7500 2050
Wire Wire Line
	8500 1900 8500 2050
Wire Wire Line
	8500 2050 8000 2050
Wire Wire Line
	7700 1500 7500 1500
Wire Wire Line
	7500 1500 7500 1600
Wire Wire Line
	7500 1500 7000 1500
Wire Wire Line
	7000 1500 7000 1600
Connection ~ 7500 1500
Wire Wire Line
	8300 1500 8500 1500
Wire Wire Line
	8500 1500 8500 1600
$Comp
L power:+12V #PWR01
U 1 1 5E2EA75F
P 7000 1350
F 0 "#PWR01" H 7000 1200 50  0001 C CNN
F 1 "+12V" H 7015 1523 50  0000 C CNN
F 2 "" H 7000 1350 50  0001 C CNN
F 3 "" H 7000 1350 50  0001 C CNN
	1    7000 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 1350 7000 1500
Connection ~ 7000 1500
$Comp
L power:+5V #PWR02
U 1 1 5E2EBDBF
P 8500 1350
F 0 "#PWR02" H 8500 1200 50  0001 C CNN
F 1 "+5V" H 8515 1523 50  0000 C CNN
F 2 "" H 8500 1350 50  0001 C CNN
F 3 "" H 8500 1350 50  0001 C CNN
	1    8500 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 1350 8500 1500
Connection ~ 8500 1500
$Comp
L power:GND #PWR03
U 1 1 5E2ECBCD
P 7000 2200
F 0 "#PWR03" H 7000 1950 50  0001 C CNN
F 1 "GND" H 7005 2027 50  0000 C CNN
F 2 "" H 7000 2200 50  0001 C CNN
F 3 "" H 7000 2200 50  0001 C CNN
	1    7000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 2200 7000 2050
Connection ~ 7000 2050
Wire Wire Line
	6800 4050 6550 4050
Wire Wire Line
	6550 4050 6550 3900
Wire Wire Line
	7100 3850 7100 3500
Wire Wire Line
	7100 3500 6550 3500
$Comp
L power:GND #PWR07
U 1 1 5E2F0654
P 7100 4500
F 0 "#PWR07" H 7100 4250 50  0001 C CNN
F 1 "GND" H 7105 4327 50  0000 C CNN
F 2 "" H 7100 4500 50  0001 C CNN
F 3 "" H 7100 4500 50  0001 C CNN
	1    7100 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 4500 7100 4250
Text GLabel 7100 3500 2    50   Input ~ 0
WG_SOL
$Comp
L Device:R R2
U 1 1 5E2F1611
P 6000 3700
F 0 "R2" V 5793 3700 50  0000 C CNN
F 1 "10k" V 5884 3700 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 5930 3700 50  0001 C CNN
F 3 "~" H 6000 3700 50  0001 C CNN
	1    6000 3700
	0    1    1    0   
$EndComp
Wire Wire Line
	6150 3700 6250 3700
Wire Wire Line
	5050 3700 5850 3700
Text GLabel 4050 3400 0    50   Input ~ 0
MAP
Text GLabel 5050 2400 2    50   Input ~ 0
RX
Text GLabel 5050 2500 2    50   Input ~ 0
TX
Text GLabel 4350 2000 1    50   Input ~ 0
+5V
Wire Wire Line
	4450 4000 4500 4000
$Comp
L power:GND #PWR06
U 1 1 5E2F7192
P 4500 4000
F 0 "#PWR06" H 4500 3750 50  0001 C CNN
F 1 "GND" H 4505 3827 50  0000 C CNN
F 2 "" H 4500 4000 50  0001 C CNN
F 3 "" H 4500 4000 50  0001 C CNN
	1    4500 4000
	1    0    0    -1  
$EndComp
Connection ~ 4500 4000
Wire Wire Line
	4500 4000 4550 4000
Text GLabel 5050 3400 2    50   Input ~ 0
AIR_FLOW
Text GLabel 5050 3500 2    50   Input ~ 0
SPEED
Text GLabel 4050 3500 0    50   Input ~ 0
THROTTLE
Text GLabel 5050 3600 2    50   Input ~ 0
RPM
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 5E2E2DF8
P 1250 1850
F 0 "J1" H 1168 2167 50  0000 C CNN
F 1 "Conn_01x03" H 1168 2076 50  0000 C CNN
F 2 "MyLibrary:JST_XH_3x2.50mm_TH_Angled" H 1250 1850 50  0001 C CNN
F 3 "~" H 1250 1850 50  0001 C CNN
	1    1250 1850
	-1   0    0    -1  
$EndComp
Text GLabel 1450 1950 2    50   Input ~ 0
FUEL_PUMP
$Comp
L Transistor_BJT:BC337 Q1
U 1 1 5E2E3E7F
P 6450 3000
F 0 "Q1" H 6641 3046 50  0000 L CNN
F 1 "BC337" H 6641 2955 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Wide" H 6650 2925 50  0001 L CIN
F 3 "https://diotec.com/tl_files/diotec/files/pdf/datasheets/bc337.pdf" H 6450 3000 50  0001 L CNN
	1    6450 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5E2E566D
P 6000 3000
F 0 "R1" V 5793 3000 50  0000 C CNN
F 1 "330" V 5884 3000 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P2.54mm_Vertical" V 5930 3000 50  0001 C CNN
F 3 "~" H 6000 3000 50  0001 C CNN
	1    6000 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	6150 3000 6250 3000
Wire Wire Line
	5850 3000 5700 3000
Wire Wire Line
	5700 3000 5700 3300
Wire Wire Line
	5700 3300 5050 3300
$Comp
L power:GND #PWR05
U 1 1 5E2E797A
P 6550 3200
F 0 "#PWR05" H 6550 2950 50  0001 C CNN
F 1 "GND" H 6555 3027 50  0000 C CNN
F 2 "" H 6550 3200 50  0001 C CNN
F 3 "" H 6550 3200 50  0001 C CNN
	1    6550 3200
	1    0    0    -1  
$EndComp
Text GLabel 6550 2650 2    50   Input ~ 0
FUEL_PUMP
Wire Wire Line
	6550 2650 6550 2800
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 5E2E8F42
P 1250 2450
F 0 "J2" H 1168 2767 50  0000 C CNN
F 1 "Conn_01x04" H 1168 2676 50  0000 C CNN
F 2 "MyLibrary:JST_XH_4x2.50mm_TH_Angled" H 1250 2450 50  0001 C CNN
F 3 "~" H 1250 2450 50  0001 C CNN
	1    1250 2450
	-1   0    0    -1  
$EndComp
Text GLabel 1450 2650 2    50   Input ~ 0
GND
Text GLabel 1450 2550 2    50   Input ~ 0
FUEL_PRES
Text GLabel 4050 3600 0    50   Input ~ 0
FUEL_PRES
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 5E2EA50A
P 1250 3300
F 0 "J3" H 1168 3617 50  0000 C CNN
F 1 "Conn_01x03" H 1168 3526 50  0000 C CNN
F 2 "MyLibrary:JST_XH_3x2.50mm_TH_Angled" H 1250 3300 50  0001 C CNN
F 3 "~" H 1250 3300 50  0001 C CNN
	1    1250 3300
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J4
U 1 1 5E2EB278
P 1250 4150
F 0 "J4" H 1168 4567 50  0000 C CNN
F 1 "Conn_01x05" H 1168 4476 50  0000 C CNN
F 2 "MyLibrary:JST_XH_5x2.50mm_TH_Angled" H 1250 4150 50  0001 C CNN
F 3 "~" H 1250 4150 50  0001 C CNN
	1    1250 4150
	-1   0    0    -1  
$EndComp
$EndSCHEMATC
