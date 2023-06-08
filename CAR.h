//
// Created by kurum on 西暦2023年6月8日.
//

#ifndef STC32_CAR_H
#define STC32_CAR_H
#include <STC32G.H>
#define CARSTART() P04=1;P05=0;P06=1;P07=0;
#define CARSTOP() P04=1;P05=1;P06=1;P07=1;
#define CARBACK() P04=0;P05=1;P06=0;P07=1;
#define CARLEFT() P04=1;P05=0;P06=0;P07=1;
#define CARRIGHT() P04=0;P05=1;P06=1;P07=0;
#endif //STC32_CAR_H
