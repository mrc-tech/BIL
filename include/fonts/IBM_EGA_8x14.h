/****************************************************************
Font data in array format. [charWidth x charHeight length array]
std::vector is sorted by ASCII codes from 32 (space) to  126 (~)

parameters:
	font[0][0] : characters Width
	font[0][1] : characters Height

example of data storage ('#' are the printed pixels of the char)
	--#--    00100
	-#-#- -> 01010 -> 00100 01010 01100 -> 001000101001100
	-##--    01100

Copyright (c) Andrea Marchi 2021
****************************************************************/


std::vector<std::string> IBM_EGA_8x14 = {
	{8, 14}, // width and height of character in pixels {charWidth, charHeight}
	"----------------------------------------------------------------------------------------------------------------", // <space>
	"-------------------00------00-----0000----0000----0000-----00------00--------------00------00-------------------", // !
	"---------00--00--00--00--00--00---0--0--------------------------------------------------------------------------", // "
	"-----------------00-00---00-00--0000000--00-00---00-00---00-00--0000000--00-00---00-00--------------------------", // #
	"---00------00----00000--00---00-00----0-00-------00000-------00-0----00-00---00--00000-----00------00-----------", // $
	"--------------------------------00----0-00---00-----00-----00-----00-----00--00-00---00-------------------------", // %
	"------------------000----00-00---00-00----000----000-00-00-000--00--00--00--00---000-00-------------------------", // &
	"----------00------00------00-----00-----------------------------------------------------------------------------", // '
	"--------------------00-----00-----00------00------00------00------00-------00-------00--------------------------", // (
	"------------------00-------00-------00------00------00------00------00-----00-----00----------------------------", // )
	"---------------------------------00--00---0000--00000000--0000---00--00-----------------------------------------", // *
	"-----------------------------------00------00----000000----00------00-------------------------------------------", // +
	"-------------------------------------------------------------------00------00------00-----00--------------------", // ,
	"------------------------------------------------0000000---------------------------------------------------------", // -
	"---------------------------------------------------------------------------00------00---------------------------", // .
	"----------------------0------00-----00-----00-----00-----00-----00------0---------------------------------------", // /
	"-----------------00000--00---00-00--000-00-0000-0000-00-000--00-00---00-00---00--00000--------------------------", // 0
	"-------------------00-----000----0000------00------00------00------00------00----000000-------------------------", // 1
	"-----------------00000--00---00------00-----00-----00-----00-----00-----00---00-0000000-------------------------", // 2
	"-----------------00000--00---00------00------00---0000-------00------00-00---00--00000--------------------------", // 3
	"----------------------------00-----000----0000---00-00--00--00--0000000-----00------00-----0000-----------------", // 4
	"----------------0000000-00------00------00------000000-------00------00-00---00--00000--------------------------", // 5
	"------------------000----00-----00------00------000000--00---00-00---00-00---00--00000--------------------------", // 6
	"----------------0000000-00---00------00-----00-----00-----00------00------00------00----------------------------", // 7
	"-----------------00000--00---00-00---00-00---00--00000--00---00-00---00-00---00--00000--------------------------", // 8
	"-----------------00000--00---00-00---00-00---00--000000------00------00-----00---0000---------------------------", // 9
	"---------------------------00------00------------------------------00------00-----------------------------------", // :
	"---------------------------00------00------------------------------00------00-----00----------------------------", // ;
	"---------------------00-----00-----00-----00-----00-------00-------00-------00-------00-------------------------", // <
	"-----------------------------------------000000------------------000000-----------------------------------------", // =
	"-----------------00-------00-------00-------00-------00-----00-----00-----00-----00-----------------------------", // >
	"-----------------00000--00---00-00---00-----00-----00------00--------------00------00---------------------------", // ?
	"-----------------00000--00---00-00---00-00-0000-00-0000-00-0000-00-000--00-------00000--------------------------", // @
	"-------------------0------000----00-00--00---00-00---00-0000000-00---00-00---00-00---00-------------------------", // A
	"----------------000000---00--00--00--00--00--00--00000---00--00--00--00--00--00-000000--------------------------", // B
	"------------------0000---00--00-00----0-00------00------00------00----0--00--00---0000--------------------------", // C
	"----------------00000----00-00---00--00--00--00--00--00--00--00--00--00--00-00--00000---------------------------", // D
	"----------------0000000--00--00--00---0--00-0----0000----00-0----00---0--00--00-0000000-------------------------", // E
	"----------------0000000--00--00--00---0--00-0----0000----00-0----00------00-----0000----------------------------", // F
	"------------------0000---00--00-00----0-00------00------00-0000-00---00--00--00---000-0-------------------------", // G
	"----------------00---00-00---00-00---00-00---00-0000000-00---00-00---00-00---00-00---00-------------------------", // H
	"------------------0000-----00------00------00------00------00------00------00-----0000--------------------------", // I
	"-------------------0000-----00------00------00------00------00--00--00--00--00---0000---------------------------", // J
	"----------------000--00--00--00--00-00---00-00---0000----00-00---00-00---00--00-000--00-------------------------", // K
	"----------------0000-----00------00------00------00------00------00---0--00--00-0000000-------------------------", // L
	"----------------00---00-000-000-0000000-0000000-00-0-00-00---00-00---00-00---00-00---00-------------------------", // M
	"----------------00---00-000--00-0000-00-0000000-00-0000-00--000-00---00-00---00-00---00-------------------------", // N
	"------------------000----00-00--00---00-00---00-00---00-00---00-00---00--00-00----000---------------------------", // O
	"----------------000000---00--00--00--00--00--00--00000---00------00------00-----0000----------------------------", // P
	"-----------------00000--00---00-00---00-00---00-00---00-00-0-00-00-0000--00000------00------000-----------------", // Q
	"----------------000000---00--00--00--00--00--00--00000---00-00---00--00--00--00-000--00-------------------------", // R
	"-----------------00000--00---00-00---00--00-------000-------00--00---00-00---00--00000--------------------------", // S
	"-----------------000000--000000--0-00-0----00------00------00------00------00-----0000--------------------------", // T
	"----------------00---00-00---00-00---00-00---00-00---00-00---00-00---00-00---00--00000--------------------------", // U
	"----------------00---00-00---00-00---00-00---00-00---00-00---00--00-00----000------0----------------------------", // V
	"------------------------00---00-00---00-00---00-00---00-00---00-00-0-00-0000000--00-00--------------------------", // W
	"----------------00---00-00---00--00-00----000-----000-----000----00-00--00---00-00---00-------------------------", // X
	"-----------------00--00--00--00--00--00--00--00---0000-----00------00------00-----0000--------------------------", // Y
	"----------------0000000-00---00-0---00-----00-----00-----00-----00----0-00---00-0000000-------------------------", // Z
	"------------------0000----00------00------00------00------00------00------00------0000--------------------------", // [
	"----------------0-------00------000------000------000------000------000------00-------0-------------------------", // '\'
	"------------------0000------00------00------00------00------00------00------00----0000--------------------------", // ]
	"---0------000----00-00--00---00---------------------------------------------------------------------------------", // ^
	"------------------------------------------------------------------------------------------------00000000--------", // _
	"--00------00-------00-------------------------------------------------------------------------------------------", // `
	"-----------------------------------------0000-------00---00000--00--00--00--00---000-00-------------------------", // a
	"----------------000------00------00------0000----00-00---00--00--00--00--00--00--00000--------------------------", // b
	"-----------------------------------------00000--00---00-00------00------00---00--00000--------------------------", // c
	"-------------------000------00------00----0000---00-00--00--00--00--00--00--00---000-00-------------------------", // d
	"-----------------------------------------00000--00---00-0000000-00------00---00--00000--------------------------", // d
	"------------------000----00-00---00--0---00-----0000-----00------00------00-----0000----------------------------", // f
	"-----------------------------------------000-00-00--00--00--00--00--00---00000------00--00--00---0000-----------", // g
	"----------------000------00------00------00-00---000-00--00--00--00--00--00--00-000--00-------------------------", // h
	"-------------------00------00-------------000------00------00------00------00-----0000--------------------------", // i
	"---------------------00------00-------------000------00------00------00------00--00--00--00--00---0000----------", // j
	"----------------000------00------00------00--00--00-00---0000----00-00---00--00-000--00-------------------------", // k
	"------------------000------00------00------00------00------00------00------00-----0000--------------------------", // l
	"----------------------------------------000-00--0000000-00-0-00-00-0-00-00-0-00-00---00-------------------------", // m
	"----------------------------------------00-000---00--00--00--00--00--00--00--00--00--00-------------------------", // n
	"-----------------------------------------00000--00---00-00---00-00---00-00---00--00000--------------------------", // o
	"----------------------------------------00-000---00--00--00--00--00--00--00000---00------00-----0000------------", // p
	"-----------------------------------------000-00-00--00--00--00--00--00---00000------00------00-----0000---------", // q
	"----------------------------------------00-000---000-00--00--00--00------00-----0000----------------------------", // r
	"-----------------------------------------00000--00---00--000-------000--00---00--00000--------------------------", // s
	"-------------------0------00------00----000000----00------00------00------00-00----000--------------------------", // t
	"----------------------------------------00--00--00--00--00--00--00--00--00--00---000-00-------------------------", // u
	"-----------------------------------------00--00--00--00--00--00--00--00---0000-----00---------------------------", // v
	"----------------------------------------00---00-00---00-00-0-00-00-0-00-0000000--00-00--------------------------", // w
	"----------------------------------------00---00--00-00----000-----000----00-00--00---00-------------------------", // x
	"----------------------------------------00---00-00---00-00---00-00---00--000000------00-----00--00000-----------", // y
	"----------------------------------------0000000-00--00-----00-----00-----00--00-0000000-------------------------", // z
	"--------------------000----00------00------00----000-------00------00------00-------000-------------------------", // {
	"-------------------00------00------00------00--------------00------00------00------00---------------------------", // |
	"-----------------000-------00------00------00-------000----00------00------00----000----------------------------", // }
	"-----------------000-00-00-000----------------------------------------------------------------------------------"  // ~
};