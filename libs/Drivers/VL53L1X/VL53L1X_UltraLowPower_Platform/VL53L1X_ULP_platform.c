/*
 Copyright (c) 2021, STMicroelectronics - All Rights Reserved

 This file : part of VL53L1X ULP and : dual licensed,
 either 'STMicroelectronics
 Proprietary license'
 or 'BSD 3-clause "New" or "Revised" License' , at your option.

*******************************************************************************

 'STMicroelectronics Proprietary license'

*******************************************************************************

 License terms: STMicroelectronics Proprietary in accordance with licensing
 terms at www.st.com/sla0081

 STMicroelectronics confidential
 Reproduction and Communication of this document : strictly prohibited unless
 specifically authorized in writing by STMicroelectronics.


*******************************************************************************

 Alternatively, VL53L1X ULP may be distributed under the terms of
 'BSD 3-clause "New" or "Revised" License', in which case the following
 provisions apply instead of the ones mentioned above :

*******************************************************************************

 License terms: BSD 3-clause "New" or "Revised" License.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 3. Neither the name of the copyright holder nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************
*/

#include "VL53L1X_ULP_platform.h"

/* Read 32 bits */
uint8_t VL53L1X_ULP_RdDWord(uint16_t dev, uint16_t RegisterAdress, uint32_t *value)
{
	uint8_t status 					= 255;
	uint8_t regAddr[2]				;
	uint8_t regvalue[4]				;
	i2c_inst_t *i2c					= i2c0;

	int nbytes						= 4;
	int n_bytes_read				;

	// uint16_t Register Adress -> uint8_t array
	regAddr[0] = RegisterAdress >> 8;
	regAddr[1] = RegisterAdress & 0xFF;
	
	sleep_ms(5);

	// (1) Write to register that is to be read
	i2c_write_blocking(i2c, dev, regAddr, 2, true);

	// (2) Read from register
	n_bytes_read = i2c_read_blocking(i2c, dev, regvalue, nbytes, false);

	// (3) Check operation
	if (n_bytes_read == 4) {
		*value = (regvalue[0] << 8) | (regvalue[1] << 16) | (regvalue[2] << 24) | regvalue[3];
		sleep_ms(5);

		status = 0;
	}
	return status;
}

/* Read 16 bits */
uint8_t VL53L1X_ULP_RdWord(uint16_t dev, uint16_t RegisterAdress, uint16_t *value)
{
	uint8_t status 					= 255;
	uint8_t regAddr[2]				;
	uint8_t regvalue[2]				;
	i2c_inst_t *i2c					= i2c0;

	int nbytes					= 2;
	int n_bytes_read;

	// uint16_t Register Adress -> uint8_t array
	regAddr[0] = RegisterAdress >> 8;
	regAddr[1] = RegisterAdress & 0xFF;

	sleep_ms(5);

	// (1) Write to register that is to be read
	i2c_write_blocking(i2c, dev, regAddr, 2, true);

	// (2) Read from register
	n_bytes_read = i2c_read_blocking(i2c, dev, regvalue, nbytes, false);

	// (3) Check operation
	if (n_bytes_read == 2) {
		*value = (regvalue[0] << 8) | regvalue[1];
		sleep_ms(5);

		status = 0;
	}
	return status;
}

/* Read 8 bits */
uint8_t VL53L1X_ULP_RdByte(uint16_t dev, uint16_t RegisterAdress, uint8_t *value)
{
	uint8_t status 					= 255;
	uint8_t regAddr[2]				;
	i2c_inst_t *i2c					= i2c0;

	int nbytes						= 1;
	int n_bytes_read				;

	// uint16_t Register Adress -> uint8_t array
	regAddr[0] = RegisterAdress >> 8;
	regAddr[1] = RegisterAdress & 0xFF;
	
	sleep_ms(5);

	// (1) Write to register that is to be read
	i2c_write_blocking(i2c, dev, regAddr, 2, true);

	// (2) Read from register
	n_bytes_read = i2c_read_blocking(i2c, dev, value, nbytes, false);

	// (3) Check operation
	if (n_bytes_read == 1) {
		status = 0;
	}

	return status;
}

/* Write 8 bits */
uint8_t VL53L1X_ULP_WrByte(uint16_t dev, uint16_t RegisterAdress, uint8_t value)
{
	uint8_t status 					= 255;
	i2c_inst_t *i2c 				= i2c0;

	int nbytes						= 1;
	int n_bytes_sent				;
	uint8_t msg[nbytes + 2]			;

	// (1.1) Append register address to front of data packet
	msg[0] = RegisterAdress >> 8;
	msg[1] = RegisterAdress & 0xFF;

	sleep_ms(5);

	// (1.2) Append message
	for (int i = 0; i < nbytes; i++) {
		msg[i+2] = (value >> i * 8) & 0xFF;
	}

	// (2) Write data to register over I2C.
	n_bytes_sent = i2c_write_blocking(i2c, dev, msg, (nbytes+2), false);

	// (3) Check how many bytes were sent
	// If 1 byte was sent after address, operation was succesful
	if ((n_bytes_sent - 2) == 1) {
		status = 0;
	}

	return status;
}

/* Write 16 bits */
uint8_t VL53L1X_ULP_WrWord(uint16_t dev, uint16_t RegisterAdress, uint16_t value)
{
	uint8_t status 					= 255;
	i2c_inst_t *i2c 				= i2c0;

	int nbytes						= 2;
	int n_bytes_sent				;
	uint8_t msg[nbytes + 2]			;

	// (1.1) Append register address to front of data packet
	msg[0] = RegisterAdress >> 8;
	msg[1] = RegisterAdress & 0xFF;

	sleep_ms(5);

	// (1.2) Append message
	for (int i = 0; i < nbytes; i++) {
		msg[i+2] = (value >> i * 8) & 0xFF;
	}

	// (2) Write data to register over I2C.
	n_bytes_sent = i2c_write_blocking(i2c, dev, msg, (nbytes+2), false);

	// (3) Check how many bytes were sent
	// If 2 bytes were sent after address, operation was succesful
	if ((n_bytes_sent - 2) == 2) {
		status = 0;
	}

	return status;
}

/* Write 32 bits */
uint8_t VL53L1X_ULP_WrDWord(uint16_t dev, uint16_t RegisterAdress, uint32_t value)
{
	uint8_t status 					= 255;
	i2c_inst_t *i2c 				= i2c0;

	int nbytes						= 4;
	int n_bytes_sent				;
	uint8_t msg[nbytes + 2]			;

	// (1.1) Append register address to front of data packet
	msg[0] = RegisterAdress >> 8;
	msg[1] = RegisterAdress & 0xFF;
	sleep_ms(5);

	// (1.2) Append message
	for (int i = 0; i < nbytes; i++) {
		msg[i+2] = (value >> i * 8) & 0xFF;
	}

	// (2) Write data to register over I2C.
	n_bytes_sent = i2c_write_blocking(i2c, dev, msg, (nbytes+2), false);

	// (3) Check how many bytes were sent
	// If 4 bytes were sent after address, operation was succesful
	if ((n_bytes_sent - 2) == 4) {
		status = 0;
	}

	return status;
}

/* Wait for N milliseconds */
uint8_t VL53L1X_ULP_WaitMs(uint32_t TimeMs)
{
	uint8_t status = 255;
	
	sleep_ms(TimeMs);
	
	// Forced status = 0
	status = 0;

	return status;
}
