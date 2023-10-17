#ifndef __ACORDE_H__
#define __ACORDE_H__

#include <Arduino.h>

#define ACOR_7 0x10
#define ACOR_INV1 0x20
#define ACOR_INV2 0x40
#define ACOR_INV3 0x80


namespace Chordifyer {
	
    namespace Acorde
    {

        // ****************************************
        // {a,b,c}  
        // a: tick (0-15)  
        // b: nota ( 0:fund-1 oct 1:fund 2: 3: 4:7ª 7:fund+1 oct )
        // c: duración 1-16 ticks , 0: fin de la secuencia

        const uint8_t ritmo_LegatoNat[][3]=  { {0,1,16}, {0,2,16}, {0,3,16}, {0,4,16}, {0,0,0} };
        const uint8_t ritmo_Legato   [][3]=  { {0,0,16}, {0,1,16}, {0,2,16}, {0,3,16}, {0,4,16}, {0,0,0} };
        const uint8_t ritmo_Spicy    [][3] = { {0,0,2},  {0,7,2},  {4,0,2},  {8,0,2},  {12,0,2}, {5,1,1},  {9,1,1},  {13,1,1}, {2,2,1},  {6,2,1},  {10,2,1}, {14,2,1} ,{1,3,1},{3,3,1},{7,3,1},{11,3,1},{15,3,1}, {0,0,0} };
        const uint8_t ritmo_Ultra    [][3] = { {0,0,2},  {0,2,2},  {0,3,2},  {0,7,2},  {4,0,2},  {4,2,2},  {4,3,2},  {4,7,2},  {7,0,2},  {7,2,2},  {7,3,2}, {7,7,2}, {11,0,2}, {11,2,2}, {11,3,2}, {11,7,2}, {14,0,2}, {14,2,2}, {14,3,2}, {14,7,2}, {0,0,0} };
        const uint8_t ritmo_Fine     [][3] = { {0,0,8},  {8,1,8},  {2,2,2},  {4,3,2},  {6,7,2},  {10,7,2}, {12,3,2}, {14,2,2}, {0,0,0} };
        const uint8_t ritmo_Energico [][3] = { {0,0,8},  {2,1,2},  {6,1,2},  {8,1,8},  {0,2,2},  {4,2,2},  {10,3,2}, {14,3,2}, {8,7,2},  {12,7,2}, {0,0,0}};
        const uint8_t ritmo_Petite   [][3] = { {0,0,8},  {3,1,3},  {0,2,3},  {9,2,3},  {13,2,1}, {7,3,1},  {12,3,1}, {14,3,2}, {6,7,1},  {8,7,1},  {0,0,0}};
        const uint8_t ritmo_Rainy    [][3] = { {0,0,3},  {10,0,3}, {13,0,3}, {0,2,3},  {10,2,3}, {13,2,3}, {0,3,3},  {10,3,3}, {13,3,3}, {0,7,3},  {10,7,3},  {13,7,3}, {0,0,0}};
        const uint8_t ritmo_Snobbish [][3] = { {0,0,8},  {2,1,2},  {6,1,2},  {0,2,2},  {4,2,2},  {10,3,2},  {14,3,2}, {8,7,2}, {10,7,2}, {0,0,0}};
        
        const uint8_t progresiones[12][8] 
        {
            {1,5,6,4,0,0,0,0},
            {1,5,6,3,4,0,0,0},
            {6,5,4,5,0,0,0,0},
            {6,4,1,5,0,0,0,0},
            {1,6,4,5,0,0,0,0},
            {4,2,1+ACOR_INV1,5,1,0,0,0},
            {1,2+ACOR_7,1+ACOR_INV1,4,0,0,0,0},
            {1,5+ACOR_INV1,6,5,0,0,0,0},
            {1,3+ACOR_INV2,6,4,0,0,0,0},
            {1,4,6,5,0,0,0,0},
            {1,5,4,5,0,0,0,0},
            {0,0,0,0,0,0,0,0}
        };

        const uint8_t grados3[3][4] = 
        {
             // El primer valor es la longitud del acorde
             {3,1,3,5},  // normal
             {3,3,5,8},  // 1º Inv
             {3,5,8,10}  // 2ª Inv
        };

        const uint8_t grados4[4][5] = 
        {
             // El primer valor es la longitud del acorde
             {4,1,3,5,7},
             {4,3,5,7,8},
             {4,5,7,8,10},
             {4,7,8,10,12}
        };


        const uint8_t* getGradosNotas (uint8_t acordeProg )  
        {
            if ( acordeProg & ACOR_7 ) {
                if ( acordeProg & ACOR_INV1 ) {
                    return grados4[1];
                } else if ( acordeProg & ACOR_INV2 ) {
                    return grados4[2];
                } else if ( acordeProg & ACOR_INV3 ) {
                    return grados4[3];
                } else {
                    return grados4[0];
                }                    
            } else {
                if ( acordeProg & ACOR_INV1 ) {
                    return grados3[1];
                } else if ( acordeProg & ACOR_INV2 ) {
                    return grados3[2];
                } else {
                    return grados3[0];
                }                    

            }
        }
    };
}
#endif // __ACORDE_H__
