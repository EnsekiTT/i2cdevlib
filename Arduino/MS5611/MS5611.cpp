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

#include "MS5611.h"

/** Default constructor, uses default I2C address.
 * @see HMC5843_DEFAULT_ADDRESS
 */
HMC5843::HMC5843() {
    devAddr = MS5611_DEFAULT_ADDRESS;
    regAddr = MS5611_DEFAULT_REGISTER;
}

/** Specific address constructor.
 * @param address I2C address
 * @see HMC5843_DEFAULT_ADDRESS
 * @see HMC5843_ADDRESS
 */
HMC5843::HMC5843(uint8_t address) {
    devAddr = address;
}

/** Power on and prepare for general usage.
 * This will prepare the magnetometer with default settings, ready for single-
 * use mode (very low power requirements). Default settings include 8-sample
 * averaging, 15 Hz data output rate, normal measurement bias, a,d 1090 gain (in
 * terms of LSB/Gauss). Be sure to adjust any settings you need specifically
 * after initialization, especially the gain settings if you happen to be seeing
 * a lot of -4096 values (see the datasheet for mor information).
 */
void MS5611::initialize() {
	MS5611::setC1(MS5611_C1_DEFAULT);
	MS5611::setC2(MS5611_C2_DEFAULT);
	MS5611::setC3(MS5611_C3_DEFAULT);
	MS5611::setC4(MS5611_C4_DEFAULT);
	MS5611::setC5(MS5611_C5_DEFAULT);
	MS5611::setC6(MS5611_C6_DEFAULT);
	
}

/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool MS5611::testConnection() {
    if (I2Cdev::readBytes(devAddr, HMC5843_RA_ID_A, 3, buffer) == 3) {
        return (buffer[0] == 'H' && buffer[1] == '4' && buffer[2] == '3');
    }
    return false;
}


void MS5611::reset(){
    I2Cdev::writeByte(devAddr, revAddr, MS5611_RESET);
}
void MS5611::readPROM(){
	
}
void MS5611::setD1(int type = 4096){
    MS5611::time_now = micros();
    if (MS5611::flag_d && (MS5611::time_now - MS5611::time_d1 > MS5611_CONVERT_TIME) ) {
        return;
    }
    switch (type) {
        case 256:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D1_256);
            break;
        case 512:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D1_512);
            break;
        case 1024:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D1_1024);
            break;
        case 2048:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D1_2048);
            break;
        case 4096:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D1_4096);
            break;
        default:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D1_4096);
            break;
    }
    MS5611::flag_d = TRUE;
    MS5611::time_d1 = MS5611::time_now;
}
uint32_t MS5611::setD2(int type = 4096){
    MS5611::time_now = micros();
    if (!MS5611::flag_d && (MS5611::time_now - MS5611::time_d2 > MS5611_CONVERT_TIME)) {
        return;
    }
    switch (type) {
        case 256:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D2_256);
            break;
        case 512:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D2_512);
            break;
        case 1024:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D2_1024);
            break;
        case 2048:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D2_2048);
            break;
        case 4096:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D2_4096);
            break;
        default:
            I2Cdev::writeByte(devAddr, revAddr, MS5611_CONVERT_D2_4096);
            break;
    }
    MS5611::flag_d = FALSE;
    MS5611::time_d2 = MS5611::time_now;
}

uint32_t MS5611::readADC(){
    uint32_t buffer[3];
    uint32_t adc;
    I2Cdev::writeByte(devAddr, regAddr, MS5611_ADC_READ);
    I2Cdev::readBytes(devAddr, regAddr, 3, buffer);
    adc = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
    return adc;
}

void MS5611::setdT(){
	MS5611::dT = MS5611::getD2() - MS5611::getC5()*256;
}

void MS5611::setTEMP(bool second_order){
	MS5611::TEMP = 2000 + MS5611::getdT()*MS5611::getC6()/8388608;
	if(second_order){
		MS5611::setT2();
		MS5611::TEMP = 2000 + MS5611::getdT()*MS5611::getC6()/8388608 - MS5611::getT2();
	}
}

void MS5611::setOFF(bool second_order){
	MS5611::OFF = MS5611::getC2() * 65536 + (MS5611::getC4() * MS5611::getdT()) / 128;
	if(second_order){
		MS5611::setOFF2();
		MS5611::OFF = MS5611::getC2() * 65536 + (MS5611::getC4() * MS5611::getdT()) / 128 - MS5611::getOFF2();
	}
}

void MS5611::setSENS(bool second_order){
	MS5611::SENS = MS5611::getC1() * 32768 + (MS5611::getC3 * MS5611::getdT()) / 256;
	if(second_order){
		MS5611::setSENS2();
		MS5611::SENS = MS5611::getC1() * 32768 + (MS5611::getC3 * MS5611::getdT()) / 256 - MS5611::getSENS2();
	}
}

void MS5611::setP(){
	MS5611::P = (MS5611::getD1() * MS5611::getSENS() / 2097152 - MS5611::getOFF()) / 32768;
}

void MS5611::setT2(){
	int32_t tTEMP = MS5611::getTEMP();
	if(tTEMP < 2000){
		MS5611::T2 = MS5611::getdT()*MS5611::getdT() / 2147483648;
	}else{
		MS5611::T2 = 0;
	}
}

void MS5611::setOFF2(){
	int32_t tTEMP = MS5611::getTEMP();
	if(tTEMP < 2000){
		MS5611::OFF2 = 5 * (tTEMP - 2000)*(tTEMP - 2000) / 2;
		if(tTEMP < -1500){
			MS5611::OFF2 = MS5611::OFF2 + 7 * (tTEMP + 1500) * (tTEMP + 1500);
		}
	}else{
		MS5611::OFF2 = 0;
	}
}

void MS5611::setSENS2(){
	int32_t tTEMP = MS5611::getTEMP();
	if(tTEMP < 2000){
		MS5611::SENS2 = 5 * (tTEMP - 2000)*(tTEMP - 2000) / 4;
		if(tTEMP < -1500){
			MS5611::SENS2 = MS5611::SENS2 + 11 * (tTEMP + 1500) * (tTEMP + 1500) / 2;
		}
	}else{
		MS5611::SENS2 = 0;
	}
}

int32_t MS5611::getdT(){
	return MS5611::dT;
}
int32_t MS5611::getTEMP(){
	return MS5611::TEMP;
}
int64_t MS5611::getOFF(){
	return MS5611::OFF;
}
int64_t MS5611::getSENS(){
	return MS5611::SENS;
}
int32_t MS5611::getP(){
	return MS5611::P;
}
int32_t MS5611::getT2(){
	return MS5611::T2;
}
int32_t MS5611::getOFF2(){
	return MS5611::OFF2;
}
int64_t MS5611::getSENS2(){
	return MS5611::SENS2;
}

void MS5611::setC1(uint16_t c){
	MS5611::c1_val = c;
}
void MS5611::setC2(uint16_t c){
	MS5611::c2_val = c;
}
void MS5611::setC3(uint16_t c){
	MS5611::c3_val = c;
}
void MS5611::setC4(uint16_t c){
	MS5611::c4_val = c;
}
void MS5611::setC5(uint16_t c){
	MS5611::c5_val = c;
}
void MS5611::setC6(uint16_t c){
	MS5611::c6_val = c;
}

uint16_t MS5611::getC1(){
	return MS5611::c1_val;
}
uint16_t MS5611::getC2(){
	return MS5611::c2_val;
}
uint16_t MS5611::getC3(){
	return MS5611::c3_val;
}
uint16_t MS5611::getC4(){
	return MS5611::c4_val;
}
uint16_t MS5611::getC5(){
	return MS5611::c5_val;
}
uint16_t MS5611::getC6(){
	return MS5611::c6_val;
}

