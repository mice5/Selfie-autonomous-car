/*
 * Allshit.cpp
 *
 *  Created on: 22.03.2018
 *      Author: mice
 */
#include "cmsis_os.h"
#include "Allshit.h"

#include "Futaba.h"

#include "usart.h"
#include "tim.h"

class TestClass{
	float nic = 0;
public:
	float cos = 5.f;
	void init (void);
	TestClass();
	~TestClass();
};
TestClass::TestClass():nic(5){};
void TestClass::init(void){
	cos+=nic;
}
TestClass::~TestClass(){};

TestClass testclass;

void StartLightingTask(void const * argument) {
	for (;;) {
		testclass.init();
		osDelay(1);
	}

}
void StartGyroTask(void const * argument) {
	for (;;) {
		testclass.cos -= 3.f;
		osDelay(1);
	}

}
void StartBatteryManager(void const * argument) {
	for (;;) {
		osDelay(1);
	}

}
void StartSteeringTask(void const * argument) {
	MX_TIM2_Init();
	MX_TIM4_Init();

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	static uint16_t servo_middle = 1500;
	static uint16_t servo_band = 800;
	TIM2->CCR4 = (uint16_t)servo_middle;

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	static float esc_middle = 1500.f;
	static float esc_band = 500.f;
	TIM4->CCR4 = (uint16_t)esc_middle;
	osDelay(500);

	for (;;) {

		TIM2->CCR4 = (uint16_t)( servo_middle + 2.f * servo_band*(FutabaChannelData[3] - 1000.f) / (1921.f - 80.f) );
		TIM4->CCR4 = (uint16_t)( esc_middle +  2.f * esc_band*(FutabaChannelData[1] - 1000.f) / (1921.f - 80.f) );
		osDelay(50);
	}

}
void StartBTTask(void const * argument) {
	for (;;) {
		osDelay(1);
	}

}
void StartCzujnikiTask(void const * argument) {
	for (;;) {
		osDelay(1);
	}

}
void StartFutabaTask(void const * argument) {
	Futaba_init();
	for (;;) {
		osDelay(5);
	}

}
void StartGovernorTask(void const * argument) {
	for (;;) {
		osDelay(1);
	}

}
void StartUSBTask(void const * argument) {
	for (;;) {
		osDelay(1);
	}

}
