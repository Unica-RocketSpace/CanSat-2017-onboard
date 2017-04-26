/*
 * kinematic_unit.h
 *
 *  Created on: 23 февр. 2017 г.
 *      Author: RaKetov
 */

/*
 * МОДУЛЬ, ОТВЕЧАЮЩИЙ ЗА ОПРЕДЕЛЕНИЕ
 * ПОЛОЖЕНИЯ И ОРИНЕТАЦИИ АППАРАТА
 * В ПРОСТРАНСТВЕ
 * */
#ifndef KINEMATIC_UNIT_H_
#define KINEMATIC_UNIT_H_

//давление на земле
//TODO: задать
#define ZERO_PRESSURE 1

extern rscs_bmp280_descriptor_t * bmp280;
extern rscs_ds18b20_t * ds18b20;

/*=================================================================================*/
/*===============================ОПИСАНИЕ=СТРУКТУР=================================*/
/*=================================================================================*/
//СТРУКТУРА КИНЕМАТИЧЕСКОГО СОСТОЯНИЯ АППАРАТА
typedef struct
{

	float aRelatedXYZ[3];	//ускорения в единицах g (в ССК)
	float gRelatedXYZ[3];	//угловые скорости в degps (в ССК)
	float cRelatedXYZ[3];	//косинусы углов вектора магнитного поля с осями ССК
	float pressure;

	float a_XYZ[3];		//ускорения в м/с^2 (ИСК)
	float v_XYZ[3];		//скорости в м/с (ИСК)
	float s_XYZ[3];		//перемещения в м (ИСК)
	float w_XYZ[3];		//угловые скорости в 1/с (ИСК)

	//Матрица поворота ССК относительно ИСК
	float f_XYZ[3][3];	//строка, столбец

	//Единичный вектор магнитного поля
	float B_XYZ[3];

	uint32_t Time;

}state;


//Матрица поворота
typedef struct
{
	float f_XYZ[3][3];
}rotation_matrix;

//Необработанные данные для передачи по радиоканалу
typedef struct
{
	int16_t aTransmitXYZ[3];
	int16_t gTransmitXYZ[3];
	int16_t cTransmitXYZ[3];

	int16_t temperature;
	int16_t pressure;

}transmit_data;
/*=================================================================================*/
/*=================================================================================*/

//КИНЕМАИТЧЕСКОЕ СОСТОЯНИЕ АППАРАТА
extern state STATE;

extern transmit_data TRANSMIT_DATA;


/*=================================================================================*/
/*===============================ОПИСАНИЕ=ФУНКЦИЙ==================================*/
/*=================================================================================*/
//ИНИЦИАЛИЗИРУЕТ ПРОГРАММУ КИНЕМАТИЧЕСКОГО СОСТОЯНИЯ АППАРАТА
void kinematicInit();

//ИНИЦИАЛИЗИРУЕТ ШИНЫ И ДАТЧИКИ
void hardwareInit();

//ЧИТАЕТ BMP280 И ПЕРЕВОДИТ В uint16_t
void BMP280_read_uint16(int32_t * raw_pressure32, int32_t * raw_temp32, uint16_t * raw_pressure, uint16_t * raw_temp);

//ПЕРЕВОДИТ ПОЛУЧЕННЫЙ ВЕКТОР ИЗ СВЯЗАННОЙ СИСТЕМЫ КООРДИНАТ В ИНЕРЦИАЛЬНУЮ, ИСПОЛЬЗУЯ МАТРИЦУ ПОВОРОТА STATE.fXYZ[3][3]
void RSC_to_ISC_recalc(float * RSC_vect, float * ISC_vect);		//R - related, I - inertional

//УСТАНОВКА ИСК (запись поправочных направляющих косинусов)
void set_ISC_offset();

//ЗАПИСЬ МАТРИЦЫ НАПРАВЛЯЮЩИХ КОСИНУСОВ ВЕКТОРА МАГНИТНОГО ПОЛЯ С ОСЯМИ ИСК
void set_magn_dir();

//ОСУЩЕСТВЛЯЕТ КОРРЕКТИРОВКУ МАТРИЦЫ НАПРАВЛЯЮЩИХ КОСИНУСОВ ПО ПОКАЗАНИЯМ МАГНИТОМЕТРА
void recalc_ISC();

//РАССЧИТЫВАЕТ ТЕКУЩЕЕ СОСТОЯНИЕ АППАРАТА
void recon_AGC_STATE_TRANSMIT_DATA();

//ОПРЕДЕЛЯЕТ ВЫСОТУ ПО ДАВЛЕНИЮ
void calculate_height();

//РАССЧИТЫВАЕТ МАТРИЦУ ПОВОРОТА, УГЛОВЫЕ СКОРОСТИ, СКОРОСТИ, УСКОРЕНИЯ И ПЕРЕМЕЩЕНИЯ В ИСК И ЗАПИСЫВАЕТ ИХ В STATE
void trajectoryConstruction();


//ПЕРЕДАЕТ ПЕРЕМЕЩЕНИЯ АППАРАТА В ИСК
//ПАРАМЕТР:		* translations - ссылка на массив[3], в который будут записаны перемещения аппарата по осям X, Y и Z соответственно (м)
void getTranslations (float * translations);

//ПЕРЕДАЕТ УГЛОВЫЕ СКОРОСТИ АППАРАТА В ИСК
//ПАРАМЕТР:		* angVelocityX - ссылка на массив[3], в который будут записаны угловые скорости аппарата вокруг осей X, Y и Z соответственно (рад/с)
void getAngVelocity (float * angVelocity);

//ПЕРЕДАЕТ МАТРИЦУ ПОВОРОТА АППАРАТА
//ПАРАМЕТР:		* RotationMatrix - ссылка на массив[9], в который будут записаны косинусы углов поворота ССК относительно ИСК (-)
void getRotationMatrix (float * RotationMatrix);



#endif /* KINEMATIC_UNIT_H_ */
