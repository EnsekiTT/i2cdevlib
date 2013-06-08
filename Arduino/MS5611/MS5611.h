// I2Cdev library collection - MS5611 I2C device class header file
// Based on MS5611-01BA03.pdf , 6/2012 
// 6/3/2013 by EnsekiTT <ensekitt@curbsci.com>
//

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef _MS5611_H_
#define _MS5611_H_

#include "I2Cdev.h"

#define MS5611_ADDRESS            0x76 // this device only has one address
#define MS5611_DEFAULT_ADDRESS    0x76 // 0111 011(CSB)
#define MS5611_DEFAULT_REGISTER   MS5611_DEFAULT_ADDRESS

#define MS5611_RESET               0x1E
#define MS5611_PROM_READ           0xA6

#define MS5611_CONVERT_D1_256      0x40
#define MS5611_CONVERT_D1_512      0x42
#define MS5611_CONVERT_D1_1024     0x44
#define MS5611_CONVERT_D1_2048     0x46
#define MS5611_CONVERT_D1_4096     0x48

#define MS5611_CONVERT_D2_256      0x50
#define MS5611_CONVERT_D2_512      0x52
#define MS5611_CONVERT_D2_1024     0x54
#define MS5611_CONVERT_D2_2048     0x56
#define MS5611_CONVERT_D2_4096     0x58

#define MS5611_ADC_READ            0x00
#define MS5611_PROM_READ_MAN       0xA0
#define MS5611_PROM_READ_C1        0xA2
#define MS5611_PROM_READ_C2        0xA4
#define MS5611_PROM_READ_C3        0xA6
#define MS5611_PROM_READ_C4        0xA8
#define MS5611_PROM_READ_C5        0xAA
#define MS5611_PROM_READ_C6        0xAC
#define MS5611_PROM_READ_CRC       0xAE

#define MS5611_C1_DEFAULT          40127
#define MS5611_C2_DEFAULT          36924
#define MS5611_C3_DEFAULT          23317
#define MS5611_C4_DEFAULT          23317
#define MS5611_C5_DEFAULT          23317
#define MS5611_C6_DEFAULT          23317

#define MS5611_CONVERT_TIME        10000

class MS5611 {
    public:
        MS5611();
        MS5611(uint8_t address);
        
        void initialize();
        bool testConnection();
		
		// System
		void reset();
        void readPROM();
        uint32_t getD1();
        uint32_t getD2();
        void setD1();
        void setD2();
		uint32_t readADC();
		
		void setdT();
		void setTEMP();
		void setOFF();
		void setSENS();
		void setP();
		void setT2();
		void setOFF2();
		void setSENS2();
		
		int32_t getdT();
		int32_t getTEMP();
		int64_t getOFF();
		int64_t getSENS();
		int32_t getP();
		int32_t getT2();
		int32_t getOFF2();
		int64_t getSENS2();
		
		void setC1(uint16_t c);
		void setC2(uint16_t c);
		void setC3(uint16_t c);
		void setC4(uint16_t c);
		void setC5(uint16_t c);
		void setC6(uint16_t c);
		
		uint16_t getC1();
		uint16_t getC2();
		uint16_t getC3();
		uint16_t getC4();
		uint16_t getC5();
		uint16_t getC6();
		
    private:
		uint16_t c1_val;
		uint16_t c2_val;
		uint16_t c3_val;
		uint16_t c4_val;
		uint16_t c5_val;
		uint16_t c6_val;
		
		uint32_t d1;
		uint32_t d2;
		
		int32_t dT;
		int32_t TEMP;
		int32_t OFF;
		int64_t SENS;
		int32_t P;
		int32_t T2;
		int32_t OFF2;
        int64_t SENS2;
    
        uint8_t devAddr;
        uint8_t regAddr;
    
        uint64_t time_now;
        uint64_t time_d1;
        uint64_t time_d2;
        bool flag_d1;
        bool flag_d2;
};

#endif /* _HMC5843_H_ */
